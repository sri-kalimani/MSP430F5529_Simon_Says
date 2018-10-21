#include <msp430.h>
#include <stdio.h>
#include <stdlib.h>
#include "peripherals.h"

#define NUM_NOTES 4

char note;
int time;
char win;
int state;
volatile int timer_cnt = 0;
int leap_cnt = 0;
char currKey = 0;
int sec_count = 0;


void swDelay (char numLoops);
void runTimerA2(void);
void stoptimerA2(int reset);

//create structs to hold notes and time
struct song {
    char note;
    int time;
} s1, s2, s3, s4;



/*
struct note2{
    note = B;
    time = 500;
}*/

//create more notes similarly



//create array to hold all structs
struct song songArr[NUM_NOTES];


int main(void){

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    _BIS_SR(GIE);


    s1.note = 'A';
    s1.time = 2000;
    s2.note = 'Z';
    s2.time = 3000;
    s3.note = 'B';
    s3.time = 4000;
    s4.note = 'C';
    s4.time = 5000;

    songArr[0] = s1;
    songArr[1] = s2;
    songArr[2] = s3;
    songArr[3] = s4;

    initLeds();
    configDisplay();
    configKeypad();
    Graphics_clearDisplay(&g_sContext);


   int i = 0;
   state = 1;
   win = 0;
   //create state machine around this while loop
   //state machine will first display welcome message
   //display LED
   //accept user input and compare it to the correct note. Play buzzer for the user pressed note
   //

   int currentScore;
   int maxMiss = 10;
   int missStreak = 0;
   int totalPoints = 0;

   Graphics_clearDisplay(&g_sContext);
   while (1){
       currKey = getKey();
       switch (state){

           case 1:



               //display welcome message and ask to press '*' to start
               Graphics_drawStringCentered(&g_sContext, "MSP430 Hero", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
               Graphics_drawStringCentered(&g_sContext, "Press '*'!", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
               Graphics_flushBuffer(&g_sContext);
               if (currKey == '*'){

                   runTimerA2();
                   Graphics_clearDisplay(&g_sContext); // Clear the display
                   Graphics_drawStringCentered(&g_sContext, "3", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
                   Graphics_flushBuffer(&g_sContext);

                   while (timer_cnt < 1000){
                       setLeds (0x2);
                   }
                   setLeds(0);
                   timer_cnt = 0;
                   stoptimerA2(1);

                   runTimerA2();
                   Graphics_clearDisplay(&g_sContext); // Clear the display
                   Graphics_drawStringCentered(&g_sContext, "2", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
                   Graphics_flushBuffer(&g_sContext);
                   while (timer_cnt < 1000){
                       setLeds (0x4);
                   }
                   setLeds(0);
                   timer_cnt = 0;
                   stoptimerA2(1);

                   runTimerA2();
                   Graphics_clearDisplay(&g_sContext); // Clear the display
                   Graphics_drawStringCentered(&g_sContext, "1", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
                   Graphics_flushBuffer(&g_sContext);
                   while (timer_cnt < 1000){
                       setLeds (0x8);
                   }
                   setLeds(0);
                   timer_cnt = 0;
                   stoptimerA2(1);

                   runTimerA2();
                   Graphics_clearDisplay(&g_sContext); // Clear the display
                   Graphics_drawStringCentered(&g_sContext, "GO!", AUTO_STRING_LENGTH, 48, 55, TRANSPARENT_TEXT);
                   Graphics_flushBuffer(&g_sContext);
                   while (timer_cnt < 1000){
                       setLeds (0xF);
                   }
                   setLeds(0);
                   timer_cnt = 0;
                   stoptimerA2(1);


                   state = 2;
               }
               break;

           case 2:
               //play buzzer and lights

               Graphics_clearDisplay(&g_sContext); // Clear the display
               runTimerA2();

               for (i = 0; i<NUM_NOTES; i++){
                   while (timer_cnt < songArr[i].time){
                       BuzzerOn(songArr[i].note);
//                       if (songArr[i].note = 'A'|'Y'|'F'|'P'){
//                           setLeds(0x1);
//                       }
//                       else if (songArr[i].note = 'Z'|'D'|'K'){
//                           setLeds(0x2);
//                       }
//                       else if (songArr[i].note = 'B'|'X'|'G'){
//                           setLeds(0x4);
//                       }
//                       else {
//                           setLeds(0x8);
//                       }

                   }
//                 setLeds(0);

               }

               stoptimerA2(1);
               break;

           case 3:
               //This case compares the user input to correct note to be pressed
               //Keeps track of score
               //Score has 2 parts: Max miss: The number of notes you can miss until that given point in time
               //And total score = Max points- miss

               while (missStreak < maxMiss)
               {
                      currKey = getKey;

                      playLED(songArr[i].note);
                      checkInput(songArr[i].note);
                      if (currKey!=songArr[i].note){
                          missStreak++;

                      }
                      //this function "outputs" the frequency to the buzzer
                      //but to actually play it you need a delay function from timer A
                     //call the timerA delay after you call bozzerOn();
                          BuzzerOn(songArr[i].note);
                          duration(songArr[i].time);

                     i++;


               }


           case 4:
               if (win == 1){

                   Graphics_clearDisplay(&g_sContext); //Makes it blink
                   Graphics_drawStringCentered(&g_sContext, "You WIN!", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
                   Graphics_drawStringCentered(&g_sContext, "Press '#'", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
                   Graphics_drawStringCentered(&g_sContext, "To Play Game", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);
                   Graphics_flushBuffer(&g_sContext);
                  //runtimer
                   if (currKey == '#'){ //Replay game

                       //runtimer
                       state = 1;
                       Graphics_clearDisplay(&g_sContext);
                  }
               }

               if (win == 0){

                   Graphics_clearDisplay(&g_sContext); //Makes it blink
                   Graphics_drawStringCentered(&g_sContext, "You LOSE!", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
                   Graphics_drawStringCentered(&g_sContext, "Press '#'", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
                   Graphics_drawStringCentered(&g_sContext, "To Play Game", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);
                   Graphics_flushBuffer(&g_sContext);
                   //runtimer
                   if (currKey == '#'){ //Replay game

                       //runtimer
                       state = 1;
                       Graphics_clearDisplay(&g_sContext);
                  }
               }

                break;
       }//switch
   }//while
}//main

int duration (int time){
    //Use TimerA to create a delay for notes to be played
}

void playLED(char note){
    //Use setLed() function
    //**Assign LEDs to notes in a one->many configuration
    //basically make 1 LED flash for 2 notes

}

void checkInput(char note){
    //use getKey() function to compare user input to existing song
}

void runTimerA2(void){
    TA2CTL = TASSEL_2 | MC_1 | ID_0;
    TA2CCR0 = 1048;
    TA2CCTL0 = CCIE;
}

void stoptimerA2(int reset){
    TA2CTL = MC_0; //stop timer
    TA2CCTL0 &= ~CCIE;
    if (reset)
        timer_cnt = 0;
}

#pragma vector=TIMER2_A0_VECTOR
__interrupt void TimerA2_ISR (void){
    if (leap_cnt < 2473056){
        timer_cnt++;
        leap_cnt++;
    }
    else{ //skip a count
        leap_cnt = 0;
        timer_cnt+= 2;
    }
}

void swDelay(char numLoops)
{
    // This function is a software delay. It performs
    // useless loops to waste a bit of time
    //
    // Input: numLoops = number of delay loops to execute
    // Output: none
    //
    // smj, ECE2049, 25 Aug 2013

    volatile unsigned int i,j;  // volatile to prevent removal in optimization
                                // by compiler. Functionally this is useless code

    for (j=0; j<numLoops; j++)
    {
        i = 50000 ;                 // SW Delay
        while (i > 0)               // could also have used while (i)
           i--;
    }
}
