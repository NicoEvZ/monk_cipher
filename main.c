#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "ciphers.h"

#define LOOP_INTERVAL_NSEC 1000000000L
// #define PRINT_CHARACTER "\u25cf" //circle
#define PRINT_CHARACTER "\u25a0 " //square with space


char * flip_fragment_vert(char * save_locaion, const char * input_fragment)
{
    for(int y = 0; y < fragment_height; y++)
    {
        for(int x = 0; x < fragment_width; x++)
        {
            save_locaion[((fragment_height - 1) - y) * fragment_width + x] = input_fragment[y * fragment_width  + x];
        }
    }
}

char * flip_fragment_hor(char * save_locaion, const char * input_fragment)
{
    for(int y = 0; y < fragment_height; y++)
    {
        for(int x = 0; x < fragment_width; x++)
        {
            save_locaion[y * fragment_width + ((fragment_width - 1) -x)] = input_fragment[y * fragment_width  + x];
        }
    }
}

void print_pixel(int binary_val)
{
    if (binary_val > 0) //if the value at position is 1, print yellow spaces
    {
        printf("\e[93m%s",PRINT_CHARACTER); //yellow background and 2 spaces
    }
    else 
    {
        printf("\e[90m%s",PRINT_CHARACTER); //black background and 2 spaces
        // printf("\e[104m  "); //blue background and 2 spaces
    }
}

void print_row(cipher_t * cipher_to_print, int row_to_print)
{
    for (int x = 0; x < cipher_width; x++)
    {
        print_pixel(cipher_to_print->cipher_array[row_to_print * cipher_width  + x]);
    }
    printf("\e[0m");
    printf(" ");
}

void print_row_binary(int row_to_print)
{
    for (int x = 0; x < cipher_height; x++)
    {
        print_pixel(bin_cipher_1234[row_to_print] & (0b10000 >> x));
    }
}

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
    fflush(stdout); // Ensure output is displayed
}

void draw_quad_cipher(cipher_t * first_cipher, cipher_t * seccond_cipher, cipher_t * third_cipher, cipher_t * fourth_cipher){
    printf("\n");
    for (int y = 0; y < cipher_height; y++)
    {
        print_row(first_cipher, y);
        print_row(seccond_cipher, y);
        print_row(third_cipher, y);
        print_row(fourth_cipher, y);

        printf("\e[0m"); //reset to default
        printf("\n"); //new line to start new row
    }
    printf("\e[0m"); //reset to default
    printf("\n");
    fflush(stdout); // Ensure output is displayed
}

void create_all_on_cipher(cipher_t * output_cipher)
{
    for (int i = 0; i < cipher_len; i++)
    {
          output_cipher->cipher_array[i] = full_cipher[i];
    }
}

void create_cipher_from_ints(cipher_t * output_cipher, int ones_val, int tens_val, int hundreds_val, int thousands_val)
{

    for (int i = 0; i < cipher_len; i++)
    {   
        // print blank if all 0s, otherwise would be a cipher with a line down the middle
        if ((ones_val == 0) & (tens_val == 0) & (hundreds_val == 0) & (thousands_val == 0))
        {
            output_cipher->cipher_array[i] = blank_cipher[i];
        }
        else
        {
            output_cipher->cipher_array[i] = ones_place_ciphers[ones_val][i] || 
                                            tens_place_ciphers[tens_val][i] || 
                                            hundreds_place_ciphers[hundreds_val][i] || 
                                            thousands_place_ciphers[thousands_val][i];
        }
    }
}

void create_cipher(cipher_t * cipher)
{
    for (int i = 0; i < cipher_len; i++)
    {
        if ((cipher->extracted_place_values.ones_place == 0) & 
            (cipher->extracted_place_values.tens_place == 0) & 
            (cipher->extracted_place_values.hundrends_place == 0) & 
            (cipher->extracted_place_values.thousands_place == 0))
        {
            cipher->cipher_array[i] = blank_cipher[i];
        }
        else
        {
            cipher->cipher_array[i] = ones_place_ciphers[cipher->extracted_place_values.ones_place][i] || 
                                            tens_place_ciphers[cipher->extracted_place_values.tens_place][i] || 
                                            hundreds_place_ciphers[cipher->extracted_place_values.hundrends_place][i] || 
                                            thousands_place_ciphers[cipher->extracted_place_values.thousands_place][i];
        }
    }   
}

