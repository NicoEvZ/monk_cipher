#pragma once
#include<stdint.h>

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

#define MAX_COLOUR_INCREMENT 800

// #define PRINT_CHARACTER "\u25cf" //circle
// #define PRINT_CHARACTER "\u25a0 " //Black Square + space
// #define PRINT_CHARACTER "\u25fc " //Black Square
#define PRINT_CHARACTER "  " //Black Square
// #define PRINT_ON_COLOUR "\e[95m" // bright pink foreground
#define PRINT_ON_COLOUR "\e[93m" // bright yellow foreground
#define PRINT_BG_COLOUR "\e[43m" //yellow BG
#define PRINT_OFF_COLOUR "\e[90m" // bright black foreground
// #define PRINT_OFF_COLOUR "\e[30m" // dark black foreground
// #define PRINT_CHARACTER "\u25fc " //Black Mediun Square
// #define PRINT_CHARACTER "\u25fb " //White Mediun Square

enum place_value
{
    ONES_PLACE = 0,
    TENS_PLACE,
    HUNDREDS_PLACE,
    THOUSANDS_PLACE
};

typedef struct cipher
{
    uint8_t cipher_array[cipher_len];
    int number_to_display;
    uint8_t place_values[4];
} cipher_t;

typedef struct quad_display
{
    cipher_t* display[4];
    int colour_incrementer;
}quad_display_t;

void display_quad_ciphers(quad_display_t * quad_display);

void clear_quad_display(quad_display_t * display_to_clear);




