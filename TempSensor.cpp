#include <Arduino.h>
#include "TempSensor.h"

const int thermistorPin = A2; // Analog pin for temperature sensor
const int thermistorPin2 = A3;
const int BETA = 3950; // B coefficient from datasheet
const int THERMISTOR = 10000; // Resistance at 25°C (10K)
const int R1 = 22000; // Voltage divider resistance
const int TEMPNOMINAL = 25.00; // 25°C
const float V_supply = 5.0;

const bool apply_averaging = true;
const float averaging_pow = 0.99;
float last_temp = apply_averaging ? -11111.0 : 0.0;
float last_temp2 = apply_averaging ? -11111.0 : 0.0;

void loopTempSensor() {
int sensorValue = analogRead(thermistorPin); // Convert ADC reading to resistance
float V_out = sensorValue * ( V_supply / 1023.0); // Convert ADC value to voltage
float R3 = (R1 * V_out) / (V_supply - V_out); // in ohms
float tempK = R3 / THERMISTOR;
tempK = log(tempK);
tempK /= BETA;
tempK += 1.0 / (TEMPNOMINAL + 273.15);
tempK = 1.0 / tempK;
float tempC = tempK - 273.15;

if (apply_averaging) {
if (last_temp == -11111.0) { last_temp = tempC; }
tempC = (last_temp * averaging_pow) + (tempC * (1.0 - averaging_pow));
last_temp = tempC;
}

float tempF = (tempC * 9.0/5.0) + 32.0;

int sensorValue2 = analogRead(thermistorPin2); // Convert ADC reading to resistance
float V_out2 = sensorValue2 * ( V_supply / 1023.0); // Convert ADC value to voltage
float R32 = (R1 * V_out2) / (V_supply - V_out2); // in ohms
float tempK2 = R32 / THERMISTOR;
tempK2 = log(tempK2);
tempK2 /= BETA;
tempK2 += 1.0 / (TEMPNOMINAL + 273.15);
tempK2 = 1.0 / tempK2;
float tempC2 = tempK2 - 273.15;

if (apply_averaging) {
if (last_temp2 == -11111.0) { last_temp2 = tempC2; }
tempC2 = (last_temp2 * averaging_pow) + (tempC2 * (1.0 - averaging_pow));
last_temp2 = tempC2;
}

float tempF2 = (tempC2 * 9.0/5.0) + 32.0;
}