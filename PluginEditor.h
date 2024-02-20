/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
//==============================================================================
/**
*/
class Mu45compressorAudioProcessorEditor  : public juce::AudioProcessorEditor,public juce::Slider::Listener
{
public:
    Mu45compressorAudioProcessorEditor (Mu45compressorAudioProcessor&);
    ~Mu45compressorAudioProcessorEditor() override;

    //==============================================================================
    void timerCallback();
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override;
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Mu45compressorAudioProcessor& audioProcessor;
    
    juce::Slider thresholdSlider;
    juce::Slider ratioSlider;
    juce::Slider attackSlider;
    juce::Slider releaseSlider;
    
    juce::Label thresholdLabel;
    juce::Label ratioLabel;
    juce::Label attackLabel;
    juce::Label releaseLabel;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Mu45compressorAudioProcessorEditor)
};
