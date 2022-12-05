/*
  ==============================================================================

    SynthAudioSource.h
    Created: 21 Nov 2022 3:47:35pm
    Author:  muche

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
//==============================================================================
struct SineWaveSound : public juce::SynthesiserSound
{
    SineWaveSound() {}
    bool appliesToNote(int) override { return true; }
    bool appliesToChannel(int) override { return true; }
};
//==============================================================================
struct SineWaveVoice : public juce::SynthesiserVoice
{
    SineWaveVoice(const juce::AudioSampleBuffer& wavetableToUse, unsigned
        int voices) : wavetable(wavetableToUse)
    {
        globalLevel = 0.5 / (float)voices;
        jassert(wavetable.getNumChannels() == 1);
    }

    void setFrequency(float frequency, float sampleRate)
    {
        auto tableSizeOverSampleRate = wavetable.getNumSamples() /sampleRate;
        tableDelta = frequency * tableSizeOverSampleRate;
    }

    forcedinline float getNextSample() noexcept
    {
        auto tableSize = wavetable.getNumSamples();
        auto index0 = (unsigned int)currentIndex;
        auto index1 = index0 == (tableSize - 1) ? (unsigned int)0 :index0 + 1;
        auto frac = currentIndex - (float)index0;
        auto* table = wavetable.getReadPointer(0);
        auto value0 = table[index0];
        auto value1 = table[index1];
        auto currentSample = value0 + frac * (value1 - value0);
        if ((currentIndex += tableDelta) > tableSize)
            currentIndex -= tableSize;
        return currentSample;
    }

    bool canPlaySound(juce::SynthesiserSound* sound) override
    {
        return dynamic_cast<SineWaveSound*> (sound) != nullptr;
    }
    
    void startNote(int midiNoteNumber, float velocity,
        juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        currentIndex = 0.0; // *** currentAngle = 0.0;
        level = velocity * globalLevel;
        tailOff = 0.0;

        setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber), getSampleRate()); 
    }

    void stopNote(float /*velocity*/, bool allowTailOff) override
    {
        if (allowTailOff)
        {
            if (tailOff == 0.0)
                tailOff = 1.0;
        }
        else
        {
            clearCurrentNote();
            tableDelta = 0.0f; // *** angleDelta = 0.0;
        }
    }
    
    void pitchWheelMoved(int) override {}
    void controllerMoved(int, int) override {}
    void renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
    {
        if (tableDelta != 0.0) // ***
        {
            if (tailOff > 0.0)
            {
                while (--numSamples >= 0)
                {
                    // faza zanika

                    auto currentSample = getNextSample() * level * tailOff;


                    ++startSample;
                    tailOff *= 0.999;
                    if (tailOff <= 0.005)
                    {
                        clearCurrentNote();
                        tableDelta = 0.0f; // *** angleDelta = 0.0;
                        break;
                    }
                }
            }
            else
            {
                while (--numSamples >= 0)
                {
                    // faza podtrzymania
                    auto currentSample = getNextSample(); // ***


                    if (currentSample >= 0)
                        currentSample = 1 * level;
                    else
                        currentSample = -1 * level;

                    for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                        outputBuffer.addSample(i, startSample, currentSample);

                    ++startSample;
                }
            }
        }
    }

    float pitchBend;

private:
    
    float tailOff = 0.0;
    const juce::AudioSampleBuffer& wavetable;
    float currentIndex = 0.0f, tableDelta = 0.0f, level = 0.0f, globalLevel = 0.0f;
};
//==============================================================================
class SynthAudioSource : public juce::AudioSource
{
public:
    SynthAudioSource(juce::MidiKeyboardState& keyState)
        : keyboardState(keyState)
    {
        for (auto i = 0; i < numVoices; ++i)
            synth.addVoice(new SineWaveVoice(signalTable, numVoices));
        synth.addSound(new SineWaveSound());
    }
    void setUsingSineWaveSound()
    {
        synth.clearSounds();
    }
    void prepareToPlay(int /*samplesPerBlockExpected*/, double
        sampleRate) override
    {
        synth.setCurrentPlaybackSampleRate(sampleRate);
        createWavetable(); // ***
    }
    void releaseResources() override {}
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& 
        bufferToFill) override
    {
        bufferToFill.clearActiveBufferRegion();
        juce::MidiBuffer incomingMidi;
        midiCollector.removeNextBlockOfMessages(incomingMidi, bufferToFill.numSamples);
        keyboardState.processNextMidiBuffer(incomingMidi, bufferToFill.startSample, bufferToFill.numSamples, true);
        synth.renderNextBlock(*bufferToFill.buffer, incomingMidi, bufferToFill.startSample, bufferToFill.numSamples);
    }
    juce::MidiMessageCollector* getMidiCollector()
    {
        return &midiCollector;
    }
    juce::MidiMessageCollector midiCollector;

    void createWavetable()
    {
        signalTable.setSize(1, tableSize);
        auto* samples = signalTable.getWritePointer(0);
        auto angleDelta = juce::MathConstants<double>::twoPi / (double)(tableSize - 1);
        auto currentAngle = 0.0;
        for (auto i = 0; i < tableSize; ++i)
        {
            auto sample = std::sin(currentAngle);
            if (sample >= 0)
              sample = 1;
            else
              sample = -1;
            samples[i] = (float)sample;
            currentAngle += angleDelta;
        }
    }


private:
    juce::MidiKeyboardState& keyboardState;
    juce::Synthesiser synth;
    int tableSize = 128;
    int numVoices = 4;
    juce::AudioSampleBuffer signalTable;
};

