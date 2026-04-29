#include "plugin-processor.h"
#include "plugin-editor.h"

NoiseGateEditor::NoiseGateEditor(NoiseGateProcessor& processor)
    : AudioProcessorEditor(&processor), audioProcessor(processor)
{
    setSize(400, 300);

    thresholdSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    thresholdSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(thresholdSlider);

    thresholdAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.treeState, "threshold", thresholdSlider);

    thresholdLabel.setText("Threshold (dB)", juce::dontSendNotification);
    thresholdLabel.setJustificationType(juce::Justification::centred);
    thresholdLabel.attachToComponent(&thresholdSlider, false);
    addAndMakeVisible(thresholdLabel);
}

NoiseGateEditor::~NoiseGateEditor() {}

void NoiseGateEditor::paint(juce::Graphics& graphics)
{
    graphics.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void NoiseGateEditor::resized()
{
    auto componentBounds = getLocalBounds();
    auto sliderBounds = componentBounds.reduced(50).withSizeKeepingCentre(150, 150);
    thresholdSlider.setBounds(sliderBounds);
}
