#include "ciphers.h"
#include <stdint.h>

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