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
    SineWaveVoice(const juce::AudioSampleBuffer& wavetableToUse, unsigned int voices,
        float* ptrA, float* ptrD, float* ptrS, float* ptrR, float* ptrMId, float* ptrMRt,
        float* ptrPitchBend, float *ptrLFO, int* ptrWaveShape, int* ptrModulatorShape) : wavetable(wavetableToUse)
    {
        globalLevel = 0.5 / (float)voices;
        jassert(wavetable.getNumChannels() == 1);
        voiceA = ptrA;
        voiceD = ptrD;
        voiceS = ptrS;
        voiceR = ptrR;
        modId = ptrMId;
        modRatio = ptrMRt;
        pitchBend = ptrPitchBend;
        freqLFO = ptrLFO;
        waveShape = ptrWaveShape;
        modulatorShape = ptrModulatorShape;
    }


    void setFrequency(float frequency, float sampleRate)
    {
        auto tableSizeOverSampleRate = wavetable.getNumSamples() /sampleRate;
        tableDelta = frequency * tableSizeOverSampleRate * (*pitchBend);
        tableDeltaModulator = frequency * (*modRatio) * tableSizeOverSampleRate;
        tableDeltaLFO = (*freqLFO) * tableSizeOverSampleRate;
    }

    forcedinline float getNextSample() noexcept
    {
        auto tableSize = wavetable.getNumSamples();
        auto index0 = (unsigned int)currentIndex;
        auto index1 = index0 == (tableSize - 1) ? (unsigned int)0 :index0 + 1;
        auto frac = currentIndex - (float)index0;
        auto* table = wavetable.getReadPointer(*waveShape);
        auto value0 = table[index0];
        auto value1 = table[index1];
        auto currentSample = value0 + frac * (value1 - value0);
        if ((currentIndex += tableDelta) > tableSize)
            currentIndex -= tableSize;

        return currentSample;
    }

    forcedinline float getNextSampleModulator() noexcept        // tu mamy modulator sygna??
    {
        auto tableSize = wavetable.getNumSamples();
        auto index0 = (unsigned int)currentIndexModulator;
        auto index1 = index0 == (tableSize - 1) ? (unsigned int)0 : index0 + 1;
        auto frac = currentIndexModulator - (float)index0;
        auto* table = wavetable.getReadPointer(*modulatorShape);
        auto value0 = table[index0];
        auto value1 = table[index1];
        auto currentSample = value0 + frac * (value1 - value0);
        if ((currentIndexModulator += tableDeltaModulator) > tableSize)          
            currentIndexModulator -= tableSize;
        return currentSample;
    }

    forcedinline float getNextSampleLFO() noexcept        // LFO
    {
        auto tableSize = wavetable.getNumSamples();
        auto index0 = (unsigned int)currentIndexLFO;
        auto index1 = index0 == (tableSize - 1) ? (unsigned int)0 : index0 + 1;
        auto frac = currentIndexLFO - (float)index0;
        auto* table = wavetable.getReadPointer(0);
        auto value0 = table[index0];
        auto value1 = table[index1];
        auto currentSample = value0 + frac * (value1 - value0);
        if ((currentIndexLFO += tableDeltaLFO) > tableSize)         
            currentIndexLFO -= tableSize;
        return currentSample;
    }

    bool canPlaySound(juce::SynthesiserSound* sound) override
    {
        return dynamic_cast<SineWaveSound*> (sound) != nullptr;
    }
    
    void startNote(int midiNoteNumber, float velocity,
        juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        currentIndex = 0.0f;
        currentSegment = 1;
        currentPosInSegment = 0;
        level = velocity * globalLevel;
        tmpLevel = 0.0f;
        ampIncrease = level / (*voiceA);
        setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber), getSampleRate());
    }

    void stopNote(float /*velocity*/, bool allowTailOff) override
    {
        currentPosInSegment = 0;
        currentSegment = 4;
        ampIncrease = -1.0 * (tmpLevel / (*voiceR));
    }
    
    void pitchWheelMoved(int) override {}
    void controllerMoved(int, int) override {}
    void renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
    {
        if (currentSegment > 0)
        {
            while (--numSamples >= 0)
            {
                tmpLevel += ampIncrease;

                auto currentSampleModulator = getNextSampleModulator() * ((*modId) / 10);
                auto currentSample = (getNextSample() + currentSampleModulator) * tmpLevel;// tu modulowany syg!
                if (*freqLFO != 0)
                    currentSample = currentSample * getNextSampleLFO();

                for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                    outputBuffer.addSample(i, startSample, currentSample);
                ++startSample;
                ++currentPosInSegment;
                // A -> D
                if ((currentSegment == 1) && (currentPosInSegment > juce::roundToInt(*voiceA)))
                {
                    currentPosInSegment = 0;
                    currentSegment++;
                    ampIncrease = -1.0 * (level * (1.0 - (*voiceS))) / (*voiceD);
                }
                // D -> S
                if ((currentSegment == 2) && (currentPosInSegment > juce::roundToInt(*voiceD)))
                {
                    tmpLevel = level * (*voiceS);
                }
                // R -> stop
                if ((currentSegment == 4) && (currentPosInSegment > juce::roundToInt(*voiceR)))     
                {
                    clearCurrentNote();
                    tableDelta = 0.0f;
                    tableDeltaModulator = 0.0f;     // remember to zero all deltas
                    tableDeltaLFO = 0.0f;
                    currentSegment = 0;
                    break;
                }

            }
        }
    }

