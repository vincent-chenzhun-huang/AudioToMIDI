/*
  ==============================================================================

    ParamPanel.cpp
    Created: 7 Nov 2023 10:30:03am
    Author:  Vincent Huang

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ParamPanel.h"

//==============================================================================
ParamPanel::ParamPanel()
{
    // Note Segmentation Slider and Label
    addAndMakeVisible(noteSegSlider);
    noteSegSlider.setRange(0, 100, 1.0);
    addAndMakeVisible(noteSegLabel);
    noteSegLabel.setText("Note Segmentation", juce::dontSendNotification);
    noteSegLabel.setJustificationType(juce::Justification::centred);

    // Model Confidence Threshold Slider and Label
    addAndMakeVisible(modelConfThresholdSlider);
    modelConfThresholdSlider.setRange(0, 100, 1.0);
    addAndMakeVisible(modelConfThresholdLabel);
    modelConfThresholdLabel.setText("Model Confidence Threshold", juce::dontSendNotification);
    modelConfThresholdLabel.setJustificationType(juce::Justification::centred);

    // Minimum Pitch Slider and Label
    addAndMakeVisible(minPitchSlider);
    minPitchSlider.setRange(0, 100, 1.0);
    addAndMakeVisible(minPitchLabel);
    minPitchLabel.setText("Minimum Pitch", juce::dontSendNotification);
    minPitchLabel.setJustificationType(juce::Justification::centred);

    // Maximum Pitch Slider and Label
    addAndMakeVisible(maxPitchSlider);
    maxPitchSlider.setRange(0, 100, 1.0);
    addAndMakeVisible(maxPitchLabel);
    maxPitchLabel.setText("Maximum Pitch", juce::dontSendNotification);
    maxPitchLabel.setJustificationType(juce::Justification::centred);

    // Minimum Note Length Slider and Label
    addAndMakeVisible(minNoteLenSlider);
    minNoteLenSlider.setRange(0, 100, 1.0);
    addAndMakeVisible(minNoteLenLabel);
    minNoteLenLabel.setText("Minimum Note Length", juce::dontSendNotification);
    minNoteLenLabel.setJustificationType(juce::Justification::centred);

    // MIDI File Tempo Slider and Label
    addAndMakeVisible(midiTempoSlider);
    midiTempoSlider.setRange(0, 100, 1.0);
    addAndMakeVisible(midiTempoLabel);
    midiTempoLabel.setText("MIDI File Tempo", juce::dontSendNotification);
    midiTempoLabel.setJustificationType(juce::Justification::centred);

    // Optionally add your checkbox for enabling denoising here
    addAndMakeVisible(enableDenoising);
    enableDenoising.setButtonText("Enable Denoising");
    
    addAndMakeVisible(enableInstrumentClassifier);
    enableInstrumentClassifier.setButtonText("Enable Instrument Classifier");
}


ParamPanel::~ParamPanel()
{

}

void ParamPanel::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
}

void ParamPanel::resized()
{
    int NUM = 13;
    auto area = getLocalBounds().reduced(10);
    auto unitHeight = area.getHeight() / NUM;

    // Set bounds for each slider and its corresponding label
    noteSegLabel.setBounds(area.removeFromTop(unitHeight));
    noteSegSlider.setBounds(area.removeFromTop(unitHeight));

    modelConfThresholdLabel.setBounds(area.removeFromTop(unitHeight));
    modelConfThresholdSlider.setBounds(area.removeFromTop(unitHeight));

    minPitchLabel.setBounds(area.removeFromTop(unitHeight));
    minPitchSlider.setBounds(area.removeFromTop(unitHeight));

    maxPitchLabel.setBounds(area.removeFromTop(unitHeight));
    maxPitchSlider.setBounds(area.removeFromTop(unitHeight));

    minNoteLenLabel.setBounds(area.removeFromTop(unitHeight));
    minNoteLenSlider.setBounds(area.removeFromTop(unitHeight));

    midiTempoLabel.setBounds(area.removeFromTop(unitHeight));
    midiTempoSlider.setBounds(area.removeFromTop(unitHeight));

    auto row = area.removeFromTop(unitHeight);
    enableDenoising.setBounds(row.removeFromLeft(0.5 * getWidth()));
    enableInstrumentClassifier.setBounds(row.removeFromLeft(0.5 * getWidth()));
}

