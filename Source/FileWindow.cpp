#include "FileWindow.h"

//==============================================================================
FileWindow::FileWindow()
{
    setName("File Window");
    setOpaque(true);
    setInterceptsMouseClicks(true, true);
    
    addAndMakeVisible(playButton);
    playButton.setButtonText("Play Audio");
    addAndMakeVisible(convertButton);
    convertButton.setButtonText("Convert to MIDI");
}

FileWindow::~FileWindow()
{
}

bool FileWindow::isInterestedInFileDrag(const juce::StringArray& files) {
    auto droppedArea = getLocalBounds().withTrimmedBottom(getHeight() / 5);
    return droppedArea.contains(getMouseXYRelative());
}

void FileWindow::filesDropped(const juce::StringArray& filenames, int x, int y) {
    juce::Array<juce::File> files;
    std::cout << "file dropped" << std::endl;
    
    for (auto& f: filenames) {
        files.add({ f });
    }
    
    for (auto& file: files) {
        auto content = file.loadFileAsString();
        std::cout << "File content: " << content << std::endl;
    }
}

void FileWindow::fileDragMove(const juce::StringArray &files, int x, int y) {}

void FileWindow::fileDragExit(const juce::StringArray &files) {}

//==============================================================================
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
    playButton.setBounds(controlRow.removeFromLeft(getWidth() * 0.5));
    convertButton.setBounds(controlRow.removeFromLeft(getWidth() * 0.5));
}
