#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/tm4c123gh6pm.h"

#define STEPS_PER_REVOLUTION 200 // Replace this with the actual steps per revolution for your motor
#define DELAY 4000000

int mainTivaware(void)
{
    volatile unsigned int x;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // Enable clock of PORTF
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); // Enable clock of PORTB

    // Unlock and commit for PORTF
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = 0xFF;

    // Set up pins for SW1 and SW2
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Set up pins for LED
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2);

    // Set up pins for Stepper
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    while (1)
    {
            if (!(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) & GPIO_PIN_4))
            {
                SysCtlDelay(SysCtlClockGet() / 30); // Delay for 50ms
                if (!(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) & GPIO_PIN_4))
                {
                    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2, GPIO_PIN_2); // Turn on BLUE LED
                    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, GPIO_PIN_0); // Set DIR pin

                    for (x = 0; x < STEPS_PER_REVOLUTION; x++)
                    {
                        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, GPIO_PIN_1); // Set STEP pin
                        SysCtlDelay(DELAY); // Delay for 2ms
                        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0x00); // Clear STEP pin
                        SysCtlDelay(DELAY); // Delay for 2ms
                    }
                }
            }
            else
            {
                if (!(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) & GPIO_PIN_0))
                {
                    SysCtlDelay(SysCtlClockGet() / 30); // Delay for 50ms
                    if (!(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) & GPIO_PIN_0))
                    {
                        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2, GPIO_PIN_2); // Turn on BLUE LED
                        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0x00); // Clear DIR pin

                        for (x = 0; x < STEPS_PER_REVOLUTION; x++)
                        {
                            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, GPIO_PIN_1); // Set STEP pin
                            SysCtlDelay(DELAY); // Delay for 5ms
                            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0x00); // Clear STEP pin
                            SysCtlDelay(DELAY); // Delay for 5ms
                        }
                    }
                }
                else
                {
                    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2, 0x00); // Turn off LEDs
                    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x00); // Clear DIR and STEP pins
                }
            }
        }
    }
