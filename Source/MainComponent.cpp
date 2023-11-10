#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() {
    setSize (1000, 800);
    addAndMakeVisible(fileWindowComponent);
    addAndMakeVisible(paramPanel);
    addAndMakeVisible(midiOutput);
}

MainComponent::~MainComponent() {}

//==============================================================================
void MainComponent::paint (juce::Graphics& g) {
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized() {
    fileWindowComponent.setBounds(0, 0, getWidth() / 2, getHeight() / 3);
    paramPanel.setBounds(0, getHeight() / 3, getWidth() / 2, getHeight() * 2 / 3);
    midiOutput.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight());
}
