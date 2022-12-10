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
    addAndMakeVisible(keyboardComponent);

    sliderAttack.setSliderStyle(juce::Slider::Rotary);
    sliderAttack.setRange(1.0, 250.0, 1.0);
    sliderAttack.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    sliderAttack.setPopupDisplayEnabled(true, false, this);
    sliderAttack.setTextValueSuffix(" ms");
    sliderAttack.setValue(25.0);
    addAndMakeVisible(&sliderAttack);
    sliderAttack.addListener(this);

    sliderDecay.setSliderStyle(juce::Slider::Rotary);
    sliderDecay.setRange(1.0, 500.0, 1.0);
    sliderDecay.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    sliderDecay.setPopupDisplayEnabled(true, false, this);
    sliderDecay.setTextValueSuffix(" ms");
    sliderDecay.setValue(100.0);
    addAndMakeVisible(&sliderDecay);
    sliderDecay.addListener(this);

    sliderSustain.setSliderStyle(juce::Slider::Rotary);
    sliderSustain.setRange(1.0, 100.0, 1.0);
    sliderSustain.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    sliderSustain.setPopupDisplayEnabled(true, false, this);
    sliderSustain.setTextValueSuffix(" %");
    sliderSustain.setValue(30.0);
    addAndMakeVisible(&sliderSustain);
    sliderSustain.addListener(this);

    sliderRelease.setSliderStyle(juce::Slider::Rotary);
    sliderRelease.setRange(10.0, 4000.0, 10.0);
    sliderRelease.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    sliderRelease.setPopupDisplayEnabled(true, false, this);
    sliderRelease.setTextValueSuffix(" ms");
    sliderRelease.setValue(500.0);
    addAndMakeVisible(&sliderRelease);
    sliderRelease.addListener(this);

    sliderModulationIndex.setSliderStyle(juce::Slider::Rotary);
    sliderModulationIndex.setRange(0.0, 10.0, 0.1);
    sliderModulationIndex.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    sliderModulationIndex.setPopupDisplayEnabled(true, false, this);
    sliderModulationIndex.setTextValueSuffix(" %");
    sliderModulationIndex.setValue(0.0);
    addAndMakeVisible(&sliderModulationIndex);
    sliderModulationIndex.addListener(this);

    sliderModulationRatio.setSliderStyle(juce::Slider::Rotary);
    sliderModulationRatio.setRange(1, 10.0, 0.5);
    sliderModulationRatio.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    sliderModulationRatio.setPopupDisplayEnabled(true, false, this);
    sliderModulationRatio.setTextValueSuffix(" %");
    sliderModulationRatio.setValue(1);
    addAndMakeVisible(&sliderModulationRatio);
    sliderModulationRatio.addListener(this);

    sliderPitchBend.setSliderStyle(juce::Slider::Rotary);
    sliderPitchBend.setRange(-12, 12.0, 0.5);
    sliderPitchBend.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    sliderPitchBend.setPopupDisplayEnabled(true, false, this);
    sliderPitchBend.setTextValueSuffix(" semitones");
    sliderPitchBend.setValue(0);
    addAndMakeVisible(&sliderPitchBend);
    sliderPitchBend.addListener(this);

    sliderLFO.setSliderStyle(juce::Slider::Rotary);
    sliderLFO.setRange(0, 20, 1);
    sliderLFO.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    sliderLFO.setPopupDisplayEnabled(true, false, this);
    sliderLFO.setTextValueSuffix(" Hz");
    sliderLFO.setValue(0);
    addAndMakeVisible(&sliderLFO);
    sliderLFO.addListener(this);

    sliderWaveShaper.setSliderStyle(juce::Slider::Rotary);
    sliderWaveShaper.setRange(0, 3, 1);
    sliderWaveShaper.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    //sliderWaveShaper.setPopupDisplayEnabled(true, false, this);
    //sliderWaveShaper.setTextValueSuffix(" Shape");
    sliderWaveShaper.setValue(0);
    addAndMakeVisible(&sliderWaveShaper);
    sliderWaveShaper.addListener(this);

    sliderModulatorShaper.setSliderStyle(juce::Slider::Rotary);
    sliderModulatorShaper.setRange(0, 3, 1);
    sliderModulatorShaper.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    //sliderModulatorShaper.setPopupDisplayEnabled(true, false, this);
    //sliderModulatorShaper.setTextValueSuffix(" Modulo Shape");
    sliderModulatorShaper.setValue(0);
    addAndMakeVisible(&sliderModulatorShaper);
    sliderModulatorShaper.addListener(this);


    setSize(420, 500);
    startTimer(400);

}

GeneratorADSRAudioProcessorEditor::~GeneratorADSRAudioProcessorEditor()
{
}

//==============================================================================
void GeneratorADSRAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    shapes = juce::ImageCache::getFromMemory(BinaryData::shapes_png, BinaryData::shapes_pngSize);
    g.drawImageWithin(shapes, 28, 22, 162, 123, juce::RectanglePlacement::stretchToFit);
    g.drawImageWithin(shapes, 28, 262, 162, 123, juce::RectanglePlacement::stretchToFit);

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);

    g.drawSingleLineText("Attack", 60, 150, juce::Justification::centred);
    g.drawSingleLineText("Decay", 160, 150, juce::Justification::centred);
    g.drawSingleLineText("Sustain", 260, 150, juce::Justification::centred);
    g.drawSingleLineText("Release", 360, 150, juce::Justification::centred);

    g.drawSingleLineText("Main", 110, 30, juce::Justification::centred);
    g.drawSingleLineText("LFO", 260, 30, juce::Justification::centred);
    g.drawSingleLineText("Pitch Bend", 360, 30, juce::Justification::centred);

    g.drawSingleLineText("Mod", 110, 270, juce::Justification::centred);
    g.drawSingleLineText("Mod Index", 260, 270, juce::Justification::centred);
    g.drawSingleLineText("Mod Ratio", 360, 270, juce::Justification::centred);
}

void GeneratorADSRAudioProcessorEditor::resized()
{
    keyboardComponent.setBounds(10, 390, getWidth() - 20, 110);
    sliderAttack.setBounds(10, 150, 100, 100);
    sliderDecay.setBounds(110, 150, 100, 100);
    sliderSustain.setBounds(210, 150, 100, 100);
    sliderRelease.setBounds(310, 150, 100, 100);

    sliderModulatorShaper.setBounds(60, 270, 100, 100);
    sliderModulationIndex.setBounds(210, 270, 100, 100);
    sliderModulationRatio.setBounds(310, 270, 100, 100);

    sliderWaveShaper.setBounds(60, 30, 100, 100);
    sliderLFO.setBounds(210, 30, 100, 100);
    sliderPitchBend.setBounds(310, 30, 100, 100);
}

void  GeneratorADSRAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    float sr = (float)audioProcessor.getSampleRate();
    audioProcessor.procA = sr * (sliderAttack.getValue() / 1000.0f);
    audioProcessor.procD = sr * (sliderDecay.getValue() / 1000.0f);
    audioProcessor.procS = sliderSustain.getValue() / 100.0f;
    audioProcessor.procR = sr * (sliderRelease.getValue() / 1000.0f);

    audioProcessor.procModId = sliderModulationIndex.getValue();
    audioProcessor.procModRatio = sliderModulationRatio.getValue();
    audioProcessor.procPitchBend = pow(2, (sliderPitchBend.getValue() / 12));
    audioProcessor.procLFO = sliderLFO.getValue();
    audioProcessor.waveShape = sliderWaveShaper.getValue();
    audioProcessor.modulatorShape = sliderModulatorShaper.getValue();

}


