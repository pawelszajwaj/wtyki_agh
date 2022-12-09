/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GenerationLossAudioProcessorEditor::GenerationLossAudioProcessorEditor (GenerationLossAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    sliderLowPassFilter.setSliderStyle(juce::Slider::Rotary);
    sliderLowPassFilter.setRange(50, 10000, 1.0);
    sliderLowPassFilter.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    sliderLowPassFilter.setPopupDisplayEnabled(true, false, this);
    sliderLowPassFilter.setTextValueSuffix(" Hz");
    sliderLowPassFilter.setValue(2000.0);
    addAndMakeVisible(&sliderLowPassFilter);
    sliderLowPassFilter.addListener(this);

    sliderHighPassFilter.setSliderStyle(juce::Slider::Rotary);
    sliderHighPassFilter.setRange(20, 250.0, 0.5);
    sliderHighPassFilter.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    sliderHighPassFilter.setPopupDisplayEnabled(true, false, this);
    sliderHighPassFilter.setTextValueSuffix(" Hz");
    sliderHighPassFilter.setValue(30.0);
    addAndMakeVisible(&sliderHighPassFilter);
    sliderHighPassFilter.addListener(this);

    sliderPinkNoise.setSliderStyle(juce::Slider::Rotary);
    sliderPinkNoise.setRange(0.0f, 1, 0.001);
    sliderPinkNoise.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    sliderPinkNoise.setPopupDisplayEnabled(true, false, this);
    sliderPinkNoise.setTextValueSuffix(" Amount");
    sliderPinkNoise.setValue(0.075);
    addAndMakeVisible(&sliderPinkNoise);
    sliderPinkNoise.addListener(this);

    sliderSawLevel.setSliderStyle(juce::Slider::Rotary);
    sliderSawLevel.setRange(0.0f, 1, 0.001);
    sliderSawLevel.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    sliderSawLevel.setPopupDisplayEnabled(true, false, this);
    sliderSawLevel.setTextValueSuffix(" Amount");
    sliderSawLevel.setValue(0.075);
    addAndMakeVisible(&sliderSawLevel);
    sliderSawLevel.addListener(this);

    sliderQuantization.setSliderStyle(juce::Slider::Rotary);
    sliderQuantization.setRange(1, 16, 1);
    sliderQuantization.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    sliderQuantization.setPopupDisplayEnabled(true, false, this);
    sliderQuantization.setTextValueSuffix(" Bits");
    sliderQuantization.setValue(8);
    addAndMakeVisible(&sliderQuantization);
    sliderQuantization.addListener(this);

    setSize (700, 350);
}

GenerationLossAudioProcessorEditor::~GenerationLossAudioProcessorEditor()
{
}

//==============================================================================
void GenerationLossAudioProcessorEditor::paint (juce::Graphics& g)
{
   background = juce::ImageCache::getFromMemory(BinaryData::stripes_jpg, BinaryData::stripes_jpgSize);
   g.drawImageWithin(background, 25 ,25, getWidth() - 400, getHeight() - 50, juce::RectanglePlacement::stretchToFit);
  
   g.setColour(juce::Colours::white);
   g.setFont(15.0f);
   g.drawSingleLineText("Low Pass", 370, 82, juce::Justification::centred);
   g.drawSingleLineText("High Pass", 660, 82, juce::Justification::centred);
   g.drawSingleLineText("Pink Noise", 370, 182, juce::Justification::centred);
   g.drawSingleLineText("Saw", 660, 182, juce::Justification::centred);
   g.drawSingleLineText("Quantization", 425, 282, juce::Justification::centred);
    
}

void GenerationLossAudioProcessorEditor::resized()
{
    sliderLowPassFilter.setBounds(403, 30, 100, 100);
    sliderHighPassFilter.setBounds(528, 30, 100, 100);
    sliderPinkNoise.setBounds(403, 130, 100, 100);
    sliderSawLevel.setBounds(528, 130, 100, 100);
    sliderQuantization.setBounds(475, 230, 100, 100);
}

void  GenerationLossAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    //float sr = (float)audioProcessor.getSampleRate();

    audioProcessor.lpfreq = sliderLowPassFilter.getValue();
    audioProcessor.hpfreq = sliderHighPassFilter.getValue();
    audioProcessor.pinkNoiseLevel = (sliderPinkNoise.getValue()/100);
    audioProcessor.sawLevel = (sliderSawLevel.getValue() / 200);
    audioProcessor.quantizationSteps = sliderQuantization.getValue();
}
