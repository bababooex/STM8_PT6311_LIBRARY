#ifndef STM8_PT6311_H_
#define STM8_PT6311_H_

#include "stm8s_delay.h"
#include "stm8s.h"
#include <stdbool.h>

/*
 * stm8_pt6311(pt6315).h
 *
 *  Created on: 22. 6. 2026
 *      Author: Adam Fucik
 *  
 */

/*
  14 segment driver for PT6311/PT6315 or equivalents
  V1.0.1
	
  Library needs external _delay_us() function (like in "delay.h" library). Digit positions start from 1!
 
  How to use this?
	1. Set your chip type and verify your digits correctly - try to init display via pt6311_init(DIG_COUNT);
		And then test, correct digit mapping is when digits start from left not missing any digit or full digits
		with this function: pt6311_test_digit_positions(DIG_COUNT, 400); this will turn on every segment of each digit
	2.  Because every manufacturer makes this different, you need to map each segment yourself!
			Use this function for that: pt6311_test_segments(1 for example, 400) and observe each, start from 0 and map each segment to this mapping for 14 segments
 			Two shifted + places to right will blink so you can count ok

   Typical 14 segment layout
	 
	   +--- A ---+

       |\   |   /|
       F  H I J  B

       |   \|/   |
       +-G1-+-G2-+

       |   /|\   |
       E  K L M  C

       |/   |   \/
       +--- D ---+    . DP
*/

//choose variant, only difference is in more digits
//#define PT6311
#define PT6315

// define your own pinout.
#define PT6311_DIN_PORT   GPIOD
#define PT6311_DIN_PIN    GPIO_PIN_4
#define PT6311_CLK_PORT   GPIOD
#define PT6311_CLK_PIN    GPIO_PIN_6
#define PT6311_STB_PORT   GPIOA
#define PT6311_STB_PIN    GPIO_PIN_1
//set via guide - count from 0, meaning 0 to 23 bits, but this is made for 14 segments - that doesnt mean that zero segment is equal to 0 bit, it can be shifted by manufacturer
#define PSEG_A   4
#define PSEG_B   5
#define PSEG_C   16
#define PSEG_D   14
#define PSEG_E   15
#define PSEG_F   6
#define PSEG_G1  11
#define PSEG_G2  10
#define PSEG_H   9
#define PSEG_I   8
#define PSEG_J   7
#define PSEG_K   17
#define PSEG_L   13
#define PSEG_M   12
#define PSEG_DP   2

//PRIVATE SECTION
//macros for toggle
#define PT6311_DIN_HIGH()   GPIO_WriteHigh(PT6311_DIN_PORT, PT6311_DIN_PIN)
#define PT6311_DIN_LOW()    GPIO_WriteLow(PT6311_DIN_PORT, PT6311_DIN_PIN)
#define PT6311_CLK_HIGH()   GPIO_WriteHigh(PT6311_CLK_PORT, PT6311_CLK_PIN)
#define PT6311_CLK_LOW()    GPIO_WriteLow(PT6311_CLK_PORT, PT6311_CLK_PIN)
#define PT6311_STB_HIGH()   GPIO_WriteHigh(PT6311_STB_PORT, PT6311_STB_PIN)
#define PT6311_STB_LOW()    GPIO_WriteLow(PT6311_STB_PORT, PT6311_STB_PIN)
//defaults
#define PT6311_BRIGHTNESS PT6311_BRIGHTNESS_4
#define PT6311_STATE PT6311_DISPLAY_ON
//this is never correct, you must correct it
#define SEG_A (1u << 0)
#define SEG_B (1u << 1)
#define SEG_C (1u << 2)
#define SEG_D (1u << 3)
#define SEG_E (1u << 4)
#define SEG_F (1u << 5)
#define SEG_G1 (1u << 6)
#define SEG_G2 (1u << 7)
#define SEG_H (1u << 8)
#define SEG_I (1u << 9)
#define SEG_J (1u << 10)
#define SEG_K (1u << 11)
#define SEG_L (1u << 12)
#define SEG_M (1u << 13)
#define SEG_DP (1u << 14)
#define SEG_SC (1u << 15)

typedef enum {
    PT6311_CMD1 = 0b00000000,
    PT6311_CMD2 = 0b01000000,
    PT6311_CMD3 = 0b11000000,
    PT6311_CMD4 = 0b10000000
} pt6311_cmd_t;

#if defined(PT6311)

#define PT6311_MIN_DIGITS  8
#define PT6311_MAX_DIGITS  16

typedef enum {
    PT6311_DIG8_SEG20  = 0b0000,
    PT6311_DIG9_SEG19  = 0b1000,
    PT6311_DIG10_SEG18 = 0b1001,
    PT6311_DIG11_SEG17 = 0b1010,
    PT6311_DIG12_SEG16 = 0b1011,
    PT6311_DIG13_SEG15 = 0b1100,
    PT6311_DIG14_SEG14 = 0b1101,
    PT6311_DIG15_SEG13 = 0b1110,
    PT6311_DIG16_SEG12 = 0b1111
} pt6311_digit_mode_t;

#elif defined(PT6315)

#define PT6311_MIN_DIGITS  4
#define PT6311_MAX_DIGITS  12

typedef enum {
    PT6311_DIG4_SEG24  = 0b0000,
    PT6311_DIG5_SEG23  = 0b0001,
    PT6311_DIG6_SEG22  = 0b0010,
    PT6311_DIG7_SEG21  = 0b0011,
    PT6311_DIG8_SEG20  = 0b0100,
    PT6311_DIG9_SEG19  = 0b0101,
    PT6311_DIG10_SEG18 = 0b0110,
    PT6311_DIG11_SEG17 = 0b0111,
    PT6311_DIG12_SEG16 = 0b1000
} pt6311_digit_mode_t;

#endif

typedef enum {
    PT6311_DISPLAY_OFF = 0b0000,
    PT6311_DISPLAY_ON  = 0b1000
} pt6311_display_state_t;
typedef enum {
    PT6311_BRIGHTNESS_1 = 0b000,
    PT6311_BRIGHTNESS_2 = 0b001,
    PT6311_BRIGHTNESS_3 = 0b010,
    PT6311_BRIGHTNESS_4 = 0b011,
    PT6311_BRIGHTNESS_5 = 0b100,
    PT6311_BRIGHTNESS_6 = 0b101,
    PT6311_BRIGHTNESS_7 = 0b110,
    PT6311_BRIGHTNESS_8 = 0b111
} pt6311_brightness_t;
//main functions
void pt6311_init(uint8_t num_digits); //init display based on digits
void pt6311_set_brightness(pt6311_brightness_t brightness); //set brightness
void pt6311_set_display_state(pt6311_display_state_t state); //set state
void pt6311_write_char(uint8_t digit_pos, char chr); //write char to specific position
void pt6311_write_char_dot(uint8_t digit_pos, char chr, bool dot) ;
void pt6311_write_string(uint8_t digit_pos, const char *str); //write string from left to right
void pt6311_write_int(uint8_t digit_pos,int value); //write int (good for 4 digits max)
void pt6311_clear_display(void);//clear whole display
//helpers/testers
void pt6311_test_digit_positions(uint8_t num_digits, uint16_t delay_per_digit_ms); //test position of each digit
void pt6311_test_segments(uint8_t digit_pos, uint16_t delay_per_segment_ms); //test segments in chosen digit
//others important
void pt6311_write_digit(uint8_t digit_pos, uint32_t segments); //write digit via correct SPI LIKE sequence
void pt6311_setup_io(void); //setup pins as push_pull out
#endif /*STM8_PT6311_H_*/
