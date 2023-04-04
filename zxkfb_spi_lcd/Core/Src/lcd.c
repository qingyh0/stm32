#include "stdio.h"
#include "stm32f1xx_hal.h"
#include "string.h"
#include "lcd.h"

extern SPI_HandleTypeDef hspi2;

void luat_lcd_fill(uint16_t color);


//向reg写入数据
int32_t st7735_write(void *ctx, uint8_t reg, uint8_t* data, uint32_t len)
{
	uint8_t buf[256]={reg,};
	
	memcpy(buf+1, data, len);
	
	HAL_StatusTypeDef ret = HAL_SPI_Transmit(&hspi2, buf, len+1, 1000);
	return (int32_t)ret;
}

//接收数据 4字节
int32_t st7735_read(void *ctx, uint8_t reg, uint8_t* data)
{
	st7735_write(ctx, reg, NULL, 0);
	
	HAL_StatusTypeDef ret = HAL_SPI_Receive(&hspi2, data, 4, 1000);
	
	return (int32_t)ret;
}

int32_t st7735_send(void *ctx, uint8_t* data, uint32_t len)
{
	//HAL_StatusTypeDef ret = HAL_SPI_Transmit(&hspi2, data, len, 1000);
	HAL_StatusTypeDef ret = HAL_SPI_Transmit_DMA(&hspi2, data, len);
	return (int32_t)ret;
}


int32_t st7735_recv(void *ctx, uint8_t* data, uint32_t len)
{
	HAL_StatusTypeDef ret = HAL_SPI_Receive(&hspi2, data, len, 1000);
	
	return (int32_t)ret;
}

ST7735_Object_t st7735_lcd ={
	.Ctx.WriteReg = st7735_write,
	.Ctx.ReadReg = st7735_read,
	.Ctx.SendData = st7735_send,
	.Ctx.RecvData = st7735_recv
};


//需要手动控制cs
void lcd_write_cmd(uint8_t cmd)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
	st7735_send(&st7735, &cmd, 1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
}
//需要手动控制cs
void lcd_write_data(uint8_t cmd)
{
	st7735_send(&st7735, &cmd, 1);
}


void st7735_init(void)
{
	//SCREEN_CS_1;	A9
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);

	//SCREEN_BL_0;	A8
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

	//SCREEN_DC_1;	A10
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
	//SCREEN_RST_0;	A11
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_Delay(100);
	//SCREEN_RST_1;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
	HAL_Delay(120);

	//SCREEN_CS_0;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
    lcd_write_cmd(0x11);

    HAL_Delay(120);//ms

    lcd_write_cmd(0x20);//lcd_inv_off如果颜色错了就用0x21

    lcd_write_cmd(0xB1);
    lcd_write_data(0x05);
    lcd_write_data(0x3A);
    lcd_write_data(0x3A);

    lcd_write_cmd(0xB2);
    lcd_write_data(0x05);
    lcd_write_data(0x3A);
    lcd_write_data(0x3A);

    lcd_write_cmd(0xB3);
    lcd_write_data(0x05);
    lcd_write_data(0x3A);
    lcd_write_data(0x3A);
    lcd_write_data(0x05);
    lcd_write_data(0x3A);
    lcd_write_data(0x3A);

    lcd_write_cmd(0xB4);//Dotinversion
    lcd_write_data(0x03);

    lcd_write_cmd(0xC0);
    lcd_write_data(0x62);
    lcd_write_data(0x02);
    lcd_write_data(0x04);

    lcd_write_cmd(0xC1);
    lcd_write_data(0xC0);

    lcd_write_cmd(0xC2);
    lcd_write_data(0x0D);
    lcd_write_data(0x00);

    lcd_write_cmd(0xC3);
    lcd_write_data(0x8D);
    lcd_write_data(0x6A);

    lcd_write_cmd(0xC4);
    lcd_write_data(0x8D);
    lcd_write_data(0xEE);

    lcd_write_cmd(0xC5);//VCOM
    lcd_write_data(0x0E);

    lcd_write_cmd(0xE0);
    lcd_write_data(0x10);
    lcd_write_data(0x0E);
    lcd_write_data(0x02);
    lcd_write_data(0x03);
    lcd_write_data(0x0E);
    lcd_write_data(0x07);
    lcd_write_data(0x02);
    lcd_write_data(0x07);
    lcd_write_data(0x0A);
    lcd_write_data(0x12);
    lcd_write_data(0x27);
    lcd_write_data(0x37);
    lcd_write_data(0x00);
    lcd_write_data(0x0D);
    lcd_write_data(0x0E);
    lcd_write_data(0x10);

    lcd_write_cmd(0xE1);
    lcd_write_data(0x10);
    lcd_write_data(0x0E);
    lcd_write_data(0x03);
    lcd_write_data(0x03);
    lcd_write_data(0x0F);
    lcd_write_data(0x06);
    lcd_write_data(0x02);
    lcd_write_data(0x08);
    lcd_write_data(0x0A);
    lcd_write_data(0x13);
    lcd_write_data(0x26);
    lcd_write_data(0x36);
    lcd_write_data(0x00);
    lcd_write_data(0x0D);
    lcd_write_data(0x0E);
    lcd_write_data(0x10);

    lcd_write_cmd(0x3A);
    lcd_write_data(0x05);

	//direction
    lcd_write_cmd(0x36);
    lcd_write_data(0x78);

    lcd_write_cmd(0x29);

    HAL_Delay(100);

	//SCREEN_CS_1;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);

    luat_lcd_fill(0x00);
	//SCREEN_BL_1;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
}

int luat_lcd_set_address(uint16_t x1,uint16_t y1, uint16_t x2, uint16_t y2) {
    lcd_write_cmd(0x2a);
    lcd_write_data((x1+0)>>8);
    lcd_write_data(x1+0);
    lcd_write_data((x2+0)>>8);
    lcd_write_data(x2+0);
    lcd_write_cmd(0x2b);
    lcd_write_data((y1+24)>>8);
    lcd_write_data(y1+24);
    lcd_write_data((y2+24)>>8);
    lcd_write_data(y2+24);
    lcd_write_cmd(0x2C);
    return 0;
}

void luat_lcd_fill(uint16_t color)
{
	uint32_t i;
	uint8_t c1 = *((uint8_t*)&color), c2 = *((uint8_t*)&color + 1);
	//SCREEN_CS_0;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
    luat_lcd_set_address(0,0,160-1,80-1);
	for(i = 0; i < 160 * 80; i++)
	{
		lcd_write_data(c1);
		lcd_write_data(c2);
	}
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
}
