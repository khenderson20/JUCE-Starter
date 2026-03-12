//
// Created by venus on 3/7/26.
//

#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

class Oscillator
{
public:
    void prepare (double newSampleRate);
    void setFrequency (float newFrequency);
    void setGain (float newGain);
    void reset();
    float getNextSineSample();
    float getNextSawSample();

private:
    double sampleRate = 44100.0;
    float phase = 0.0f;
    float frequency = 220.0f;
    juce::SmoothedValue<float> gain { 0.05f };
};