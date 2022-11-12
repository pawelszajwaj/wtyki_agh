/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Zad1AudioProcessorEditor::Zad1AudioProcessorEditor (Zad1AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    setSize(500, 150);
    qStep.setSliderStyle(juce::Slider::Rotary);
    qStep.setRange(1.0, 32.0, 1.0);
    qStep.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    qStep.setPopupDisplayEnabled(true, false, this);
    qStep.setTextValueSuffix(" steps");
    qStep.setValue(16.0);
    addAndMakeVisible(&qStep);

    qStep.addListener(this);
    turnOn.setButtonText("On");
    turnOn.setToggleState(true, juce::sendNotification);
    addAndMakeVisible(&turnOn);
    turnOn.onClick = [this] {Zad1AudioProcessorEditor ::onStateSwitch(); };

    drywet.setSliderStyle(juce::Slider::Rotary);
    drywet.setRange(0.0, 100.0, 0.1);
    drywet.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    drywet.setPopupDisplayEnabled(true, false, this);
    drywet.setTextValueSuffix(" %");
    drywet.setValue(16.0);
    addAndMakeVisible(&drywet);
    drywet.addListener(this);

}

Zad1AudioProcessorEditor::~Zad1AudioProcessorEditor()
{
}

//==============================================================================
void Zad1AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
    g.drawFittedText("Bit Crusher", 0, 0, getWidth(), 30, juce::Justification::centred, 1);
}

void Zad1AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    qStep.setBounds(30, 30, 100, 100);

    drywet.setBounds(170, 30, 100, 100);
    turnOn.setBounds(getWidth() - 80, 30, 80, 30);

}

void Zad1AudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.steps = qStep.getValue();
    audioProcessor.wetdryMix = drywet.getValue();
}
void Zad1AudioProcessorEditor::onStateSwitch()
{
    auto state = turnOn.getToggleState();
    audioProcessor.turnedOn = (bool)state;
}

