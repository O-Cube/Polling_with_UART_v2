//===========================================================
// Example Program for the TIVA TM4C1294XL Evaluation Board
// With this file you can print text to the console of the
// Code Composer Studio.
//==========================================================
//                     ! IMPORTANT !
// This program runs endless. Stop with the "Red Square Button"
// in Debug Mode (Terminate = CTRL + F2)
//==========================================================
// Include the Header File for controller tm4c1294ncpdt
#include "inc/tm4c1294ncpdt.h"
#include <stdint.h>
#include <stdio.h>
void main(void){
    int i,cnt;  // variable definition
    cnt = 0;    // initialize variable

    while(1)
    {
        printf("%05d\n",cnt);
        // Edit your name and your student number
        printf("Name    : XXXXX XXXXX \n");
        printf("Matr.No.: XXXXXXXXXX \n");
        cnt++;
        for(i=0;i<500000;i++);      // wait loop
    }
}
