#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "ciphers.h"

#define LOOP_INTERVAL_NSEC 1000000000L
// #define PRINT_CHARACTER "\u25cf" //circle
// #define PRINT_CHARACTER "\u25a0 " //Black Square + space
#define PRINT_CHARACTER "\u25fc " //Black Square
// #define PRINT_ON_COLOUR "\e[95m" // bright pink foreground
#define PRINT_ON_COLOUR "\e[93m" // bright yellow foreground
#define PRINT_BG_COLOUR "\e[43m" //yellow BG
#define PRINT_OFF_COLOUR "\e[90m" // bright black foreground
// #define PRINT_OFF_COLOUR "\e[30m" // dark black foreground
// #define PRINT_CHARACTER "\u25fc " //Black Mediun Square
// #define PRINT_CHARACTER "\u25fb " //White Mediun Square

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

void copy_bin_fragment(char * save_location, const uint8_t * input_bin_fragment)
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
            
            if (*input_bin_fragment & (0b100000 >> len_count))
            {
                save_location[y * fragment_width + x] = 1;
            }
            else
            {
                save_location[y * fragment_width + x] = 0;
            }
            // if ((input_bin_fragment[y]) & (0b10 >> x))
            // {
            //     save_location[y * fragment_width + x] = 1;
            // }
            // else
            // {
            //     save_location[y * fragment_width + x] = 0;
            // }

            
            // printf("save_location[%d * %d + %d] = \t\t",y,fragment_width,x);
            // debug_print_binary(save_location[y * fragment_width + x]);
            // printf("\n");
            len_count++;
        }
    }
}

void flip_bin_fragment_vert(char * save_location, const uint8_t * input_bin_fragment)
{
    int len_count = 0;
    for(int y = 0; y < fragment_height; y++)
    {
        for(int x = 0; x < fragment_width; x++)
        {
            // printf("input_bin_fragment[%d] =\t\t\t",y);
            // debug_print_binary(input_bin_fragment[y]);
            // printf("input_bin_fragment[%d] & (0b10 >> %d) =\t",y,x);
            // debug_print_binary(input_bin_fragment[y] & (0b10 >> x));

            // save_location[((fragment_height - 1) - y) * fragment_width + x] = input_bin_fragment[y] & (0b10 >> x);

            if ((*input_bin_fragment) & (0b100000 >> len_count))
            {
                save_location[((fragment_height - 1) - y) * fragment_width + x] = 1;
            }
            else
            {
                save_location[((fragment_height - 1) - y) * fragment_width + x] = 0;
            }

            // printf("save_location([(%d - 1) - %d) * %d + %d] = \t",fragment_height,y,fragment_width,x);
            // debug_print_binary(save_location[((fragment_height - 1) - y) * fragment_width + x]);
            // printf("\n");
            len_count++;
        }
    }
}

void flip_bin_fragment_hor(char * save_location, const uint8_t * input_bin_fragment)
{
    int len_count = 0;
    for(int y = 0; y < fragment_height; y++)
    {
        for(int x = 0; x < fragment_width; x++)
        {
            // printf("input_bin_fragment[%d] =\t\t\t",y);
            // debug_print_binary(input_bin_fragment[y]);
            // printf("input_bin_fragment[%d] & (0b10 >> %d) =\t",y,x);
            // debug_print_binary(input_bin_fragment[y] & (0b10 >> x));

            // save_location[y * fragment_width + ((fragment_width - 1) - x)] = input_bin_fragment[y] & (0b10 >> x);

            if ((*input_bin_fragment) & (0b100000 >> len_count))
            {
                save_location[y * fragment_width + ((fragment_width - 1) - x)] = 1;
            }
            else
            {
                save_location[y * fragment_width + ((fragment_width - 1) - x)]= 0;
            }

            // printf("save_location[(%d * %d + (( %d - 1) - %d] = ",y,fragment_width,fragment_height,x);
            // debug_print_binary(save_location[y * fragment_width + ((fragment_width - 1) - x)]);
            // printf("\n");
            len_count++;
        }
    }
}

