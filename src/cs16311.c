#include "cs16311.h"

uint8_t cs16311_num_digits;
cs16311_brightness_t set_brightness = CS16311_BRIGHTNESS;
cs16311_display_state_t set_state = CS16311_STATE;

uint16_t cs16311_font(char chr)
{
    switch(chr)
    {
        /* Letters */
        case 'A': return 0x478C;
        case 'B': return 0x962C;
        case 'C': return 0xC084;
        case 'D': return 0x942C;
        case 'E': return 0xC384;
        case 'F': return 0x4384;
        case 'G': return 0xC684;
        case 'H': return 0x4788;
        case 'I': return 0x9024;
        case 'J': return 0xC408;
        case 'K': return 0x4990;
        case 'L': return 0xC080;
        case 'M': return 0x54D8;
        case 'N': return 0x4CC8;
        case 'O': return 0xC48C;
        case 'P': return 0x438C;
        case 'Q': return 0xCC8C;
        case 'R': return 0x4B8C;
        case 'S': return 0x8784;
        case 'T': return 0x1024;
        case 'U': return 0xC488;
        case 'V': return 0x6090;
        case 'W': return 0x6CA8;
        case 'X': return 0x2850;
        case 'Y': return 0x1388;
        case 'Z': return 0xA314;

        /* Numbers */
        case '0': return 0xE49C;
        case '1': return 0x0408;
        case '2': return 0xC30C;
        case '3': return 0x870C;
        case '4': return 0x0788;
        case '5': return 0x8784;
        case '6': return 0xC784;
        case '7': return 0x040C;
        case '8': return 0xC78C;
        case '9': return 0x078C;

        /* Symbols */
        case ' ': return 0x0000;
        case ',': return 0x2000;
        case '+': return 0x1320;
        case '-': return 0x0300;
        case ':': return 0x1020;
        case '*': return 0x3B70;
        case '/': return 0x2010;
        case '\\': return 0x0840;
        case '|': return 0x1020;

        /* Unknown char */
        default:
            return 0x0000;
    }
}

static uint16_t cs16311_convert(uint16_t v) //fix for my type display
{
    uint16_t r = 0,i;
    v >>= 2;
    for (i = 0; i < 14; i++)
    {
        if (v & (1 << i))
            r |= (1 << (13 - i));
    }

    return r;
}

