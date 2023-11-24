#pragma once

#include <sstream>
#include <JuceHeader.h>
#include <Python.h>
#include <cassert>


class FileWindow: public juce::AudioAppComponent,
                  public juce::FileDragAndDropTarget,
                  public juce::ChangeListener {
public:
    FileWindow(juce::ValueTree&);
    ~FileWindow() override;

    void paint (juce::Graphics&) override;
    
    // FileDragAndDropTarget functions
    bool isInterestedInFileDrag (const juce::StringArray&) override;
    void filesDropped (const juce::StringArray&, int, int) override;
    void fileDragMove(const juce::StringArray& files, int x, int y) override;
    void fileDragExit(const juce::StringArray& files) override;
    
    // AudioAppComponent functions
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) override;
    void releaseResources() override;
    
    // ChangeListener functions
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    int convertMidiClicked();

private:
    juce::ValueTree& parameters;
    // Playback buttons
    juce::TextButton playButton;
    juce::TextButton convertButton;
    juce::TextButton stopButton;
    
    void playButtonClicked();
    void stopButtonClicked();
//    void convertButtonClicked();
    
    // Audio-playback-related modules
    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Stopping
    };
    void changeState(TransportState newState);

    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    TransportState state;

    // I/O
    juce::File droppedFile;
    std::string droppedFilePath;
    std::string outputDirectory;
    // Python API
    int callBasicPitch(std::vector<std::string> audioPathList,
                       std::string outputDirectory,
                       bool saveMidi,
                       int minNoteLength,
                       int minPitch,
                       int maxPitch,
                       bool multiplePitchBends,
                       int midiTempo);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FileWindow);
};
