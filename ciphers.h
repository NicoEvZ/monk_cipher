#pragma once

#include "ciphers.c"

typedef struct four_digit_place_values
{
   int ones_place, tens_place, hundrends_place, thousands_place;
} four_digit_place_values_t;

typedef struct cipher
{
    char cipher_array[cipher_len];
    int number_to_display;
    four_digit_place_values_t extracted_place_values;
} cipher_t;

typedef struct quad_display
{
    cipher_t *display_one, *display_two, *display_three, *display_four;
}quad_display_t;



