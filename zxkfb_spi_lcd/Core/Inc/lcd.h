#ifndef _LCD_H_
#define _LCD_H_

#include "st7735.h"
extern ST7735_Object_t st7735;

int32_t st7735_write(void *ctx, uint8_t reg, uint8_t* data, uint32_t len);
int32_t st7735_read(void *ctx, uint8_t reg, uint8_t* data);
int32_t st7735_send(void *ctx, uint8_t* data, uint32_t len);
int32_t st7735_recv(void *ctx, uint8_t* data, uint32_t len);

void st7735_init(void);
void luat_lcd_fill(uint16_t color);
#endif
