#include "plugin-processor.h"
#include "plugin-editor.h"

NoiseGateProcessor::NoiseGateProcessor()
    : AudioProcessor(BusesProperties()
                     .withInput("Input", juce::AudioChannelSet::stereo(), true)
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
}

NoiseGateProcessor::~NoiseGateProcessor() {}

const juce::String NoiseGateProcessor::getName() const { return JucePlugin_Name; }
bool NoiseGateProcessor::acceptsMidi() const { return false; }
bool NoiseGateProcessor::producesMidi() const { return false; }
bool NoiseGateProcessor::isMidiEffect() const { return false; }
double NoiseGateProcessor::getTailLengthSeconds() const { return 0.0; }

int NoiseGateProcessor::getNumPrograms() { return 1; }
int NoiseGateProcessor::getCurrentProgram() { return 0; }
void NoiseGateProcessor::setCurrentProgram(int index) { juce::ignoreUnused(index); }
const juce::String NoiseGateProcessor::getProgramName(int index) { juce::ignoreUnused(index); return {}; }
void NoiseGateProcessor::changeProgramName(int index, const juce::String& newName) { juce::ignoreUnused(index, newName); }

void NoiseGateProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(sampleRate, samplesPerBlock);
}

void NoiseGateProcessor::releaseResources() {}

void NoiseGateProcessor::processBlock(juce::AudioBuffer<float>& audioBuffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto outputChannelIndex = totalNumInputChannels; outputChannelIndex < totalNumOutputChannels; ++outputChannelIndex)
    {
        audioBuffer.clear(outputChannelIndex, 0, audioBuffer.getNumSamples());
    }

    const float noiseThreshold = 0.1f;

    for (int channelIndex = 0; channelIndex < totalNumInputChannels; ++channelIndex)
    {
        auto* channelData = audioBuffer.getWritePointer(channelIndex);
        
        for (int sampleIndex = 0; sampleIndex < audioBuffer.getNumSamples(); ++sampleIndex)
        {
            if (std::abs(channelData[sampleIndex]) < noiseThreshold)
            {
                channelData[sampleIndex] = 0.0f;
            }
        }
    }
}

bool NoiseGateProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* NoiseGateProcessor::createEditor()
{
    return new NoiseGateEditor(*this);
}

void NoiseGateProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    juce::ignoreUnused(destData);
}

void NoiseGateProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    juce::ignoreUnused(data, sizeInBytes);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NoiseGateProcessor();
}
