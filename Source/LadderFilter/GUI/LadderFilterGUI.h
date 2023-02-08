/*
  ==============================================================================

    LadderFilterGUI.h
    Created: 5 Apr 2022 7:33:16pm
    Author:  Wesley Scott

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../DSP/LadderFilter.h"

class LadderFilterGUI  : public Component, public Slider::Listener, public LookAndFeel_V4
{
public:
    LadderFilterGUI(AudioProcessorValueTreeState&);

    ~LadderFilterGUI() override;

    void paint (Graphics&) override;

    void resized() override;

    void sliderValueChanged(Slider*) override;

private:

    AudioProcessorValueTreeState& m_APVTS;
    
    Slider m_cutSlider, m_resSlider, m_driveSlider, m_modeSlider;

    Label m_cutLabel, m_resLabel, m_driveLabel, m_modeSelectLabel;

    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> m_cutoffAttachment, m_resonanceAttachment, m_driveAttachment, m_modeSelectAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LadderFilterGUI)
};
