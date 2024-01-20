

/**
 * main.c
 */
#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

#define DEBOUNCE_DELAY 1000

void delay(uint32_t count) {
    while (count--) {
        // Delay loop
    }
}

void debounceDelay(void) {
    delay(DEBOUNCE_DELAY);
}
int main(void)
{
    volatile uint32_t count;
    uint8_t currentState = 0;
    const uint8_t numStates = 3;

    //enable clock for portF
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){

    }

    //configure port F pins as output (LEDs)
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);

    //configure port F pins as input (switches)

    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_4);

    GPIOUnlockPin(GPIO_PORTF_BASE,GPIO_PIN_0);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_0);

    //set the pull up resistors for switches

    GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);

    while(1){
        if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0) {
                    debounceDelay();
                    if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0) {
                        currentState = (currentState + 1) % numStates;
                        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 1 << (currentState+1));
                     while (GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4) == 0); // Wait for button release
                    }
                }


        if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) == 0) {
            debounceDelay();
            if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) == 0) {
                currentState = (currentState - 1 + numStates) % numStates;
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 1 << (currentState+1));
                while (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) == 0); // Wait for button release
            }
        }
    }
	//return 0;
}
