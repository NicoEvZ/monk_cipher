#pragma once

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

static const uint8_t bin_cipher_1234[cipher_height];
extern const uint8_t bin_cipher_fragments[10][fragment_height];
extern const char cipher_fragments[10][fragment_len];

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



