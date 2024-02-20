/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class Mu45compressorAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    Mu45compressorAudioProcessor();
    ~Mu45compressorAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    juce::AudioParameterFloat* threshold;
    juce::AudioParameterFloat* ratio;
    juce::AudioParameterFloat* attack;
    juce::AudioParameterFloat* release;

    float mFs;
    
    float currentGain = 1.0;
    float rmslevel;
    float mEnvOut = 0.0;
    float mEnvB0;
    float mEnvTau = 0.010;
    //float sumOfSquares=0.0;
    //int sampleCount=0;
    
    
    
    void calculateRMSLevel(const float* samples, int numSamples);
    float calcGain(float inputLevel);
    float applyGain(float desiredGain);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Mu45compressorAudioProcessor)
};
