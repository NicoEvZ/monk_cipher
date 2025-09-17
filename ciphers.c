#define _POSIX_C_SOURCE 199309L
#include <stdint.h>
#include <stdio.h>
#include "ciphers.h"

/*
MONK CIPHER

Monk Cipher is read as such:

            |
    tens    |   ones
------------|------------
thousands   |   hundreds
            |

*/

const uint8_t bin_cipher_1234[cipher_height] =  {
    0b01100,
    0b10101,
    0b00110,
    0b00100,
    0b00111,
    0b00100,
    0b11100,
};

const uint8_t bin_cipher_fragments[10] = 
{
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

//itterate over fragment and moves each point to save location without any changes
void copy_fragment(char * save_location, const char * input_fragment)
{
    for(int y = 0; y < fragment_height; y++)
    {
        for(int x = 0; x < fragment_width; x++)
        {
            save_location[y * fragment_width + x] = input_fragment[y * fragment_width  + x];
        }
    }
}

//itterate over fragment array amd change save location position so that it appears vertically flipped
void flip_fragment_vert(char * save_locaion, const char * input_fragment)
{
    for(int y = 0; y < fragment_height; y++)
    {
        for(int x = 0; x < fragment_width; x++)
        {
            save_locaion[((fragment_height - 1) - y) * fragment_width + x] = input_fragment[y * fragment_width  + x];
        }
    }
}

//itterate over fragment array and change save location position so that it appears horrizontally flipped
void flip_fragment_hor(char * save_locaion, const char * input_fragment)
{
    for(int y = 0; y < fragment_height; y++)
    {
        for(int x = 0; x < fragment_width; x++)
        {
            save_locaion[y * fragment_width + ((fragment_width - 1) -x)] = input_fragment[y * fragment_width  + x];
        }
    }
}

//itterate over fragment array and change save location position so that it apprears flipped in both vertical and horizontal axes
void flip_fragment_diag(char * save_locaion, const char * input_fragment)
{
    for(int y = 0; y < fragment_height; y++)
    {
        for(int x = 0; x < fragment_width; x++)
        {
            save_locaion[((fragment_height - 1) - y) * fragment_width + ((fragment_width - 1) -x)] = input_fragment[y * fragment_width  + x];
        }
    }
}

void copy_bin_fragment(char * save_location, const uint8_t * input_bin_fragment, int start_index)
{
    int len_count = 0;
    for(int y = 0; y < fragment_height; y++)
    {
        for(int x = 0; x < fragment_width; x++)
        {
            // printf("input_bin_fragment =\t\t\t");
            // debug_print_binary(*input_bin_fragment);
            // printf("input_bin_fragment & (0b100000 >> %d) =\t",len_count);
            // debug_print_binary(*input_bin_fragment & (0b100000 >> len_count));
            
            // switch on start_index to know how to orient fragment before saving
            switch (start_index)
            {
            case ones_place_fragment_start_index:
                // does not modify the orientation of the fragment before saving
                save_location[y * fragment_width + x] = (*input_bin_fragment & (0b100000 >> len_count)) ? 1 : 0;
                break;
            
            case tens_place_fragment_start_index:
                // flips the fragment horrizontaly before saving
                save_location[y * fragment_width + ((fragment_width - 1) - x)] = (*input_bin_fragment & (0b100000 >> len_count)) ? 1 : 0;
                break;
            
            case hundreds_place_fragment_start_index:
                // flips the fragment verically before saving
                save_location[((fragment_height - 1) - y) * fragment_width + x] = (*input_bin_fragment & (0b100000 >> len_count)) ? 1 : 0;
                break;

            case thousands_place_fragment_start_index:
                // flips the fragment both veritcally and horrizontaly before saving
                save_location[((fragment_height - 1) - y) * fragment_width + ((fragment_width - 1) - x)] = (*input_bin_fragment & (0b100000 >> len_count)) ? 1 : 0;
                break;

            default:
                printf("Undefined start index");
                break;
            }

            // printf("save_location[%d * %d + %d] = \t\t",y,fragment_width,x);
            // debug_print_binary(save_location[y * fragment_width + x]);
            // printf("\n");
            len_count++;
        }
    }
}

//print and on or off visual indicator, based off binary value.
void print_pixel(int binary_val)
{   
    if (binary_val == 1) 
    {
        printf("%s%s%s",PRINT_ON_COLOUR,PRINT_BG_COLOUR,PRINT_CHARACTER);
    }
    else if (binary_val == 0)
    {
        printf("%s\e[49m%s",PRINT_OFF_COLOUR,PRINT_CHARACTER);
    }
}

// prints a specified row of a cipher
void print_row(cipher_t * cipher_to_print, int row_to_print)
{
    for (int x = 0; x < cipher_width; x++)
    {
        print_pixel(cipher_to_print->cipher_array[row_to_print * cipher_width  + x]);
    }
    printf("\e[0m");
    printf(" ");
}

//prints a specific row of a cipher defined with binary values
void print_row_binary(int row_to_print)
{
    for (int x = 0; x < cipher_width; x++)
    {
        print_pixel(bin_cipher_1234[row_to_print] & (0b10000 >> x));
    }
}

//print a specified column of a cipher
void print_column(cipher_t * cipher_to_print, int column_to_print)
{
    for (int y = 0; y < cipher_height; y++)
    {
        print_pixel(cipher_to_print->cipher_array[y * cipher_width  + column_to_print]);
        printf("\e[0m");
        // printf("\e[2B\e[D");
        printf("\n");
    }
    printf("\e[%dA",cipher_height);
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
    // fflush(stdout); // Ensure output is displayed
}

//draw 4 ciphers, as part of a quad_display struct
void draw_quad_cipher(quad_display_t * quad_ciphers){
    printf("\n");
    for (int y = 0; y < cipher_height; y++)
    {
        print_row(quad_ciphers->display[0], y);
        print_row(quad_ciphers->display[1], y);
        print_row(quad_ciphers->display[2], y);
        print_row(quad_ciphers->display[3], y);

        printf("\e[0m"); //reset to default
        printf("\n"); //new line to start new row
    }
    printf("\e[0m"); //reset to default
    printf("\n");
    // fflush(stdout); // Ensure output is displayed
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

//fill in cipher array place value section with the correct fragment, in the correct orientation, for the digit specified 
void fill_cipher_section(cipher_t * cipher, int digit, int start_index)
{
    char final_fragment_array[fragment_len];

    if (start_index == ones_place_fragment_start_index)
    {
        //copy the fragment to the display array
        copy_fragment(final_fragment_array, cipher_fragments[digit]);
    }
    else if (start_index == tens_place_fragment_start_index)
    {
        //tens place = ones place with horizontal flip
        flip_fragment_hor(final_fragment_array, cipher_fragments[digit]);
    }
    else if (start_index == hundreds_place_fragment_start_index)
    {
        //hundreds place = ones place with vertical flip
        flip_fragment_vert(final_fragment_array, cipher_fragments[digit]);
    }
    else if (start_index == thousands_place_fragment_start_index)
    {
        //thousands place = ones place with vertical flip and horrizontal flip
        flip_fragment_diag(final_fragment_array, cipher_fragments[digit]);
    }

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

//sets up an empty cipher, and fills in each place value section
void create_cipher(cipher_t * cipher)
{
    //clear the cipher, so we start with all 0s
    make_display_cipher_empty(cipher);
       
    if ((cipher->extracted_place_values.ones_place == 0) & 
        (cipher->extracted_place_values.tens_place == 0) & 
        (cipher->extracted_place_values.hundrends_place == 0) & 
        (cipher->extracted_place_values.thousands_place == 0))
    {
        //if all place values are 0, dont draw the center line, just return early.
        return;  
    }
    else
    {        
        //draw center line
        for (int i = fragment_width; i < cipher_len; i = (i + cipher_width))
        {
            cipher->cipher_array[i] = 1;
        }
        
        //ones place
        fill_cipher_section(cipher, cipher->extracted_place_values.ones_place, ones_place_fragment_start_index);

        //tens place
        fill_cipher_section(cipher, cipher->extracted_place_values.tens_place, tens_place_fragment_start_index);

        //hundres place
        fill_cipher_section(cipher, cipher->extracted_place_values.hundrends_place, hundreds_place_fragment_start_index);

        //thousands place
        fill_cipher_section(cipher, cipher->extracted_place_values.thousands_place, thousands_place_fragment_start_index);
    }
}

void fill_bin_cipher_section(cipher_t * bin_cipher, int digit, int start_index)
{
    char final_fragment_array[fragment_len];

    // copy to temp location, as some fragments need manipulation, depending on place value
    // (represented here by the start index)
    copy_bin_fragment(final_fragment_array, &bin_cipher_fragments[digit], start_index);

    //generic for loop that works for any of the defined start_index values
    int fragment_index = 0;
    int bin_cipher_index = 0;
    for (int i = 0; i < fragment_height; i++)
    {
        for (int j = 0; j < fragment_width; j++)
        {
            bin_cipher->cipher_array[bin_cipher_index + start_index + j] = final_fragment_array[fragment_index + j];
        }
        bin_cipher_index = (bin_cipher_index + cipher_width);
        fragment_index = (fragment_index + fragment_width);
    }

}

void create_binary_cipher(cipher_t * bin_cipher)
{
    make_display_cipher_empty(bin_cipher);
    if ((bin_cipher->extracted_place_values.ones_place == 0) & 
        (bin_cipher->extracted_place_values.tens_place == 0) & 
        (bin_cipher->extracted_place_values.hundrends_place == 0) & 
        (bin_cipher->extracted_place_values.thousands_place == 0))
    {
        //if all place values are 0, dont draw the center line, just return early.
        return;  
    }
    else
    {
        //draw center line
        for (int i = fragment_width; i < cipher_len; i = (i + cipher_width))
        {
            bin_cipher->cipher_array[i] = 1;
        }

        // ones place
        fill_bin_cipher_section(bin_cipher, bin_cipher->extracted_place_values.ones_place, ones_place_fragment_start_index);

        // tens place
        fill_bin_cipher_section(bin_cipher, bin_cipher->extracted_place_values.tens_place, tens_place_fragment_start_index);
        
        // hundres place
        fill_bin_cipher_section(bin_cipher, bin_cipher->extracted_place_values.hundrends_place, hundreds_place_fragment_start_index);

        // thousands place
        fill_bin_cipher_section(bin_cipher, bin_cipher->extracted_place_values.thousands_place, thousands_place_fragment_start_index);

    }

}

void get_place_values(four_digit_place_values_t * output_val, int input_val)
{
    output_val->ones_place = input_val % 10;
    output_val->tens_place = (input_val / 10) % 10;
    output_val->hundrends_place = ((input_val / 10) / 10) % 10;
    output_val->thousands_place = (((input_val / 10) / 10) / 10) % 10;
};

//take the int in the cipher struct and get place values, storing in cipher struct
void extract_place_values_cipher(cipher_t * cipher)
{
    cipher->extracted_place_values.ones_place = cipher->number_to_display % 10;
    cipher->extracted_place_values.tens_place = (cipher->number_to_display / 10) % 10;
    cipher->extracted_place_values.hundrends_place = ((cipher->number_to_display/ 10) / 10) % 10;
    cipher->extracted_place_values.thousands_place = (((cipher->number_to_display / 10) / 10) / 10) % 10;
}

//display an interger value as a cipher
void display_as_cipher(int input_val){
    cipher_t cipher_to_display;
    cipher_to_display.number_to_display = input_val;
    get_place_values(&cipher_to_display.extracted_place_values, cipher_to_display.number_to_display);
    create_cipher(&cipher_to_display);
    draw_cipher(&cipher_to_display);
}

//display two interger values as as ciphers
void display_as_dual_cipher(int left_val, int right_val){
    cipher_t left_cipher;
    cipher_t right_cipher;

    left_cipher.number_to_display = left_val;
    right_cipher.number_to_display = right_val;

    get_place_values(&left_cipher.extracted_place_values, left_cipher.number_to_display);
    get_place_values(&right_cipher.extracted_place_values, right_cipher.number_to_display);
   
    create_cipher(&left_cipher);
    create_cipher(&right_cipher);
    draw_dual_cipher(&left_cipher, &right_cipher);
}

//nulls the pointer for each display
void clear_quad_display(quad_display_t * display_to_clear) 
{
    display_to_clear->display[0] = NULL;
    display_to_clear->display[1] = NULL;
    display_to_clear->display[2] = NULL;
    display_to_clear->display[3] = NULL;
}

//display a quad_cipher struct
void display_quad_ciphers(quad_display_t * quad_display)
{
    cipher_t backup_blank_cipher;
    backup_blank_cipher.number_to_display = 0000;
    extract_place_values_cipher(&backup_blank_cipher);
    create_cipher(&backup_blank_cipher);
    
    for (int i = 0; i < 4; i++)
    {
        if (!(quad_display->display[i] == NULL))
        {
            extract_place_values_cipher(quad_display->display[i]);
            create_cipher(quad_display->display[i]);
        }
        else
        {
            printf("Display %d not initialised, proceeding with blank display\n",i);
            quad_display->display[i] = &backup_blank_cipher;
        }
    }
    
    draw_quad_cipher(quad_display);
}

void display_quad_ciphers_bin(quad_display_t * quad_display)
{
    cipher_t backup_blank_cipher;
    backup_blank_cipher.number_to_display = 0000;
    extract_place_values_cipher(&backup_blank_cipher);
    create_cipher(&backup_blank_cipher);
    
    // iterate over all displays in quad display, and if initialised, 
    // then create the cipher for that display
    for (int i = 0; i < 4; i++)
    {
        if (!(quad_display->display[i] == NULL))
        {
            extract_place_values_cipher(quad_display->display[i]);
            create_binary_cipher(quad_display->display[i]);
        }
        else
        {
            printf("Display %d not initialised, proceeding with blank display\n",i+1);
            quad_display->display[i] = &backup_blank_cipher;
        }
    }
    // drawing to the screen
    draw_quad_cipher(quad_display);
}

void debug_print_binary(int num) {
    // Iterate from the most significant bit to the least significant bit
    for (int i = sizeof(int) * 8 - 1; i >= 0; i--) {
        // Check if the i-th bit is set (1) or not (0)
        // (num >> i) shifts the bit at position 'i' to the least significant position
        // & 1 then isolates that bit
        printf("%d", (num >> i) & 1);
    }
    printf("\n"); // Print a newline character after the binary representation
}

//display a cipher specified with binary values
void print_bin_cipher()
{
    for (int y = 0; y < cipher_height; y++)
    {
        print_row_binary(y);
        printf("\e[0m");
        printf("\n");
    }
}
