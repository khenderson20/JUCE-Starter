#pragma once

#include "PluginProcessor.h"
#include "BinaryData.h"
#include "melatonin_inspector/melatonin_inspector.h"

//==============================================================================
class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor (PluginProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void setupSlider (juce::Slider& slider, juce::Label& label,
        const juce::String& name, double min, double max,
        double step, double initialValue,
        std::function<void()> onChange,
        double skewMidPoint = 0.0);

    void drawWaveformPreview (juce::Graphics& g, juce::Rectangle<int> area) const;
    static void drawWaveformBackground (juce::Graphics& g, juce::Rectangle<int> area);
    juce::Path buildWaveformPath (juce::Rectangle<int> area, float amplitude) const;
    void drawWaveformInfoText (juce::Graphics& g, juce::Rectangle<int> area) const;

    PluginProcessor& processorRef;
    std::unique_ptr<melatonin::Inspector> inspector;
    juce::TextButton inspectButton { "Inspect the UI" };

    juce::Slider frequencySlider;
    juce::Slider gainSlider;
    juce::Label frequencyLabel;
    juce::Label gainLabel;
    juce::ComboBox waveformBox;
    juce::Label waveformLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};