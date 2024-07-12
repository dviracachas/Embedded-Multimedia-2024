import sys
from faster_whisper import WhisperModel
import timeit


def transcribe(file_path):
    print("transcribe start")
    start = timeit.default_timer()
    model = WhisperModel("large")###
    print("model loaded")
    stop = timeit.default_timer()
    init_time = stop - start
    
    start = timeit.default_timer()
    segments, info = model.transcribe(file_path)###
    print("transcribe finished")
    text = ' '.join([segment.text for segment in segments])###
    stop = timeit.default_timer()
    transcribe_time = stop - start
    
    return text + f"\nPROCESSED_INIT {init_time:.2f}\nPROCESSED_TC {transcribe_time:.2f}"

if __name__ == "__main__":
    print("main start")
    if len(sys.argv) != 2:
        sys.exit(1)

    audio_file = sys.argv[1]
    transcription = transcribe(audio_file)
    print(transcription, end="")
