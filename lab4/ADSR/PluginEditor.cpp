/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GeneratorADSRAudioProcessorEditor::GeneratorADSRAudioProcessorEditor(GeneratorADSRAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    keyboardComponent(audioProcessor.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)

{
    //addAndMakeVisible(keyboardComponent);

    sliderAttack.setSliderStyle(juce::Slider::Rotary);
    sliderAttack.setRange(1.0, 250.0, 1.0);
    sliderAttack.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    sliderAttack.setPopupDisplayEnabled(true, false, this);
    sliderAttack.setTextValueSuffix(" attack ms");
    sliderAttack.setValue(25.0);
    addAndMakeVisible(&sliderAttack);
    sliderAttack.addListener(this);

    sliderDecay.setSliderStyle(juce::Slider::Rotary);
    sliderDecay.setRange(1.0, 500.0, 1.0);
    sliderDecay.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    sliderDecay.setPopupDisplayEnabled(true, false, this);
    sliderDecay.setTextValueSuffix(" decay ms");
    sliderDecay.setValue(100.0);
    addAndMakeVisible(&sliderDecay);
    sliderDecay.addListener(this);

    sliderSustain.setSliderStyle(juce::Slider::Rotary);
    sliderSustain.setRange(1.0, 100.0, 1.0);
    sliderSustain.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    sliderSustain.setPopupDisplayEnabled(true, false, this);
    sliderSustain.setTextValueSuffix(" sustain %");
    sliderSustain.setValue(30.0);
    addAndMakeVisible(&sliderSustain);
    sliderSustain.addListener(this);

    sliderRelease.setSliderStyle(juce::Slider::Rotary);
    sliderRelease.setRange(10.0, 4000.0, 10.0);
    sliderRelease.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    sliderRelease.setPopupDisplayEnabled(true, false, this);
    sliderRelease.setTextValueSuffix(" release ms");
    sliderRelease.setValue(500.0);
    addAndMakeVisible(&sliderRelease);
    sliderRelease.addListener(this);


    setSize(600, 160);
    startTimer(400);

}

GeneratorADSRAudioProcessorEditor::~GeneratorADSRAudioProcessorEditor()
{
}

//==============================================================================
void GeneratorADSRAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("ADSR", getLocalBounds(), juce::Justification::centredTop, 1);
}

void GeneratorADSRAudioProcessorEditor::resized()
{
    //keyboardComponent.setBounds(10, 10, getWidth() - 20, getHeight() - 20);
    sliderAttack.setBounds(30, 30, 100, 100);
    sliderDecay.setBounds(130, 30, 100, 100);
    sliderSustain.setBounds(230, 30, 100, 100);
    sliderRelease.setBounds(330, 30, 100, 100);
}

void  GeneratorADSRAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    float sr = (float)audioProcessor.getSampleRate();
    audioProcessor.procA = sr * (sliderAttack.getValue() / 1000.0f);
    audioProcessor.procD = sr * (sliderDecay.getValue() / 1000.0f);
    audioProcessor.procS = sliderSustain.getValue() / 100.0f;
    audioProcessor.procR = sr * (sliderRelease.getValue() / 1000.0f);

}


