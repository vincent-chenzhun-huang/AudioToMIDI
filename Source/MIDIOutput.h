/*
  ==============================================================================

    MIDIOutput.h
    Created: 8 Nov 2023 10:32:42pm
    Author:  Vincent Huang

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class MIDIOutput  : public juce::Component,
                    public juce::DragAndDropContainer
{
public:
    MIDIOutput(juce::ValueTree& params);
    ~MIDIOutput() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseDrag(const juce::MouseEvent& event) override;

private:
    juce::ValueTree& parameters;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MIDIOutput)
};
