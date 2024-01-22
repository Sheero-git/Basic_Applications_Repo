/**
 * main.c
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
void displayDigit(uint8_t digit) {
    const uint8_t sevenSegmentCodes[] = {
        0x3F, // 0
        0x06, // 1
        0x5B, // 2
        0x4F, // 3
        0x66, // 4
        0x6D, // 5
        0x7D, // 6
        0x07, // 7
        0x7F, // 8
        0x6F  // 9
    };

    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, sevenSegmentCodes[digit]);
}
int main(void)
{
    volatile uint32_t delay;
    volatile uint32_t i;
    unsigned char currState, prevState, currState2, prevState2;
    prevState = 0;
    currState = 0;
    prevState2 = 0;
    currState2 = 0;
    volatile uint32_t count = 0;
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){}
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)){}
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, 0xFF);
    GPIOUnlockPin(GPIO_PORTF_BASE,  GPIO_PIN_0);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOUnlockPin(GPIO_PORTF_BASE,  GPIO_PIN_4);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    while(1){
        if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) == 0 && count<10) {
            for(i=0; i<50; i++){
                if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) == 0 && count<10){
                    currState = 1;
                }
            }
        } else {
            currState = 0;
        }
        if(!currState && prevState){
            displayDigit(count);
            count++;
        }
        prevState = currState;
        for(delay=0; delay<50000; delay++){}
        /////////////////////////
        if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0 && count>0) {
            for(i=0;i<50;i++){
                if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0 && count>0){
                    currState2 = 1;
                }
            }
        } else {
            currState2 = 0;
        }
        if(!currState2 && prevState2){
            count--;
            displayDigit(count);
        }
        prevState2 = currState2;
    }
    return 0;
}
