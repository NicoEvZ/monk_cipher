#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "ciphers.h"

#define LOOP_INTERVAL_SEC 0.02
#define LOOP_INTERVAL_NSEC (long)(LOOP_INTERVAL_SEC * 1000000000)
// #define LOOP_INTERVAL_NSEC 500000

typedef enum 
{
    CIPHER = 0,
    DIGIT_YYYY,
    DIGIT_MMDD,
    DIGIT_HHMM,
} display_state;

const int display_state_times[] =
{
    18, 4, 4, 4,
};

const display_state display_state_order[] = 
{
    CIPHER, DIGIT_YYYY, DIGIT_MMDD, DIGIT_HHMM,
};


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

    atexit(cleanup);
    signal(SIGTERM, cleanup_die);
    signal(SIGINT, cleanup_die);
    signal(SIGKILL, cleanup_die);
}

void wait_remainder(struct timespec* sleep_ns, struct timespec *end, struct timespec *start)
{
    sleep_ns->tv_sec = end->tv_sec - start->tv_sec;
    sleep_ns->tv_nsec = LOOP_INTERVAL_NSEC - (end->tv_nsec - start->tv_nsec);
    if (sleep_ns->tv_sec <= 0) {
        nanosleep(sleep_ns, NULL);
    }
}

void update_time_ciphers(cipher_t * year_cipher, cipher_t * date_cipher, cipher_t * hours_mins_cipher, cipher_t * seconds_cipher)
{
    time_t rawtime;
    struct tm *info;

    time(&rawtime);
    info = localtime(&rawtime);

    //update the int variables in each cipher
    year_cipher->number_to_display = 1900 + info->tm_year;
    extract_place_values_cipher(year_cipher);
    date_cipher->number_to_display = ((info->tm_mon + 1) * 100) + info->tm_mday;
    extract_place_values_cipher(date_cipher);
    hours_mins_cipher->number_to_display = (info->tm_hour * 100) + info->tm_min;
    extract_place_values_cipher(hours_mins_cipher);
    seconds_cipher->number_to_display = info->tm_sec;
    extract_place_values_cipher(seconds_cipher);

}

void one_second_toggle_states(quad_display_t * display_ciphers, quad_display_t * display_digits, int tic, display_state state)
{
    int max_run_time_s = display_state_times[state];
    
    // we only want to toggle even number of times.
    
    if (max_run_time_s%2 != 0)
    {
        //odd, so remove one to make it even
        max_run_time_s = max_run_time_s-1;
    }

    if (max_run_time_s == 0)
    {
        // display ciphers
        display_quad_ciphers(display_ciphers);
    }

    // int max_run_time_tics = max_run_time_s * 50; //50 tics in 1 seccond
    
    // 0 < tic < 51     -> we are in the first second
    // 50 < tic < 101   -> we are in the second second
    // 100 < tic < 151  -> we are in the third second

    int second_we_are_in = tic / 50 + 1;

    // alternate every second

    if (second_we_are_in%2 != 0)
    {
        //odd secconds
        //display ciphers
        display_quad_ciphers(display_ciphers);
    }
    else
    {
        //display digits
        display_quad_cipher_digits(display_digits);
    }
}

void show_time(display_state state, int tic)
{

    quad_display_t time_and_date_as_monk_cipher, quad_display_digits;
    clear_quad_display(&time_and_date_as_monk_cipher);
    clear_quad_display(&quad_display_digits);

    cipher_t year_cipher, date_cipher, hours_mins_cipher, seconds_cipher;
    cipher_t disp_zero, disp_one, disp_two, disp_three; 

    time_and_date_as_monk_cipher.display[0] = &year_cipher;
    time_and_date_as_monk_cipher.display[1] = &date_cipher;
    time_and_date_as_monk_cipher.display[2] = &hours_mins_cipher;
    time_and_date_as_monk_cipher.display[3] = &seconds_cipher;

    quad_display_digits.display[0] = &disp_zero;
    quad_display_digits.display[1] = &disp_one;
    quad_display_digits.display[2] = &disp_two;
    quad_display_digits.display[3] = &disp_three;

    update_time_ciphers(&year_cipher, &date_cipher, &hours_mins_cipher, &seconds_cipher);

    // printf("%d",state);

    switch (state)
    {
    case CIPHER:
        display_quad_ciphers(&time_and_date_as_monk_cipher); 
        break;
    
    case DIGIT_YYYY:
        disp_zero.number_to_display = year_cipher.place_values[THOUSANDS_PLACE];
        disp_one.number_to_display = year_cipher.place_values[HUNDREDS_PLACE];
        disp_two.number_to_display = year_cipher.place_values[TENS_PLACE];
        disp_three.number_to_display = year_cipher.place_values[ONES_PLACE];

        date_cipher.number_to_display = 0;
        hours_mins_cipher.number_to_display = 0;
        seconds_cipher.number_to_display = 0;

        // display_quad_cipher_digits(&quad_display_digits);
        one_second_toggle_states(&time_and_date_as_monk_cipher,&quad_display_digits,tic,state);
        break;
    
    case DIGIT_MMDD:
        disp_zero.number_to_display = date_cipher.place_values[THOUSANDS_PLACE];
        disp_one.number_to_display = date_cipher.place_values[HUNDREDS_PLACE];
        disp_two.number_to_display = date_cipher.place_values[TENS_PLACE];
        disp_three.number_to_display = date_cipher.place_values[ONES_PLACE];

        year_cipher.number_to_display = 0;
        hours_mins_cipher.number_to_display = 0;
        seconds_cipher.number_to_display = 0;

        // display_quad_cipher_digits(&quad_display_digits);
        one_second_toggle_states(&time_and_date_as_monk_cipher,&quad_display_digits,tic,state);

        break;

    case DIGIT_HHMM:
        disp_zero.number_to_display = hours_mins_cipher.place_values[THOUSANDS_PLACE];
        disp_one.number_to_display = hours_mins_cipher.place_values[HUNDREDS_PLACE];
        disp_two.number_to_display = hours_mins_cipher.place_values[TENS_PLACE];
        disp_three.number_to_display = hours_mins_cipher.place_values[ONES_PLACE];

        year_cipher.number_to_display = 0;
        date_cipher.number_to_display = 0;
        seconds_cipher.number_to_display = 0;

        // display_quad_cipher_digits(&quad_display_digits);
        one_second_toggle_states(&time_and_date_as_monk_cipher,&quad_display_digits,tic,state);
        break;
    default:
        break;
    }

    // Ensure output is displayed
    fflush(stdout); 
    //move cursor back %d lines
    printf("\e[G\e[%dA", 9); 
}

