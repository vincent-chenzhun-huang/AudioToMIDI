#include "FileWindow.h"

FileWindow::FileWindow(juce::ValueTree& params): parameters(params), state(Stopped) {
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
    convertButton.onClick = [this] {
        convertMidiClicked();
    };
    convertButton.setEnabled(false);
    
    formatManager.registerBasicFormats(); // register the basic audio file formats, wav, aiff files
    transportSource.addChangeListener(this); // when there is a change in the transportSource, we trigger the changeListenerCallback function
    setAudioChannels(0, 2);
    
    Py_Initialize();
}

FileWindow::~FileWindow() {
    shutdownAudio();
    Py_Finalize();

}

void FileWindow::playButtonClicked() {
    changeState(Starting);
}

void FileWindow::stopButtonClicked() {
    changeState(Stopping);
}

int FileWindow::convertMidiClicked() {
    // Set the PYTHONHOME environment variable to the Python interpreter's directory
    // This helps in locating the Python runtime
    // wchar_t *pythonHome = Py_DecodeLocale("basic_pitch_test", NULL);
    // Py_SetPythonHome(pythonHome);

    // Initialize the Python interpreter
    assert (droppedFile != NULL);
    if (!droppedFile.exists()) {
        juce::Logger::writeToLog("The file does not exist, aborting the conversion");
        return -1;
    }
    
    std::vector<std::string> audioPathList;
    audioPathList.push_back(droppedFilePath);

    std::string outputDirectory = juce::File::getSpecialLocation(juce::File::tempDirectory).getFullPathName().toStdString();
    juce::Logger::writeToLog("Writing to " + outputDirectory);
    bool saveMidi = true;
    if (!parameters.isValid()) {
        juce::Logger::writeToLog("The parameters are not initialized, aborting the conversion");
        return -1;
    }
    
    int minNoteLength, minPitch, maxPitch, midiTempo;
    minNoteLength = (parameters.hasProperty("minNoteLength")) ? int(parameters.getProperty("minNoteLength")) : 58;
    minPitch = (parameters.hasProperty("minPitch")) ? int(parameters.getProperty("minPitch")) : 0;
    maxPitch = (parameters.hasProperty("maxPitch")) ? int(parameters.getProperty("maxPitch")) : 0;
    
    bool multiplePitchBends = false;
    midiTempo = (parameters.hasProperty("midiTempo")) ? int(parameters.getProperty("midiTempo")) : 120;
    
    std::ostringstream logStream;
    logStream << "Converting to MIDI using values: minNoteLength: " << minNoteLength << " - minPitch: " << minPitch << " - maxPitch: " << maxPitch << " - midiTempo: " << midiTempo;
    juce::Logger::writeToLog(logStream.str());
    int res = callBasicPitch(audioPathList, outputDirectory, saveMidi, minNoteLength, minPitch, maxPitch, multiplePitchBends, midiTempo);
    
    if (res != 0) {
        juce::Logger::writeToLog("An error occurred while converting the audio file to MIDI");
        return -1;
    }
    
    
    // otherwise, write the output file object to the value tree
    juce::String outputFileName = droppedFile.getFileNameWithoutExtension() + "_basic_pitch.mid";
    outputFile = juce::File(outputDirectory).getChildFile(outputFileName);
    juce::Logger::writeToLog(outputFile.getFullPathName());
    parameters.setProperty("outputFilePath", outputFile.getFullPathName(), nullptr);
    return 0;
}

int FileWindow::callBasicPitch(std::vector<std::string> audioPathList,
                               std::string outputDirectory,
                               bool saveMidi,
                               int minNoteLength,
                               int minPitch,
                               int maxPitch,
                               bool multiplePitchBends,
                               int midiTempo) {
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;
    // Py_SetPath(L"/Users/vincenthuang/Development/basic_pitch_test");

    PyRun_SimpleString("import sys, os");
    PyRun_SimpleString("sys.path.append('/Users/vincenthuang/Development/basic_pitch_test')");
//    PyRun_SimpleString("sys.path.append(os.getcwd())");
//    PyRun_SimpleString("print(os.getcwd())");


    pName = PyUnicode_DecodeFSDefault("call_basic_pitch");

    pModule = PyImport_Import(pName);
    Py_DECREF(pName); // Decrement the reference count for pName, preventing memory leak

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, "convert_to_midi");

        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(10);
            PyObject *pList = PyList_New(0);
            for (const auto &path : audioPathList) {
                PyObject *pString = PyUnicode_FromString(path.c_str());
                PyList_Append(pList, pString);
                Py_DECREF(pString);
            }

            /*
            Function signature
            def convert_to_midi(
                audio_path_list, 0
                output_directory, 1
                save_midi, 2
                onset_threshold=0.5, 3
                frame_threshold=0.3, 4
                minimum_note_length=58, 5
                minimum_frequency=None, 6
                maximum_frequency=None, 7
                multiple_pitch_bends=False, 8
                midi_tempo=120, 9
            )
            */

            PyTuple_SetItem(pArgs, 0, pList);
            PyTuple_SetItem(pArgs, 1, PyUnicode_FromString(outputDirectory.c_str()));
            PyTuple_SetItem(pArgs, 2, PyBool_FromLong(saveMidi));
            PyTuple_SetItem(pArgs, 3, PyFloat_FromDouble(0.5)); // onset_threshold
            PyTuple_SetItem(pArgs, 4, PyFloat_FromDouble(0.3)); // frame_threshold
            PyTuple_SetItem(pArgs, 5, PyLong_FromLong(minNoteLength)); // minimum_note_length
            if (minPitch == 0) {
                PyTuple_SetItem(pArgs, 6, Py_None); // minimum_frequency
            } else {
                PyTuple_SetItem(pArgs, 6, PyLong_FromLong(minPitch)); // minimum_frequency
            }
            if (maxPitch == 0) {
                PyTuple_SetItem(pArgs, 7, Py_None); // maximum_frequency
            } else {
                PyTuple_SetItem(pArgs, 7, PyLong_FromLong(maxPitch)); // maximum_frequency
            }
            PyTuple_SetItem(pArgs, 8, PyBool_FromLong(multiplePitchBends)); // multiple_pitch_bends
            PyTuple_SetItem(pArgs, 9, PyLong_FromLong(midiTempo)); // midi_tempo

            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs); // decrement reference count for pArgs, preventing memory leak

            if (pValue != NULL) {
                Py_DECREF(pValue);
            } else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr, "Call failed\n");
                return 1;
            }
        }
    } else {
        PyErr_Print();
        fprintf(stderr, "Failed to load call_basic_pitch.py\n");
        return 1;
    }

    return 0;
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
    droppedFile = juce::File(filenames[0]); // store the file object
    droppedFilePath = droppedFile.getFullPathName().toStdString();
    juce::Logger::writeToLog("File Dropped: " + droppedFile.getFullPathName());

    auto* reader = formatManager.createReaderFor(droppedFile);
    juce::Logger::writeToLog("Reader initialized");
    
    if (reader != nullptr) {
        auto newSource = std::make_unique<juce::AudioFormatReaderSource> (reader, true);
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        juce::Logger::writeToLog("Activate playback button");
        playButton.setEnabled(true);
        convertButton.setEnabled(true);
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
