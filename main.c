#include<stdint.h>
#include "inc/tm4c123gh6pm.h"
int main(void)
{
volatile uint32_t ui32Loop;
//Enable the GPIO port (used for LED)
SYSCTL_RCGC2_R=SYSCTL_RCGC2_GPIOF;
//Do a dummy read for inserting a few cycles after enabling peripheral.
ui32Loop=SYSCTL_RCGC2_R;
//Enble GPIO pins for output (LED)
GPIO_PORTF_DIR_R=0x08;
//Enble GPIO pins for Digital functionality
GPIO_PORTF_DEN_R=0x08;
// forever loop
while(1)
{
// trun on greeen LED
GPIO_PORTF_DATA_R|=0x08;
//Delay
for(ui32Loop=0;ui32Loop<200000; ui32Loop++)
{
}
//Turn off LED
GPIO_PORTF_DATA_R&=~(0x08);
//Delay
for(ui32Loop=0;ui32Loop<200000; ui32Loop++)
{
}
}
}
