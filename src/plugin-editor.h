#pragma once
#include <JuceHeader.h>
#include "plugin-processor.h"

class NoiseGateEditor : public juce::AudioProcessorEditor
{
public:
    explicit NoiseGateEditor(NoiseGateProcessor& processor);
    ~NoiseGateEditor() override;

    void paint(juce::Graphics& graphics) override;
    void resized() override;

private:
    NoiseGateProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NoiseGateEditor)
};
