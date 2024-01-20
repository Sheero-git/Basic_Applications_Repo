#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h" 
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/tm4c123gh6pm.h"
#define SIMULATION_DELAY 1000000
// function for PWM
// function to play Sound

unsigned static int frequency = 5000000;

void toggleBuzzerPin(void) {
    static bool buzzerState = false; // Static variable to keep track of the buzzer state
    buzzerState = !buzzerState; // Toggle the state
    int x;
    // Set the buzzer pin based on the state
   // GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, buzzerState ? GPIO_PIN_1 : 0);
    for (x = 0; x < 10; x++)
                    {
                        GPIO_PORTF_DATA_R |= GPIO_PIN_1; // Set
                        SysCtlDelay(frequency/2000); // Delay
                        GPIO_PORTF_DATA_R &= 0xFD;; // Clear 
                        SysCtlDelay(frequency/2000); // Delay
                    }
    
}
int main(void) {
    unsigned int counter = 0;
    // Set the system clock to 50MHz
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
                           
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = 0xFF;
    // Configure switches as inputs
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    // configure output
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);

    while (1) {
        //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
      if (counter == 0)
      {
        GPIO_PORTF_DATA_R &= 0x00; // initialize Port F (simulation)
        GPIO_PORTF_DATA_R |= GPIO_PIN_0;
      }
      else
      {
          if (counter == 1)
          {
            GPIO_PORTF_DATA_R &= 0xFE; // clear pin 0
            GPIO_PORTF_DATA_R = GPIO_PIN_4;
          }
          else{
              GPIO_PORTF_DATA_R &= 0x0E;; // clear 0 and 4
          }
      }
        // Check if the first switch (SW1) is pressed
        if (GPIO_PORTF_DATA_R & GPIO_PIN_0) {
            frequency += 5000;
            toggleBuzzerPin();
        }
        // Check if the second switch (SW2) is pressed
        else
        {
          if (GPIO_PORTF_DATA_R & GPIO_PIN_4) {
            frequency > 5000 ? frequency-= 5000 : frequency;
            toggleBuzzerPin();
            }
        else {
            // No switch is pressed, simulate turning off the buzzer
            GPIO_PORTF_DATA_R &= 0x00; //OFF
              }
        }
        counter++;
        // Introduce a delay for simulation purposes
        SysCtlDelay(SIMULATION_DELAY);
    }
}