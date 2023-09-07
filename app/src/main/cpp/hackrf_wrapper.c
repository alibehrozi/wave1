#include <jni.h>
#include "hackrf.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <android/log.h>
#include <jni.h>

#define TAG "Hackrf Wrapper"

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,    TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,     TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,     TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,    TAG, __VA_ARGS__)

static hackrf_device *device;

static JNIEnv *callbackEnv;
static JavaVM *jvm;

static jclass callbackClass;
static jobject callbackObject;
static jmethodID callbackMethod;

int open_device(int file_descriptor);
int rx_callback_t(hackrf_transfer *);

extern JNIEXPORT jint JNICALL Java_com_github_alibehrozi_wave_Hackrf_open
        (JNIEnv *env, jclass class, jint file_descriptor) {

    if (device != NULL) {
        LOGE("device already open..");
        return EXIT_FAILURE;
    }

    int result = open_device(file_descriptor);
    if (result != HACKRF_SUCCESS) {
        LOGE("hackrf_open() failed: %s (%d)\n",
             hackrf_error_name(result), result);
    }
    return result;
}

extern JNIEXPORT jint JNICALL Java_com_github_alibehrozi_wave_Hackrf_close
        (JNIEnv *env, jclass class) {
    if (device == NULL) {
        LOGE("no open hackrf devices");
        return EXIT_FAILURE;
    }

    int result = hackrf_close(device);
    if (result != HACKRF_SUCCESS) {
        LOGE("hackrf_close() failed: %s (%d)\n",
             hackrf_error_name(result), result);
    }
    return result;
}

extern JNIEXPORT jint JNICALL Java_com_github_alibehrozi_wave_Hackrf_startRx
        (JNIEnv *env, jclass class, jobject callback) {
    callbackEnv = env;

    (*callbackEnv)->GetJavaVM(callbackEnv, &jvm);

    // Create a global reference to the callback object
    callbackObject = (*callbackEnv)->NewGlobalRef(callbackEnv, callback);

    // Obtain a class reference for the callback interface
    callbackClass = (*env)->GetObjectClass(env, callback);
    if (NULL == callbackClass) {
        LOGE("callback method could not be found");
        return EXIT_FAILURE;
    }

    // Find the method ID of the onData method
    callbackMethod = (*env)->GetMethodID(env, callbackClass, "onData", "([B)V");
    if (NULL == callbackMethod) {
        LOGE("callback method could not be found");
        return EXIT_FAILURE;
    }

    int result = hackrf_start_rx(device, rx_callback_t, NULL);
    if (result != HACKRF_SUCCESS) {
        LOGE("hackrf_start_rx() failed: %s (%d)\n", hackrf_error_name(result), result);
        return result;
    }

    LOGE("hackrf_start_rx() done\n");
    return result;
}

extern JNIEXPORT jint JNICALL Java_com_github_alibehrozi_wave_Hackrf_stopRx
        (JNIEnv *env, jclass class) {
    int result = hackrf_stop_rx(device);
    if (result != HACKRF_SUCCESS) {
        LOGE("hackrf_stop_rx() failed: %s (%d)\n",
             hackrf_error_name(result), result);
    } else {
        LOGE("hackrf_stop_rx() done\n");
    }
    return result;
}

extern JNIEXPORT jint JNICALL Java_com_github_alibehrozi_wave_Hackrf_startTx
        (JNIEnv *env, jclass class, jobject callback) {
    int result = hackrf_stop_tx(device);
    if (result != HACKRF_SUCCESS) {
        LOGE("hackrf_stop_tx() failed: %s (%d)\n",
             hackrf_error_name(result), result);
    } else {
        LOGE("hackrf_stop_rx() done\n");
    }
    return result;
}

extern JNIEXPORT jint JNICALL Java_com_github_alibehrozi_wave_Hackrf_stopTx
        (JNIEnv *env, jclass class) {
    // TODO : implement stopTx()
    return EXIT_FAILURE;
}

extern JNIEXPORT jint JNICALL Java_com_github_alibehrozi_wave_Hackrf_startRxSweep
(JNIEnv *env, jclass clazz, jobject callback) {
    // TODO: implement startRxSweep()
    return EXIT_FAILURE;
}

extern JNIEXPORT jint JNICALL Java_com_github_alibehrozi_wave_Hackrf_setFrequency
        (JNIEnv *env, jclass class, jlong freq_hz) {
    if (device == NULL) {
        LOGE("no open hackrf devices");
        return EXIT_FAILURE;
    }
    int result = hackrf_set_freq(device, freq_hz);
    if (result != HACKRF_SUCCESS) {
        LOGE("hackrf_set_freq() failed: %s (%d)\n",
             hackrf_error_name(result), result);
    }

    return result;
}

extern JNIEXPORT jint JNICALL Java_com_github_alibehrozi_wave_Hackrf_setSampleRate
        (JNIEnv *env, jclass class, jlong freq_hz) {
    int result = hackrf_set_sample_rate_manual(device, freq_hz, 1);
    if (result != HACKRF_SUCCESS) {
        LOGE("hackrf_sample_rate_set() failed: %s (%d)\n",
             hackrf_error_name(result), result);
    }

    return result;
}

extern JNIEXPORT jint JNICALL Java_com_github_alibehrozi_wave_Hackrf_setLNAGain
        (JNIEnv *env, jclass class, jint gain) {
    int result = hackrf_set_lna_gain(device, gain);
    if (result != HACKRF_SUCCESS) {
        LOGE("hackrf_set_lna_gain() failed: %s (%d)\n",
             hackrf_error_name(result), result);
    }

    return result;
}

