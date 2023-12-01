/*
  ==============================================================================

    MIDIOutput.cpp
    Created: 8 Nov 2023 10:32:42pm
    Author:  Vincent Huang

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MIDIOutput.h"

//==============================================================================
MIDIOutput::MIDIOutput(juce::ValueTree& params): parameters(params) {
    addAndMakeVisible(&description);
}

MIDIOutput::~MIDIOutput() {}

void MIDIOutput::paint (juce::Graphics& g) {
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    
    auto localBounds = getLocalBounds();
    auto titleArea = localBounds.withHeight(4 * getHeight() / 5);
    
    g.setColour (juce::Colours::white);

    description.setText("Output will show here when it is ready.", juce::dontSendNotification);
}

void MIDIOutput::mouseDrag(const juce::MouseEvent &event) {
    
//    juce::File dummyFile("/Users/vincenthuang/Development/audio_to_midi/Source/test_midi.mid");
    
    if (!parameters.isValid() || !parameters.hasProperty("outputFilePath")) {
        juce::Logger::writeToLog("The output file cannot be found, cannot drag");
        return;
    }
//    juce::Logger::writeToLog("The output file path is: " + parameters.getProperty("outputFile"));
    juce::File outputFile = juce::File(parameters.getProperty("outputFilePath"));
    
    juce::FileInputStream dummyStream(outputFile);
    juce::MidiFile midiFile;
    midiFile.readFrom(dummyStream);
    
    std::cout << "Number of tracks in this file: " << midiFile.getNumTracks() << std::endl;
    
    
    juce::File outf = juce::File::getSpecialLocation(juce::File::SpecialLocationType::tempDirectory).getChildFile("tempFile.mid");
    juce::TemporaryFile tempf (outf);
    
    if (auto p_os = std::unique_ptr<juce::FileOutputStream> (tempf.getFile().createOutputStream())) {
        midiFile.writeTo(*p_os, 1);
        bool succeed = tempf.overwriteTargetFileWithTemporary();
        std::cout << succeed << std::endl;
        juce::Logger::writeToLog("Write succeed");
        juce::Logger::writeToLog(outf.getFullPathName());
        performExternalDragDropOfFiles({ outf.getFullPathName() }, false, this, [outf] (void) {
//            outf.deleteFile();
        });
    }
}

void MIDIOutput::resized() {
//    description.setBounds(getLocalBounds());
    auto area = getLocalBounds().reduced(10);
    auto height = area.getHeight() / 3;
    auto width = area.getWidth() / 3;
    description.setBounds(area.reduced(0, height));
    description.setJustificationType(juce::Justification::centred);
}

void MIDIOutput::reset() {
    description.setText("Output will show here when it is ready.", juce::dontSendNotification);
}

void MIDIOutput::onConvert() {
    description.setText("Drag this area and drop into your favorite DAW", juce::dontSendNotification);
}

