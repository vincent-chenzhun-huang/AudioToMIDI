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

class ConvertListener {
public:
    virtual ~ConvertListener() = default;
    virtual void reset() = 0;
    virtual void onConvert() = 0;
};


class MIDIOutput  : public juce::Component,
                    public juce::DragAndDropContainer,
                    public ConvertListener
{
public:
    MIDIOutput(juce::ValueTree& params);
    ~MIDIOutput() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void setDescription(juce::String);
    void onConvert() override;
    void reset() override;

private:
    juce::ValueTree& parameters;
    juce::Label description;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MIDIOutput)
};
