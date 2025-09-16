#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "ciphers.h"

#define LOOP_INTERVAL_NSEC 1000000000L


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