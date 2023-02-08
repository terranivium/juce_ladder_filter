/*
  ==============================================================================

    LadderFilterGUI.cpp
    Created: 5 Apr 2022 7:33:16pm
    Author:  Wesley Scott

  ==============================================================================
*/

#include "LadderFilterGUI.h"

LadderFilterGUI::LadderFilterGUI(AudioProcessorValueTreeState& vts)
    : m_APVTS(vts)
{
    // Filter Mode Slider
    m_modeSlider.setSliderStyle(Slider::SliderStyle::IncDecButtons);
    m_modeSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow,
                                       false,
                                       60,
                                       20);
    m_modeSlider.setRange(0.0, static_cast<double>(6), 1.0); // number of modes
    m_modeSlider.setTextBoxIsEditable(false);
    m_modeSlider.addListener(this);
    addAndMakeVisible(m_modeSlider);
    
    m_modeSelectLabel.setText("Filter Mode", NotificationType::dontSendNotification);
    m_modeSelectLabel.attachToComponent(&m_modeSlider, false);
    m_modeSelectLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(m_modeSelectLabel);
    
    m_modeSelectAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(m_APVTS, LadderFilter::Parameters::modeID, m_modeSlider);
    
    // Cutoff Slider
    m_cutSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    m_cutSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow,
                                false,
                                60,
                                20);
    m_cutSlider.setRange(LadderFilter::Parameters::cutMin, LadderFilter::Parameters::cutMax, LadderFilter::Parameters::cutInterval);
    m_cutSlider.setTextBoxIsEditable(true);
    m_cutSlider.addListener(this);
    m_cutSlider.setTextValueSuffix("Hz");
    addAndMakeVisible(m_cutSlider);
    
    m_cutLabel.setText("Cutoff Freq.", NotificationType::dontSendNotification);
    m_cutLabel.attachToComponent(&m_cutSlider, false);
    m_cutLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(m_cutLabel);
    
    m_cutoffAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(m_APVTS, LadderFilter::Parameters::cutID, m_cutSlider);

    // Resonance Slider
    m_resSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    m_resSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow,
                                false,
                                60,
                                20);
    m_resSlider.setRange(LadderFilter::Parameters::resMin, LadderFilter::Parameters::resMax, LadderFilter::Parameters::resInterval);
    m_resSlider.setTextBoxIsEditable(true);
    m_resSlider.addListener(this);
    addAndMakeVisible(m_resSlider);
    
    m_resLabel.setText("Resonance", NotificationType::dontSendNotification);
    m_resLabel.attachToComponent(&m_resSlider, false);
    m_resLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(m_resLabel);
    
    m_resonanceAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(m_APVTS, LadderFilter::Parameters::resID, m_resSlider);

    // Drive Slider
    m_driveSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    m_driveSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow,
                                  false,
                                  60,
                                  20);
    m_driveSlider.setRange(LadderFilter::Parameters::driveMin, LadderFilter::Parameters::driveMax, LadderFilter::Parameters::driveInterval);
    m_driveSlider.setTextBoxIsEditable(true);
    m_driveSlider.addListener(this);
    addAndMakeVisible(m_driveSlider);
    
    m_driveLabel.setText("Drive", NotificationType::dontSendNotification);
    m_driveLabel.attachToComponent(&m_driveSlider, false);
    m_driveLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(m_driveLabel);
    
    m_driveAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(m_APVTS, LadderFilter::Parameters::driveID, m_driveSlider);
}

LadderFilterGUI::~LadderFilterGUI()
{
}

void LadderFilterGUI::resized()
{
    auto bounds = getLocalBounds();
    auto cutArea = bounds.removeFromTop(getHeight() / 4);
    auto cutLabelArea = cutArea.removeFromTop(cutArea.getHeight() * 0.2f);
    m_cutSlider.setBounds(cutArea.reduced(5));
    m_cutLabel.setBounds(cutLabelArea);
    
    auto resArea = bounds.removeFromTop(getHeight() / 4);
    auto resLabelArea = resArea.removeFromTop(resArea.getHeight() * 0.2f);
    m_resSlider.setBounds(resArea.reduced(5));
    m_resLabel.setBounds(resLabelArea);
    
    auto driveArea = bounds.removeFromTop(getHeight() / 4);
    auto driveLabelArea = driveArea.removeFromTop(driveArea.getHeight() * 0.2f);
    m_driveSlider.setBounds(driveArea.reduced(5));
    m_driveLabel.setBounds(driveLabelArea);
    
    auto modeArea = bounds.removeFromTop(getHeight() / 4);
    auto modeLabelArea = modeArea.removeFromTop(modeArea.getHeight() * 0.2f);
    m_modeSlider.setBounds(modeArea.reduced(5));
    m_modeSelectLabel.setBounds(modeLabelArea);
}

void LadderFilterGUI::paint(Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::DialogWindow::backgroundColourId));
}

void LadderFilterGUI::sliderValueChanged(Slider*)
{
}
