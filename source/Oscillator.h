//
// Created by venus on 3/7/26.
//

#pragma once

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
    float gain = 0.15f;
};