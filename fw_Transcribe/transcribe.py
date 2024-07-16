import sys
from faster_whisper import WhisperModel
import timeit



def transcribe(file_path):
    #print("transcribe start")
    start = timeit.default_timer()
    try:
        model = WhisperModel("base")###
    except InitExcept:
        f = open("/mnt/ramdisk/transcription.txt", "w")
        f.write("error")
        f.close()
        print("tr",end="");
        return "INIT FAILED"
        
    #print("model loaded")
    stop = timeit.default_timer()
    init_time = stop - start
    
    start = timeit.default_timer()
    try:
        segments, info = model.transcribe(file_path)###
    except TranscribeExcept:
        f = open("/mnt/ramdisk/transcription.txt", "w")
        f.write("error")
        f.close()
        print("tr",end="");
        return "TRANSCRIBE FAILED"
        
    #print("transcribe finished")
    text = ' '.join([segment.text for segment in segments])###
    stop = timeit.default_timer()
    transcribe_time = stop - start
    
    f = open("/mnt/ramdisk/transcription.txt", "w")
    f.write(text)
    f.close()
    
    #del model
    return text + f"\nPROCESSED_INIT {init_time:.2f}\nPROCESSED_TC {transcribe_time:.2f}"

if __name__ == "__main__":
    #print("main start")
    if len(sys.argv) != 1:
        sys.exit(1)

    #audio_file = sys.argv[1]
    _ = transcribe("/mnt/ramdisk/output.wav")
    print("tr",end="");
    sys.exit(0);
    #print(transcription, end="")
