#pragma once

#include <JuceHeader.h>
#include "FileWindow.h"
#include "ParamPanel.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
    FileWindow fileWindowComponent; // the area to drop a file
    ParamPanel paramPanel; // the area for the collection of sliders and checkboxes

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
