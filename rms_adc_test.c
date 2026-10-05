#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define PI                  3.14159265359f
#define NUM_SAMPLES         1000U

#define ADC_MAX_VALUE       4095.0f
#define ADC_REFERENCE_V     3.3f
#define ADC_MIDPOINT        2048.0f

#define FREQUENCY           50.0f
#define SAMPLE_RATE         5000.0f

/* Simulated AC RMS voltage at the ADC input */
#define INPUT_RMS           1.0f

/* Illustrative conversion factor for the software test */
#define VOLTAGE_SCALE       415.0f

float calculate_rms_from_adc(const uint16_t samples[],
                             uint32_t count)
{
    float sum_squares = 0.0f;

    float midpoint_voltage =
        (ADC_MIDPOINT / ADC_MAX_VALUE) * ADC_REFERENCE_V;

    for (uint32_t i = 0; i < count; i++)
    {
        float adc_voltage =
            ((float)samples[i] / ADC_MAX_VALUE)
            * ADC_REFERENCE_V;

        /* Remove the DC midpoint bias */
        float ac_voltage = adc_voltage - midpoint_voltage;

        sum_squares += ac_voltage * ac_voltage;
    }

    /* RMS voltage at the ADC input */
    return sqrtf(sum_squares / (float)count);
}

int main(void)
{
    uint16_t samples[NUM_SAMPLES];

    float midpoint_voltage =
        (ADC_MIDPOINT / ADC_MAX_VALUE) * ADC_REFERENCE_V;

    float peak_voltage = INPUT_RMS * sqrtf(2.0f);

    /* Generate simulated ADC samples */
    for (uint32_t i = 0; i < NUM_SAMPLES; i++)
    {
        float time = (float)i / SAMPLE_RATE;

        float signal =
            peak_voltage *
            sinf(2.0f * PI * FREQUENCY * time);

        float adc_voltage = midpoint_voltage + signal;

        float adc_value =
            (adc_voltage / ADC_REFERENCE_V) * ADC_MAX_VALUE;

        /* Keep simulated ADC values within the 12-bit range */
        if (adc_value < 0.0f)
            adc_value = 0.0f;

        if (adc_value > ADC_MAX_VALUE)
            adc_value = ADC_MAX_VALUE;

        samples[i] = (uint16_t)(adc_value + 0.5f);
    }

    /* Calculate RMS at the ADC input */
    float adc_rms =
        calculate_rms_from_adc(samples, NUM_SAMPLES);

    /* Convert to the simulated original input-voltage scale */
    float rms_voltage = adc_rms * VOLTAGE_SCALE;

    printf("Expected RMS:          %.3f V\n",
           INPUT_RMS * VOLTAGE_SCALE);

    printf("ADC Input RMS:         %.3f V\n",
           adc_rms);

    printf("Voltage Scale Factor:  %.3f\n",
           VOLTAGE_SCALE);

    printf("Calculated RMS:        %.3f V\n",
           rms_voltage);

    if (fabsf(rms_voltage - INPUT_RMS * VOLTAGE_SCALE) < 1.0f)
        printf("RMS Test: PASS\n");
    else
        printf("RMS Test: CHECK RESULT\n");

    return 0;
}