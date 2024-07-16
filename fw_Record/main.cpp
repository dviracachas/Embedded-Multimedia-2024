#include <QCoreApplication>
#include <QAudioSource>
#include <QMediaDevices>
#include <QBuffer>
#include <QFile>
#include <QTimer>
#include <QDebug>
#include <QtMath>
#include <QProcess>
#include <QElapsedTimer>
#include <iostream>
#include <fstream>
#include <string>

class AudioRecorder : public QObject
{
    Q_OBJECT
public:
    AudioRecorder() {
        audioInput = nullptr;
        inputDevice = nullptr;
        silenceDuration = 0;
        silenceThreshold = 0.001f; // Anpassbare Schwelle für die Stilleerkennung
        silenceTimeout = 1000;  // 1000 Millisekunden (2 Sekunden)
        speechDetected = false;
        recordingStarted = false;
	loudChunkCount = 0;
	maxAllowedLoudChunks = 2;
    }

    void startRecording() {
        QAudioFormat format;
        format.setSampleRate(16000);
        format.setChannelCount(1);
        format.setSampleFormat(QAudioFormat::Int16);

        QAudioDevice inputDeviceInfo = QMediaDevices::defaultAudioInput();

        audioInput = new QAudioSource(inputDeviceInfo, format, this);
        inputDevice = audioInput->start();

        connect(inputDevice, &QIODevice::readyRead, this, &AudioRecorder::processAudio);

        audioBuffer.open(QIODevice::WriteOnly | QIODevice::Truncate);
        startTime.start(); // Startzeit für die Aufnahme messen
        recordingStarted = true;
    }

private slots:
    void stopRecording() {
        audioInput->stop();
        audioBuffer.close();
        saveWavFile("/mnt/ramdisk/output.wav", audioBuffer.buffer());
        std::cout << "rec";
        exit(0);
    }

    void processAudio() {
        QByteArray data = inputDevice->readAll();
        audioBuffer.write(data);

		if (data.size() != 0) {
			const int16_t* samples = reinterpret_cast<const int16_t*>(data.constData());
			int sampleCount = data.size() / sizeof(int16_t);
			
			qreal chunkMeanEnergy = 0;
			for (int k = 0; k < sampleCount; ++k) {
				chunkMeanEnergy += qPow(static_cast<qfloat16>(samples[k])/32768.0f,2);
			}
			chunkMeanEnergy = chunkMeanEnergy / sampleCount;
			
			bool silent = true;
			

            if (chunkMeanEnergy > 0.002){//silenceThreshold) {
				loudChunkCount++;
				silent = false;
				speechDetected = true;
				silenceDuration = 0;
			} else {
				loudChunkCount = 0;
			}

			if (recordingStarted && speechDetected) {
				if (silent) {
					silenceDuration += audioInput->format().durationForBytes(data.size()) / 1000; //chunk dauer in mikrosekunden warum auch immer

					if (silenceDuration >= silenceTimeout) {
						stopRecording();
					}
				} else if (loudChunkCount > maxAllowedLoudChunks) {
					silenceDuration = 0; // Reset silence duration if speech is detected
				}
			}
		}
	}

    void handleStateChanged(QAudio::State newState) {
	if (newState == QAudio::IdleState) {
            stopRecording();
        } else if (newState == QAudio::StoppedState) {
            if (audioInput->error() != QAudio::NoError) {
                qWarning() << "Audio input error";
            }
        }
    }

private:
    QAudioSource *audioInput;
    QIODevice *inputDevice;
    QBuffer audioBuffer;
    int silenceThreshold;
    qint64 silenceDuration;
    qint64 silenceTimeout;
    bool speechDetected;
    bool recordingStarted;
    QElapsedTimer startTime;
    short loudChunkCount;
    short maxAllowedLoudChunks;

    void saveWavFile(const QString &fileName, const QByteArray &data) {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            qWarning() << "Could not open file for writing";
            return;
        }

        // WAV header
        QDataStream out(&file);
        out.setByteOrder(QDataStream::LittleEndian);
        out.writeRawData("RIFF", 4);
        quint32 fileSize = 36 + data.size();
        out.writeRawData(reinterpret_cast<const char*>(&fileSize), 4);
        out.writeRawData("WAVE", 4);
        out.writeRawData("fmt ", 4);
        quint32 subchunk1Size = 16;
        out.writeRawData(reinterpret_cast<const char*>(&subchunk1Size), 4);
        quint16 audioFormat = 1;
        out.writeRawData(reinterpret_cast<const char*>(&audioFormat), 2);
        quint16 numChannels = 1;
        out.writeRawData(reinterpret_cast<const char*>(&numChannels), 2);
        quint32 sampleRate = 16000;
        out.writeRawData(reinterpret_cast<const char*>(&sampleRate), 4);
        quint32 byteRate = sampleRate * numChannels * 2;
        out.writeRawData(reinterpret_cast<const char*>(&byteRate), 4);
        quint16 blockAlign = numChannels * 2;
        out.writeRawData(reinterpret_cast<const char*>(&blockAlign), 2);
        quint16 bitsPerSample = 16;
        out.writeRawData(reinterpret_cast<const char*>(&bitsPerSample), 2);
        out.writeRawData("data", 4);
        quint32 subchunk2Size = data.size();
        out.writeRawData(reinterpret_cast<const char*>(&subchunk2Size), 4);
        out.writeRawData(data.data(), data.size());

        file.close();
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    AudioRecorder recorder;
    recorder.startRecording();
    QCoreApplication::quit();
    return a.exec();
}

#include "main.moc"
