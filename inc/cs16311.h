#ifndef CS6311_H 
#define CS6311_H

#include "stm8s.h"
#include "stm8s_delay.h"
//define your pinout
#define CS16311_DIN_PORT   GPIOC
#define CS16311_DIN_PIN    GPIO_PIN_6

#define CS16311_DOUT_PORT  GPIOC
#define CS16311_DOUT_PIN   GPIO_PIN_4

#define CS16311_CLK_PORT   GPIOC
#define CS16311_CLK_PIN    GPIO_PIN_7

#define CS16311_STB_PORT   GPIOC
#define CS16311_STB_PIN    GPIO_PIN_5

//macros for toggle
#define CS16311_DIN_HIGH()   GPIO_WriteHigh(CS16311_DIN_PORT, CS16311_DIN_PIN)
#define CS16311_DIN_LOW()    GPIO_WriteLow(CS16311_DIN_PORT, CS16311_DIN_PIN)
#define CS16311_CLK_HIGH()   GPIO_WriteHigh(CS16311_CLK_PORT, CS16311_CLK_PIN)
#define CS16311_CLK_LOW()    GPIO_WriteLow(CS16311_CLK_PORT, CS16311_CLK_PIN)
#define CS16311_STB_HIGH()   GPIO_WriteHigh(CS16311_STB_PORT, CS16311_STB_PIN)
#define CS16311_STB_LOW()    GPIO_WriteLow(CS16311_STB_PORT, CS16311_STB_PIN)
#define CS16311_DOUT_READ()  GPIO_ReadInputPin(CS16311_DOUT_PORT, CS16311_DOUT_PIN)

//defaults
#define CS16311_BRIGHTNESS CS16311_BRIGHTNESS_5
#define CS16311_STATE CS16311_DISPLAY_ON

typedef enum {
    CS16311_CMD1 = 0b00000000,
    CS16311_CMD2 = 0b01000000,
    CS16311_CMD3 = 0b11000000,
    CS16311_CMD4 = 0b10000000
} cs16311_cmd_t;

typedef enum {
    CS16311_DIG8_SEG20  = 0b0000,
    CS16311_DIG9_SEG19  = 0b1000,
    CS16311_DIG10_SEG18 = 0b1001,
    CS16311_DIG11_SEG17 = 0b1010,
    CS16311_DIG12_SEG16 = 0b1011,
    CS16311_DIG13_SEG15 = 0b1100,
    CS16311_DIG14_SEG14 = 0b1101,
    CS16311_DIG15_SEG13 = 0b1110,
    CS16311_DIG16_SEG12 = 0b1111
} cs16311_digit_mode_t;

typedef enum {
    CS16311_DISPLAY_OFF = 0b0000,
    CS16311_DISPLAY_ON  = 0b1000
} cs16311_display_state_t;

typedef enum {
    CS16311_BRIGHTNESS_1 = 0b000,
    CS16311_BRIGHTNESS_2 = 0b001,
    CS16311_BRIGHTNESS_3 = 0b010,
    CS16311_BRIGHTNESS_4 = 0b011,
    CS16311_BRIGHTNESS_5 = 0b100,
    CS16311_BRIGHTNESS_6 = 0b101,
    CS16311_BRIGHTNESS_7 = 0b110,
    CS16311_BRIGHTNESS_8 = 0b111
} cs16311_brightness_t;


//main functions
void cs16311_init(uint8_t num_digits); //init display based on digits
void cs16311_set_brightness(cs16311_brightness_t brightness); //set brightness
void cs16311_set_display_state(cs16311_display_state_t state); //set state
void cs16311_write_char(uint8_t digit_pos, char chr); //write char to specific position
void cs16311_write_string(const char *str); //write string from left to right
void cs16311_write_int(int value, uint8_t digit_pos); //write int (good for 4 digits max)
//others
void cs16311_write_digit(uint8_t digit_pos, uint32_t segments);
void cs16311_setup_io(void);
uint8_t cs16311_read_sw(void);

#endif // CS16311_H