extern JNIEXPORT jint JNICALL Java_com_github_alibehrozi_wave_Hackrf_setVGAGain
        (JNIEnv *env, jclass class, jint gain) {
    int result = hackrf_set_vga_gain(device, gain);
    if (result != HACKRF_SUCCESS) {
        LOGE("hackrf_set_vga_gain() failed: %s (%d)\n",
             hackrf_error_name(result), result);
    }

    return result;
}

extern JNIEXPORT jint JNICALL Java_com_github_alibehrozi_wave_Hackrf_setTxVGAGain
        (JNIEnv *env, jclass class, jint gain) {
    int result = hackrf_set_txvga_gain(device, gain);
    if (result != HACKRF_SUCCESS) {
        LOGE("hackrf_set_txvga_gain() failed: %s (%d)\n",
             hackrf_error_name(result), result);
    }

    return result;
}

extern JNIEXPORT jint JNICALL Java_com_github_alibehrozi_wave_Hackrf_setAmpEnable
        (JNIEnv *env, jclass class, jboolean enable) {
    int result = hackrf_set_amp_enable(device, enable);
    if (result != HACKRF_SUCCESS) {
        LOGE("hackrf_set_txvga_gain() failed: %s (%d)\n",
             hackrf_error_name(result), result);
    }

    return result;
}

extern JNIEXPORT jint JNICALL Java_com_github_alibehrozi_wave_Hackrf_setAntennaEnable
        (JNIEnv *env, jclass class, jboolean enable) {
    int result = hackrf_set_antenna_enable(device, enable);
    if (result != HACKRF_SUCCESS) {
        LOGE("hackrf_set_txvga_gain() failed: %s (%d)\n",
             hackrf_error_name(result), result);
    }

    return result;
}

int open_device(int file_descriptor) {
    int result = HACKRF_SUCCESS;
    uint8_t board_id = BOARD_ID_INVALID;
    char version[255 + 1];
    read_partid_serialno_t read_partid_serialno;

    result = hackrf_init();
    if (result != HACKRF_SUCCESS) {
        LOGE("hackrf_init() failed: %s (%d)\n",
             hackrf_error_name(result), result);
        return result;
    }

    result = hackrf_open_by_file_descriptor(file_descriptor, &device);
    if (result != HACKRF_SUCCESS) {
        LOGE("hackrf_open() failed: %s (%d)\n",
             hackrf_error_name(result), result);
        return result;
    }

    int i = 0;

    LOGE("Found HackRF board %d:\n", i);

    result = hackrf_board_id_read(device, &board_id);
    if (result != HACKRF_SUCCESS) {
        LOGE("hackrf_board_id_read() failed: %s (%d)\n",
             hackrf_error_name(result), result);
        return result;
    }
    LOGE("Board ID Number: %d (%s)\n", board_id,
         hackrf_board_id_name(board_id));

    result = hackrf_version_string_read(device, &version[0], 255);
    if (result != HACKRF_SUCCESS) {
        LOGE("hackrf_version_string_read() failed: %s (%d)\n",
             hackrf_error_name(result), result);
        return result;
    }
    LOGE("Firmware Version: %s\n", version);

    result = hackrf_board_partid_serialno_read(device, &read_partid_serialno);
    if (result != HACKRF_SUCCESS) {
        LOGE("hackrf_board_partid_serialno_read() failed: %s (%d)\n",
             hackrf_error_name(result), result);
        return result;
    }
    LOGE("Part ID Number: 0x%08x 0x%08x\n",
         read_partid_serialno.part_id[0],
         read_partid_serialno.part_id[1]);
    LOGE("Serial Number: 0x%08x 0x%08x 0x%08x 0x%08x\n",
         read_partid_serialno.serial_no[0],
         read_partid_serialno.serial_no[1],
         read_partid_serialno.serial_no[2],
         read_partid_serialno.serial_no[3]);

    return result;
}

int rx_callback_t(hackrf_transfer *transfer) {
//    LOGE("got data of size %d\n", transfer->valid_length);

    if (callbackEnv == NULL)
        LOGE("call.ckEnv was null.");

    if (callbackObject == NULL)
        LOGE("call.object was null.");

    // Attach the current thread to the Java VM
    (*jvm)->AttachCurrentThread(jvm, &callbackEnv, NULL);

    // Create a jbyteArray to hold the data
    jbyteArray byteArray = (*callbackEnv)->NewByteArray(callbackEnv, transfer->valid_length);
    jbyte *buf = (*callbackEnv)->GetByteArrayElements(callbackEnv, byteArray, NULL);

    memcpy(buf, transfer->buffer, transfer->valid_length);

    // Call the Java callback
    (*callbackEnv)->ReleaseByteArrayElements(callbackEnv, byteArray, buf, 0);
    (*callbackEnv)->CallVoidMethod(callbackEnv, callbackObject, callbackMethod, byteArray);

    // Detach the thread when done
    (*jvm)->DetachCurrentThread(jvm);

    return 0;
}

int transfer_callback_t(hackrf_transfer *transfer) {
//    int8_t *signed_buffer = (int8_t *) transfer->buffer;
//    for (int i = 0; i < transfer->buffer_length; i += 2) {
//        phasor *= cexp(I * 6.28 * 3000 / sample_rate * triangle());
//        // any IQ samples can be written here, now I'm doing FM modulation with a triangle wave
//        *signed_buffer[i] = 128 * creal(phasor);
//        signed_buffer[i + 1] = 128 * cimag(phasor);
//    }
//    transfer->valid_length = transfer->buffer_length;
//    xfered_samples += transfer->buffer_length;
//    if (xfered_samples >= samples_to_xfer) {
//        return 1;
//    }
    return 0;
}

void flush_callback(hackrf_transfer *transfer) {
//    pthread_mutex_lock(&mutex);
//    pthread_cond_broadcast(&cond);
//    pthread_mutex_unlock(&mutex);
}