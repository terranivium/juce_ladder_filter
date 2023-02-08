/*
  ==============================================================================

    LadderFilter.h
    Created: 4 Apr 2022 11:32:03am
    Author:  Wesley Scott

  ==============================================================================
*/

#pragma once

namespace LadderFilter
{
    namespace Parameters
    {
        // Mode
        static const juce::String modeID = "mode";
    
        // Filter Cutoff
        static const juce::String cutID = "cutoff";
        static const float cutMin = 30.0f;
        static const float cutMax = 20000.0f;
        static const float cutSkew = 0.5f;
        static const float cutInterval = 0.01f;

        // Drive
        static const juce::String driveID = "drive";
        static const float driveMin = 1.0f;
        static const float driveMax = 10.0f;
        static const float driveSkew = 1.0f;
        static const float driveInterval = 0.01f;
    
        // Resonance
        static const juce::String resID = "resonance";
        static const float resMin = 0.0f;
        static const float resMax = 1.0f;
        static const float resSkew = 1.0f;
        static const float resInterval = 0.01f;
    }
}
