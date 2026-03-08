//
// Created by venus on 3/7/26.
//
#include "Oscillator.h"

#include <algorithm>
#include <cmath>
#include <juce_core/juce_core.h>

void Oscillator::prepare (double newSampleRate)
{
    sampleRate = (newSampleRate > 0.0) ? newSampleRate : 44100.0;
    reset();
}

void Oscillator::setFrequency (float newFrequency)
{
    frequency = std::max (0.0f, newFrequency);
}

void Oscillator::setGain (float newGain)
{
    gain = std::max (0.0f, newGain);
}

void Oscillator::reset()
{
    phase = 0.0f;
}

float Oscillator::getNextSineSample()
{
    const float outputSample =
        std::sin (phase * juce::MathConstants<float>::twoPi) * gain;

    phase += frequency / static_cast<float> (sampleRate);
    if (phase >= 1.0f)
        phase -= 1.0f;

    return outputSample;
}

float Oscillator::getNextSawSample()
{
    const float outputSample = (2.0f * phase - 1.0f) * gain;

    phase += frequency / static_cast<float> (sampleRate);
    if (phase >= 1.0f)
        phase -= 1.0f;

    return outputSample;
}