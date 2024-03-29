#include <stdio.h>
#include <math.h>
#include <max6675.h>
#include <avr/io.h>

#include "src/triac_phase_control/TimerISR.h"
#include "src/temperature_profiles/GenericPasteProfile.h"
#include "src/pid_control/PIDControl.h"
#include "src/triac_phase_control/PowerLevel.h"


#define SNPRINTF_BUFFER_SIZE 500
#define FREQUENCY_MEASUREING_TIME_MS 4 * 1000
#define TRIAC_PULSE_DURATION_US 250

// snprintf output buffer
char sprintf_buffer[SNPRINTF_BUFFER_SIZE];


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

volatile uint16_t count = 0;
float mains_frequency;

void measureFrequencyISR() { count++; } // Counter ISR


// Triac phase control

volatile unsigned int zero_crossing_delay_us = 0;
volatile bool no_power = true;

void setPower(float new_power_level) {
    if (new_power_level <= 0) {
        no_power = true;
    }else{
        no_power = false;
        zero_crossing_delay_us = calculateDelay(new_power_level, mains_frequency);
    }
}


void triacPhaseControlISR() {
    
    // Do not trigger triac
    if (no_power) return;

    // Schedule triac activation
    Timer1ISR::setHandler(&startTriacPulseISR);
    Timer1ISR::scheduleOneShot(zero_crossing_delay_us);
}


// Starts triac pulse

void startTriacPulseISR() {
    
    // Set pin high.
    digitalWrite(triac_trigger_pin, HIGH);

    // Schedule end of pulse.
    Timer1ISR::setHandler(&endTriacPulseISR);
    Timer1ISR::scheduleOneShot(TRIAC_PULSE_DURATION_US);
}


// Ends triac pulse

void endTriacPulseISR() {
    digitalWrite(triac_trigger_pin, LOW);  // Set pin low.
}


PIDControl pidControl(0.5, 0, -2.25, 10);     // PID Control instance
GenericPasteProfile temperature_profile;  // Temperature profile instance


unsigned long start_millis;

void setup() {

    // Configure Timer1
    Timer1ISR::setup();

    // Open serial connection
    Serial.begin(9600);
    
    // Configure pins
    
    pinMode(zero_crossing_detect_pin, INPUT_PULLUP); // Enable pull-up resistors on zero crossing detect.
    pinMode(triac_trigger_pin, OUTPUT);              // Set TRIAC trigger as output.
    digitalWrite(triac_trigger_pin, LOW);            // Make sure TRIAC trigger is off.

    // Measure mains frequency

    Serial.print("Measuring mains frequecy... (");
    Serial.print(FREQUENCY_MEASUREING_TIME_MS / 1000);
    Serial.println(" seconds)");
    
    attachInterrupt(digitalPinToInterrupt(zero_crossing_detect_pin), measureFrequencyISR, FALLING);
    delay(FREQUENCY_MEASUREING_TIME_MS);
    detachInterrupt(digitalPinToInterrupt(zero_crossing_detect_pin));

    mains_frequency = ((float) 1000 * count) / (2 * FREQUENCY_MEASUREING_TIME_MS);

    Serial.print("Measured mains frequecy: ");
    Serial.print(mains_frequency);
    Serial.println("Hz");

    // Set power to zero
    setPower(0);
    
    // Attach triac phase control ISR
    attachInterrupt(digitalPinToInterrupt(zero_crossing_detect_pin), triacPhaseControlISR, FALLING);
    
    // MAX6675 initialization time
    delay(500);
    
    Serial.println("Waiting...");
    delay(3 * 1000);
    Serial.println("Starting profile.");
    Serial.println("time_ms,temperature,target_temperature,p_component,i_component,d_component,target_power");

    start_millis = millis();
}


void loop() {
    
    unsigned long time_ms = millis() - start_millis;
    float temperature = max6675_chip.readCelsius();
    float target_temperature = temperature_profile.computeTemperature(time_ms);

    // Update PID target value.
    pidControl.setTarget(target_temperature);

    // Update TRIAC phase control.
    float target_power = pidControl.iterate(time_ms, temperature);
    setPower(target_power);

    // Print CSV line.
    Serial.print(time_ms);
    Serial.print(",");
    Serial.print(temperature);
    Serial.print(",");
    Serial.print(target_temperature);
    Serial.print(",");
    Serial.print(pidControl.getPvalue());
    Serial.print(",");
    Serial.print(pidControl.getIvalue());
    Serial.print(",");
    Serial.print(pidControl.getDvalue());
    Serial.print(",");
    Serial.print(target_power);
    Serial.println();
    
    // Wait for next reading.
    delay(250);
}