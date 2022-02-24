#include <stdio.h>
#include <math.h>
#include <max6675.h>

#include "temperature_profiles/GenericPasteProfile.h"


#define SNPRINTF_BUFFER_SIZE 500
#define FREQUENCY_MEASUREING_TIME_MS 4 * 1000


/**
 * Board Connections
**/

// Zero crossing detection (active-low) pin number

const uint8_t zero_crossing_detect_pin = PIN2;
const uint8_t triac_trigger_pin = PIN6;

// MAX6675 Pin number connections

const uint8_t serial_clock_pin = PIN3;
const uint8_t chip_select_pin = PIN4;
const uint8_t slave_out_pin = PIN5;



// Temperature sensor interface (MAX6675 library by Adafruit)

MAX6675 max6675_chip = MAX6675(serial_clock_pin, chip_select_pin, slave_out_pin);


// Mains frequency

uint16_t count;
uint16_t frequency;

// Zero-crossing ISR to measure mains frequency

void measureFrequencyISR() { count++; }


// snprintf output buffer
char sprintf_buffer[SNPRINTF_BUFFER_SIZE];

void setup() {

    // Open serial connection.
    Serial.begin(9600);
    
    // Configure pins

    Serial.print("Configuring pins...");

    pinMode(zero_crossing_detect_pin, INPUT_PULLUP); // Enable pull-up resistors on zero crossing detect.
    pinMode(triac_trigger_pin, OUTPUT);              // Set TRIAC trigger as output.
    digitalWrite(triac_trigger_pin, LOW);            // Make sure TRIAC trigger is off.

    // Measure mains frequency
    snprintf(sprintf_buffer, SNPRINTF_BUFFER_SIZE, "Measuring mains frequecy... (%f seconds)", FREQUENCY_MEASUREING_TIME_MS / 1000);

    attachInterrupt(digitalPinToInterrupt(zero_crossing_detect_pin), measureFrequencyISR, FALLING);
    delay(FREQUENCY_MEASUREING_TIME_MS);
    detachInterrupt(digitalPinToInterrupt(zero_crossing_detect_pin));

    frequency = count / (FREQUENCY_MEASUREING_TIME_MS * 2);

    // Set power level to zero
    
    attachInterrupt(digitalPinToInterrupt(zero_crossing_detect_pin), zeroCrossingISR, FALLING);

    // Open serial connection.
    Serial.begin(9600);
    
    // MAX6675 stabilize time.
    delay(500);
}

void loop() {
    
    float timeSeconds = (millis() - start_millis) / 1000.0f;

    // Print CSV line.

    Serial.print(timeSeconds);
    Serial.print(",");
    Serial.print(max6675_chip.readCelsius());
    Serial.println();

    // Wait for next reading.

    delay(250);
}


double mainsFrequency = -1;

void zeroCrossingISR() {

}