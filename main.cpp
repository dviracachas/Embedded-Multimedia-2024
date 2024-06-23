#include <QCoreApplication>
#include <QDebug>

#include <QDir>
#include <fstream>
#include <pv_porcupine.h>
#include <QMediaDevices>
#include <QAudioFormat>
#include <QAudioSource>

// Generate Porcupine's paths using SOURCE_DIR defined in PRO config to work with relative paths
const char* genFullPath(std::string relative_path) {
    std::string source_dir = SOURCE_DIR;
    std::string path_str = source_dir + relative_path;
    size_t path_size = path_str.length() + 1;
    char* path = new char[path_size];
    std::strncpy(path, path_str.c_str(), path_size);
    return path;
}

// Read Access Key from file
const char* getAccessKeyFromFile(const std::string& fileName) {
    std::string source_dir = SOURCE_DIR;
    std::string path_str = source_dir + fileName;
    std::ifstream infile(path_str);
    if (!infile) {
        qDebug() << "Failed to access Access Key file";
        return nullptr;
    }
    std::string access_key_str;
    std::getline(infile, access_key_str);
    infile.close();
    if (access_key_str.empty()) {
        qDebug() << "Access Key file is empty";
        return nullptr;
    }
    size_t size = access_key_str.length() + 1;
    char* access_key = new char[size];
    std::strncpy(access_key, access_key_str.c_str(), size);
    access_key[size - 1] = '\0'; // Ensure null termination

    return access_key;
}


// Declare Porcupine's parameters
static const char* MODEL_PATH = genFullPath("/porcupine/lib/common/porcupine_params_es.pv");
static const char* HEYPOLA_PATH = genFullPath("/porcupine/resources/Hey-Pola_es_raspberry-pi_v3_0_0.ppn");
static const char *KEYWORDS_PATH[] = {
    HEYPOLA_PATH,
    nullptr
};
static const char* ACCESS_KEY = getAccessKeyFromFile("/AccessKey.txt");
static const int num_kewords = 1;
static const float sensitivities[] = {0.5f};


// Declare global variables
static pv_porcupine_t *porcupineObject;
static QAudioSource *audioInput;
static QIODevice *ioDevice;
static QByteArray audioDataBuffer;

// Check if we have enough bytes of audio data for Porcupine to work with
bool hasEnoughSamples(int porcupineFrameLength)
{
    // We use porcupineFrameLength * 2 because frame length type is int16_t while audioDataBuffer's type is byte
    return audioDataBuffer.size() >= porcupineFrameLength * 2;
}

// Go through the audio data and detect the wake word if there's any
static void processSamples(int porcupineFrameLength)
{
    while (hasEnoughSamples(porcupineFrameLength)) {
        const int16_t *audioData = reinterpret_cast<int16_t*>(audioDataBuffer.data());
        int32_t keyword_index;

        pv_porcupine_process(porcupineObject, &audioData[0], &keyword_index);
        if (keyword_index != -1) {
            // Detected keywords. Do something!
            qDebug() << "Detected keywords!";
        }

        // Remove the audio data that we have processed
        audioDataBuffer.remove(0, porcupineFrameLength * 2);
    }
}

// Listen to the microphone's audio data and look for the keyword
static void listen(const QByteArray &audioData)
{
    const int porcupineFrameLength = pv_porcupine_frame_length();
    audioDataBuffer.append(audioData);
    processSamples(porcupineFrameLength);
}

// Setup Porcupine library
static void setupPorcupine()
{
    pv_status_t status = pv_porcupine_init(ACCESS_KEY, MODEL_PATH, num_kewords, KEYWORDS_PATH, sensitivities, &porcupineObject);
    if (status != PV_STATUS_SUCCESS) {
        qDebug() << "Failed to initialize Porcupine";
        exit(2);
    }
}

// Setup microphone
static void setupMicrophone()
{
    QAudioFormat qtFormat;

    // Get default audio input device
    QAudioDevice qtInfo = QMediaDevices::defaultAudioInput();

    // Set the audio format settings
    qtFormat.setChannelCount(1);
    qtFormat.setSampleRate(16000);
    qtFormat.setSampleFormat(QAudioFormat::Int16);

    // Check whether the format is supported
    if (!qtInfo.isFormatSupported(qtFormat)) {
        qWarning() << "Default format is not supported";
        exit(3);
    }

    // Instantiate QAudioInput with the settings
    audioInput = new QAudioSource(qtFormat);

    // Start receiving data from audio input
    ioDevice = audioInput->start();

    // Listen to the received data for wake words
    QObject::connect(ioDevice, &QIODevice::readyRead, [=] {
        listen(ioDevice->readAll());
    });
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    setupPorcupine();
    setupMicrophone();

    qDebug() << "Say the keywords! I'm listening.";

    return a.exec();
}
