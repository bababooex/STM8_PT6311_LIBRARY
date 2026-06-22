#include "stm8_pt6311.h"

uint8_t pt6311_num_digits;
pt6311_brightness_t set_brightness = PT6311_BRIGHTNESS;
pt6311_display_state_t set_state = PT6311_STATE;

static uint32_t pt6311_font(char chr)
{
    switch(chr)
    {
        /* Numbers */
        case '0': return SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F;
        case '1': return SEG_B | SEG_C | SEG_J ;
        case '2': return SEG_A | SEG_B | SEG_G1 | SEG_G2 | SEG_D | SEG_E;
        case '3': return SEG_A | SEG_B | SEG_C | SEG_D | SEG_G1 | SEG_G2;
        case '4': return SEG_F | SEG_G1 | SEG_G2 | SEG_B | SEG_C;
        case '5': return SEG_A | SEG_F | SEG_G1 | SEG_M | SEG_D;
        case '6': return SEG_A | SEG_F | SEG_E | SEG_D | SEG_C | SEG_G1 | SEG_G2;
        case '7': return SEG_A | SEG_B | SEG_C;
        case '8': return SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G1 | SEG_G2;
        case '9': return SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G1 | SEG_G2;

				/* Uppercase */
				case 'A': return SEG_A|SEG_B|SEG_C|SEG_E|SEG_F|SEG_G1|SEG_G2;
				case 'B': return SEG_A|SEG_B|SEG_C|SEG_D|SEG_G1|SEG_G2|SEG_I|SEG_L;
				case 'C': return SEG_A|SEG_D|SEG_E|SEG_F;
				case 'D': return SEG_A|SEG_B|SEG_C|SEG_D|SEG_I|SEG_L;
				case 'E': return SEG_A|SEG_D|SEG_E|SEG_F|SEG_G1|SEG_G2;
				case 'F': return SEG_A|SEG_E|SEG_F|SEG_G1|SEG_G2;
				case 'G': return SEG_A|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G2;
				case 'H': return SEG_B|SEG_C|SEG_E|SEG_F|SEG_G1|SEG_G2;
				case 'I': return SEG_A|SEG_D|SEG_I|SEG_L;
				case 'J': return SEG_B|SEG_C|SEG_D|SEG_E;
				case 'K': return SEG_E|SEG_F|SEG_G1|SEG_J|SEG_M;
				case 'L': return SEG_D|SEG_E|SEG_F;
				case 'M': return SEG_B|SEG_C|SEG_E|SEG_F|SEG_H|SEG_J;
				case 'N': return SEG_B|SEG_C|SEG_E|SEG_F|SEG_H|SEG_M;
				case 'O': return SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F;
				case 'P': return SEG_A|SEG_B|SEG_E|SEG_F|SEG_G1|SEG_G2;
				case 'Q': return SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_M;
				case 'R': return SEG_A|SEG_B|SEG_E|SEG_F|SEG_G1|SEG_G2|SEG_M;
				case 'S': return SEG_A|SEG_C|SEG_D|SEG_F|SEG_G1|SEG_G2;
				case 'T': return SEG_A|SEG_I|SEG_L;
				case 'U': return SEG_B|SEG_C|SEG_D|SEG_E|SEG_F;
				case 'V': return SEG_E|SEG_F|SEG_J|SEG_K;
				case 'W': return SEG_B|SEG_C|SEG_E|SEG_F|SEG_K|SEG_M;
				case 'X': return SEG_H|SEG_J|SEG_K|SEG_M;
				case 'Y': return SEG_B|SEG_C|SEG_D|SEG_F|SEG_G1|SEG_G2;
				case 'Z': return SEG_A|SEG_D|SEG_J|SEG_K;
				
				/* Lowercase (not every is probably correct)*/
				case 'a': return SEG_B|SEG_C|SEG_D|SEG_E|SEG_G1|SEG_G2;
				case 'b': return SEG_C|SEG_D|SEG_E|SEG_F|SEG_G1|SEG_G2;
				case 'c': return SEG_D|SEG_E|SEG_G1|SEG_G2;
				case 'd': return SEG_B|SEG_C|SEG_D|SEG_E|SEG_G1|SEG_G2;
				case 'e': return SEG_A|SEG_B|SEG_D|SEG_E|SEG_G1|SEG_G2;
				case 'f': return SEG_A|SEG_E|SEG_F|SEG_G1|SEG_G2;
				case 'g': return SEG_A|SEG_B|SEG_C|SEG_D|SEG_F|SEG_G1|SEG_G2;
				case 'h': return SEG_C|SEG_E|SEG_F|SEG_G1|SEG_G2;
				case 'i': return SEG_L;
				case 'j': return SEG_C|SEG_D;
				case 'k': return SEG_E|SEG_F|SEG_G1|SEG_M;
				case 'l': return SEG_D|SEG_E|SEG_F;
				case 'm': return SEG_C|SEG_E|SEG_G1|SEG_G2|SEG_K|SEG_M;
				case 'n': return SEG_C|SEG_E|SEG_G1|SEG_G2;
				case 'o': return SEG_C|SEG_D|SEG_E|SEG_G1|SEG_G2;
				case 'p': return SEG_A|SEG_B|SEG_E|SEG_F|SEG_G1|SEG_G2;
				case 'q': return SEG_A|SEG_B|SEG_C|SEG_F|SEG_G1|SEG_G2;
				case 'r': return SEG_E|SEG_G1|SEG_G2;
				case 's': return SEG_A|SEG_C|SEG_D|SEG_F|SEG_G1|SEG_G2;
				case 't': return SEG_D|SEG_E|SEG_F|SEG_G1|SEG_G2;
				case 'u': return SEG_C|SEG_D|SEG_E;
				case 'v': return SEG_E|SEG_K|SEG_M;
				case 'w': return SEG_C|SEG_E|SEG_K|SEG_M;
				case 'x': return SEG_H|SEG_J|SEG_K|SEG_M;
				case 'y': return SEG_B|SEG_C|SEG_D|SEG_F|SEG_G1|SEG_G2;
				case 'z': return SEG_A|SEG_D|SEG_J|SEG_K;
        /* Symbols */
        case ' ': return 0;
        case '-': return SEG_G1 | SEG_G2;
        case '_': return SEG_D;
        case '"': return SEG_H | SEG_J;
        case '\'': return SEG_H | SEG_M;
        case '+': return SEG_G1 | SEG_G2 | SEG_I | SEG_L;
        case '*': return SEG_G1 | SEG_G2 | SEG_H | SEG_I | SEG_J | SEG_K | SEG_L | SEG_M;
        case '/': return SEG_J | SEG_K;
				case '.': return SEG_DP;//if avaiable
        default: return 0;
    }
}
static const uint8_t logical_to_physical[16] = {
		PSEG_A, 
		PSEG_B,
		PSEG_C,
		PSEG_D,
		PSEG_E,
		PSEG_F,
		PSEG_G1,
		PSEG_G2,
		PSEG_H,
		PSEG_I,
		PSEG_J,
		PSEG_K,
		PSEG_L,
		PSEG_M,
		PSEG_DP,
};

