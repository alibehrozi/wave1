package com.github.alibehrozi.wave;

import androidx.annotation.NonNull;

public class Hackrf {

    /**
     * Opens a connection to the HackRF device.
     *
     * @param fileDescriptor The file descriptor of the HackRF device.
     * @return The status of the operation.
     */
    public static native int open(int fileDescriptor);

    /**
     * Close a previously opened device
     *
     * @return @ref HACKRF_SUCCESS on success or variant of @ref hackrf_error
     */
    public static native int close();

    /**
     * Start receiving
     * <p>
     * Should be called after setting gains, frequency and sampling rate,
     * as these values won't get reset but instead keep their last value,
     * thus their state is unknown.
     * <p>
     * The callback is called with a @ref hackrf_transfer object whenever the buffer is full.
     * The callback is called in an async context so no libhackrf functions should be called from it.
     * The callback should treat its argument as read-only.
     *
     * @param callback rx_callback
     * @return @{@link hackrf_error#HACKRF_SUCCESS}  on success or @{@link hackrf_error} variant
     */
    public static native int startRx(RxCallback callback);

    /**
     * Start RX sweep
     * <p>
     * See @ref hackrf_init_sweep for more info
     *
     * @param callback rx callback processing the received data
     * @return @{@link hackrf_error#HACKRF_SUCCESS}  on success or @{@link hackrf_error} variant
     */
    public static native int startRxSweep(RxCallback callback);

    /**
     * Stop receiving
     *
     * @return @{@link hackrf_error#HACKRF_SUCCESS}  on success or @{@link hackrf_error} variant
     */
    public static native int stopRx();

    /**
     * Start transmitting
     * <p>
     * Should be called after setting gains, frequency and sampling rate,
     * as these values won't get reset but instead keep their last value,
     * thus their state is unknown.
     * Setting flush function (using @ref hackrf_enable_tx_flush) and/or setting block complete callback (using @ref hackrf_set_tx_block_complete_callback) (if these features are used) should also be done before this.
     * <p>
     * The callback is called with a @ref hackrf_transfer object whenever a transfer buffer is needed to be filled with samples. The callback is called in an async context so no libhackrf functions should be called from it. The callback should treat its argument as read-only, except the @ref hackrf_transfer.buffer and @ref hackrf_transfer.valid_length.
     *
     * @param callback tx_callback
     * @return @{@link hackrf_error#HACKRF_SUCCESS} on success or @ref hackrf_error variant
     */
    public static native int startTx(byte[] data, TxCallback callback);

    /**
     * Start transmitting
     * <p>
     * Should be called after setting gains, frequency and sampling rate,
     * as these values won't get reset but instead keep their last value,
     * thus their state is unknown.
     * Setting flush function (using @ref hackrf_enable_tx_flush) and/or setting block complete callback (using @ref hackrf_set_tx_block_complete_callback) (if these features are used) should also be done before this.
     * <p>
     * The callback is called with a @ref hackrf_transfer object whenever a transfer buffer is needed to be filled with samples. The callback is called in an async context so no libhackrf functions should be called from it. The callback should treat its argument as read-only, except the @ref hackrf_transfer.buffer and @ref hackrf_transfer.valid_length.
     *
     * @param callback tx_callback
     * @return @{@link hackrf_error#HACKRF_SUCCESS} on success or @ref hackrf_error variant
     */
    public static native int startTx(String path, TxCallback callback);

    /**
     * Stop transmission
     *
     * @return @{@link hackrf_error#HACKRF_SUCCESS}  on success or @{@link hackrf_error} variant
     */
    public static native int stopTx();

    /**
     * Sets the frequency of the HackRF device.
     *
     * @param frequency The desired frequency in Hz.
     * @return The status of the operation.
     */
    public static native int setFrequency(long frequency);

    /**
     * Sets the sample rate of the HackRF device.
     *
     * @param sampleRate The desired sample rate in Hz.
     * @return The status of the operation.
     */
    public static native int setSampleRate(long sampleRate);

    /**
     * Set LNA gain
     * <p>
     * Set the RF RX gain of the MAX2837 transceiver IC ("IF" gain setting) in decibels.
     * Must be in range 0-40dB, with 8dB steps.
     *
     * @param value RX IF gain value in dB
     * @return @{@link hackrf_error#HACKRF_SUCCESS}  on success or @{@link hackrf_error} variant
     */
    public static native int setLNAGain(int value);

    /**
     * Set baseband RX gain of the MAX2837 transceier IC ("BB" or "VGA" gain setting) in decibels.
     * Must be in range 0-62dB with 2dB steps.
     *
     * @param value RX BB gain value in dB
     * @return @{@link hackrf_error#HACKRF_SUCCESS}  on success or @{@link hackrf_error} variant
     */
    public static native int setVGAGain(int value);

    /**
     * Set RF TX gain of the MAX2837 transceiver IC ("IF" or "VGA" gain setting) in decibels.
     * Must be in range 0-47dB in 1dB steps.
     *
     * @param value TX IF gain value in dB
     * @return @{@link hackrf_error#HACKRF_SUCCESS}  on success or @{@link hackrf_error} variant
     */
    public static native int setTxVGAGain(int value);

