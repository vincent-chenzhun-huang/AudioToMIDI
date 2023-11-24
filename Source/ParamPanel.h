/*
  ==============================================================================

    ParamPanel.h
    Created: 7 Nov 2023 10:30:03am
    Author:  Vincent Huang

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

/*
*/
class ParamPanel  : public juce::Component {
public:
    ParamPanel(juce::ValueTree& params);
    ~ParamPanel() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    /*
     6 items with height / 6
     - note segmentation
     - model confidence threshold
     - minimum pitch
     - maximum pitch
     - minimum note length
     - midi file tempo
     
     - enable denoising checkbox
     - enable classifier checkbox
     */
    juce::ValueTree& parameters;
    
    juce::Slider noteSegSlider;
    juce::Label noteSegLabel;
    
    
    juce::Slider modelConfThresholdSlider;
    juce::Label modelConfThresholdLabel;
    
    juce::Slider minPitchSlider;
    juce::Label minPitchLabel;
    
    juce::Slider maxPitchSlider;
    juce::Label maxPitchLabel;
    
    juce::Slider minNoteLenSlider;
    juce::Label minNoteLenLabel;
    
    juce::Slider midiTempoSlider;
    juce::Label midiTempoLabel;
    
    juce::ToggleButton enableDenoising;
    juce::ToggleButton enableInstrumentClassifier;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParamPanel)
};
