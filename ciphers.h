#pragma once

#include "ciphers.c"

const char * ones_place_ciphers[] = {
    zero_cipher,
    one_cipher, 
    two_cipher, 
    three_cipher, 
    four_cipher, 
    five_cipher, 
    six_cipher, 
    seven_cipher, 
    eight_cipher, 
    nine_cipher};

const char * tens_place_ciphers[] = {
    zero_cipher,
    ten_cipher,
    twenty_cipher,
    thirty_cipher,
    fourty_cipher,
    fifty_cipher,
    sixty_cipher,
    seventy_cipher,
    eighty_cipher,
    ninety_cipher,
};

const char * hundreds_place_ciphers[] = {
    zero_cipher,
    one_hundred_cipher,
    two_hundred_cipher,
    three_hundred_cipher,
    four_hundred_cipher,
    five_hundred_cipher,
    six_hundred_cipher,
    seven_hundred_cipher,
    eight_hundred_cipher,
    nine_hundred_cipher,
};

const char * thousands_place_ciphers[] = {
    zero_cipher,
    one_thousand_cipher,
    two_thousand_cipher,
    three_thousand_cipher,
    four_thousand_cipher,
    five_thousand_cipher,
    six_thousand_cipher,
    seven_thousand_cipher,
    eight_thousand_cipher,
    nine_thousand_cipher,
};

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



