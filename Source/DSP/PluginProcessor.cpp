/*
  ==============================================================================

    PluginProcessor.cpp
    Created: 5 Apr 2022 6:26:06pm
    Author:  Wesley Scott

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "../GUI/PluginEditor.h"

//==============================================================================
WsLdrAudioProcessor::WsLdrAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
    #if ! JucePlugin_IsMidiEffect
    #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
    #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
    #endif
                       ),
        m_APVTS
        (
            *this,
            nullptr,
            Identifier("FilterParams"),
            {
                std::make_unique<AudioParameterChoice>
                (
                    LadderFilter::Parameters::modeID,
                    LadderFilter::Parameters::modeID,
                    StringArray("LPF12",
                                "LPF24",
                                "HPF12",
                                "HPF24",
                                "BPF12",
                                "BPF24"
                    ),
                    1 // default to LPF24
                ),
                std::make_unique<AudioParameterFloat>
                (
                    LadderFilter::Parameters::cutID,
                    LadderFilter::Parameters::cutID,
                    NormalisableRange<float>
                    (
                        LadderFilter::Parameters::cutMin,
                        LadderFilter::Parameters::cutMax,
                        LadderFilter::Parameters::cutInterval,
                        LadderFilter::Parameters::cutSkew,
                        false
                    ),
                    7000.0f
                ),
                std::make_unique<AudioParameterFloat>
                (
                    LadderFilter::Parameters::resID,
                    LadderFilter::Parameters::resID,
                    NormalisableRange<float>
                    (
                        LadderFilter::Parameters::resMin,
                        LadderFilter::Parameters::resMax,
                        LadderFilter::Parameters::resInterval,
                        LadderFilter::Parameters::resSkew,
                        false
                    ),
                    0.64f
                ),
                std::make_unique<AudioParameterFloat>
                (
                    LadderFilter::Parameters::driveID,
                    LadderFilter::Parameters::driveID,
                    NormalisableRange<float>
                    (
                        LadderFilter::Parameters::driveMin,
                        LadderFilter::Parameters::driveMax,
                        LadderFilter::Parameters::driveInterval,
                        LadderFilter::Parameters::driveSkew,
                        false
                    ),
                    LadderFilter::Parameters::driveMax
                )
            }
         )
#endif
{
    addParameterListeners();
}

WsLdrAudioProcessor::~WsLdrAudioProcessor()
{
}

//==============================================================================
const juce::String WsLdrAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WsLdrAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WsLdrAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WsLdrAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WsLdrAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WsLdrAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WsLdrAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WsLdrAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String WsLdrAudioProcessor::getProgramName (int index)
{
    return {};
}

void WsLdrAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void WsLdrAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    m_filter.reset();
    m_filter.prepare(spec);
    m_filter.setEnabled(true);
    
    // Match default param values
    m_filter.setCutoffFrequencyHz(7000.0f);
    m_filter.setResonance(0.64f);
    m_filter.setDrive(LadderFilter::Parameters::driveMax);
    m_filter.setMode(static_cast<dsp::LadderFilterMode>(1));
}

void WsLdrAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WsLdrAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void WsLdrAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
//    for (int channel = 0; channel < totalNumInputChannels; ++channel)
//    {
//        auto* channelData = buffer.getWritePointer (channel);
//
//        // ..do something to the data...
//    }
    dsp::AudioBlock<float> block(buffer);
    m_filter.process(dsp::ProcessContextReplacing<float>(block));
}

void WsLdrAudioProcessor::parameterChanged(const String& parameterID, float newValue)
{
    if (parameterID.equalsIgnoreCase(LadderFilter::Parameters::cutID))
    {
        m_filter.setCutoffFrequencyHz(newValue);
    }
    
    if (parameterID.equalsIgnoreCase(LadderFilter::Parameters::resID))
    {
        m_filter.setResonance(newValue);
    }
    
    if (parameterID.equalsIgnoreCase(LadderFilter::Parameters::driveID))
    {
        m_filter.setDrive(newValue);
    }

    if (parameterID.equalsIgnoreCase(LadderFilter::Parameters::modeID))
    {
        switch ((int)newValue)
        {
            case 0:
                m_filter.setMode(dsp::LadderFilter<float>::Mode::LPF12);
                break;
            case 1:
                m_filter.setMode(dsp::LadderFilter<float>::Mode::LPF24);
                break;
            case 2:
                m_filter.setMode(dsp::LadderFilter<float>::Mode::HPF12);
                break;
            case 3:
                m_filter.setMode(dsp::LadderFilter<float>::Mode::HPF24);
                break;
            case 4:
                m_filter.setMode(dsp::LadderFilter<float>::Mode::BPF12);
                break;
            case 5:
                m_filter.setMode(dsp::LadderFilter<float>::Mode::BPF24);
                break;
        }
    }
}

void WsLdrAudioProcessor::addParameterListeners()
{
    for (int i = 0; i < m_APVTSID.size(); i++)
    {
        m_APVTS.addParameterListener(m_APVTSID[i], this);
    }
}

//==============================================================================
bool WsLdrAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* WsLdrAudioProcessor::createEditor()
{
    return new WsLdrAudioProcessorEditor (*this, m_APVTS);
}

//==============================================================================
void WsLdrAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
     
}

void WsLdrAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WsLdrAudioProcessor();
}