void fill_cipher_from_fragment(cipher_t * cipher, int digit, int start_index)
{
    char * final_fragment_array;

    if (start_index == ones_place_fragment_start_index)
    {
        final_fragment_array = cipher_fragments[digit];
    }
    else if (start_index == tens_place_fragment_start_index)
    {
        //tens place = ones place with horizontal flip
        char tens_fragment[fragment_len];
        flip_fragment_hor(tens_fragment,cipher_fragments[digit]);
        final_fragment_array = tens_fragment;

    }
    else if (start_index == hundreds_place_fragment_start_index)
    {
        //hundreds place = ones place with vertical flip
        char hundreds_fragment[fragment_len];
        flip_fragment_vert(hundreds_fragment, cipher_fragments[digit]);
        final_fragment_array = hundreds_fragment;
    }
    else if (start_index == thousands_place_fragment_start_index)
    {
        //thousands place = ones place with vertical flip and horrizontal flip
        char thousands_fragment_temp[fragment_len];
        char thousands_fragment[fragment_len];
        flip_fragment_hor(thousands_fragment_temp,cipher_fragments[digit]);
        flip_fragment_vert(thousands_fragment, thousands_fragment_temp);
        final_fragment_array = thousands_fragment;
    }

    //generic for loop called for each start_index
    int fragment_index = 0;
    int cipher_index = 0;
    for (int i = 0; i < fragment_height; i++)
    {
        cipher->cipher_array[cipher_index + start_index] = final_fragment_array[fragment_index];
        cipher->cipher_array[cipher_index + start_index +1] = final_fragment_array[fragment_index + 1];
        cipher_index = (cipher_index + cipher_width);
        fragment_index = (fragment_index + fragment_width);
    }
}

void create_cipher_from_fragments(cipher_t * cipher)
{
    for (int i = 0; i < cipher_len; i++)
    {
        cipher->cipher_array[i] = 0;
    }
       
    if ((cipher->extracted_place_values.ones_place == 0) & 
        (cipher->extracted_place_values.tens_place == 0) & 
        (cipher->extracted_place_values.hundrends_place == 0) & 
        (cipher->extracted_place_values.thousands_place == 0))
    {
        return;  
    }
    else
    {        
        //draw center line
        for (int i = 2; i < cipher_len; i = (i + cipher_width))
        {
            cipher->cipher_array[i] = 1;
        }
        
        //ones place
        fill_cipher_from_fragment(cipher, cipher->extracted_place_values.ones_place, ones_place_fragment_start_index);

        //tens place
        fill_cipher_from_fragment(cipher, cipher->extracted_place_values.tens_place, tens_place_fragment_start_index);

        //hundres place
        fill_cipher_from_fragment(cipher, cipher->extracted_place_values.hundrends_place, hundreds_place_fragment_start_index);

        //thousands place
        fill_cipher_from_fragment(cipher, cipher->extracted_place_values.thousands_place, thousands_place_fragment_start_index);
    }
}

char * flip_cipher_vert(char * save_locaion, const char * input_cipher)
{
    for(int y = 0; y < cipher_height; y++)
    {
        for(int x = 0; x < cipher_width; x++)
        {
            save_locaion[((cipher_height - 1) - y) * cipher_width + x] = input_cipher[y * cipher_width  + x];
        }
    }
}

char * flip_cipher_hor(char * save_locaion, const char * input_cipher)
{
    for(int y = 0; y < cipher_height; y++)
    {
        for(int x = 0; x < cipher_width; x++)
        {
            save_locaion[y * cipher_width + ((cipher_width - 1) -x)] = input_cipher[y * cipher_width  + x];
        }
    }
}

void get_place_values(four_digit_place_values_t * output_val, int input_val)
{
    output_val->ones_place = input_val % 10;
    output_val->tens_place = (input_val / 10) % 10;
    output_val->hundrends_place = ((input_val / 10) / 10) % 10;
    output_val->thousands_place = (((input_val / 10) / 10) / 10) % 10;
};

void display_as_cipher(int input_val){
    cipher_t cipher_to_display;
    cipher_to_display.number_to_display = input_val;
    get_place_values(&cipher_to_display.extracted_place_values, cipher_to_display.number_to_display);
    create_cipher(&cipher_to_display);
    draw_cipher(&cipher_to_display);
}

void sleep_ms_nanosleep(int milliseconds) {
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000; // Convert remaining milliseconds to nanoseconds
    nanosleep(&ts, NULL);
}

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

