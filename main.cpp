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

class AudioRecorder : public QObject
{
    Q_OBJECT
public:
    AudioRecorder() {
        audioInput = nullptr;
        inputDevice = nullptr;
        silenceDuration = 0;
        silenceThreshold = 0.001f; // Anpassbare Schwelle für die Stilleerkennung
        silenceTimeout = 2000;  // 2000 Millisekunden (2 Sekunden)
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

        std::cout << "WAITING_FOR_SPEECH\n" << std::endl;
        startTime.start(); // Startzeit für die Aufnahme messen
        recordingStarted = true;
    }

private slots:
    void stopRecording() {
        audioInput->stop();
        audioBuffer.close();

        qint64 durationInSeconds = startTime.elapsed() / 1000; // Dauer der Aufnahme in Sekunden

        std::cout << "RECORDED " << durationInSeconds << std::endl;

        //saveWavFile("output.wav", audioBuffer.buffer());
        saveWavFile("/mnt/ramdisk/output.wav", audioBuffer.buffer());

        // Transkription der Audiodatei
        QElapsedTimer timer;
        timer.start();
        //transcribe("output.wav");
        transcribe("/mnt/ramdisk/output.wav");
        qint64 transcribeTimeInSeconds = timer.elapsed() / 1000; // Zeit für die Transkription in Sekunden
        std::cout << "PROCESSED_TOTAL " << transcribeTimeInSeconds << std::endl;
    }

    void transcribe(const QString &fileName) {
        QProcess process;
        process.start("python3", QStringList() << "transcribe.py" << fileName);
        process.waitForFinished();
        QString output(process.readAllStandardOutput());
        std::cout << "TRANSCRIBED " << output.toStdString() << std::endl;
        //qDebug() << output;
    }

    void processAudio() {
        QByteArray data = inputDevice->readAll();
        audioBuffer.write(data);

		if (data.size() != 0) {
			const int16_t* samples = reinterpret_cast<const int16_t*>(data.constData());
			int sampleCount = data.size() / sizeof(int16_t);
			
			//std::cout << "Chuck size: " << sampleCount << std::endl;
			//std::cout << "Int16 size: " << sizeof(int16_t) << std::endl;
			
			
			/*
			qfloat16 *shortTimeEnergy = new qfloat16[sampleCount];
			qfloat16 alpha = 0.997f;
			for (int k = 1; k < sampleCount; ++k) {
				shortTimeEnergy[k] = alpha*shortTimeEnergy[k-1] + (1.0f-alpha)*qPow(static_cast<qfloat16>(samples[k])/32768.0f,2);
			}

			bool silent = true;
			for (int i = 0; i < sampleCount; ++i) {
				if (shortTimeEnergy[i] > silenceThreshold) {
					silent = false;
					speechDetected = true;
					silenceDuration = 0;
					break;
				}
			}
			*/
			
			qreal chunkMeanEnergy = 0;
			for (int k = 0; k < sampleCount; ++k) {
				chunkMeanEnergy += qPow(static_cast<qfloat16>(samples[k])/32768.0f,2);
			}
			chunkMeanEnergy = chunkMeanEnergy / sampleCount;
			//std::cout << "Chunk energy: " << chunkMeanEnergy << std::endl;
			//short plotHeight = chunkMeanEnergy *1000;
			//std::cout << std::string(plotHeight,' ') << '*' << std::endl;
			
			bool silent = true;
			

			if (chunkMeanEnergy > 0.001){//silenceThreshold) {
				loudChunkCount++;
				std::cout << chunkMeanEnergy << std::endl;
				silent = false;
				speechDetected = true;
				silenceDuration = 0;
			} else {
				loudChunkCount = 0;
			}

			if (recordingStarted && speechDetected) {
				if (silent) {
					silenceDuration += audioInput->format().durationForBytes(data.size()) / 1000; //chunk dauer in mikrosekunden warum auch immer
					//std::cout << "Silence duration: " << silenceDuration << std::endl;
					std::cout << 0 << std::endl;
					
					if (silenceDuration >= silenceTimeout) {
						std::cout << "STOPPED" << std::endl;
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
    return a.exec();
}

#include "main.moc"
