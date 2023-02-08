/*
  ==============================================================================

    PluginProcessor.h
    Created: 5 Apr 2022 6:26:06pm
    Author:  Wesley Scott

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../LadderFilter/DSP/LadderFilter.h"

//==============================================================================
/**
*/
class WsLdrAudioProcessor  : public juce::AudioProcessor, public AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    WsLdrAudioProcessor();
    ~WsLdrAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    void parameterChanged(const juce::String& parameterID, float newValue) override;
    
    void addParameterListeners();

private:
    //==============================================================================
    juce::dsp::LadderFilter<float> m_filter;
    AudioProcessorValueTreeState m_APVTS;
    
    const StringArray m_APVTSID =
    {
        LadderFilter::Parameters::cutID,
        LadderFilter::Parameters::resID,
        LadderFilter::Parameters::driveID,
        LadderFilter::Parameters::modeID
    };
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WsLdrAudioProcessor)
};
