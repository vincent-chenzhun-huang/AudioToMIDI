#include "FileWindow.h"

FileWindow::FileWindow(): state(Stopped) {
    setName("File Window");
    setOpaque(true);
    setInterceptsMouseClicks(true, true);
    
    addAndMakeVisible(&playButton);
    playButton.setButtonText("Play Audio");
    playButton.onClick = [this] {
        playButtonClicked();
    };
    playButton.setEnabled(false);
    
    addAndMakeVisible(&stopButton);
    stopButton.setButtonText("Stop Audio");
    stopButton.onClick = [this] {
        stopButtonClicked();
    };
    stopButton.setEnabled(false);
    
    addAndMakeVisible(&convertButton);
    convertButton.setButtonText("Convert to MIDI");
    convertButton.setEnabled(false);
    
    formatManager.registerBasicFormats(); // register the basic audio file formats, wav, aiff files
    transportSource.addChangeListener(this); // when there is a change in the transportSource, we trigger the changeListenerCallback function
    setAudioChannels(0, 2);
}

FileWindow::~FileWindow() {
    shutdownAudio();
}

void FileWindow::playButtonClicked() {
    changeState(Starting);
}

void FileWindow::stopButtonClicked() {
    changeState(Stopping);
}

void FileWindow::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void FileWindow::getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) {
    if (readerSource.get() == nullptr) {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    
    transportSource.getNextAudioBlock(bufferToFill);
}

void FileWindow::releaseResources() {
    transportSource.releaseResources();
}

void FileWindow::changeListenerCallback(juce::ChangeBroadcaster *source) {
    if (source == &transportSource) {
        if (transportSource.isPlaying()) changeState(Playing);
        else changeState(Stopped);
    }
}

bool FileWindow::isInterestedInFileDrag(const juce::StringArray& files) {
    auto droppedArea = getLocalBounds().withTrimmedBottom(getHeight() / 5);
    return droppedArea.contains(getMouseXYRelative());
}

void FileWindow::filesDropped(const juce::StringArray& filenames, int x, int y) {
    if (filenames.size() != 1) {
        juce::Logger::writeToLog("Invalid format of file dropped, skipping");
        return;
    }
    droppedFile = juce::File(filenames[0]); // store the file path
    juce::Logger::writeToLog("File Dropped: " + droppedFile.getFullPathName());

    auto* reader = formatManager.createReaderFor(droppedFile);
    juce::Logger::writeToLog("Reader initialized");
    
    if (reader != nullptr) {
        auto newSource = std::make_unique<juce::AudioFormatReaderSource> (reader, true);
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        juce::Logger::writeToLog("Activate playback button");
        playButton.setEnabled(true);
        readerSource.reset(newSource.release());  // assign the ownership of the resource to the class field so that the memory doesn't get released when this function finishes running
    }
}

void FileWindow::changeState(TransportState newState) {
    if (state != newState) {
        state = newState;
        
        switch (state) {
            case Stopped:
                stopButton.setEnabled(false);
                playButton.setEnabled(true);
                transportSource.setPosition(0.0);  // reset the position
                break;
                
            case Starting:
                playButton.setEnabled(false);
                transportSource.start();  // start playing
                break;
                
            case Playing:
                stopButton.setEnabled(true);
                break;
            
            case Stopping:
                transportSource.stop();
                break;
        }
    }
}

void FileWindow::fileDragMove(const juce::StringArray &files, int x, int y) {}

void FileWindow::fileDragExit(const juce::StringArray &files) {}

void FileWindow::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::white);
    
    auto fileDropArea = getLocalBounds().removeFromTop(4.0 / 5.0 * getHeight());
    g.setColour(juce::Colours::grey);
    g.fillRect(fileDropArea);
    g.setOpacity(1.0);
    g.drawRect(getLocalBounds(), 1);
    g.setOpacity(1);
    g.setFont (juce::Font (25.0));
    g.setColour(juce::Colours::white);
    g.drawText ("Drop your audio file here!", fileDropArea, juce::Justification::centred, true);
    
    auto controlRow = getLocalBounds().removeFromBottom(1.0 / 5.0 * getHeight());
    playButton.setBounds(controlRow.removeFromLeft(getWidth() / 3.0));
    stopButton.setBounds(controlRow.removeFromLeft(getWidth() / 3.0));
    convertButton.setBounds(controlRow.removeFromLeft(getWidth() / 3.0));
}