    /**
     * Enable/disable 14dB RF amplifier
     *
     * Enable / disable the ~11dB RF RX/TX amplifiers U13/U25 via controlling switches U9 and U14.
     *
     * @param value enable (1) or disable (0) amplifier
     * @return @{@link hackrf_error#HACKRF_SUCCESS}  on success or @{@link hackrf_error} variant
     */
    public static native int setAmpEnable(boolean value);

    /**
     * Enable / disable bias-tee (antenna port power)
     * <p>
     * Enable or disable the **3.3V (max 50mA)** bias-tee (antenna port power).
     * Defaults to disabled.
     * <p>
     * **NOTE:** the firmware auto-disables this after returning to IDLE mode,
     * so a perma-set is not possible, which means all software supporting HackRF
     * devices must support enabling bias-tee, as setting it externally is not possible like it is with RTL-SDR for example.
     *
     * @param enable enable (1) or disable (0) bias-tee
     * @return @{@link hackrf_error#HACKRF_SUCCESS} on success or @ref hackrf_error variant
     */
    public static native int setAntennaEnable(boolean enable);

    /**
     * Convert @ref hackrf_error into human-readable string
     *
     * @param errcode enum to convert
     * @return human-readable name of error
     */
    public static String hackrf_error_name(hackrf_error errcode) {
        switch (errcode) {
            case HACKRF_SUCCESS:
                return "HACKRF_SUCCESS";

            case HACKRF_TRUE:
                return "HACKRF_TRUE";

            case HACKRF_ERROR_INVALID_PARAM:
                return "invalid parameter(s)";

            case HACKRF_ERROR_NOT_FOUND:
                return "HackRF not found";

            case HACKRF_ERROR_BUSY:
                return "HackRF busy";

            case HACKRF_ERROR_NO_MEM:
                return "insufficient memory";

            case HACKRF_ERROR_UNSUPPORTED:
                return "operation not supported by the device";

            case HACKRF_ERROR_LIBUSB:
                return "USB error";

            case HACKRF_ERROR_THREAD:
                return "transfer thread error";

            case HACKRF_ERROR_STREAMING_THREAD_ERR:
                return "streaming thread encountered an error";

            case HACKRF_ERROR_STREAMING_STOPPED:
                return "streaming stopped";

            case HACKRF_ERROR_STREAMING_EXIT_CALLED:
                return "streaming terminated";

            case HACKRF_ERROR_USB_API_VERSION:
                return "feature not supported by installed firmware";

            case HACKRF_ERROR_NOT_LAST_DEVICE:
                return "one or more HackRFs still in use";

            case HACKRF_ERROR_OTHER:
                return "unspecified error";

            default:
                return "unknown error code";
        }
    }

    /**
     * error enum, returned by many libhackrf functions
     * <p>
     * Many functions that are specified to return INT are actually returning this enum
     */
    public enum hackrf_error {

        /**
         * no error happened
         */
        HACKRF_SUCCESS(0),
        /**
         * TRUE value, returned by some functions that return boolean value. Only a few functions can return this variant, and this fact should be explicitly noted at those functions.
         */
        HACKRF_TRUE(1),
        /**
         * The function was called with invalid parameters.
         */
        HACKRF_ERROR_INVALID_PARAM(-2),
        /**
         * USB device not found, returned at opening.
         */
        HACKRF_ERROR_NOT_FOUND(-5),
        /**
         * Resource is busy, possibly the device is already opened.
         */
        HACKRF_ERROR_BUSY(-6),
        /**
         * Memory allocation (on host side) failed
         */
        HACKRF_ERROR_NO_MEM(-11),
        /**
         * The requested operation is not supported by the device.
         */
        HACKRF_ERROR_UNSUPPORTED(-12),
        /**
         * LibUSB error, use @ref hackrf_error_name to get a human-readable error string (using `libusb_strerror`)
         */
        HACKRF_ERROR_LIBUSB(-1000),
        /**
         * Error setting up transfer thread (pthread-related error)
         */
        HACKRF_ERROR_THREAD(-1001),
        /**
         * Streaming thread could not start due to an error
         */
        HACKRF_ERROR_STREAMING_THREAD_ERR(-1002),
        /**
         * Streaming thread stopped due to an error
         */
        HACKRF_ERROR_STREAMING_STOPPED(-1003),
        /**
         * Streaming thread exited (normally)
         */
        HACKRF_ERROR_STREAMING_EXIT_CALLED(-1004),
        /**
         * The installed firmware does not support this function
         */
        HACKRF_ERROR_USB_API_VERSION(-1005),
        /**
         * Can not exit library as one or more HackRFs still in use
         */
        HACKRF_ERROR_NOT_LAST_DEVICE(-2000),
        /**
         * Unspecified error
         */
        HACKRF_ERROR_OTHER(-9999);

        final int errorCode;

        hackrf_error(int error_code) {
            this.errorCode = error_code;
        }

        @NonNull
        @Override
        public String toString() {
            return hackrf_error_name(this);
        }
    }

}