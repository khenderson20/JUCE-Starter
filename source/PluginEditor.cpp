#include "PluginEditor.h"

#include <cmath>

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    addAndMakeVisible (inspectButton);

    inspectButton.onClick = [&]
    {
        if (! inspector)
        {
            inspector = std::make_unique<melatonin::Inspector> (*this);
            inspector->onClose = [this]() { inspector.reset(); };
        }

        inspector->setVisible (true);
    };

    frequencyLabel.setText ("Frequency", juce::dontSendNotification);
    frequencyLabel.attachToComponent (&frequencySlider, false);
    addAndMakeVisible (frequencyLabel);

    frequencySlider.setSliderStyle (juce::Slider::LinearHorizontal);
    frequencySlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 80, 24);
    frequencySlider.setRange (20.0, 2000.0, 0.01);
    frequencySlider.setSkewFactorFromMidPoint (220.0);
    frequencySlider.setValue (processorRef.getFrequency());
    frequencySlider.onValueChange = [this]
    {
        processorRef.setFrequency (static_cast<float> (frequencySlider.getValue()));
        repaint();
    };
    addAndMakeVisible (frequencySlider);

    gainLabel.setText ("Gain", juce::dontSendNotification);
    gainLabel.attachToComponent (&gainSlider, false);
    addAndMakeVisible (gainLabel);

    gainSlider.setSliderStyle (juce::Slider::LinearHorizontal);
    gainSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 80, 24);
    gainSlider.setRange (0.0, 1.0, 0.001);
    gainSlider.setValue (processorRef.getGain());
    gainSlider.onValueChange = [this]
    {
        processorRef.setGain (static_cast<float> (gainSlider.getValue()));
        repaint();
    };
    addAndMakeVisible (gainSlider);

    waveformLabel.setText ("Waveform", juce::dontSendNotification);
    addAndMakeVisible (waveformLabel);

    waveformBox.addItem ("Sine", 1);
    waveformBox.addItem ("Saw", 2);
    waveformBox.setSelectedId (processorRef.getWaveform() == PluginProcessor::Waveform::sine ? 1 : 2);
    waveformBox.onChange = [this]
    {
        const auto selectedWaveform = waveformBox.getSelectedId() == 1
            ? PluginProcessor::Waveform::sine
            : PluginProcessor::Waveform::saw;

        processorRef.setWaveform (selectedWaveform);
        repaint();
    };
    addAndMakeVisible (waveformBox);

    setSize (520, 360);
}

PluginEditor::~PluginEditor()
{
}

void PluginEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff1a1a1a));

    auto area = getLocalBounds().reduced (16);

    g.setColour (juce::Colours::white);
    g.setFont (20.0f);
    g.drawText ("Oscillator", area.removeFromTop (32), juce::Justification::centredLeft, false);

    area.removeFromTop (96);

    auto waveformArea = area.removeFromTop (170);
    drawWaveformPreview (g, waveformArea);

    g.setColour (juce::Colours::lightgrey);
    g.setFont (14.0f);
    g.drawFittedText (
        "Preview only: shows the selected waveform shape, not a live oscilloscope capture.",
        area.removeFromTop (28),
        juce::Justification::centredLeft,
        1);
}

void PluginEditor::resized()
{
    auto area = getLocalBounds().reduced (16);

    area.removeFromTop (40);

    auto controlsArea = area.removeFromTop (96);
    auto row1 = controlsArea.removeFromTop (32);
    auto row2 = controlsArea.removeFromTop (32);
    auto row3 = controlsArea.removeFromTop (32);

    row1.removeFromLeft (100);
    frequencySlider.setBounds (row1);

    row2.removeFromLeft (100);
    gainSlider.setBounds (row2);

    waveformLabel.setBounds (row3.removeFromLeft (100));
    waveformBox.setBounds (row3.removeFromLeft (180));

    inspectButton.setBounds (getLocalBounds().reduced (16).removeFromBottom (40).removeFromRight (140));
}

void PluginEditor::drawWaveformPreview (juce::Graphics& g, juce::Rectangle<int> area)
{
    g.setColour (juce::Colour (0xff2a2a2a));
    g.fillRoundedRectangle (area.toFloat(), 8.0f);

    g.setColour (juce::Colour (0xff404040));
    g.drawRoundedRectangle (area.toFloat(), 8.0f, 1.0f);

    const auto left = static_cast<float> (area.getX());
    const auto right = static_cast<float> (area.getRight());
    const auto top = static_cast<float> (area.getY());
    const auto bottom = static_cast<float> (area.getBottom());
    const auto width = static_cast<float> (area.getWidth());
    const auto centreY = static_cast<float> (area.getCentreY());
    const auto amplitude = static_cast<float> (area.getHeight()) * 0.35f * processorRef.getGain();

    g.setColour (juce::Colour (0xff555555));
    g.drawHorizontalLine (area.getCentreY(), left, right);

    juce::Path path;

    for (int x = 0; x < area.getWidth(); ++x)
    {
        const float phase = static_cast<float> (x) / width;
        float sample = 0.0f;

        switch (processorRef.getWaveform())
        {
            case PluginProcessor::Waveform::sine:
                sample = std::sin (phase * juce::MathConstants<float>::twoPi);
                break;

            case PluginProcessor::Waveform::saw:
                sample = 2.0f * phase - 1.0f;
                break;
        }

        const float y = centreY - (sample * amplitude);
        const float drawX = static_cast<float> (area.getX() + x);

        if (x == 0)
            path.startNewSubPath (drawX, y);
        else
            path.lineTo (drawX, y);
    }

    g.setColour (juce::Colours::cyan);
    g.strokePath (path, juce::PathStrokeType (2.0f));

    g.setColour (juce::Colours::lightgreen);
    g.setFont (13.0f);
    g.drawText (
        "Freq: " + juce::String (processorRef.getFrequency(), 1) + " Hz    Gain: " + juce::String (processorRef.getGain(), 2),
        area.reduced (10).removeFromBottom (24),
        juce::Justification::centredLeft,
        false);
}