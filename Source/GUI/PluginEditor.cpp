/*
  ==============================================================================

    PluginEditor.cpp
    Created: 5 Apr 2022 6:26:06pm
    Author:  Wesley Scott

  ==============================================================================
*/

#include "../DSP/PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WsLdrAudioProcessorEditor::WsLdrAudioProcessorEditor (WsLdrAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), m_audioProcessor (p), m_APVTS(vts), m_filterGUI(vts)
{
    setSize (80, 440);
    setResizable(true, true);
    
    setLookAndFeel(&m_filterGUI);
    if (auto laf = dynamic_cast<LookAndFeel_V4*> (&getLookAndFeel()))
    {
        laf->setColourScheme(LookAndFeel_V4::getMidnightColourScheme());
    }
    
    addAndMakeVisible(m_filterGUI);
}

WsLdrAudioProcessorEditor::~WsLdrAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void WsLdrAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::DialogWindow::backgroundColourId));
}

void WsLdrAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    auto bounds = getLocalBounds();
    m_filterGUI.setBounds(bounds.reduced(5));
}
