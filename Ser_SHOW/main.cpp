/*
Ser SHOW
serial port lcd display

copyright (c) Bogdan Raducanu, 2015
Created: April 2015
Author: Bogdan Raducanu
bogdan@electrobob.com

www.electrobob.com


Released under GPLv3.
Please refer to LICENSE file for licensing information.
*/


#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <asf.h>
#include <spi.h>

#define GFX_ILI9341_SDT028ATFT

#include "ili9341.h"
#include "ili9341_regs.h"


#define LCD_lines 20
#define LCD_chars 24

#include "serial.h"

char router_rx_buf[64];
char router_rx_buf_ptr=0;
char router_data_ready=0;
uint32_t time;
uint8_t time_ss=0;
uint8_t time_ss_last=0;
uint8_t time_mm=0;
uint16_t time_hh=0;
uint32_t time_last_cnt=0;

char LCD_char_buffer[LCD_lines][LCD_chars];
uint8_t LCD_char_buffer_x=0;
uint8_t LCD_char_buffer_y=0;


uint16_t 	lcdx=0;
uint16_t lcdy=17;


uart ser_1(&USARTE0, 115200);
uart ser_2(&USARTD0, 115200);
ISR (USARTE0_RXC_vect){ ser_1.rxInterrupt(); }
ISR (USARTE0_DRE_vect){ ser_1.txInterrupt(); }
ISR (USARTD0_RXC_vect){ ser_2.rxInterrupt(); }
ISR (USARTD0_DRE_vect){ ser_2.txInterrupt(); }

char out_str[32];
char cmd[256];

int main(void)
{
	sysclk_init(); //this SOB turns off clock to all peripherals. Disable that "FEATURE".
	rtc_init();
	
	//Enable all interrupts
	PMIC.CTRL = PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm;
	//PMIC.CTRL =  PMIC_MEDLVLEN_bm ;
	sei();

	ser_1.sendStringPgm(PSTR("\r\r\\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\r\rSer show\r"));
	PORTC.DIRSET = 0xFB; //make sure RX is still IN why does the driver not do this ?????????????
	PORTD.DIRSET = 0x03; //why does the driver not do this ????????????? PD0, PD1 used for LCD only
	ser_1.sendStringPgm(PSTR("LCD port init\r"));
	
	gfx_ili9341_init();
	ili9341_backlight_on();
//	ser_1.sendStringPgm(PSTR("LCD init\r"));
	gfx_draw_string("\n\n\n\n\n\nwww.electrobob.com\nSer SHOW v1\nApril 2015\n\nbogdan@electrobob.com", 0, 0, &sysfont, GFX_COLOR_BLACK,  GFX_COLOR_WHITE);
	
	//_delay_ms(1000);
	
	ili9341_set_scroll(0, 320, 0); //scroll over whole screen
	
	char new_line_set =0;
	uint16_t scroll_pos=0;
	char last_serial=1;
	lcdy=304;
	while(1){
		while((ser_1.dataAvailable()>0))
		{
			char RX_char = ser_1.getChar();
			if((RX_char=='\n') | (RX_char=='\r')) {
				new_line_set=1;
				} else {
				if(new_line_set | last_serial==2){
					last_serial=1;
					new_line_set = 0;
					scroll_pos+=16; if(scroll_pos>=320) scroll_pos = scroll_pos - 320;
					ili9341_scroll_lins(scroll_pos);
					lcdy=lcdy+16; if(lcdy>=320) lcdy = lcdy - 320;
					lcdx=0;
					gfx_draw_string("                        ", 0, lcdy, &sysfont, GFX_COLOR_BLACK,  GFX_COLOR_WHITE);
					gfx_draw_char(RX_char, lcdx, lcdy, &sysfont, GFX_COLOR_BLACK,  GFX_COLOR_GREEN);
					lcdx+=10; if(lcdx >=240) {lcdx=0; new_line_set=1;} //if x overflows, new line
					
				} else {
				gfx_draw_char(RX_char, lcdx, lcdy, &sysfont, GFX_COLOR_BLACK,  GFX_COLOR_GREEN);
				lcdx+=10; if(lcdx >=240) {lcdx=0; new_line_set=1;} //if x overflows, new line
				}
			}
		}
		
		while((ser_2.dataAvailable()>0))
		{
			char RX_char = ser_2.getChar();
			if((RX_char=='\n') | (RX_char=='\r')) {
				new_line_set=1;
				} else {
				if(new_line_set | last_serial==1){
					new_line_set = 0;
					last_serial=2;
					scroll_pos+=16; if(scroll_pos>=320) scroll_pos = scroll_pos - 320;
					ili9341_scroll_lins(scroll_pos);
					lcdy=lcdy+16; if(lcdy>=320) lcdy = lcdy - 320;
					lcdx=0;
					gfx_draw_string("                        ", 0, lcdy, &sysfont, GFX_COLOR_BLACK,  GFX_COLOR_WHITE);
					gfx_draw_char(RX_char, lcdx, lcdy, &sysfont, GFX_COLOR_BLACK,  GFX_COLOR_WHITE);
					lcdx+=10; if(lcdx >=240) {lcdx=0; new_line_set=1;} //if x overflows, new line
					
					} else {
					gfx_draw_char(RX_char, lcdx, lcdy, &sysfont, GFX_COLOR_BLACK,  GFX_COLOR_WHITE);
					lcdx+=10; if(lcdx >=240) {lcdx=0; new_line_set=1;} //if x overflows, new line
				}
			}
		}
	}
}