void cs16311_setup_io(void)
{
    GPIO_Init(CS16311_DIN_PORT,  CS16311_DIN_PIN,  GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_Init(CS16311_CLK_PORT,  CS16311_CLK_PIN,  GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_Init(CS16311_STB_PORT,  CS16311_STB_PIN,  GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_Init(CS16311_DOUT_PORT, CS16311_DOUT_PIN, GPIO_MODE_IN_PU_NO_IT);

    CS16311_DIN_HIGH();
    CS16311_CLK_HIGH();
    CS16311_STB_HIGH();
}

static void cs16311_start(void)
{
    CS16311_STB_LOW();
    delay_us(10);        
}

static void cs16311_stop(void)
{
    CS16311_STB_HIGH();
    delay_us(10);
}

static void cs16311_shift_out(uint8_t data)
{
	  uint8_t i;
    for (i = 0; i < 8; i++)
    {
        if (data & 0x01)
            CS16311_DIN_HIGH();
        else
            CS16311_DIN_LOW();

        CS16311_CLK_LOW();
        delay_us(5);    
        CS16311_CLK_HIGH();
        delay_us(5);

        data >>= 1;
    }
}

static uint8_t cs16311_shift_in(void)
{
    uint8_t data = 0,i;
    for (i = 0; i < 8; i++)
    {
        CS16311_CLK_LOW();
        delay_us(5);

        if (CS16311_DOUT_READ())
            data |= (1 << i); 

        CS16311_CLK_HIGH();
        delay_us(5);
    }
    return data;
}

void cs16311_init(uint8_t num_digits)
{
	  uint8_t i,cmd1;
    cs16311_num_digits = num_digits;
    cs16311_setup_io();

    delay_ms(50);
    cs16311_start();
    cs16311_shift_out(CS16311_CMD2 | 0b00000000);
    cs16311_stop();

    cs16311_start();
    cs16311_shift_out(CS16311_CMD3); 
    for (i = 0; i < 48; i++) {  
        cs16311_shift_out(0x00);
    }
    cs16311_stop();

    cmd1 = CS16311_CMD1;
    switch(cs16311_num_digits) {
				case 16:  cmd1 |= CS16311_DIG16_SEG12; break;
				case 15:  cmd1 |= CS16311_DIG15_SEG13; break;
        case 14:  cmd1 |= CS16311_DIG14_SEG14; break;
        case 13:  cmd1 |= CS16311_DIG13_SEG15; break;
        case 12:  cmd1 |= CS16311_DIG12_SEG16; break;
        case 11:  cmd1 |= CS16311_DIG11_SEG17; break;
        case 10:  cmd1 |= CS16311_DIG10_SEG18; break;
        case 9:   cmd1 |= CS16311_DIG9_SEG19;  break;
        default:  cmd1 |= CS16311_DIG8_SEG20;  break;
    }
    cs16311_start();
    cs16311_shift_out(cmd1);
    cs16311_stop();

    cs16311_set_display_state(set_state);
    cs16311_set_brightness(set_brightness);
}

void cs16311_set_brightness(cs16311_brightness_t brightness)
{
    set_brightness = brightness;

    cs16311_start();
    cs16311_shift_out(CS16311_CMD4 | set_state | set_brightness);
    cs16311_stop();
}

void cs16311_set_display_state(cs16311_display_state_t state)
{
    set_state = state;

    cs16311_start();
    cs16311_shift_out(CS16311_CMD4 | set_state | set_brightness);
    cs16311_stop();
}

void cs16311_write_digit(uint8_t logical_digit, uint32_t segments)
{
    uint8_t physical_pos;

		physical_pos = cs16311_num_digits - 1 - logical_digit;

    cs16311_start();
    cs16311_shift_out(CS16311_CMD3 | (physical_pos * 3u));
    cs16311_shift_out((uint8_t)segments);
    cs16311_shift_out((uint8_t)(segments >> 8));
    cs16311_shift_out((uint8_t)(segments >> 16));
    cs16311_stop();
}

void cs16311_write_char(uint8_t logical_digit, char chr)
{
		uint16_t unconverted,converted;
		unconverted = cs16311_font(chr);   
    converted = cs16311_convert(unconverted);        
    cs16311_write_digit(logical_digit, converted);            
}

void cs16311_write_string(const char *str)
{
    uint8_t i = 0;

    while (*str && i < cs16311_num_digits)
    {
        cs16311_write_char(i, *str);
        str++;
        i++;
    }
}

void cs16311_write_int(int value,uint8_t digit_pos)
{
    char buf[8];
    uint8_t i = 0;

    if (value == 0)
    {
        cs16311_write_char(digit_pos, '0');
        return;
    }

    if (value < 0)
    {
        cs16311_write_char(digit_pos, '-');
        value = -value;
        digit_pos++;
    }

    while (value > 0 && i < sizeof(buf))
    {
        buf[i++] = (value % 10) + '0';
        value /= 10;
    }

    while (i > 0 && digit_pos < cs16311_num_digits)
    {
        cs16311_write_char(digit_pos++, buf[--i]);
    }
}

uint8_t cs16311_read_sw(void)
{
    uint8_t sw = 0;
    cs16311_start();
    cs16311_shift_out(CS16311_CMD2 | 0b00000011);
    cs16311_stop();

    cs16311_start();
    sw = cs16311_shift_in() & 0x0F;
    cs16311_stop();

    return sw;
}
