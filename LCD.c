#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

#include "LCD_Functions (1).h"



int main(void)
{

    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    //Enable Port B Clock
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))
    {
    }

    //Enable Port D Clock
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD))
    {
    }

    //Enable Port E Clock
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE))
    {
    }

    //Enable Port A Clock
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
    {
    }

    //Enable Port F Clock
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }

    /*Initialize the GPIO Lock Register to unlock the commit register*/
    GPIOUnlockPin(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);

    //Set PF0, PF4 as Digital Input pins
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);

    //Sets PF0 & PF4 internal pull-up
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //Set PB0, PB1, PB4 as Digital Output pins
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4);

    //Set PD0, PD2 as Digital Output pins
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_2);

    //Set PE4, PE5 as Digital Output pins
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4|GPIO_PIN_5);

    //Set PA5, PA6, PA7 as Digital Output pins
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);

    LCD_Init();

    while(1)
    {
        if ((GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) == 0) && (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0)) // SW1 & SW2 are Pressed
        {
            Delay_ms(100);
            if ((GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) == 0) && (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0))
            {
                voidSendCmd(1);//clear LCD
            }
        }
        else if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0)// SW2 Pressed
        {
            Delay_ms(100);
            if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0){
                voidSendCmd(1);//clear LCD
                LCD_print("Mariouma");
            }
        }
        else if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) == 0){//SW1 Pressed
            Delay_ms(100);
            if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) == 0)
            {
                voidSendCmd(1);//clear LCD
                LCD_print("Hamada");
            }
        }
    }

}


void delayMs(int n) {
    SysCtlDelay((SysCtlClockGet()/(3*1000))*n);
}

void Delay_ms(uint32_t Milliseconds){
    volatile uint32_t i,j;

    for(i=0;i<Milliseconds;i++){
        for(j=0;j<3180;j++){

        }
    }
}

void Delay_us(uint32_t Microseconds){
    volatile uint32_t i,j;

    for(i=0;i<Microseconds;i++){
        for(j=0;j<3;j++){

        }
    }
}

void SetHalfByte(uint8_t Data){ /*4-bit Mode*/

    GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_5,(Data & 1u)<<5);
    GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_4,((Data>>1) & 1u)<<4);
    GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_1,((Data>>2) & 1u)<<1);
    GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0,((Data>>3) & 1u)<<0);

    /*GPIO_PORTE_DATA_R |= ((GET_BIT(Data,0))<<5u) | ((GET_BIT(Data,1))<<4u);
    GPIO_PORTB_DATA_R |= ((GET_BIT(Data,2))<<1u) | ((GET_BIT(Data,3))<<0);*/

}

void SetByte(uint8_t Data){ /*8-bit mode*/

    GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0,((Data>>7) & 1)<<0);
    GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_1,((Data>>6) & 1u)<<1);
    GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_4,((Data>>5) & 1u)<<4);
    GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_5,((Data>>4) & 1u)<<5);

    GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4,((Data>>3) & 1u)<<4);
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_5,((Data>>2) & 1u)<<5);
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_6,((Data>>1) & 1u)<<6);
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_7,((Data & 1u)<<7));

}

void setEnablePulse(void){
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2,GPIO_PIN_2);
    //GPIO_PORTD_DATA_R |= GPIO_PIN_2;
    Delay_ms(3);
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0);
    //GPIO_PORTD_DATA_R &= ~(GPIO_PIN_2);
}

void voidSendCmd(uint8_t Cmd){

    /*Send 4 MSB bits followed by Least Significant Bits*/
    // SetHalfByte(Cmd>>4);
    // GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0);
    // setEnablePulse();
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0);

    SetByte(Cmd);

    setEnablePulse();

    /*Set the RS Pin to LOW*/
    //GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0);
    //GPIO_PORTD_DATA_R &= ~(GPIO_PIN_0);
}

void LCD_print(char *string) {
    while (*string != '\0') {
        voidSendData(*string);
        string++;
    }
}


void voidSendData(uint8_t Data){

    /*Send 4 MSB bits followed by Least Significant Bits*/
    // SetHalfByte(Data>>4);
    // GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 1);
    // setEnablePulse();
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 1);

    SetByte(Data);

    setEnablePulse();

    /*Set the RS Pin to HIGH*/
    //GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 1);
    //GPIO_PORTD_DATA_R |= GPIO_PIN_0;

}

void LCD_Init(void){

    Delay_ms(50);

    voidSendCmd(0b00111000);


    voidSendCmd(0b00001100);


    voidSendCmd(0b00000001);


    voidSendCmd(0b00000110);

}