void flip_bin_fragment_diag(char * save_location, const uint8_t * input_bin_fragment)
{
    int len_count = 0;
    for(int y = 0; y < fragment_height; y++)
    {
        for(int x = 0; x < fragment_width; x++)
        {
            // printf("input_bin_fragment[%d] =\t\t\t\t\t",y);
            // debug_print_binary(input_bin_fragment[y]);
            // printf("input_bin_fragment[%d] & (0b10 >> %d) =\t\t\t",y,x);
            // debug_print_binary(input_bin_fragment[y] & (0b10 >> x));

            // save_location[((fragment_height - 1) - y) * fragment_width + ((fragment_width - 1) - x)] = input_bin_fragment[y] & (0b10 >> x);

            if ((*input_bin_fragment) & (0b100000 >> len_count))
            {
                save_location[((fragment_height - 1) - y) * fragment_width + ((fragment_width - 1) - x)] = 1;
            }
            else
            {
                save_location[((fragment_height - 1) - y) * fragment_width + ((fragment_width - 1) - x)] = 0;
            }

        //     printf("save_location[((%d - 1) - %d) * %d + ((%d - 1) - %d)] = \t",fragment_height,y,fragment_width,fragment_width,x);
        //     debug_print_binary(save_location[((fragment_height - 1) - y) * fragment_width + ((fragment_width - 1) - x)]);
        //     printf("\n");
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
        print_row(quad_ciphers->display_one, y);
        print_row(quad_ciphers->display_two, y);
        print_row(quad_ciphers->display_three, y);
        print_row(quad_ciphers->display_four, y);

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

    if (start_index == ones_place_fragment_start_index)
    {
        //copy the fragment to the display array
        copy_bin_fragment(final_fragment_array, &bin_cipher_fragments[digit]);
    }
    else if (start_index == tens_place_fragment_start_index)
    {
        //tens place = ones place with horizontal flip
        flip_bin_fragment_hor(final_fragment_array, &bin_cipher_fragments[digit]);
    }
    else if (start_index == hundreds_place_fragment_start_index)
    {
        //hundreds place = ones place with vertical flip
        flip_bin_fragment_vert(final_fragment_array, &bin_cipher_fragments[digit]);
    }
    else if (start_index == thousands_place_fragment_start_index)
    {
        //thousands place = ones place with vertical flip and horrizontal flip
        flip_bin_fragment_diag(final_fragment_array, &bin_cipher_fragments[digit]);
    }

    // printf("digit: %d, start_index: %d",digit,start_index);
    // draw_fragment(final_fragment_array);

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

        fill_bin_cipher_section(bin_cipher, bin_cipher->extracted_place_values.ones_place, ones_place_fragment_start_index);

        //tens place
        fill_bin_cipher_section(bin_cipher, bin_cipher->extracted_place_values.tens_place, tens_place_fragment_start_index);
        
        //hundres place
        fill_bin_cipher_section(bin_cipher, bin_cipher->extracted_place_values.hundrends_place, hundreds_place_fragment_start_index);

        //thousands place
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
    display_to_clear->display_one = NULL;
    display_to_clear->display_two = NULL;
    display_to_clear->display_three = NULL;
    display_to_clear->display_four = NULL;
}

//display a quad_cipher struct
void display_quad_ciphers(quad_display_t * quad_display)
{
    cipher_t backup_blank_cipher;
    backup_blank_cipher.number_to_display = 0000;
    extract_place_values_cipher(&backup_blank_cipher);
    create_cipher(&backup_blank_cipher);
    
    //Display 1
    if (!(quad_display->display_one == NULL))
    {
        extract_place_values_cipher(quad_display->display_one);
        create_cipher(quad_display->display_one);
    }
    else
    {
        printf("Display one not initialised, proceeding with blank display\n");
        quad_display->display_one = &backup_blank_cipher;
    }

    //Display 2
    if (!(quad_display->display_two == NULL))
    {
        extract_place_values_cipher(quad_display->display_two);
        create_cipher(quad_display->display_two);
    }
    else
    {
        printf("Display two not initialised, proceeding with blank display\n");
        quad_display->display_two = &backup_blank_cipher;
    }

    //Display 3
    if (!(quad_display->display_three == NULL))
    {
        extract_place_values_cipher(quad_display->display_three);
        create_cipher(quad_display->display_three);
    }
    else
    {
        printf("Display three not initialised, proceeding with blank display\n");
        quad_display->display_three = &backup_blank_cipher;
    }

    //Display 4
    if (!(quad_display->display_four == NULL))
    {
        extract_place_values_cipher(quad_display->display_four);
        create_cipher(quad_display->display_four);
    }
    else
    {
        printf("Display four not initialised, proceeding with blank display\n");
        quad_display->display_four = &backup_blank_cipher;
    }
    draw_quad_cipher(quad_display);
}

void display_quad_ciphers_bin(quad_display_t * quad_display)
{
    cipher_t backup_blank_cipher;
    backup_blank_cipher.number_to_display = 0000;
    extract_place_values_cipher(&backup_blank_cipher);
    create_cipher(&backup_blank_cipher);
    
    //Display 1
    if (!(quad_display->display_one == NULL))
    {
        extract_place_values_cipher(quad_display->display_one);
        create_binary_cipher(quad_display->display_one);
    }
    else
    {
        printf("Display one not initialised, proceeding with blank display\n");
        quad_display->display_one = &backup_blank_cipher;
    }

    //Display 2
    if (!(quad_display->display_two == NULL))
    {
        extract_place_values_cipher(quad_display->display_two);
        create_binary_cipher(quad_display->display_two);
    }
    else
    {
        printf("Display two not initialised, proceeding with blank display\n");
        quad_display->display_two = &backup_blank_cipher;
    }

    //Display 3
    if (!(quad_display->display_three == NULL))
    {
        extract_place_values_cipher(quad_display->display_three);
        create_binary_cipher(quad_display->display_three);
    }
    else
    {
        printf("Display three not initialised, proceeding with blank display\n");
        quad_display->display_three = &backup_blank_cipher;
    }

    //Display 4
    if (!(quad_display->display_four == NULL))
    {
        extract_place_values_cipher(quad_display->display_four);
        create_binary_cipher(quad_display->display_four);
    }
    else
    {
        printf("Display four not initialised, proceeding with blank display\n");
        quad_display->display_four = &backup_blank_cipher;
    }
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

//initial setup fuction for displaying ciphers
void setup(void)
{
    //hide cursor
    printf("\e[?25l");

    // enter alternate buffer
    printf("\e[?1049h");

    //clear alternate buffer
    printf("\e[2J");

    //set cursor to top left
    printf("\e[H");

    setbuf(stdout, NULL);
}

//cleanup function to restore terminal behavior at program exit
void cleanup(void)
{
    //clean up the alternate buffer
	printf("\e[2J");

    // back to standard buffer
    printf("\e[?1049l");

    //show the cursor again
	printf("\e[?25h");

    //reset the stdout buffer to line buffering
    setvbuf(stdout,NULL,_IOLBF,BUFSIZ);
}

void cleanup_die(int i)
{
    exit(i);
}

void wait_remainder(struct timespec* sleep_ns, struct timespec *end, struct timespec *start)
{
    sleep_ns->tv_sec = end->tv_sec - start->tv_sec;
    sleep_ns->tv_nsec = LOOP_INTERVAL_NSEC - (end->tv_nsec - start->tv_nsec);
    if (sleep_ns->tv_sec <= 0) {
        nanosleep(sleep_ns, NULL);
    }
}

int main () 
{
    setup();
    atexit(cleanup);
    signal(SIGTERM, cleanup_die);
    signal(SIGINT, cleanup_die);

    //start of binary cipher testing
    // cipher_t binary_cipher;

    // for (int i = 0; i < 10; i++)
    // {
    //     binary_cipher.number_to_display = 1111 * i;
    //     extract_place_values_cipher(&binary_cipher);
    //     create_binary_cipher(&binary_cipher);
    //     draw_cipher(&binary_cipher);
    //     fflush(stdout);
    // }
    // while (1)
    // {
    //     /* code */
    // }
    
    // return 0;
    //end of binary cipher testing
    

    struct timespec start, end, sleep_ns;

    time_t rawtime;
    struct tm *info;

    quad_display_t time_as_monk_cipher;
    clear_quad_display(&time_as_monk_cipher);

    cipher_t year_cipher;
    cipher_t date_cipher;
    cipher_t hours_mins_cipher;
    cipher_t seconds_cipher;

    time_as_monk_cipher.display_one = &year_cipher;
    time_as_monk_cipher.display_two = &date_cipher;
    time_as_monk_cipher.display_three = &hours_mins_cipher;
    time_as_monk_cipher.display_four = &seconds_cipher;

    while(1)
    {
        //start measuring time
        clock_gettime(CLOCK_MONOTONIC, &start);

        time(&rawtime);
        info = localtime(&rawtime);
        // printf("Current time:\t[YYYY] [MM:DD] [HH:mm] [00:ss]\n");

        //update the int varialbes in each cipher
        year_cipher.number_to_display = 1900 + info->tm_year;
        date_cipher.number_to_display = ((info->tm_mon + 1) * 100) + info->tm_mday;
        hours_mins_cipher.number_to_display = (info->tm_hour * 100) + info->tm_min;
        seconds_cipher.number_to_display = info->tm_sec;

        // // display_as_cipher(local_time_int);
        // printf("debug\t\t[%04d] [%04d]  [%04d]  [%04d]\n",  year_cipher.number_to_display,
        //                                                 date_cipher.number_to_display,
        //                                                 hours_mins_cipher.number_to_display, 
        //                                                 seconds_cipher.number_to_display);
        
        //main drawing call for updated quad_display
        display_quad_ciphers_bin(&time_as_monk_cipher);

        // Ensure output is displayed
        fflush(stdout); 

        //grab another time to compare with start
        clock_gettime(CLOCK_MONOTONIC, &end);

        //calculates how much time is left in the second, and will wait that much time
        wait_remainder(&sleep_ns, &end, &start);
        
        //move cursor back %d lines
        printf("\e[G\e[%dA", 9); 
    }

    return 0;
};