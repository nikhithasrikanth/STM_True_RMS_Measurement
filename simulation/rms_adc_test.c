#include <stdio.h>
#include <stdint.h>
#include <math.h>

/* Mathematical constant */
#define PI                  3.14159265359f

/* Number of samples used for the RMS calculation */
#define NUM_SAMPLES         1000U

/* ADC parameters */
#define ADC_MAX_VALUE       4095.0f
#define ADC_REFERENCE_V     3.3f
#define ADC_MIDPOINT        2048.0f

/* AC signal and sampling parameters */
#define FREQUENCY           50.0f
#define SAMPLE_RATE         5000.0f

/* Simulated AC RMS voltage at the ADC input */
#define INPUT_RMS           1.0f

/* Illustrative conversion factor used for the software simulation */
#define VOLTAGE_SCALE       415.0f


/*
 * Calculate AC True RMS from simulated ADC samples.
 * The ADC samples are converted back to voltage,
 * the DC midpoint is removed, and the RMS value
 * is calculated from the AC component.
 */
float calculate_rms_from_adc(const uint16_t samples[],
                             uint32_t count)
{
    float sum_squares = 0.0f;

    /* Convert the ADC midpoint value into voltage */
    float midpoint_voltage =
        (ADC_MIDPOINT / ADC_MAX_VALUE) * ADC_REFERENCE_V;

    /* Process each ADC sample */
    for (uint32_t i = 0; i < count; i++)
    {
        /* Convert ADC count to voltage */
        float adc_voltage =
            ((float)samples[i] / ADC_MAX_VALUE)
            * ADC_REFERENCE_V;

        /* Remove the DC midpoint bias */
        float ac_voltage =
            adc_voltage - midpoint_voltage;

        /* Accumulate the squared AC voltage */
        sum_squares += ac_voltage * ac_voltage;
    }

    /* Calculate RMS = sqrt(mean of squared samples) */
    return sqrtf(sum_squares / (float)count);
}


int main(void)
{
    /* Array to store simulated ADC samples */
    uint16_t samples[NUM_SAMPLES];

    /* Calculate the ADC midpoint voltage */
    float midpoint_voltage =
        (ADC_MIDPOINT / ADC_MAX_VALUE) * ADC_REFERENCE_V;

    /* Convert the simulated RMS value to peak value */
    float peak_voltage =
        INPUT_RMS * sqrtf(2.0f);


    /* Generate simulated ADC samples representing a 50 Hz AC waveform */
    for (uint32_t i = 0; i < NUM_SAMPLES; i++)
    {
        /* Calculate the time corresponding to each sample */
        float time = (float)i / SAMPLE_RATE;

        /* Generate the instantaneous AC voltage */
        float signal =
            peak_voltage *
            sinf(2.0f * PI * FREQUENCY * time);

        /* Add the DC midpoint so that the AC waveform can be represented by the ADC */
        float adc_voltage =
            midpoint_voltage + signal;

        /* Convert voltage into a 12-bit ADC value */
        float adc_value =
            (adc_voltage / ADC_REFERENCE_V) * ADC_MAX_VALUE;

        /* Keep simulated ADC values within the 12-bit range */
        if (adc_value < 0.0f)
            adc_value = 0.0f;

        if (adc_value > ADC_MAX_VALUE)
            adc_value = ADC_MAX_VALUE;

        /* Store the simulated ADC sample */
        samples[i] = (uint16_t)(adc_value + 0.5f);
    }


    /* Calculate RMS at the simulated ADC input */
    float adc_rms =
        calculate_rms_from_adc(samples, NUM_SAMPLES);


    /* Convert the ADC-side RMS value to the simulated original input-voltage scale */
    float rms_voltage =
        adc_rms * VOLTAGE_SCALE;


    /* Display the expected RMS value */
    printf("Expected RMS:          %.3f V\n",
           INPUT_RMS * VOLTAGE_SCALE);

    /* Display the RMS value calculated at the ADC input */
    printf("ADC Input RMS:         %.3f V\n",
           adc_rms);

    /* Display the illustrative scaling factor */
    printf("Voltage Scale Factor:  %.3f\n",
           VOLTAGE_SCALE);

    /* Display the final calculated RMS value */
    printf("Calculated RMS:        %.3f V\n",
           rms_voltage);


    /* Verify the calculated result against the expected RMS value */
    if (fabsf(rms_voltage - INPUT_RMS * VOLTAGE_SCALE) < 1.0f)
        printf("RMS Test: PASS\n");
    else
        printf("RMS Test: CHECK RESULT\n");

    return 0;
}
