#include "avr/io.h"
#include "avr/interrupt.h"
#include "TimerISR.h"
#include <HardwareSerial.h>


const float CPU_CLOCK_PERIOD_US = 1000000 * (float) 1 / F_CPU;

const uint16_t CLOCK_PRESCALER_VALUES[5] = {
    1, 8, 64, 256, 1024
};


// Pointer to interrupt handler
void (*isr_handler)();


/**
 * @brief Initialize Timer1 configuration and reset it.
 */
void Timer1ISR::setup() {

    // COM1A/COM1B: 0 (disable output compare pins)
    // WGM1: 4 (select Clear Timer on Compare match with register OCR1A mode)
    // ICNC1: 0 (disable Input Capture Noise Canceler)
    // ICES1: 0 (Input Capture Edge Select not relevant)
    // CS1: 0 (stop timer by selecting no clock source)
    // ICIE1: 0 (disable Input Capture interrupts)
    // OCIE1B: 0 (disable Output Compare Unit B interrupts)
    // OCIE1A: 1 (enable Output Compare Unit A interrupts)
    // TOIE1: 0 (disable Timer1 Overflow interrupts)

    // Disable OC1A/OC1B and clear WGM1 bits 1:0
    TCCR1A = 0;

    // Set WGM1 bit 2, clear WGM1 bit 3 and disable ICNC1, ICES1 and the clock source
    TCCR1B = (1 << WGM12);

    // Clear timer
    TCNT1 = 0;
    
    // Enable interrupts
    TIMSK1 = (1 << OCIE1A);
}


/**
 * @brief Set the ISR handler to be scheduled.
 * 
 * @param handler the ISR handler
 */
void Timer1ISR::setHandler(void (*handler)()) {
    isr_handler = handler;
}


/**
 * @brief Use Timer1 to generate an interrupt and call the configured ISR handler once.
 *  
 * @param delay_us delay to schedule ISR for in microseconds
 */
void Timer1ISR::scheduleOneShot(unsigned int delay_us) {

    uint8_t CS_BITS = 1;

    // Check for overflows and set clock prescaling accordingly

    uint64_t timer_target_count;
    
    while (true) {
        
        timer_target_count = delay_us / (CPU_CLOCK_PERIOD_US * CLOCK_PRESCALER_VALUES[CS_BITS - 1]);

        if (timer_target_count <= 0xFFFF) {
            break;  // No prescaling required
        }else{

            // Overflow detected, step up prescaler and retry

            // Check if max prescaling has been reached
            if (CS_BITS == 0b101) {
                Serial.println("// TODO: Chain multiple calls of handleISR() together in order to count unlimitedly.\n\r         Contribute if you can :)\n");
                Serial.println("!  Timer 1 isn't large enough to count up to this value! (maybe split into smaller intervals?)");
                Serial.println("!  Refer to the ATmega328P datasheet, page 110, Table 15-6 (Clock Select Bit Description)");
                Serial.println("!  No interrupt was scheduled.\n");
                return;
            }

            // Increment Counter Select value and loop to recalculate
            CS_BITS++;
        }
    }
    
    // Set target value for Output Compare Unit A
    OCR1A = timer_target_count;

    // Reset Timer1
    TCNT1 = 0;

    // Start timer
    TCCR1B = (TCCR1B | (CS_BITS << CS10));
}


/**
 * @brief Intermediate interrupt handler, stops timer and calls scheduled handler.
 */
void Timer1ISR::handleISR() {
    
    // Stop Timer1 while keeping WGM
    TCCR1B = (1 << WGM12);

    // Call scheduled handler
    (*(isr_handler))();
}


// Real Timer1 Compare Match A ISR
ISR(TIMER1_COMPA_vect) {
    Timer1ISR::handleISR();
}