private:

    
    float *voiceA, * voiceD, * voiceS, * voiceR, *modId, *modRatio, *pitchBend, *freqLFO;

    float tableDeltaModulator = 0.0f, currentIndexModulator = 0.0f;
    float tableDeltaLFO = 0.0f, currentIndexLFO = 0.0f;
    int* waveShape = 0, *modulatorShape = 0;

    int currentPosInSegment = 0;
    int currentSegment = 0;
    float ampIncrease = 0.0f, tmpLevel = 0.0f;
    const juce::AudioSampleBuffer& wavetable;
    float currentIndex = 0.0f, tableDelta = 0.0f, level = 0.0f, globalLevel = 0.0f;

};
//==============================================================================
class SynthAudioSource : public juce::AudioSource
{
public:
    SynthAudioSource(juce::MidiKeyboardState& keyState,
        float* ptrA, float* ptrD, float* ptrS, float* ptrR, float* ptrMId, float* ptrMRt, float * ptrPitchBend, float *ptrLFO, int* ptrWaveShape, int* ptrModulatorShaper) : keyboardState(keyState)
    {
        for (auto i = 0; i < numVoices; ++i)
            synth.addVoice(new SineWaveVoice(signalTable, numVoices, ptrA, ptrD, ptrS, ptrR, ptrMId, ptrMRt, ptrPitchBend, ptrLFO, ptrWaveShape, ptrModulatorShaper));
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
        signalTable.setSize(4, tableSize);
        auto* samples = signalTable.getWritePointer(0);
        auto angleDelta = juce::MathConstants<double>::twoPi / (double)(tableSize - 1);
        auto currentAngle = 0.0;
        for (auto i = 0; i < tableSize; ++i)// sin
        {
            auto sample = std::sin(currentAngle);
            samples[i] = (float)sample;
            currentAngle += angleDelta;
        }

        samples = signalTable.getWritePointer(1);// square
        currentAngle = 0.0;
        for (auto i = 0; i < tableSize; ++i)
        {
            auto sample = std::sin(currentAngle);
            if (sample > 0)
                sample = 1;
            else
                sample = -1;

            samples[i] = (float)sample;
            currentAngle += angleDelta;
        }

        samples = signalTable.getWritePointer(3);// saw
        currentAngle = -1;
        for (auto i = 0; i < tableSize; ++i)
        {
            auto sample = currentAngle;

            samples[i] = (float)sample;
            currentAngle += angleDelta;
        }

        samples = signalTable.getWritePointer(2);// triangle
        currentAngle = 0.0;
        for (auto i = 0; i < tableSize; ++i)
        {
            auto sample = currentAngle;

            if (i < tableSize / 4)
                currentAngle += angleDelta;
            if ((i >= tableSize / 4) && (i < (tableSize / 4) * 3))
                currentAngle -= angleDelta;
            if (i >= (tableSize / 4) * 3)
                currentAngle += angleDelta;

            samples[i] = (float)sample;
            
        }
    }

private:
    juce::MidiKeyboardState& keyboardState;
    juce::Synthesiser synth;
    int tableSize = 128;
    int numVoices = 8;
    juce::AudioSampleBuffer signalTable;
};

