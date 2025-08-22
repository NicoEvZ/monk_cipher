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
thousands   |   hundreds
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