void display_quad_ciphers(quad_display_t * quad_display)
{
    //Display 1
    if (!(quad_display->display_one == NULL))
    {
        get_place_values(&quad_display->display_one->extracted_place_values, quad_display->display_one->number_to_display);
        // create_cipher(quad_display->display_one);
        create_cipher_from_fragments(quad_display->display_one);
    }
    else
    {
        printf("Display one not initialised, proceeding with blank display\n");
        cipher_t backup_blank_cipher;
        create_cipher_from_ints(&backup_blank_cipher,0,0,0,0);
        quad_display->display_one = &backup_blank_cipher;
    }

    //Display 2
    if (!(quad_display->display_two == NULL))
    {
        get_place_values(&quad_display->display_two->extracted_place_values, quad_display->display_two->number_to_display);
        create_cipher(quad_display->display_two);
    }
    else
    {
        printf("Display one not initialised, proceeding with blank display\n");
        cipher_t backup_blank_cipher;
        create_cipher_from_ints(&backup_blank_cipher,0,0,0,0);
        quad_display->display_two = &backup_blank_cipher;
    }

    //Display 3
    if (!(quad_display->display_three == NULL))
    {
        get_place_values(&quad_display->display_three->extracted_place_values, quad_display->display_three->number_to_display);
        create_cipher(quad_display->display_three);
    }
    else
    {
        printf("Display one not initialised, proceeding with blank display\n");
        cipher_t backup_blank_cipher;
        create_cipher_from_ints(&backup_blank_cipher,0,0,0,0);
        quad_display->display_three = &backup_blank_cipher;
    }

    //Display 4
    if (!(quad_display->display_four == NULL))
    {
        get_place_values(&quad_display->display_four->extracted_place_values, quad_display->display_four->number_to_display);
        create_cipher(quad_display->display_four);
    }
    else
    {
        printf("Display one not initialised, proceeding with blank display\n");
        cipher_t backup_blank_cipher;
        create_cipher_from_ints(&backup_blank_cipher,0,0,0,0);
        quad_display->display_four = &backup_blank_cipher;
    }
    draw_quad_cipher(quad_display->display_one, quad_display->display_two, quad_display->display_three, quad_display->display_four);
}

void printBinary(int num) {
    // Iterate from the most significant bit to the least significant bit
    for (int i = sizeof(int) * 8 - 1; i >= 0; i--) {
        // Check if the i-th bit is set (1) or not (0)
        // (num >> i) shifts the bit at position 'i' to the least significant position
        // & 1 then isolates that bit
        printf("%d", (num >> i) & 1);
    }
    // printf("\n"); // Print a newline character after the binary representation
}

void print_bin_cipher()
{
    for (int y = 0; y < cipher_height; y++)
    {
        // printf("bin_cipher[%d] = ",y);
        // printBinary(bin_cipher_1234[y]);
        // printf("\n");
        for (int x = 0; x < cipher_width; x++)
        {
            // printf("16>>%d = ",x);
            // printBinary(16>>x);
            // printf("\n");
            print_pixel(bin_cipher_1234[y] & (0b10000>>x));
            // printf("Result of AND = ");
            // printBinary(bin_cipher_1234[y] & (16>>x));
            // printf("\n");
        }
        printf("\e[0m");
        printf("\n");
    }
}

int main () 
{
    // cipher_t test_cipher;
    // test_cipher.number_to_display = 1234;
    // get_place_values(&test_cipher.extracted_place_values,test_cipher.number_to_display);
    // printf("%4d %d %d %d %d",test_cipher.number_to_display, 
    //     test_cipher.extracted_place_values.thousands_place, 
    //     test_cipher.extracted_place_values.hundrends_place,
    //     test_cipher.extracted_place_values.tens_place,
    //     test_cipher.extracted_place_values.ones_place);
    // create_cipher_from_fragments(&test_cipher);
    // printf("Final:\n");
    // draw_cipher(&test_cipher);
    // return 0;
    // printf("Test binary number: %d\n",);
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
        clock_gettime(CLOCK_MONOTONIC, &start);

        printf("\e[?25l"); //hide cursor
        fflush(stdout); // Ensure escape codes are sent immediately

        time(&rawtime);
        info = localtime(&rawtime);
        printf("Current time:\t[YYYY] [MM:DD] [HH:mm] [00:ss]\n");

        //update the int varialbes in each cipher
        year_cipher.number_to_display = 1900 + info->tm_year;
        date_cipher.number_to_display = ((info->tm_mon + 1) * 100) + info->tm_mday;
        hours_mins_cipher.number_to_display = (info->tm_hour * 100) + info->tm_min;
        seconds_cipher.number_to_display = info->tm_sec;

        // display_as_cipher(local_time_int);
        printf("debug\t\t[%04d] [%04d]  [%04d]  [%04d]\n",  year_cipher.number_to_display,
                                                        date_cipher.number_to_display,
                                                        hours_mins_cipher.number_to_display, 
                                                        seconds_cipher.number_to_display);
        // clear_quad_display(&time_as_monk_cipher);
        
        display_quad_ciphers(&time_as_monk_cipher);

        clock_gettime(CLOCK_MONOTONIC, &end);
        sleep_ns.tv_sec = end.tv_sec - start.tv_sec;
        sleep_ns.tv_nsec = LOOP_INTERVAL_NSEC - (end.tv_nsec - start.tv_nsec);
        if (sleep_ns.tv_sec <= 0) {
            nanosleep(&sleep_ns, NULL);
        }
        printf("\e[G\e[%dA\e[J", 11); //move cursor back %d lines, and clear
        fflush(stdout); // Ensure output is displayed
    }

    return 0;
};