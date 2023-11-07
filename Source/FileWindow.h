#pragma once

#include <JuceHeader.h>


class FileWindow  : public juce::Component, public juce::FileDragAndDropTarget
{
public:
    FileWindow();
    ~FileWindow() override;

    void paint (juce::Graphics&) override;
    bool isInterestedInFileDrag (const juce::StringArray&) override;
    void filesDropped (const juce::StringArray&, int, int) override;
    void fileDragMove(const juce::StringArray& files, int x, int y) override;
    void fileDragExit(const juce::StringArray& files) override;

private:
    juce::TextButton playButton;
    juce::TextButton convertButton;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FileWindow);
};
