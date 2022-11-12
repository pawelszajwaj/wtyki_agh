/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Zad2AudioProcessorEditor::Zad2AudioProcessorEditor (Zad2AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    VolumeSlider.addListener(this);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    
    // MOD 3 parametry suwaka
    VolumeSlider.setSliderStyle(juce::Slider::LinearBarVertical);
    VolumeSlider.setRange(0.0, 1.0, 0.001);
    VolumeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    VolumeSlider.setPopupDisplayEnabled(true, false, this);
    VolumeSlider.setTextValueSuffix("Volume");
    VolumeSlider.setValue(1.0);
    // MOD 4 dodanie suwaka do edytora
    addAndMakeVisible(&VolumeSlider);

    phaseButton.setToggleState(true, true);
    phaseButton.onClick = [this] {Zad2AudioProcessorEditor::boolValueChanged(); };
    addAndMakeVisible(phaseButton);

    setSize(400, 300);
}

Zad2AudioProcessorEditor::~Zad2AudioProcessorEditor()
{
}

//==============================================================================
void Zad2AudioProcessorEditor::paint (juce::Graphics& g)
{
    // MOD 6 wypelnienie calego okna
    //g.fillAll(juce::Colours::white);
    // MOD 7 kolor rysowania
    //g.setColour(juce::Colours::black);
    // MOD 8 nowy tekst
    g.setFont(15.0f);
    g.drawFittedText("I AM SO LOUDD", 0, 0, getWidth(), 30,
        juce::Justification::centred, 1);
}

void Zad2AudioProcessorEditor::resized()
{
    phaseButton.setBounds(100, 40, getWidth() - 30, 20);
    VolumeSlider.setBounds(40, 30, 20, getHeight() - 60);
}
void Zad2AudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.gain->setValueNotifyingHost(VolumeSlider.getValue());
}

void Zad2AudioProcessorEditor::boolValueChanged()
{
    auto state = phaseButton.getToggleState();
    audioProcessor.invertPhase->operator= (state);
}