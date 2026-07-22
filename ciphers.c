#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <math.h>
#include "ciphers.h"

const int place_value_fragment_index_array[4] = {
    3, 0, 23, 20
};

/*
MONK CIPHER

Monk Cipher is read as such:

            |
    tens    |   ones
------------|------------
thousands   |   hundreds
            |

*/

const uint8_t cipher_fragments[10] = {
    // zero
    // 00
    // 00
    // 00
    0b000000,
    
    // one
    // 11
    // 00
    // 00
    0b110000,
    
    // two
    // 00
    // 00
    // 11
    0b000011,

    // three
    // 10
    // 01
    // 00
    0b100100,
    
    // four
    // 00
    // 01
    // 10
    0b000110,

    // five
    // 11
    // 01
    // 10
    0b110110,

    // six
    // 01
    // 01
    // 01
    0b010101,

    // seven
    // 11
    // 01
    // 01
    0b110101,

    // eight
    // 01
    // 01
    // 11
    0b010111,

    // nine
    // 11
    // 01
    // 11
    0b110111,
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

const char digit_zero_cipher[cipher_len] = {
    0, 1, 1, 1, 0,
    1, 0, 0, 0, 1,
    1, 0, 0, 1, 1,
    1, 0, 1, 0, 1,
    1, 1, 0, 0, 1,
    1, 0, 0, 0, 1,
    0, 1, 1, 1, 0,
};

const char digit_one_cipher[cipher_len] = {
    1, 1, 0, 0, 0,
    0, 1, 0, 0, 0,
    0, 1, 0, 0, 0,
    0, 1, 0, 0, 0,
    0, 1, 0, 0, 0,
    0, 1, 0, 0, 0,
    0, 1, 0, 0, 0,
};

const char digit_two_cipher[cipher_len] = {
    0, 1, 1, 1, 0,
    1, 0, 0, 0, 1,
    0, 0, 0, 0, 1,
    0, 0, 1, 1, 0,
    0, 1, 0, 0, 0,
    1, 0, 0, 0, 0,
    1, 1, 1, 1, 1,
};

const char digit_three_cipher[cipher_len] = {
    1, 1, 1, 1, 0,
    0, 0, 0, 0, 1,
    0, 0, 0, 0, 1,
    0, 1, 1, 1, 0,
    0, 0, 0, 0, 1,
    0, 0, 0, 0, 1,
    1, 1, 1, 1, 0,
};

const char digit_four_cipher[cipher_len] = {
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    1, 1, 1, 1, 1,
    0, 0, 0, 0, 1,
    0, 0, 0, 0, 1,
    0, 0, 0, 0, 1,
};

const char digit_five_cipher[cipher_len] = {
    1, 1, 1, 1, 1,
    1, 0, 0, 0, 0,
    1, 0, 0, 0, 0,
    1, 1, 1, 1, 0,
    0, 0, 0, 0, 1,
    0, 0, 0, 0, 1,
    1, 1, 1, 1, 0,
};

const char digit_six_cipher[cipher_len] = {
    0, 1, 1, 1, 1,
    1, 0, 0, 0, 0,
    1, 0, 0, 0, 0,
    0, 1, 1, 1, 0,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    0, 1, 1, 1, 0,
};

const char digit_seven_cipher[cipher_len] = {
    1, 1, 1, 1, 1,
    0, 0, 0, 0, 1,
    0, 0, 0, 0, 1,
    0, 0, 1, 1, 0,
    0, 1, 0, 0, 0,
    0, 1, 0, 0, 0,
    0, 1, 0, 0, 0,
};

const char digit_eight_cipher[cipher_len] = {
    0, 1, 1, 1, 0,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    0, 1, 1, 1, 0,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    0, 1, 1, 1, 0,
};

const char digit_nine_cipher[cipher_len] = {
    0, 1, 1, 1, 0,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    0, 1, 1, 1, 1,
    0, 0, 0, 0, 1,
    0, 0, 0, 0, 1,
    0, 1, 1, 1, 0,
};


// void cycleMeshColour(uint8_t output_rgb[3], int incriment)
// {
//     output_rgb[0] = (uint8_t) 125.5 * sin(( incriment / (double)MAX_COLOUR_INCREMENT) * 2 * 3.14) + 125.5;
//     output_rgb[1] = (uint8_t) 125.5 * sin((( incriment / (double)MAX_COLOUR_INCREMENT) * 2 * 3.14) + (1.33 * 3.14)) + 125.5;
//     output_rgb[2] = (uint8_t) 125.5 * sin((( incriment / (double)MAX_COLOUR_INCREMENT) * 2 * 3.14) + (0.66 * 3.14)) + 125.5; 
// }

int is_cipher_all_zeros(cipher_t *bin_cipher)
{
    if((bin_cipher->place_values[ONES_PLACE] == 0) & 
        (bin_cipher->place_values[TENS_PLACE] == 0) & 
        (bin_cipher->place_values[HUNDREDS_PLACE] == 0) & 
        (bin_cipher->place_values[THOUSANDS_PLACE] == 0))
        {
            return 1;
        }

    return 0;
}

void copy_fragment(char * save_location, const uint8_t * input_fragment, int start_index)
{
    //len_count is used here to keep track of where we are in the current fragment, and construct a bitmask, to check one individual bit at a time.
    int len_count = 0;
    for(int y = 0; y < fragment_height; y++)
    {
        for(int x = 0; x < fragment_width; x++)
        {
            uint8_t output_bit = (*input_fragment & (0b100000 >> len_count)) ? 1 : 0;
            
            // switch on start_index to know how to orient fragment before saving
            switch (start_index)
            {
            case ones_place_fragment_start_index:
                // does not modify the orientation of the fragment before saving
                save_location[y * fragment_width + x] = output_bit;
                break;
            
            case tens_place_fragment_start_index:
                // flips the fragment horrizontaly before saving
                save_location[y * fragment_width + ((fragment_width - 1) - x)] = output_bit;
                break;
            
            case hundreds_place_fragment_start_index:
                // flips the fragment verically before saving
                save_location[((fragment_height - 1) - y) * fragment_width + x] = output_bit;
                break;

            case thousands_place_fragment_start_index:
                // flips the fragment both veritcally and horrizontaly before saving
                save_location[((fragment_height - 1) - y) * fragment_width + ((fragment_width - 1) - x)] = output_bit;
                break;

            default:
                printf("Undefined start index");
                break;
            }

            len_count++;
        }
    }
}

//print an on or off visual indicator, based off binary value.
void print_pixel(int binary_val)
{  
    if (binary_val > 0) 
    {
        printf("%s%s%s",PRINT_ON_FG_COLOUR,PRINT_ON_BG_COLOUR,PRINT_CHARACTER);

    }
    else
    {
        printf("%s%s%s",PRINT_OFF_FG_COLOUR,PRINT_OFF_BG_COLOUR,PRINT_CHARACTER);
    }
}

// prints a specified row of a cipher
void print_row(cipher_t * cipher_to_print, int row_to_print)
{
    printf("   ");
    for (int x = 0; x < cipher_width; x++)
    {
        print_pixel(cipher_to_print->cipher_array[row_to_print * cipher_width  + x]);
    }
    printf("\e[0m");
}

// draw a single cipher
void draw_cipher(cipher_t * input_cipher) 
{
    printf("\n");
    for (int y = 0; y < cipher_height; y++)
    {
        print_row(input_cipher, y);

        printf("\e[0m");
        printf("\n");
    }
    printf("\e[0m");
    printf("\n");
}

//draw a single fragment
void draw_fragment(char * fragment)
{
    printf("\n");
    for (int y = 0; y < fragment_height; y++)
    {
        for (int x = 0; x < fragment_width; x++)
        {
            print_pixel(fragment[y * fragment_width + x]);
        }
        printf("\e[0m");
        printf("\n");
    }
    printf("\e[0m");
    printf("\n");
}

//draw two ciphers, specified by individual ciphers
void draw_dual_cipher(cipher_t * left_cipher, cipher_t * right_cipher){
    printf("\n");
    for (int y = 0; y < cipher_height; y++)
    {
        print_row(left_cipher, y);
        print_row(right_cipher, y);
        
        printf("\e[0m");
        printf("\n");
    }
    printf("\e[0m");
    printf("\n");
}

//draw 4 ciphers, as part of a quad_display struct
void draw_quad_display(quad_display_t * quad_display){

    printf("\n");
    for (int y = 0; y < cipher_height; y++)
    {
        for (int i = 0; i < 4; i++)
        {
            print_row(quad_display->display[i], y);
        }

        printf("\e[0m"); //reset to default
        printf("\n"); //new line to start new row
    }
    printf("\e[0m"); //reset to default
    printf("\n");
    printf("      %04d         %04d         %04d         %04d",  quad_display->display[0]->number_to_display,
                                                        quad_display->display[1]->number_to_display,
                                                        quad_display->display[2]->number_to_display,
                                                        quad_display->display[3]->number_to_display);
    
}

//places a '1' in every position of the cipher_array
void make_display_cipher_full(cipher_t * output_cipher)
{
    for (int i = 0; i < cipher_len; i++)
    {
          output_cipher->cipher_array[i] = 1;
    }
}

//places a '0' in every position of the cipher array
void make_display_cipher_empty(cipher_t * output_cipher)
{
    for (int i = 0; i < cipher_len; i++)
    {
          output_cipher->cipher_array[i] = 0;
    }
}

void fill_cipher_section(cipher_t * cipher, int digit, int start_index)
{
    char final_fragment_array[fragment_len];

    // copy to temp location, as some fragments need manipulation, depending on place value
    // (represented here by the start index)
    copy_fragment(final_fragment_array, &cipher_fragments[digit], start_index);

    //generic for loop that works for any of the defined start_index values
    int fragment_index = 0;
    int cipher_index = 0;
    for (int i = 0; i < fragment_height; i++)
    {
        for (int j = 0; j < fragment_width; j++)
        {
            cipher->cipher_array[cipher_index + start_index + j] = final_fragment_array[fragment_index + j];
        }
        cipher_index = (cipher_index + cipher_width);
        fragment_index = (fragment_index + fragment_width);
    }
}

/*
Returns an array of 4 values, with the digits of `input_val` in each corresponding index.
Indexes defined by enum `place_value`.
*/
void get_place_values(uint8_t output_val[4], int input_val)
{
    output_val[ONES_PLACE] = (uint8_t)(input_val % 10);
    output_val[TENS_PLACE] = (uint8_t)((input_val / 10) % 10);
    output_val[HUNDREDS_PLACE] = (uint8_t)(((input_val / 10) / 10) % 10);
    output_val[THOUSANDS_PLACE] = (uint8_t)((((input_val / 10) / 10) / 10) % 10);
};

/*Wrapper of `get_place_values()` for `cipher_t*`.
Takes `number_to_display`, and places the digit of each place value into `cipher->place_values`.*/
void extract_place_values_cipher(cipher_t * cipher)
{
    get_place_values(cipher->place_values, cipher->number_to_display);
}

void convert_cipher_to_bytes(cipher_t * complete_cipher)
{
    // objective, start at bottom left of array, (0,6), and move down to collect the column.
    // MSB is 0

    int cipher_bytes_per_column[5] = {0};

    // iterate over every column of the cipher
    for (int j = 0; j < cipher_width; j++)
    {
        int hex_number = 0;
        
        // starting at the MSB (lowest row), iterate upwards towards LSB (top row)
        for (int i = cipher_height-1; i >= 0; i--)
        {
            // accumulate the binary value of the column, essentially converting it to hex.
            // the "complete_cipher->cipher_array[i*cipher_width+j]" section gives us the 1 or 0 stored at the current position.
            // the "<< i" shifts that binary value along, to the correct place value.
            // the "hex_number = hex_number |" ORs the binary value with what is already in the variable, such that we are accumulating the value, one bit per loop. 
            hex_number = hex_number | (complete_cipher->cipher_array[i*cipher_width+j] << i); 
        }
        cipher_bytes_per_column[j] = hex_number;
    }

    printf("cipher in hex codes is:");
    printf("\n");
    for (int count = 0; count < 5; count++)
    { 
        printf("0x%02x: ",cipher_bytes_per_column[count]);
    
        debug_print_binary(cipher_bytes_per_column[count]);
    }
    
    
    return;
}

/*
Copies the values from a `const char` array to given `save_location`.
Ensure the given location is of same size as `array_to_copy`.
*/
void copy_digit_array(uint8_t save_location[cipher_len], const char * array_to_copy)
{
    for (int i = 0; i < cipher_height; i++)
    {
        for (int j = 0; j < cipher_width; j++)
        {
            save_location[cipher_width* i + j] = array_to_copy[cipher_width * i + j];
        }
    }
};

void create_digit(cipher_t * digit_cipher)
{
    extract_place_values_cipher(digit_cipher);
    switch (digit_cipher->number_to_display)
    {
    case 0:
        copy_digit_array(digit_cipher->cipher_array, digit_zero_cipher);
        break;

    case 1:
        copy_digit_array(digit_cipher->cipher_array, digit_one_cipher);
        break;

    case 2:
        copy_digit_array(digit_cipher->cipher_array, digit_two_cipher);
        break;

    case 3:
        copy_digit_array(digit_cipher->cipher_array, digit_three_cipher);
        break;

    case 4:
        copy_digit_array(digit_cipher->cipher_array, digit_four_cipher);
        break;

    case 5:
        copy_digit_array(digit_cipher->cipher_array, digit_five_cipher);
        break;

    case 6:
        copy_digit_array(digit_cipher->cipher_array, digit_six_cipher);
        break;

    case 7:
        copy_digit_array(digit_cipher->cipher_array, digit_seven_cipher);
        break;

    case 8:
        copy_digit_array(digit_cipher->cipher_array, digit_eight_cipher);
        break;

    case 9:
        copy_digit_array(digit_cipher->cipher_array, digit_nine_cipher);
        break;

    default:
        copy_digit_array(digit_cipher->cipher_array, full_cipher);
        break;
    }
};

void create_cipher(cipher_t * bin_cipher)
{
    extract_place_values_cipher(bin_cipher);
    make_display_cipher_empty(bin_cipher);
    if (is_cipher_all_zeros(bin_cipher))
    {
        //if all place values are 0, dont draw the center line, just return early.
        return;  
    }

    // draw center line, by iterating over the whole array that is the cipher, 
    // place a '1', starting from the middle (one fragment ends in the center), 
    // and juming in step that are the width of the cipher (end in the middle, but one row down).
    // repeat until we exceed the lenth of the full array.

    for (int i = fragment_width; i < cipher_len; i = (i + cipher_width))
    {
        bin_cipher->cipher_array[i] = 1;
    }

    // for every palce value section in the cipher, fill in the corresponding fragment's bits
    for (int i = 0; i < 4; i++)
    {
        fill_cipher_section(bin_cipher, bin_cipher->place_values[i], place_value_fragment_index_array[i]);
    }
}

//nulls the pointer for each display
void clear_quad_display(quad_display_t * display_to_clear) 
{
    display_to_clear->colour_incrementer = 0;
    for (int i = 0; i < 4; i++)
    {
        display_to_clear->display[i] = NULL;
    }
}

void display_single_cipher(cipher_t * cipher)
{
    create_cipher(cipher);
    draw_cipher(cipher);

    // convert cipher to 5 hex values, compatible with hcms3967

    // int byte_array[5] = {0};
    convert_cipher_to_bytes(cipher);
    
}

void display_quad_ciphers(quad_display_t * quad_display)
{
    cipher_t backup_blank_cipher;
    backup_blank_cipher.number_to_display = 0000;
    create_cipher(&backup_blank_cipher);
    
    // iterate over all displays in quad display, and if initialised, 
    // then create the cipher for that display
    for (int i = 0; i < 4; i++)
    {
        if (quad_display->display[i] == NULL)
        {
            printf("Display %d not initialised, proceeding with blank display\n",i+1);
            quad_display->display[i] = &backup_blank_cipher;
        }
            create_cipher(quad_display->display[i]);    
    }
    // drawing to the screen
    draw_quad_display(quad_display);
}

void display_quad_cipher_digits(quad_display_t * quad_display)
{
    cipher_t backup_blank_cipher;
    backup_blank_cipher.number_to_display = 0000;
    create_cipher(&backup_blank_cipher);
    
    // iterate over all displays in quad display, and if initialised, 
    // then create the cipher for that display
    for (int i = 0; i < 4; i++)
    {
        if (quad_display->display[i] == NULL)
        {
            printf("Display %d not initialised, proceeding with blank display\n",i+1);
            quad_display->display[i] = &backup_blank_cipher;
        }
            create_digit(quad_display->display[i]);    
    }
    // drawing to the screen
    draw_quad_display(quad_display);
}

void debug_print_binary(int num) {
    // Iterate from the most significant bit to the least significant bit
    for (int i = 8 - 1; i >= 0; i--) {
        // Check if the i-th bit is set (1) or not (0)
        // (num >> i) shifts the bit at position 'i' to the least significant position
        // & 1 then isolates that bit
        printf("%d", (num >> i) & 1);
    }
    printf("\n"); // Print a newline character after the binary representation
}