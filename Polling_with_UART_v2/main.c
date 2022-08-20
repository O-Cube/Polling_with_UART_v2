   //======================================================================================================
   // Author  : Obed Oyandut
   // Date    : 04.08.2022
   // Version : v2
   //======================================================================================================
   // This program uses TIVA TM4C1294XL Evaluation Board
   //======================================================================================================
   // This is an enhancement of version 1. Here the literals are replaced with constants. This makes the
   // code more comprehensible.
   // The testing of this code is done with Realterm. The speed of the communication line is 115200MHz
   // and a 8N1 format. The debugging is done in code composer studio.
   //========================================================================================================
   //                     ! IMPORTANT !
   // This program runs endless. Stop with the "Red Square Button"
   // in Debug Mode (Terminate = CTRL + F2)
   //========================================================================================================

#include "inc/tm4c1294ncpdt.h"
#include <stdint.h>
#include <stdio.h>

    // Size of receive buffer

#define LEN 32

    // Buffer declarations

unsigned char rxBuffer[LEN];
unsigned char txBuffer[] = "Payload is received...send some more...";

    //=========================================================================================
    // Keeps count of the number of ASCII character received/sent
    //=========================================================================================

unsigned char count = 0;
unsigned char i;

    //=========================================================================================
    // Configure UART2. First assign a clock to UART2. Wait for the clock
    // to be assigned using a blocking while loop. Disable UART2 to perform configurations.
    // Set the bit rate to 115200bps. Remember clock speed is 16MHz. Receiver speed is 16 times
    // faster than transmit speed. Set the frame format to 8N1. Turn on all 16 FIFOs for receive
    // transmit. Re enable UART2 after configuration.
    //==========================================================================================

void configUart2(void){
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R2;
    while((SYSCTL_PRUART_R & SYSCTL_RCGCUART_R2)==0);
    UART2_CTL_R &= ~(UART_CTL_UARTEN);
    UART2_IBRD_R = 8;
    UART2_FBRD_R = 44;
    UART2_LCRH_R = UART_LCRH_WLEN_8 | UART_LCRH_FEN;
    UART2_CTL_R |= UART_CTL_UARTEN | UART_CTL_RXE | UART_CTL_TXE;


}

    //==========================================================================================
    // Configure port D for UART. Assign clock to PD. Wait for clock to become stable using
    //  a blocking while loop. Set alternate select of P(4) and PD(5). Set alternate function
    // of PD(4) and PD(5)to UART.
    //==========================================================================================

void configPortD(void){
    SYSCTL_RCGCGPIO_R |= (1<<3);
    while((SYSCTL_PRGPIO_R & (1<<3)) == 0);
    GPIO_PORTD_AHB_DEN_R |= 0x030;
    GPIO_PORTD_AHB_AFSEL_R |= 0x030;
    GPIO_PORTD_AHB_PCTL_R |= 0x110000;
}

void main(void){

    //==========================================================================================
    // Initialization UART2 and PD
    //==========================================================================================

    configUart2();
    configPortD();

    //======================================================================================================
    // Polling is used in this case to check if the Rx FIFO has some content in it. If there is something,
    // the processor copies the content into the receive up to 31 ASCII characters. Appends a string
    // delimiter. Prints all the content of the rxBuffer as a string. If the content of the rxBuffer is
    // not full, the the  processor continues to poll the Rx FIFOs. All 16 Rx FIFOs are used simultaneously.
    // After printing the content of the rxBuffer, the processor sends a message to the peripheral "Payload
    // is received...send some more...".
    //=======================================================================================================

    while(1)
    {
        while(UART2_FR_R & 0x10);
        rxBuffer[count] = (char)(UART2_DR_R);
        if (count == 31) {
            rxBuffer[31] = '\0';
            printf("Payload: %s\n",rxBuffer);
            for (i=0;i<sizeof(txBuffer);i++) {
                while(!(UART2_FR_R & 0x80));
                UART2_DR_R = txBuffer[i];
            }
            count = 0;
        }
        count++;
    }
}
