#include "plugin-processor.h"
#include "plugin-editor.h"

NoiseGateProcessor::NoiseGateProcessor()
    : AudioProcessor(BusesProperties()
                     .withInput("Input", juce::AudioChannelSet::stereo(), true)
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
{
    thresholdParameter = treeState.getRawParameterValue("threshold");
}

NoiseGateProcessor::~NoiseGateProcessor() {}

juce::AudioProcessorValueTreeState::ParameterLayout NoiseGateProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;

    auto thresholdParam = std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("threshold", 1),
        "Threshold",
        juce::NormalisableRange<float>(-60.0f, 0.0f, 0.1f),
        -60.0f
    );

    parameters.push_back(std::move(thresholdParam));
    return { parameters.begin(), parameters.end() };
}

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
    juce::ignoreUnused(samplesPerBlock);
    smoothedGain.reset(sampleRate, 0.05);
    smoothedGain.setCurrentAndTargetValue(1.0f);
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

    if (thresholdParameter == nullptr || totalNumInputChannels == 0)
        return;

    const float currentThresholdDecibels = thresholdParameter->load();
    const float currentThresholdLinear = juce::Decibels::decibelsToGain(currentThresholdDecibels);
    const int numSamples = audioBuffer.getNumSamples();

    for (int sampleIndex = 0; sampleIndex < numSamples; ++sampleIndex)
    {
        float maximumAbsSampleValue = 0.0f;
        
        for (int channelIndex = 0; channelIndex < totalNumInputChannels; ++channelIndex)
        {
            float absoluteSample = std::abs(audioBuffer.getReadPointer(channelIndex)[sampleIndex]);
            if (absoluteSample > maximumAbsSampleValue)
            {
                maximumAbsSampleValue = absoluteSample;
            }
        }

        const float targetGain = (maximumAbsSampleValue < currentThresholdLinear) ? 0.0f : 1.0f;
        smoothedGain.setTargetValue(targetGain);
        const float currentGainMultiplier = smoothedGain.getNextValue();

        for (int channelIndex = 0; channelIndex < totalNumInputChannels; ++channelIndex)
        {
            auto* channelData = audioBuffer.getWritePointer(channelIndex);
            channelData[sampleIndex] *= currentGainMultiplier;
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
    auto state = treeState.copyState();
    std::unique_ptr<juce::XmlElement> xmlElement(state.createXml());
    if (xmlElement != nullptr)
    {
        copyXmlToBinary(*xmlElement, destData);
    }
}

void NoiseGateProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState != nullptr)
    {
        if (xmlState->hasTagName(treeState.state.getType()))
        {
            treeState.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NoiseGateProcessor();
}
