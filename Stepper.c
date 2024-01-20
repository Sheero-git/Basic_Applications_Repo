#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/tm4c123gh6pm.h"

#define GPIO_PORTF_CLK_EN  0x20    //clock enable of PORTF
#define GPIO_PORTB_CLK_EN  0x02    //clock enable of PORTB
#define GPIO_PORTF_PIN1_EN 0x02    //Enable RED LED
#define GPIO_PORTF_PIN2_EN 0x04    //Enable BLUE LED
#define GPIO_PORTB_PIN0_DIR 0x01    //Direction Pin for Stepper
#define GPIO_PORTB_PIN1_STEP 0x02 //Step Pin for Stepper
#define GPIO_PORTF_PIN4_IN 0xEF    //Make SW1 Input
#define GPIO_PORTF_PIN4_EN 0x10    //Enable SW1
#define GPIO_PORTF_PIN0_IN 0xFE   //Make SW1 Input
#define GPIO_PORTF_PIN0_EN 0x01    //Enable SW2
#define GPIO_PORTF_LOCK_UNLOCK 0x4C4F434B //Unlock Port F
#define GPIO_PORTF_COMMIT_EN 0x01
#define GPIO_PORTF_OFF 0x00
#define stepsPerRevolution 200

void Delay(unsigned int); //delay in ms

int Leds(void)
{
    volatile unsigned int x;
    SYSCTL_RCGCGPIO_R |= GPIO_PORTF_CLK_EN;   //enable clock of PORTF
    SYSCTL_RCGCGPIO_R |= GPIO_PORTB_CLK_EN;   //enable clock of PORTB
    GPIO_PORTF_LOCK_R = GPIO_PORTF_LOCK_UNLOCK;   //unlock GPIO of PORTF
    GPIO_PORTF_CR_R = GPIO_PORTF_COMMIT_EN;       //Enable GPIOPUR register enable to commit
    GPIO_PORTF_PUR_R |= GPIO_PORTF_PIN4_EN;   //Enable Pull Down SW1 , SW2
    GPIO_PORTF_PUR_R |= GPIO_PORTF_PIN0_EN;
    GPIO_PORTF_DIR_R |= GPIO_PORTF_PIN1_EN;   //Make PF1 as output , PF4 input by default
    GPIO_PORTF_DIR_R |= GPIO_PORTF_PIN2_EN;
    GPIO_PORTB_DIR_R |= GPIO_PORTB_PIN0_DIR;
    GPIO_PORTB_DIR_R |= GPIO_PORTB_PIN1_STEP;
    GPIO_PORTF_DIR_R &= GPIO_PORTF_PIN4_IN; //Make PF4,PF0 inputs
    GPIO_PORTF_DIR_R &= GPIO_PORTF_PIN0_IN;
    GPIO_PORTF_DEN_R |= GPIO_PORTF_PIN1_EN;
    GPIO_PORTF_DEN_R |= GPIO_PORTF_PIN2_EN;
    GPIO_PORTF_DEN_R |= GPIO_PORTF_PIN4_EN;
    GPIO_PORTF_DEN_R |= GPIO_PORTF_PIN0_EN;
    GPIO_PORTB_DEN_R |= GPIO_PORTB_PIN0_DIR;
    GPIO_PORTB_DEN_R |= GPIO_PORTB_PIN1_STEP; //enable PF1 and PF4 pins as digital GPIO
    while(1)
    {
        if(!(GPIO_PORTF_DATA_R & GPIO_PORTF_PIN4_EN))
        {
            Delay(50);
            if(!(GPIO_PORTF_DATA_R & GPIO_PORTF_PIN4_EN))
            {
                GPIO_PORTF_DATA_R |= GPIO_PORTF_PIN2_EN;
                GPIO_PORTB_DATA_R |= GPIO_PORTB_PIN0_DIR; //setting dir pin
                for(x = 0; x < stepsPerRevolution; x++)
                {
                    GPIO_PORTB_DATA_R |= GPIO_PORTB_PIN1_STEP;
                    Delay(2);
                    GPIO_PORTB_DATA_R &= 0xFD;
                    Delay(2);
                }
            }
        }
        else
        {
            if(!(GPIO_PORTF_DATA_R & GPIO_PORTF_PIN0_EN))
            {
                Delay(50);
                if(!(GPIO_PORTF_DATA_R & GPIO_PORTF_PIN0_EN))
                {
                    GPIO_PORTF_DATA_R |= 0x02;
                    GPIO_PORTB_DATA_R &= 0xFE; //reversing Dir
                    for(x = 0; x < stepsPerRevolution; x++)
                    {
                        GPIO_PORTB_DATA_R |= GPIO_PORTB_PIN1_STEP;
                        Delay(5);
                        GPIO_PORTB_DATA_R &= 0xFD;
                        Delay(5);
                    }
                }
            }
            else
            {
                GPIO_PORTF_DATA_R &= GPIO_PORTF_OFF;
                GPIO_PORTB_DATA_R &= 0xDF;
                GPIO_PORTB_DATA_R &= 0xBF;
            }
        }
    }
}

void Delay(unsigned int delay)
{
    volatile unsigned int i, counter;
    counter = delay * 4000;  // 1 second (1000 msec) needs 40000000 counter so 4000000/1000 = 4000
    for(i=0;i<counter;i++);
}
