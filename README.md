# STM32 True RMS Measurement

## Overview

This project implements a general C-based True RMS calculation approach for an STM32 controller.

The RMS algorithm was developed and tested using simulated ADC samples. The STM32CubeIDE project compiles successfully.

## True RMS Calculation

The RMS voltage is calculated using:

Vrms = sqrt((v1² + v2² + ... + vn²) / N)

The algorithm:

1. Reads ADC sample values.
2. Converts ADC values into voltage.
3. Removes the DC midpoint bias.
4. Squares each AC voltage sample.
5. Calculates the mean of the squared values.
6. Takes the square root to obtain the RMS voltage.

## Simulation

A software simulation was used to test the RMS calculation.

The simulation generates a 50 Hz AC waveform and converts it into simulated 12-bit ADC samples.

### Simulation Result

```text
Expected RMS:          415.000 V
ADC Input RMS:         1.000 V
Voltage Scale Factor:  415.000
Calculated RMS:        415.003 V
RMS Test: PASS