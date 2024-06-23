# Embedded-Multimedia-2024

## Description
TODO: Project Description

## Installation & Usage

This implementation uses [Porcupine](https://github.com/Picovoice/porcupine) which requires an Access Key that you can obtain by signing up on their website. To test it, you can plug a microphone and run the following command replacing ACCESS_KEY with your own:

```
./porcupine/demo/c/build/porcupine_demo_mic -l porcupine/lib/raspberry-pi/cortex-a72-aarch64/libpv_porcupine.so -m porcupine/lib/common/porcupine_params_es.pv -k porcupine/resources/Hey-Pola_es_raspberry-pi_v3_0_0.ppn -t 0.5 -d 1 -a ACCESS_KEY

```

Saying "Hey Pola" should print "keyword detected" on your console.