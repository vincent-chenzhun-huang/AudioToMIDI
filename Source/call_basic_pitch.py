import sys
import os

class AsciiStream:
    def __init__(self, original):
        self.original = original

    def write(self, text):
        filtered_text = ''.join(char for char in text if ord(char) < 128)
        self.original.write(filtered_text)

    def flush(self):
        self.original.flush()



from basic_pitch.inference import predict_and_save

# make a wrapper function for predict_and_save so that it can be called by C++
def convert_to_midi(
        audio_path_list,
        output_directory,
        save_midi,
        onset_threshold=0.5,
        frame_threshold=0.3,
        minimum_note_length=58,
        minimum_frequency=None,
        maximum_frequency=None,
        multiple_pitch_bends=False,
        midi_tempo=120,
):
    # Save the original stdout
    original_stdout = sys.stdout

    # Set the custom stream before calling the function
    sys.stdout = AsciiStream(sys.stdout)
    # remove the outputs that are already there
    for audio_path in audio_path_list:
        # convert to os-agnostic path
        audio_path = os.path.abspath(audio_path)
        file_name = os.path.basename(audio_path).split(".")[0]
        output_path = os.path.join(output_directory, file_name) + "_basic_pitch.mid"
        if os.path.exists(output_path):
            os.remove(output_path)

    predict_and_save(
        audio_path_list=audio_path_list,
        output_directory=output_directory,
        save_midi=save_midi,
        sonify_midi=False,
        save_model_outputs=False,
        save_notes=False,
        onset_threshold=onset_threshold,
        frame_threshold=frame_threshold,
        minimum_note_length=minimum_note_length,
        minimum_frequency=minimum_frequency,
        maximum_frequency=maximum_frequency,
        multiple_pitch_bends=multiple_pitch_bends,
        midi_tempo=midi_tempo,
    )
    sys.stdout = original_stdout


# Restore the original stdout after calling the function
if __name__ == '__main__':
    convert_to_midi(
        audio_path_list=["/Users/vincenthuang/Desktop/piano.mp3"],
        output_directory="/Users/vincenthuang/Desktop/",
        minimum_frequency=30,
        maximum_frequency=2920,
        save_midi=True,
    )
