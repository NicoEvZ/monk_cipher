#include "ciphers.h"
#include <stdint.h>

#define cipher_len 35
#define cipher_width 5
#define cipher_height 7

#define fragment_len 6
#define fragment_width 2
#define fragment_height 3

#define ones_place_fragment_start_index 3
#define tens_place_fragment_start_index 0
#define hundreds_place_fragment_start_index 23
#define thousands_place_fragment_start_index 20

/*
MONK CIPHER

Monk Cipher is read as such:

            |
    tens    |   ones
------------|------------
thousands   |   tens
            |

*/

static const uint8_t bin_cipher_1234[cipher_height] =  {
    0b01100,
    0b10101,
    0b00110,
    0b00100,
    0b00111,
    0b00100,
    0b11100,
};

const char cipher_fragments[10][fragment_len] = 
{
    //zero
    {
        0, 0,
        0, 0,
        0, 0,
    },
    // one
    {
        1, 1,
        0, 0,
        0, 0,
    },
    //two
    {
        0, 0,
        0, 0,
        1, 1,
    },
    //three
    {
        1, 0,
        0, 1,
        0, 0,
    },
    //four
    {
        0, 0,
        0, 1,
        1, 0,
    },
    //five
    {
        1, 1,
        0, 1,
        1, 0,
    },
    //six
    {
        0, 1,
        0, 1,
        0, 1,
    },
    //seven
    {
        1, 1,
        0, 1,
        0, 1,
    },
    //eight
    {
        0, 1,
        0, 1,
        1, 1,
    },
    //nine
    {
        1, 1,
        0, 1,
        1, 1,
    },
};

const char blank_cipher[cipher_len] = {
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
};

const char full_cipher[cipher_len] = {
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
};

const char zero_cipher[cipher_len] = {
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
};

const char base_cipher[cipher_len] = {
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
};

const char one_cipher[cipher_len] = {
    0, 0, 1, 1, 1,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
};

const char two_cipher[cipher_len] = {
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 1, 1,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
};

const char three_cipher[cipher_len] = {
    0, 0, 1, 1, 0,
    0, 0, 1, 0, 1,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
};

const char four_cipher[cipher_len] = {
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 1,
    0, 0, 1, 1, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
};

const char five_cipher[cipher_len] = {
    0, 0, 1, 1, 1,
    0, 0, 1, 0, 1,
    0, 0, 1, 1, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
};

const char six_cipher[cipher_len] = {
    0, 0, 1, 0, 1,
    0, 0, 1, 0, 1,
    0, 0, 1, 0, 1,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
};

const char seven_cipher[cipher_len] = {
    0, 0, 1, 1, 1,
    0, 0, 1, 0, 1,
    0, 0, 1, 0, 1,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
};

const char eight_cipher[cipher_len] = {
    0, 0, 1, 0, 1,
    0, 0, 1, 0, 1,
    0, 0, 1, 1, 1,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
};

const char nine_cipher[cipher_len] = {
    0, 0, 1, 1, 1,
    0, 0, 1, 0, 1,
    0, 0, 1, 1, 1,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
};

const char ten_cipher[cipher_len] = {
    1, 1, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
};

const char twenty_cipher[cipher_len] = {
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    1, 1, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
};

const char thirty_cipher[cipher_len] = {
    0, 1, 1, 0, 0,
    1, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
};

const char fourty_cipher[cipher_len] = {
    0, 0, 1, 0, 0,
    1, 0, 1, 0, 0,
    0, 1, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
};

const char fifty_cipher[cipher_len] = {
    1, 1, 1, 0, 0,
    1, 0, 1, 0, 0,
    0, 1, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
};

const char sixty_cipher[cipher_len] = {
    1, 0, 1, 0, 0,
    1, 0, 1, 0, 0,
    1, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
};

const char seventy_cipher[cipher_len] = {
    1, 1, 1, 0, 0,
    1, 0, 1, 0, 0,
    1, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
};

const char eighty_cipher[cipher_len] = {
    1, 0, 1, 0, 0,
    1, 0, 1, 0, 0,
    1, 1, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
};

const char ninety_cipher[cipher_len] = {
    1, 1, 1, 0, 0,
    1, 0, 1, 0, 0,
    1, 1, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
};

const char one_hundred_cipher[cipher_len] = {
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 1, 1,
};

const char two_hundred_cipher[cipher_len] = {
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 1, 1,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
};

const char three_hundred_cipher[cipher_len] = {
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 1,
    0, 0, 1, 1, 0,
};

const char four_hundred_cipher[cipher_len] = {
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 1, 0,
    0, 0, 1, 0, 1,
    0, 0, 1, 0, 0,
};

const char five_hundred_cipher[cipher_len] = {
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 1, 0,
    0, 0, 1, 0, 1,
    0, 0, 1, 1, 1,
};

const char six_hundred_cipher[cipher_len] = {
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 1,
    0, 0, 1, 0, 1,
    0, 0, 1, 0, 1,
};

const char seven_hundred_cipher[cipher_len] = {
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 1,
    0, 0, 1, 0, 1,
    0, 0, 1, 1, 1,
};

const char eight_hundred_cipher[cipher_len] = {
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 1, 1,
    0, 0, 1, 0, 1,
    0, 0, 1, 0, 1,
};

const char nine_hundred_cipher[cipher_len] = {
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 1, 1,
    0, 0, 1, 0, 1,
    0, 0, 1, 1, 1,
};

const char one_thousand_cipher[cipher_len] = {
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    1, 1, 1, 0, 0,
};

const char two_thousand_cipher[cipher_len] = {
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    1, 1, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
};

const char three_thousand_cipher[cipher_len] = {
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    1, 0, 1, 0, 0,
    0, 1, 1, 0, 0,
};

const char four_thousand_cipher[cipher_len] = {
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 1, 1, 0, 0,
    1, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
};

const char five_thousand_cipher[cipher_len] = {
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 1, 1, 0, 0,
    1, 0, 1, 0, 0,
    1, 1, 1, 0, 0,
};

const char six_thousand_cipher[cipher_len] = {
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    1, 0, 1, 0, 0,
    1, 0, 1, 0, 0,
    1, 0, 1, 0, 0,
};

const char seven_thousand_cipher[cipher_len] = {
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    1, 0, 1, 0, 0,
    1, 0, 1, 0, 0,
    1, 1, 1, 0, 0,
};

const char eight_thousand_cipher[cipher_len] = {
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    1, 1, 1, 0, 0,
    1, 0, 1, 0, 0,
    1, 0, 1, 0, 0,
};

const char nine_thousand_cipher[cipher_len] = {
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    1, 1, 1, 0, 0,
    1, 0, 1, 0, 0,
    1, 1, 1, 0, 0,
};