static uint32_t pt6311_remap(uint32_t logical) {
    uint32_t physical = 0;
		uint8_t i;
    for (i = 0; i < 15; i++) {
        if (logical & (1UL << i)) {
            physical |= (1UL << logical_to_physical[i]);
        }
    }
    return physical;
}

void pt6311_setup_io(void)
{
    GPIO_Init(PT6311_DIN_PORT,  PT6311_DIN_PIN,  GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_Init(PT6311_CLK_PORT,  PT6311_CLK_PIN,  GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_Init(PT6311_STB_PORT,  PT6311_STB_PIN,  GPIO_MODE_OUT_PP_HIGH_FAST);

    PT6311_DIN_HIGH();
    PT6311_CLK_HIGH();
    PT6311_STB_HIGH();
}

static void pt6311_start(void)
{
    PT6311_STB_LOW();
    delay_us(10);        
}

static void pt6311_stop(void)
{
    PT6311_STB_HIGH();
    delay_us(10);
}

static void pt6311_shift_out(uint8_t data)
{
	  uint8_t i;
    for (i = 0; i < 8; i++)
    {
        if (data & 0x01)
            PT6311_DIN_HIGH();
        else
            PT6311_DIN_LOW();

        PT6311_CLK_LOW();
        delay_us(5);    
        PT6311_CLK_HIGH();
        delay_us(5);

        data >>= 1;
    }
}
void pt6311_init(uint8_t num_digits)
{
	  uint8_t i,cmd1;
    if (num_digits < PT6311_MIN_DIGITS) num_digits = PT6311_MIN_DIGITS;
    if (num_digits > PT6311_MAX_DIGITS) num_digits = PT6311_MAX_DIGITS;
    pt6311_num_digits = num_digits+1;//correct num digits error
    pt6311_setup_io();

    delay_ms(50);
    pt6311_start();
    pt6311_shift_out(PT6311_CMD2 | 0b00000000);
    pt6311_stop();

    pt6311_start();
    pt6311_shift_out(PT6311_CMD3); 
    for (i = 0; i < (pt6311_num_digits * 3); i++) {
    pt6311_shift_out(0x00);
		}
    pt6311_stop();

    cmd1 = PT6311_CMD1;
#if defined(PT6311)
    switch(pt6311_num_digits) {
				case 16:  cmd1 |= PT6311_DIG16_SEG12; break;
				case 15:  cmd1 |= PT6311_DIG15_SEG13; break;
        case 14:  cmd1 |= PT6311_DIG14_SEG14; break;
        case 13:  cmd1 |= PT6311_DIG13_SEG15; break;
        case 12:  cmd1 |= PT6311_DIG12_SEG16; break;
        case 11:  cmd1 |= PT6311_DIG11_SEG17; break;
        case 10:  cmd1 |= PT6311_DIG10_SEG18; break;
        case 9:   cmd1 |= PT6311_DIG9_SEG19;  break;
        default:  cmd1 |= PT6311_DIG8_SEG20;  break;
    }
#elif defined(PT6315)
    switch(pt6311_num_digits) {
        case 12:  cmd1 |= PT6311_DIG12_SEG16; break;
        case 11:  cmd1 |= PT6311_DIG11_SEG17; break;
        case 10:  cmd1 |= PT6311_DIG10_SEG18; break;
        case 9:   cmd1 |= PT6311_DIG9_SEG19;  break;
        case 8:   cmd1 |= PT6311_DIG8_SEG20;  break;
        case 7:   cmd1 |= PT6311_DIG7_SEG21;  break;
        case 6:   cmd1 |= PT6311_DIG6_SEG22;  break;
        case 5:   cmd1 |= PT6311_DIG5_SEG23;  break;
        default:  cmd1 |= PT6311_DIG4_SEG24;  break;
    }
#endif
    pt6311_start();
    pt6311_shift_out(cmd1);
    pt6311_stop();

    pt6311_set_display_state(set_state);
    pt6311_set_brightness(set_brightness);
}

void pt6311_set_brightness(pt6311_brightness_t brightness)
{
    set_brightness = brightness;

    pt6311_start();
    pt6311_shift_out(PT6311_CMD4 | set_state | set_brightness);
    pt6311_stop();
}

void pt6311_set_display_state(pt6311_display_state_t state)
{
    set_state = state;

    pt6311_start();
    pt6311_shift_out(PT6311_CMD4 | set_state | set_brightness);
    pt6311_stop();
}

void pt6311_write_digit(uint8_t logical_digit, uint32_t segments)
{
    uint8_t physical_pos;

		physical_pos = pt6311_num_digits - logical_digit;

    pt6311_start();
    pt6311_shift_out(PT6311_CMD3 | (physical_pos * 3u));
    pt6311_shift_out((uint8_t)segments);
    pt6311_shift_out((uint8_t)(segments >> 8));
    pt6311_shift_out((uint8_t)(segments >> 16));
    pt6311_stop();
}

void pt6311_write_char(uint8_t digit_pos, char chr)
{
    uint32_t logical = pt6311_font(chr);
    uint32_t physical = pt6311_remap(logical);
    pt6311_write_digit(digit_pos, physical);
}

void pt6311_write_char_dot(uint8_t digit_pos, char chr, bool dot) {
    uint32_t physical;
		uint32_t logical = pt6311_font(chr);
    if (dot) {
        logical |= SEG_DP;
    }
    physical = pt6311_remap(logical);
    pt6311_write_digit(digit_pos, physical);
}

void pt6311_write_string(const char *str)
{
    uint8_t i = 1;

    while (*str && i < pt6311_num_digits)
    {
        pt6311_write_char(i, *str);
        str++;
        i++;
    }
}

void pt6311_write_int(uint8_t digit_pos,int value)
{
    char buf[8];
    uint8_t i = 0;
		
    if (value < 0)
    {
        pt6311_write_char(digit_pos, '-');
        value = -value;
        digit_pos++;
    }

    while (value > 0 && i < sizeof(buf))
    {
        buf[i++] = (value % 10) + '0';
        value /= 10;
    }

    while (i > 0 && digit_pos < pt6311_num_digits)
    {
        pt6311_write_char(digit_pos++, buf[--i]);
    }
}
void pt6311_clear_display(void) {
		uint8_t i;
    for (i = 0; i < pt6311_num_digits; i++) {
        pt6311_write_digit(i, 0);
    }
}
//helpers
//test digit positions, so they are set correctly
void pt6311_test_digit_positions(uint8_t num_digits, uint16_t delay_per_digit_ms)
{
    uint8_t pos;
 
    for (pos = 1; pos < num_digits+1; pos++)
    {
        pt6311_write_digit(pos, 0xFFFFFF); 
        delay_ms(delay_per_digit_ms);
        pt6311_write_digit(pos, 0x000000); 
    }
}

//test each digit
void pt6311_test_segments(uint8_t digit_pos, uint16_t delay_per_segment_ms)
{
    uint8_t bit;
 
    for (bit = 0; bit < 24; bit++)
    {
        pt6311_write_digit(digit_pos, (uint32_t)1 << bit);
				pt6311_write_digit(digit_pos+2, 0x005000);//blink random other for counting
				delay_ms(delay_per_segment_ms);
        pt6311_write_digit(digit_pos, 0x000000);
				pt6311_write_digit(digit_pos+2, 0x000000);
        delay_ms(150); 
    }
}

