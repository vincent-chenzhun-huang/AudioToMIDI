/*
  ==============================================================================

    ParamPanel.cpp
    Created: 7 Nov 2023 10:30:03am
    Author:  Vincent Huang

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ParamPanel.h"

ParamPanel::ParamPanel(juce::ValueTree& params): parameters(params) {
    // Note Segmentation Slider and Label
    addAndMakeVisible(noteSegSlider);
    noteSegSlider.setRange(0, 1.0, 0.05);
    addAndMakeVisible(noteSegLabel);
    noteSegLabel.setText("Note Segmentation", juce::dontSendNotification);
    noteSegLabel.setJustificationType(juce::Justification::centred);
    noteSegSlider.onValueChange = [this]() {
        parameters.setProperty("noteSeg", noteSegSlider.getValue(), nullptr);
    };
    noteSegSlider.setValue(0.5);
    

    // Model Confidence Threshold Slider and Label
    addAndMakeVisible(modelConfThresholdSlider);
    modelConfThresholdSlider.setRange(0, 1.0, 0.05);
    addAndMakeVisible(modelConfThresholdLabel);
    modelConfThresholdLabel.setText("Model Confidence Threshold", juce::dontSendNotification);
    modelConfThresholdLabel.setJustificationType(juce::Justification::centred);
    modelConfThresholdSlider.onValueChange = [this]() {
        parameters.setProperty("modelConfThreshold", modelConfThresholdSlider.getValue(), nullptr);
    };
    modelConfThresholdSlider.setValue(0.30);
    

    // Minimum Pitch Slider and Label
    addAndMakeVisible(minPitchSlider);
    minPitchSlider.setRange(0, 2000, 10);
    addAndMakeVisible(minPitchLabel);
    minPitchLabel.setText("Minimum Pitch", juce::dontSendNotification);
    minPitchLabel.setJustificationType(juce::Justification::centred);
    minPitchSlider.onValueChange = [this]() {
        parameters.setProperty("minPitch", minPitchSlider.getValue(), nullptr);
    };
    minPitchSlider.setValue(0);

    // Maximum Pitch Slider and Label
    addAndMakeVisible(maxPitchSlider);
    maxPitchSlider.setRange(0, 3000, 10);
    addAndMakeVisible(maxPitchLabel);
    maxPitchLabel.setText("Maximum Pitch", juce::dontSendNotification);
    maxPitchLabel.setJustificationType(juce::Justification::centred);
    maxPitchSlider.onValueChange = [this]() {
        parameters.setProperty("maxPitch", maxPitchSlider.getValue(), nullptr);
    };
    maxPitchSlider.setValue(0);

    // Minimum Note Length Slider and Label
    addAndMakeVisible(minNoteLenSlider);
    minNoteLenSlider.setRange(0, 80, 1);
    addAndMakeVisible(minNoteLenLabel);
    minNoteLenLabel.setText("Minimum Note Length", juce::dontSendNotification);
    minNoteLenLabel.setJustificationType(juce::Justification::centred);
    minNoteLenSlider.onValueChange = [this]() {
        parameters.setProperty("minNoteLen", minNoteLenSlider.getValue(), nullptr);
    };
    minNoteLenSlider.setValue(58);
    

    // MIDI File Tempo Slider and Label
    addAndMakeVisible(midiTempoSlider);
    midiTempoSlider.setRange(0, 226, 1.0);
    addAndMakeVisible(midiTempoLabel);
    midiTempoLabel.setText("MIDI File Tempo", juce::dontSendNotification);
    midiTempoLabel.setJustificationType(juce::Justification::centred);
    midiTempoSlider.onValueChange = [this]() {
        parameters.setProperty("midiTempo", midiTempoSlider.getValue(), nullptr);
    };
    midiTempoSlider.setValue(120);

    // Optionally add your checkbox for enabling denoising here
    addAndMakeVisible(enableDenoising);
    enableDenoising.setButtonText("Enable Denoising");
    
    
    addAndMakeVisible(enableInstrumentClassifier);
    enableInstrumentClassifier.setButtonText("Enable Instrument Classifier");
}


ParamPanel::~ParamPanel(){}

void ParamPanel::paint (juce::Graphics& g) {
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

void ParamPanel::resized() {
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

