#ifndef LCD_FUNCTIONS_H
#define LCD_FUNCTIONS_H

#define GET_BIT(REG, BIT_NUM) (((REG)>>(BIT_NUM)) & 1U)

void Delay_ms(uint32_t Milliseconds);

void Delay_us(uint32_t Microseconds);

void delayMs(int n);

void SetHalfByte(uint8_t Data);

void setEnablePulse(void);

void voidSendCmd(uint8_t Cmd);

void voidSendData(uint8_t Data);

void LCD_Init(void);

void LCD_print(char *string);


#endif
