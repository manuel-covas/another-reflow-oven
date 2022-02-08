#include <math.h>
#include <max6675.h>

#include "temperature_profiles/GenericPasteProfile.h"


// Zero crossing detection (active-low) pin number

const uint8_t zero_crossing_detect_pin = 2;

// MAX6675 Pin number connections

const uint8_t serial_clock_pin = 3;
const uint8_t chip_select_pin = 4;
const uint8_t slave_out_pin = 5;

// Temperature sensor interface (MAX6675 library by Adafruit)

MAX6675 max6675_chip = MAX6675(serial_clock_pin,
                               chip_select_pin,
                               slave_out_pin);



void setup() {

    // Set power level to zero.
    ;

    // Enable pull-up resistors on zero crossing detect and attach ISR.
    pinMode(zero_crossing_detect_pin, INPUT_PULLUP);
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


/**
 * @brief Computes and sets the on-time for each mains half-cycle that corresponds to the specified relative power.
 * @param relative_power The desired relative power. Range: 0.0 to 1.0
 **/
void setRelativePower(float relative_power) {

    // Constrain input to range.
    if (relative_power < 0) relative_power = 0;
    if (relative_power > 1) relative_power = 1;

    /*
        We will activate the TRIAC at a calculated offset from each zero-crossing in order to apply
        the target relative power.

        By Ohm's Law we have that Power = V²R which means that power is linearly porpotional to V².

        Taking mains voltage as V = sin(t*f*2π), the integral of one full half-cycle comes out as
        ∫sin(t*f*2π)² dt from zero to 1/2f and corresponds to full power.

        The definite integral ∫sin(t*f*2π)² from an offset t to the end of the half cycle is:

        P(t) = 1 - 2ft + sin(tf4π)/2π

        Finally, we can write the following:
    */

   
}


double mainsFrequency = -1;

void zeroCrossingISR() {

}