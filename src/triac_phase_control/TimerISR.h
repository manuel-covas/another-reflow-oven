#ifndef TIMER_ISR_H
#define TIMER_ISR_H

class Timer1ISR {
    public:
        static void setup();
        static void setHandler(void (*handler)());
        static void scheduleOneShot(unsigned int delay_us);
        static void handleISR();

    private:
        Timer1ISR()  {};
        ~Timer1ISR() {};
};

#endif