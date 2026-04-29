#include "plugin-processor.h"
#include "plugin-editor.h"

NoiseGateEditor::NoiseGateEditor(NoiseGateProcessor& processor)
    : AudioProcessorEditor(&processor), audioProcessor(processor)
{
    setSize(400, 300);
}

NoiseGateEditor::~NoiseGateEditor() {}

void NoiseGateEditor::paint(juce::Graphics& graphics)
{
    graphics.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    graphics.setColour(juce::Colours::white);
    graphics.setFont(15.0f);
    graphics.drawFittedText("Noise Gate VST3", getLocalBounds(), juce::Justification::centred, 1);
}

void NoiseGateEditor::resized() {}