/*Returns the next state in the sequence, wrapping around to the start if required.*/ 
display_state next_state(display_state current_state)
{
    // calculate how many items in order array
    int array_length = sizeof(display_state_order) / sizeof(display_state_order[0]);
    
    // find the position of the current state in the state order array
    int current_state_index = 0;
    for (int i = 0; i < array_length; i++)
    {
        if(current_state == display_state_order[i])
        {
            current_state_index = i;
        }
    }

    // return the next state in the sequence, otherwise wrap around to the start
    if (current_state_index < (array_length - 1))
    {
        return display_state_order[current_state_index + 1];
    }

    // reset to the start of the state order array
    return display_state_order[0];
};

int main () 
{
    setup();
    
    struct timespec sleep_ns;
    sleep_ns.tv_sec = 0;
    sleep_ns.tv_nsec = LOOP_INTERVAL_NSEC;
    long double time_in_state = 0;
    int tic = 0;
    
    display_state current_state = CIPHER; 


 
    
    
    while(1){
        time_t rawtime;
        struct tm *info; 
        cipher_t cipher;
        time(&rawtime);
        info = localtime(&rawtime);
        cipher.number_to_display=info->tm_sec;
        display_single_cipher(&cipher);
        nanosleep(&sleep_ns, NULL);
        //move cursor back %d lines
        printf("\e[G\e[%dA", 9);
        fflush(stdout);
    }

    while (1)
    {
        time_in_state += LOOP_INTERVAL_SEC;
        tic++;
        
        show_time(current_state, tic);

        if (time_in_state >= display_state_times[current_state])
        {
            // current_state = next_state(current_state);
            current_state = next_state(current_state);
            time_in_state = 0;
            tic = 0;
        }
        nanosleep(&sleep_ns, NULL);
    }
    
    // Below is original main loop. Was canibalised a bit

    // while(1)
    // {
    //     //start measuring time
    //     clock_gettime(CLOCK_MONOTONIC, &start);

    //     time(&rawtime);
    //     info = localtime(&rawtime);

    //     //update the int variables in each cipher
    //     year_cipher.number_to_display = 1900 + info->tm_year;
    //     extract_place_values_cipher(&year_cipher);
    //     date_cipher.number_to_display = ((info->tm_mon + 1) * 100) + info->tm_mday;
    //     hours_mins_cipher.number_to_display = (info->tm_hour * 100) + info->tm_min;
    //     seconds_cipher.number_to_display = info->tm_sec;

    //     if (draw_as_cipher)
    //     {
    //         display_quad_ciphers(&time_and_date_as_monk_cipher);    
    //     }
    //     else
    //     {
    //         quad_display_digits.display[0] = &disp_zero;
    //         quad_display_digits.display[1] = &disp_one;
    //         quad_display_digits.display[2] = &disp_two;
    //         quad_display_digits.display[3] = &disp_three;

    //         disp_zero.number_to_display = year_cipher.place_values[THOUSANDS_PLACE];
    //         disp_one.number_to_display = year_cipher.place_values[HUNDREDS_PLACE];
    //         disp_two.number_to_display = year_cipher.place_values[TENS_PLACE];
    //         disp_three.number_to_display = year_cipher.place_values[ONES_PLACE];

    //         display_quad_cipher_digits(&quad_display_digits);
    //     }

    //     //main drawing call for updated quad_display

    //     // Ensure output is displayed
    //     fflush(stdout); 

    //     //grab another time to compare with start
    //     clock_gettime(CLOCK_MONOTONIC, &end);

    //     time_and_date_as_monk_cipher.colour_incrementer++;

    //     //calculates how much time is left in the second, and will wait that much time
    //     wait_remainder(&sleep_ns, &end, &start);

    //     draw_as_cipher = !draw_as_cipher;
        
    //     //move cursor back %d lines
    //     printf("\e[G\e[%dA", 9); 
    // }

    return 0;
};