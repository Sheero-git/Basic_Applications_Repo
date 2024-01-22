#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/tm4c123gh6pm.h"

#define POSITIVE_PIN     GPIO_PIN_1
#define NEGATIVE_PIN     GPIO_PIN_2
#define BUTTON_RIGHT_PIN GPIO_PIN_3
#define BUTTON_LEFT_PIN  GPIO_PIN_4

volatile int RIGHT_PRESSED = 0;
volatile int LEFT_PRESSED  = 0;

void setup() {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, POSITIVE_PIN );
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, NEGATIVE_PIN);
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;//<----
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = 0xFF;//<----
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, BUTTON_LEFT_PIN);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, BUTTON_RIGHT_PIN);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);//<----

}

void turnMotorClockwise() {
    GPIOPinWrite(GPIO_PORTF_BASE, POSITIVE_PIN | NEGATIVE_PIN , POSITIVE_PIN);
    LEFT_PRESSED = 0;

}

void turnMotorCounterclockwise() {
    GPIOPinWrite(GPIO_PORTF_BASE, POSITIVE_PIN | NEGATIVE_PIN , NEGATIVE_PIN);
    RIGHT_PRESSED = 0;

}
void Delay(unsigned int delay)
{
    volatile unsigned int i, counter;
    counter = delay * 4000;  // 1 second (1000 msec) needs 40000000 counter so 4000000/1000 = 4000
    for(i=0;i<counter;i++);
}
int main(void) {
    setup();
    while (1) {
        RIGHT_PRESSED = GPIOPinRead(GPIO_PORTF_BASE, BUTTON_RIGHT_PIN);
        LEFT_PRESSED = GPIOPinRead(GPIO_PORTF_BASE, BUTTON_LEFT_PIN);
        if (RIGHT_PRESSED==1) {
            Delay(2);
            RIGHT_PRESSED = GPIOPinRead(GPIO_PORTF_BASE, BUTTON_RIGHT_PIN);
            if (RIGHT_PRESSED==1) {
               turnMotorClockwise();
             }

        } else if (LEFT_PRESSED==1) {
            Delay(2);
            if (LEFT_PRESSED==1){
            LEFT_PRESSED = GPIOPinRead(GPIO_PORTF_BASE, BUTTON_LEFT_PIN);
            turnMotorCounterclockwise();
            }
        }
    }
    return 0;
}
