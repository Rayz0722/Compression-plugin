/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Mu45compressorAudioProcessor::Mu45compressorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    addParameter(threshold = new juce::AudioParameterFloat("threshold (db)", // parameterID,
            "Threshold", // parameterName,
            -60.0f, // minValue,
            -0.0f, // maxValue,
            -24.0f)); // defaultValue
    addParameter(ratio = new juce::AudioParameterFloat("ratio (%)", // parameterID,
            "Ratio", // parameterName,
            1.0f, // minValue,
            30.0f, // maxValue,
            4.0f)); // defaultValue
    addParameter(attack = new juce::AudioParameterFloat("attack (ms)", // parameterID,
            "Attack", // parameterName,
            0.1f, // minValue,
            100.0f, // maxValue,
            10.0f)); // defaultValue
    addParameter(release = new juce::AudioParameterFloat("release (ms)", // parameterID,
            "Release", // parameterName,
            10.0f, // minValue,
            1000.0f, // maxValue,
            10.0f)); // defaultValue
    
}

Mu45compressorAudioProcessor::~Mu45compressorAudioProcessor()
{
}

//==============================================================================
const juce::String Mu45compressorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Mu45compressorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Mu45compressorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Mu45compressorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Mu45compressorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Mu45compressorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Mu45compressorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Mu45compressorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Mu45compressorAudioProcessor::getProgramName (int index)
{
    return {};
}

void Mu45compressorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Mu45compressorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    mFs = sampleRate;
    mEnvB0 = 1 - exp(-1/(mEnvTau*mFs));
}

void Mu45compressorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Mu45compressorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else

    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif


float Mu45compressorAudioProcessor::calcGain(float inputLevel){
    float T = threshold->get();
    float R = ratio->get();

    // Check if input level is below threshold
    if (inputLevel < T)
        return 1.0f; // No gain reduction
    // Apply compression above threshold
    float compressedLeveldB = T + (inputLevel - T) / R;
    // Convert back from dB to linear gain
    float linearGain = pow(10.0f, compressedLeveldB / 20.0f);
    return linearGain;
}


float Mu45compressorAudioProcessor::applyGain(float desiredGain) {
    float A = attack->get()/1000;
    float R = release ->get()/1000;
    float attackCoefficient = 1.0 - exp(-1.0f / (mFs * A));
    float releaseCoefficient = 1.0 - exp(-1.0f / (mFs * R));
    
    float gainCoeff;
    if (desiredGain < currentGain){
        //needs reduction
        gainCoeff = attackCoefficient;
    }
    else{
        gainCoeff = releaseCoefficient;
    }
    currentGain += gainCoeff * (desiredGain - currentGain);
    return currentGain;
}

void Mu45compressorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    float* channelDataL = buffer.getWritePointer (0);
    float* channelDataR = buffer.getWritePointer (1);

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        float tempL = channelDataL[sample]*channelDataL[sample];
        float tempR = channelDataR[sample]*channelDataR[sample];
        float tempEnv = 0.5 * (tempL+tempR);
        mEnvOut += mEnvB0 * (tempEnv - mEnvOut);
        tempEnv = std::sqrt(mEnvOut);         //rms level in linear 
        float rmslevel = 20*log10(tempEnv); //convert to db
            
        float desiredGain = calcGain(rmslevel);
        float appliedGain = applyGain(desiredGain);
        
        channelDataL[sample] *= appliedGain;
        //channelDataR[sample] = appliedGain;
        channelDataR[sample] *= appliedGain;
    }
        
}

//==============================================================================
bool Mu45compressorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Mu45compressorAudioProcessor::createEditor()
{
    return new Mu45compressorAudioProcessorEditor (*this);
}

//==============================================================================
void Mu45compressorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Mu45compressorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Mu45compressorAudioProcessor();
}
