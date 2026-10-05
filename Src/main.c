
#include <stdint.h>
#include <math.h>

#define PI                  3.14159265359f

/* Number of ADC samples used for RMS calculation */
#define NUM_SAMPLES         1000U

/* ADC parameters */
#define ADC_MAX_VALUE       4095.0f
#define ADC_REFERENCE_V     3.3f
#define ADC_MIDPOINT        2048.0f

/* AC signal and sampling parameters */
#define FREQUENCY           50.0f
#define SAMPLE_RATE         5000.0f

/* Simulated AC RMS voltage at the ADC input */
#define ADC_AC_RMS_VOLTAGE  1.0f

/*
 * Calculate AC True RMS from ADC samples.
 * The samples are centred around the ADC midpoint.
 */
float calculate_rms_from_adc(
        const uint16_t adc_samples[],
        uint32_t number_of_samples,
        float adc_reference,
        float adc_max,
        float adc_midpoint,
        float voltage_scale)
{
    float sum_squares = 0.0f;

/* Convert the ADC midpoint value into volts */
    float midpoint_voltage =
        (adc_midpoint / adc_max) * adc_reference;

/* Process each ADC sample */
    for (uint32_t i = 0; i < number_of_samples; i++)
    {

/* Convert ADC count to ADC input voltage */
        float adc_voltage =
            ((float)adc_samples[i] / adc_max)
            * adc_reference;

        /* Remove the DC midpoint bias */
        float ac_voltage =
            adc_voltage - midpoint_voltage;

        /* Convert to the input-voltage scale */
        float input_voltage =
            ac_voltage * voltage_scale;

        sum_squares += input_voltage * input_voltage;
    }
         
/* Calculate True RMS voltage */

    return sqrtf(sum_squares / (float)number_of_samples);
}

int main(void)
{
    /* Array to store simulated ADC samples */
    uint16_t adc_samples[NUM_SAMPLES];

    /* Variable used to store the calculated RMS voltage */
    volatile float rms_voltage = 0.0f;

    /* Illustrative voltage scaling factor used forthe software simulation */
    const float voltage_scale = 415.0f;

    /* Calculate the ADC midpoint voltage */
    float midpoint_voltage =
        (ADC_MIDPOINT / ADC_MAX_VALUE) * ADC_REFERENCE_V;

    /* Convert the simulated RMS value to peak value */
    float peak_voltage =
        ADC_AC_RMS_VOLTAGE * sqrtf(2.0f);


    /* Generate simulated ADC samples representing a 50 Hz AC waveform */
    for (uint32_t i = 0; i < NUM_SAMPLES; i++)
    {
        /* Calculate the time corresponding to each sample */
        float time = (float)i / SAMPLE_RATE;

        /* Generate the instantaneous AC voltage */
        float ac_voltage =
            peak_voltage *
            sinf(2.0f * PI * FREQUENCY * time);

        /* Add the DC midpoint so that the AC signal can be represented by an ADC input */
        float adc_voltage = midpoint_voltage + ac_voltage;

        /* Convert voltage into a 12-bit ADC value */
        float adc_value =
            (adc_voltage / ADC_REFERENCE_V) * ADC_MAX_VALUE;

        /* Keep the simulated ADC value within the valid 12-bit ADC range */
        if (adc_value < 0.0f)
            adc_value = 0.0f;

        if (adc_value > ADC_MAX_VALUE)
            adc_value = ADC_MAX_VALUE;

        /* Store the simulated ADC sample */
        adc_samples[i] = (uint16_t)(adc_value + 0.5f);
    }


    /* Calculate the True RMS voltage from the generated ADC samples */
    rms_voltage = calculate_rms_from_adc(
        adc_samples,
        NUM_SAMPLES,
        ADC_REFERENCE_V,
        ADC_MAX_VALUE,
        ADC_MIDPOINT,
        voltage_scale
    );

    while (1)
    {
        /* Inspect rms_voltage during debugging */
    }
}
