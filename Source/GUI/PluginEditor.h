/*
  ==============================================================================

    PluginEditor.h
    Created: 5 Apr 2022 6:26:06pm
    Author:  Wesley Scott

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../DSP/PluginProcessor.h"
#include "../LadderFilter/GUI/LadderFilterGUI.h"

//==============================================================================
/**
*/
class WsLdrAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    WsLdrAudioProcessorEditor (WsLdrAudioProcessor&, AudioProcessorValueTreeState&);
    ~WsLdrAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    
    WsLdrAudioProcessor& m_audioProcessor;
    AudioProcessorValueTreeState& m_APVTS;
    LadderFilterGUI m_filterGUI;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WsLdrAudioProcessorEditor)
};
