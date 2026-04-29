#pragma once
#include <JuceHeader.h>

class NoiseGateProcessor : public juce::AudioProcessor
{
public:
    NoiseGateProcessor();
    ~NoiseGateProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>& audioBuffer, juce::MidiBuffer& midiMessages) override;
    
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState treeState;

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    std::atomic<float>* thresholdParameter = nullptr;
    juce::SmoothedValue<float> smoothedGain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NoiseGateProcessor)
};
