/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Mu45compressorAudioProcessorEditor::Mu45compressorAudioProcessorEditor (Mu45compressorAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500, 300);
    auto& params = processor.getParameters();
    juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(0);
    thresholdSlider.setBounds(50,80,60,200);
    thresholdSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    thresholdSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 150, 30);
    thresholdSlider.setRange(audioParam->range.start, audioParam->range.end);
    thresholdSlider.setValue(*audioParam);
    thresholdSlider.setNumDecimalPlacesToDisplay(2);
    thresholdSlider.setTextValueSuffix ("db");
    thresholdSlider.setDoubleClickReturnValue(true, -24.0f);
    thresholdSlider.addListener(this);
    addAndMakeVisible(thresholdSlider);
    
    audioParam = (juce::AudioParameterFloat*)params.getUnchecked(1);
    ratioSlider.setBounds(150,80,60,200);
    ratioSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    ratioSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 150, 30);
    ratioSlider.setRange(audioParam->range.start, audioParam->range.end);
    ratioSlider.setValue(*audioParam);
    ratioSlider.setNumDecimalPlacesToDisplay(2);
    ratioSlider.setDoubleClickReturnValue(true, 4.0f);
    ratioSlider.addListener(this);
    addAndMakeVisible(ratioSlider);
    
    audioParam = (juce::AudioParameterFloat*)params.getUnchecked(2);
    attackSlider.setBounds(250,80,60,200);
    attackSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 150, 30);
    attackSlider.setRange(audioParam->range.start, audioParam->range.end);
    attackSlider.setValue(*audioParam);
    attackSlider.setNumDecimalPlacesToDisplay(2);
    attackSlider.setTextValueSuffix ("ms");
    attackSlider.setDoubleClickReturnValue(true, 10.0f);
    attackSlider.addListener(this);
    addAndMakeVisible(attackSlider);
    
    
    audioParam = (juce::AudioParameterFloat*)params.getUnchecked(3);
    releaseSlider.setBounds(350,80,60,200);
    releaseSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 150, 30);
    releaseSlider.setRange(audioParam->range.start, audioParam->range.end);
    releaseSlider.setValue(*audioParam);
    releaseSlider.setNumDecimalPlacesToDisplay(2);
    releaseSlider.setTextValueSuffix ("ms");
    releaseSlider.setDoubleClickReturnValue(true, 10.0f);
    releaseSlider.addListener(this);
    addAndMakeVisible(releaseSlider);
    
    thresholdLabel.setText("threshold", juce::dontSendNotification);
    thresholdLabel.setBounds(thresholdSlider.getX(), thresholdSlider.getY() - 25, thresholdSlider.getWidth(), 20);
    addAndMakeVisible(thresholdLabel);
    
    ratioLabel.setText("ratio", juce::dontSendNotification);
    ratioLabel.setBounds(ratioSlider.getX() + 10, ratioSlider.getY() - 25, ratioSlider.getWidth(), 20);
    addAndMakeVisible(ratioLabel);
    
    attackLabel.setText("attack", juce::dontSendNotification);
    attackLabel.setBounds(attackSlider.getX() + 5, attackSlider.getY() - 25, attackSlider.getWidth(), 20);
    addAndMakeVisible(attackLabel);
    
    releaseLabel.setText("release", juce::dontSendNotification);
    releaseLabel.setBounds(releaseSlider.getX(), releaseSlider.getY() - 25, releaseSlider.getWidth(), 20);
    addAndMakeVisible(releaseLabel);
    
}

void Mu45compressorAudioProcessorEditor::sliderValueChanged(juce::Slider * slider)
{
    auto& params = processor.getParameters();
    if (slider == &thresholdSlider)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(0);
        *audioParam = thresholdSlider.getValue();
        DBG("threshold slider changed");
    }
    else if (slider == &ratioSlider)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(1);
        *audioParam = ratioSlider.getValue();
    
        DBG("ratio slider changed");
    }
    else if (slider == &attackSlider)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(2);
        *audioParam = attackSlider.getValue();
        DBG("attack slider changed");
    }
    else if (slider == &releaseSlider)
    {
        juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(3);
        *audioParam = releaseSlider.getValue();
        DBG("release slider changed");
    }

}

Mu45compressorAudioProcessorEditor::~Mu45compressorAudioProcessorEditor()
{
}

//==============================================================================
void Mu45compressorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (25.0f);
    auto localbounds = getLocalBounds();
    auto toparea = localbounds.removeFromTop(30);
    g.drawFittedText ("MuCompressor", toparea, juce::Justification::centredTop, 1);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void Mu45compressorAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
