/* 
 * File:   only_on_off.c
 * Author: Varun sahni
 *client: test
 * module: this is test code for New hardware with 8switches and one dimmer and one RGB
 * Note: for switching the led 
 * ON>>give low
 * OFF>>give 
 * Tip: use function as much possible for memory consumption(when i was using the R101 with normal TX1REG it use 39% of memory while using function it use 31% of memory(8% of memory relesed))
 * Note: LEVEL Shifter is used to supply 3.3 voltage to xbee.
 * #RA3 pin is used , keep RA3 pin to operate in 5 voltade mode, keep RA3 LOW to operate in 3.3 voltage
 * #In this hardware, input switch works diffrently then our original---->>input switch work as the keypad...all input switch initlize as high and when we press it it will give low output
 * so work opposite to the original one but output is same original -->>load is operating with transistor
 * # In this hardware we also debug the transmitting and received frame--->>>after receiveing the frame glow Blue Led
 *                                                                     ---->>>after transmitting the frame glow Green Led
 * 
 *  
 */

#include <stdio.h>
#include <stdlib.h>
// 'C' source line config statements
// CONFIG1
#pragma config FOSC = HS        // Oscillator Selection (HS Oscillator, High-speed crystal/resonator connected between OSC1 and OSC2 pins)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config VCAPEN = OFF     // Voltage Regulator Capacitor Enable bit (VCAP pin function disabled)
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will not cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
// Since we have used 16 MHz crystal
#define _XTAL_FREQ 16000000  

// Pin MACROS
#define OUTPUT_RELAY1 PORTEbits.RE0
#define OUTPUT_RELAY2 PORTEbits.RE1
#define OUTPUT_RELAY3 PORTGbits.RG0
#define OUTPUT_RELAY4 PORTGbits.RG3
#define OUTPUT_RELAY5 PORTGbits.RG4
#define OUTPUT_RELAY6 PORTFbits.RF6
#define OUTPUT_RELAY7 PORTFbits.RF5
#define OUTPUT_RELAY8 PORTFbits.RF4
#define OUTPUT_DIMMER PORTFbits.RF3 //MOC


#define OUTPUT_RELAY_DIR_1 TRISEbits.TRISE0
#define OUTPUT_RELAY_DIR_2 TRISEbits.TRISE1
#define OUTPUT_RELAY_DIR_3 TRISGbits.TRISG0
#define OUTPUT_RELAY_DIR_4 TRISGbits.TRISG3
#define OUTPUT_RELAY_DIR_5 TRISGbits.TRISG4
#define OUTPUT_RELAY_DIR_6 TRISFbits.TRISF6
#define OUTPUT_RELAY_DIR_7 TRISFbits.TRISF5
#define OUTPUT_RELAY_DIR_8 TRISFbits.TRISF4
#define OUTPUT_DIMMER_DIR TRISFbits.TRISF3

//RGB MACORS
#define OUTPUT_FOR_RED_LED PORTDbits.RD4 ///pwm for red
#define OUTPUT_FOR_GREEN_LED PORTDbits.RD3 //pwm for green
#define OUTPUT_FOR_BLUE_LED PORTDbits.RD2   // PWM for blue

#define OUTPUT_RELAY_RED_LED_DIR TRISDbits.TRISD4
#define OUTPUT_RELAY_GREEN_LED_DIR TRISDbits.TRISD3
#define OUTPUT_RELAY_BLUE_LED_DIR TRISDbits.TRISD2        // direction of PWM OUTPUT to MOC3021

#define ZCD_CCP9_DIR TRISEbits.TRISE3//for rgb
#define ZCD_CCP7_DIR TRISEbits.TRISE5
#define ZCD_CCP8_DIR TRISEbits.TRISE4

#define ZCD_CCP10_DIR TRISEbits.TRISE3//for dimmer


#define INPUTSWITCH1 PORTDbits.RD6
#define INPUTSWITCH2 PORTDbits.RD7
#define INPUTSWITCH3 PORTBbits.RB0
#define INPUTSWITCH4 PORTBbits.RB1
#define INPUTSWITCH5 PORTBbits.RB2
#define INPUTSWITCH6 PORTBbits.RB3
#define INPUTSWITCH7 PORTBbits.RB4
#define INPUTSWITCH8 PORTBbits.RB5
#define INPUTSWITCH9 PORTDbits.RD5

#define INPUT_SWITCH_DIR_1 TRISDbits.TRISD6
#define INPUT_SWITCH_DIR_2 TRISDbits.TRISD7
#define INPUT_SWITCH_DIR_3 TRISBbits.TRISB0
#define INPUT_SWITCH_DIR_4 TRISBbits.TRISB1
#define INPUT_SWITCH_DIR_5 TRISBbits.TRISB2
#define INPUT_SWITCH_DIR_6 TRISBbits.TRISB3
#define INPUT_SWITCH_DIR_7 TRISBbits.TRISB4
#define INPUT_SWITCH_DIR_8 TRISBbits.TRISB5
#define INPUT_SWITCH_DIR_9 TRISDbits.TRISD5



//macros for leds

#define OUTPUT_TRANMIT_INDICATION_LED PORTFbits.RF0 //green
#define OUTPUT_RECEIVE_INDICATION_LED PORTFbits.RF1 //blue led
#define USER_RECEIVE_INDICATION_LED PORTFbits.RF2 //blue led

#define OUTPUT_TRANMIT_INDICATION_LED_DIR TRISFbits.TRISF0
#define OUTPUT_RECEIVE_INDICATION_LED_DIR TRISFbits.TRISF1
#define USER_RECEIVE_INDICATION_LED_DIR TRISFbits.TRISF2

#define LEVEL_SHIFTER_OUTPUT_PIN PORTAbits.RA3
#define LEVEL_SHIFTER_OUTPUT_PIN_DIR TRISAbits.TRISA3
// direction of PWM OUTPUT to MOC3021

/*
 * Extra Periferals Direction and PORT
 */
//#define ZCD_CCP9_DIR TRISEbits.TRISE3
// USART Directions
#define USART_1_TRANSMIT_OUTPUT_DIR TRISCbits.TRISC6
#define USART_1_RECIEVE_INPUT_DIR TRISCbits.TRISC7

#define RECIEVED_DATA_LENGTH (16+2)
#define TOTAL_NUMBER_OF_SWITCH (12+2)

#define TRUE 1
#define FALSE 0



// Conditional compilation
//#define DEBUG
//#define RELEASE


// ALL error Definitions
/* 
 * #define WRONG_DATA_RECIEVED_ERROR_CODE ERRX
 * #define RECIVING_OVERRUN_ERROR EROV
 * #define RECEIVING_DATA_LOST_IN_MAIN ERLS
 */
/* DATA USED IN MANUAL  STARTS HERE*/



#define ON 1
#define OFF 0
#define CHAR_OFF '0'
#define CHAR_ON '1'
#define SWITCH_PRESSED 0
#define SWITCH_RELEASED 1
        
/* DATA USED IN MANUAL END HERE*/


int mainReceivedDataPosition=0, mainDataReceived=FALSE;
unsigned char mainReceivedDataBuffer[RECIEVED_DATA_LENGTH]="#"; 
unsigned char tempReceivedDataBuffer[RECIEVED_DATA_LENGTH-8]="#";
unsigned char parentalLockBuffer[TOTAL_NUMBER_OF_SWITCH]="000000000000";
unsigned char copy_parentalLockBuffer[TOTAL_NUMBER_OF_SWITCH]="000000000000";
unsigned char currentStateBuffer[(TOTAL_NUMBER_OF_SWITCH*4)+2]="#";

unsigned int M1;unsigned int M2;unsigned int M3;unsigned int M4;
unsigned int M5;unsigned int M6;unsigned int M7;unsigned int M8;
unsigned int M9;
int start_PWM_Generation_in_ISR_FLAG=FALSE;
char levelofDimmer_MSB='0',levelofDimmer_LSB='0';

void errorsISR(const char* errNum);
void errorsMain(const char* errNum);
void sendAcknowledgment(char* currentStateBuffer);
void SwitchOffStatustToGatway(char SwitchOffNumber);
void SwitchOnStatustToGatway(char SwitchOnNumber);
void TransmissionIndicationLedBlinking();
void ReceivingIndicationLedBlinking();
void clearAllPorts();
void pinINIT_extra();
void GPIO_pin_Initialize();
void EUSART_Initialize();

void TMR3_Initialize();
void TMR1_Initialize();
void TMR2_Initialize();
void TMR4_Initialize();
void TMR5_Initialize();
void TMR6_Initialize();
void TMR8_Initialize();
void TMR10_Initialize();
void CCP9_Initialize();
void CCP10_Initialize();
void CCP7_Initialize();
void CCP8_Initialize();
void allPeripheralInit();

void copyReceivedDataBuffer();

void applianceControl(char switchMSB, char switchLSB, char switchSTATE, char dimmerSpeedMSB, char dimmerSpeedLSB, char parentalControl, char finalFrameState);

interrupt void isr(){
 
    // ************************************* UART INTERRUPT *********************************************** //
    if(RC1IF){        
        if(RC1STAbits.OERR){    // If over run error, then reset the receiver
            RC1STAbits.CREN = 0; // countinuous Recieve Disable
            RC1STAbits.CREN = 1; // countinuous Recieve Enable
            errorsISR("EROV");
        } 
        
        mainReceivedDataBuffer[mainReceivedDataPosition]=RC1REG;
        #ifdef DEBUG
        TX1REG=mainReceivedDataBuffer[mainReceivedDataPosition];
        #endif
        if(mainReceivedDataBuffer[0]=='%'){
            mainReceivedDataPosition++;
            if(mainReceivedDataPosition>15){
                mainDataReceived=TRUE;
                mainReceivedDataPosition=0;                
                RC1IF=0;                
            }
        }
        else{
            RC1STAbits.CREN = 0; // countinuous Recieve Disable
            RC1STAbits.CREN = 1; // countinuous Recieve Enable
            mainReceivedDataPosition=0; // Reinitiate buffer counter
            
 
            errorsISR("ERRX");
        }
    }// End of RC1IF 
    
    
    //*******************LIGHT DIMMER*************************///
      //*******************TIMER2 INTERRUPT**************************//
     if(PIE1bits.TMR2IE==1 && PIR1bits.TMR2IF==1)
    {           
        PIR1bits.TMR2IF=0;
        OUTPUT_DIMMER = FALSE;//for rgb
        T2CONbits.TMR2ON=0;
    }   
   
     
    //*********************TIMER1 INTERRUPT**************************//
     if(PIE1bits.TMR1IE == 1 && PIR1bits.TMR1IF==1)
    {
        PIR1bits.TMR1IF=0;        
        OUTPUT_DIMMER = TRUE;            
        PR2=0x9F;
         T2CONbits.TMR2ON = 1;
        T1CONbits.TMR1ON = 0;        
    }
    
    
    
    //*************************ZCD INTERRRUPT****************************//
    if(PIR4bits.CCP8IF==1 || PIR4bits.CCP7IF == 1 || PIR4bits.CCP9IF==1 || PIR4bits.CCP10IF==1){
    if(CCP8IF){
        if(CCP8IF == 1){
             CCP8IF=0;
         if(start_PWM_Generation_in_ISR_FLAG == 1){
          switch(levelofDimmer_MSB)
                {
                case '0':           // 8.5
                    /**/
                        switch(levelofDimmer_LSB)
                             {
                             case '0':           // 8.5
                                     TMR1H=0x7B;
                                     TMR1L=0x30;
                                    T1CONbits.TMR1ON = 1;
                                  //   OUTPUT_DIMMER=1;
                                     break;
                             case '1':           // 8.4
                                     TMR1H=0x7C;
                                     TMR1L=0xC0;
                                     T1CONbits.TMR1ON = 1;
                                    // OUTPUT_DIMMER=1;
                                     break;
                             case '2':           // 8.35
                                     TMR1H=0x7D;
                                     TMR1L=0x88;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '3':           // 8.25
                                     TMR1H=0x7F;
                                     TMR1L=0x18;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '4':          // 8.15
                                     TMR1H=0x80;
                                     TMR1L=0xA8;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '5':               // 8.1
                                     TMR1H=0x81;
                                     TMR1L=0x70;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '6':               // 8.0    
                                     TMR1H=0x83;
                                     TMR1L=0x00;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '7':            //7.95
                                     TMR1H=0x83;
                                     TMR1L=0xC8;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '8':           //7.9
                                     TMR1H=0x87;
                                     TMR1L=0xB0;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '9':           // 7.85
                                     TMR1H=0x85;
                                     TMR1L=0x58;
                                     T1CONbits.TMR1ON = 1;
                                     break;

                             default:
                                 break;
                         }                    
                        break;
                case '1':           // 7.8-7.3

                            switch(levelofDimmer_LSB)
                                 {
                                 case '0':           // 7.8
                                         TMR1H=0x86;
                                         TMR1L=0x20;
                                         T1CONbits.TMR1ON = 1;
                                         break;
                                 case '1':           // 7.75
                                         TMR1H=0x86;
                                         TMR1L=0xE8;
                                         T1CONbits.TMR1ON = 1;
                                         break;
                                 case '2':           // 7.7
                                         TMR1H=0x87;
                                         TMR1L=0xB0;
                                         T1CONbits.TMR1ON = 1;
                                         break;
                                 case '3':           // 7.65
                                         TMR1H=0x88;
                                         TMR1L=0x78;
                                         T1CONbits.TMR1ON = 1;
                                         break;
                                 case '4':            // 7.6
                                         TMR1H=0x89;
                                         TMR1L=0x40;
                                         T1CONbits.TMR1ON = 1;
                                         break;
                                 case '5':               // 7.55
                                         TMR1H=0x8A;
                                         TMR1L=0x08;
                                         T1CONbits.TMR1ON = 1;
                                         break;
                                 case '6':               // 7.5    
                                         TMR1H=0x8A;
                                         TMR1L=0xD0;
                                         T1CONbits.TMR1ON = 1;
                                         break;
                                 case '7':            //7.45
                                         TMR1H=0x8B;
                                         TMR1L=0x98;
                                         T1CONbits.TMR1ON = 1;
                                         break;
                                 case '8':           //7.4
                                         TMR1H=0x8C;
                                         TMR1L=0x60;
                                         T1CONbits.TMR1ON = 1;
                                         break;
                                 case '9':           // 7.35
                                         TMR1H=0x8D;
                                         TMR1L=0x28;
                                         T1CONbits.TMR1ON = 1;
                                         break;

                                 default:
                                     break;
                                }
                        break;
                case '2':           // 7.3-
/**/
                        switch(levelofDimmer_LSB)
                             {
                             case '0':           // 7.3-6.85
                                     TMR1H=0x8D;
                                     TMR1L=0xF0;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '1':           // 7.25
                                     TMR1H=0x8E;
                                     TMR1L=0xB8;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '2':           // 7.20
                                     TMR1H=0x8F;
                                     TMR1L=0x80;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '3':           // 7.15
                                     TMR1H=0x90;
                                     TMR1L=0x48;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '4'://TX1REG='n';      // 7.1
                                     TMR1H=0x91;
                                     TMR1L=0x10;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '5':               // 7.05
                                     TMR1H=0x91;
                                     TMR1L=0xD8;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '6':               // 7.0    
                                     TMR1H=0x92;
                                     TMR1L=0xA0;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '7':            //6.95
                                     TMR1H=0x93;
                                     TMR1L=0x68;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '8':           //6.9
                                     TMR1H=0x94;
                                     TMR1L=0x30;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '9':           // 6.85
                                     TMR1H=0x94;
                                     TMR1L=0xF8;
                                     T1CONbits.TMR1ON = 1;
                                     break;

                             default:
                                 break;
                         }                    
                        break;
                case '3':           // 6.8-5.9                
/**/
                        switch(levelofDimmer_LSB)
                             {
                             case '0':           // 6.8
                                     TMR1H=0x95;
                                     TMR1L=0xC0;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '1':           // 6.7
                                     TMR1H=0x97;
                                     TMR1L=0x50;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '2':           // 6.6
                                     TMR1H=0x98;
                                     TMR1L=0xE0;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '3':           // 6.5
                                     TMR1H=0x9A;
                                     TMR1L=0x70;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '4'://TX1REG='n';      // 6.4
                                     TMR1H=0x9C;
                                     TMR1L=0x00;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '5':               // 6.3
                                     TMR1H=0x9D;
                                     TMR1L=0x90;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '6':               // 6.2   
                                     TMR1H=0x9F;
                                     TMR1L=0x20;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '7':            //6.1
                                     TMR1H=0xA0;
                                     TMR1L=0xB0;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '8':           //6.0
                                     TMR1H=0xA2;
                                     TMR1L=0x40;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '9':           // 5.9
                                     TMR1H=0xA3;
                                     TMR1L=0xD0;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             default:
                                     break;
                            }
                        break;
                case '4'://TX1REG='n';      // 5.8-4.9                    
/**/
                        switch(levelofDimmer_LSB)
                             {
                             case '0':           // 5.8
                                     TMR1H=0xA5;
                                     TMR1L=0x60;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '1':           // 5.7
                                     TMR1H=0xA6;
                                     TMR1L=0xF0;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '2':           // 5.6
                                     TMR1H=0xA8;
                                     TMR1L=0x80;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '3':           // 5.5
                                     TMR1H=0xAA;
                                     TMR1L=0x10;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '4':           // 5.4
                                     TMR1H=0xAB;
                                     TMR1L=0xA0;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '5':               // 5.3
                                     TMR1H=0xAD;
                                     TMR1L=0x30;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '6':               // 5.2    
                                     TMR1H=0xAE;
                                     TMR1L=0xC0;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '7':              // 5.1
                                     TMR1H=0xB0;
                                     TMR1L=0x50;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '8':              // 5.0
                                     TMR1H=0xB1;
                                     TMR1L=0xE0;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '9':              // 4.9
                                     TMR1H=0xB3;
                                     TMR1L=0x70;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             default:
                                     break;
                         }
                        break;
                case '5':               // 4.8-3.9
/**/
                        switch(levelofDimmer_LSB)
                             {
                             case '0':           // 4.8
                                     TMR1H=0xB5;
                                     TMR1L=0x00;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '1':           // 4.7
                                     TMR1H=0xB6;
                                     TMR1L=0x90;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '2':           // 4.6
                                     TMR1H=0xB8;
                                     TMR1L=0x20;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '3':           // 4.5
                                     TMR1H=0xB9;
                                     TMR1L=0xB0;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '4'://TX1REG='n';      // 4.4
                                     TMR1H=0xBB;
                                     TMR1L=0x40;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '5':               // 4.3
                                     TMR1H=0xBC;
                                     TMR1L=0xD0;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '6':               // 4.2   
                                     TMR1H=0xBE;
                                     TMR1L=0x60;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '7':            //4.1
                                     TMR1H=0xBF;
                                     TMR1L=0xF0;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '8':           //4.0
                                     TMR1H=0xC1;
                                     TMR1L=0x80;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '9':           // 3.9
                                     TMR1H=0xC3;
                                     TMR1L=0x10;
                                     T1CONbits.TMR1ON = 1;
                                     break;

                             default:
                                 break;
                            }                    
                        break;
                case '6':               // 3.8-2.9 
/**/
                        switch(levelofDimmer_LSB)
                             {
                             case '0':           // 3.8
                                     TMR1H=0xC4;
                                     TMR1L=0xA0;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '1':           // 3.7
                                     TMR1H=0xC6;
                                     TMR1L=0x30;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '2':           // 3.6
                                     TMR1H=0xC7;
                                     TMR1L=0xC0;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '3':           // 3.5
                                     TMR1H=0xC9;
                                     TMR1L=0x50;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '4'://TX1REG='n';      // 3.4
                                     TMR1H=0xCA;
                                     TMR1L=0xE0;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '5':               // 3.3
                                     TMR1H=0xCC;
                                     TMR1L=0x70;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '6':               // 3.2   
                                     TMR1H=0xCE;
                                     TMR1L=0x00;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '7':            //3.1
                                     TMR1H=0xCF;
                                     TMR1L=0x90;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '8':           // 3.0
                                     TMR1H=0xD1;
                                     TMR1L=0x20;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '9':           // 2.9
                                     TMR1H=0xD2;
                                     TMR1L=0xB0;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             default:
                                     break;
                            }                    
                        break;
                case '7':            //2.8-1.9
/**/
                        switch(levelofDimmer_LSB)
                             {
                             case '0':           // 2.8
                                     TMR1H=0xD4;
                                     TMR1L=0x40;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '1':           // 2.7
                                     TMR1H=0xD5;
                                     TMR1L=0xD0;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '2':           // 2.6
                                     TMR1H=0xD7;
                                     TMR1L=0x60;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '3':           // 2.5
                                     TMR1H=0xD8;
                                     TMR1L=0xF0;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '4'://TX1REG='n';      // 2.4
                                     TMR1H=0xDA;
                                     TMR1L=0x80;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '5':               // 2.3
                                     TMR1H=0xDC;
                                     TMR1L=0x10;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '6':               // 2.2  
                                     TMR1H=0xDD;
                                     TMR1L=0xA0;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '7':            // 2.1
                                     TMR1H=0xDF;
                                     TMR1L=0x30;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '8':           // 2.0
                                     TMR1H=0xE0;
                                     TMR1L=0xC0;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '9':           // 1.9
                                     TMR1H=0xE2;
                                     TMR1L=0x50;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             default:
                                     break;
                            }
                        break;
                case '8':           //1.8-1.2
/**/
                        switch(levelofDimmer_LSB)
                             {
                             case '0':           // 1.8
                                     TMR1H=0xE3;
                                     TMR1L=0xE0;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '1':           // 1.75
                                     TMR1H=0xE4;
                                     TMR1L=0xA8;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '2':           // 1.7
                                     TMR1H=0xE5;
                                     TMR1L=0x70;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '3':           // 1.65
                                     TMR1H=0xE6;
                                     TMR1L=0x38;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '4'://TX1REG='n';      // 1.6
                                     TMR1H=0xE7;
                                     TMR1L=0x00;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '5':               // 1.5
                                     TMR1H=0xE8;
                                     TMR1L=0x90;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '6':               // 1.4   
                                     TMR1H=0xEA;
                                     TMR1L=0x20;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '7':            //1.3
                                     TMR1H=0xEB;
                                     TMR1L=0xB0;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '8':           //1.25
                                     TMR1H=0xEC;
                                     TMR1L=0x78;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '9':           // 1.2
                                     TMR1H=0xED;
                                     TMR1L=0x40;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             default:
                                     break;
                            }
                        break;
                case '9':           // 1.1-0.2
/**/
                        switch(levelofDimmer_LSB)
                             {
                             case '0':           // 1.1
                                     TMR1H=0xEE;
                                     TMR1L=0xD0;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '1':           // 1.0
                                     TMR1H=0xF0;
                                     TMR1L=0x60;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '2':           // 0.9
                                     TMR1H=0xF1;
                                     TMR1L=0xF0;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '3':           // 0.8
                                     TMR1H=0xF3;
                                     TMR1L=0x80;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '4'://TX1REG='n';      // 0.7
                                     TMR1H=0xF5;
                                     TMR1L=0x10;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '5':               // 0.6
                                     TMR1H=0xF6;
                                     TMR1L=0xA0;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '6':               // 0.5    
                                     TMR1H=0xF8;
                                     TMR1L=0x30;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '7':            //0.4
                                     TMR1H=0xF9;
                                     TMR1L=0xC0;
                                     T1CONbits.TMR1ON = 1;
                                     break;
                             case '8':           //0.3
                                     TMR1H=0xFB;
                                     TMR1L=0x50;
                                    T1CONbits.TMR1ON = 1;
                                    //   OUTPUT_DIMMER=0;
                                     break;
                             case '9':           // 0.2
                                     TMR1H=0xFC;
                                    TMR1L=0xE0;
                                    T1CONbits.TMR1ON = 1;
                                    //   OUTPUT_DIMMER=0;
                                     break;
                             default:
                                     break;
                            }
                        break;
                default:
                        break;
            } 
         }
        }
       
    }//end of ccp10
    }//end of CCP
    
}




/*
 * Alfaone Main code starts here
 * For 4 switches 1 Dimmer
 */
int main() {
    __delay_ms(2000);
        M1=ON;    M2=ON;    M3=ON;    M4=ON;    M5=ON; M6=ON; M7=ON;M8=ON;
        M9=ON; 
            GPIO_pin_Initialize();
            allPeripheralInit();
    
    while(1){       
        if(mainDataReceived==TRUE){
            mainDataReceived=FALSE;
            if(mainReceivedDataBuffer[0]=='%' && mainReceivedDataBuffer[1]=='%' && mainReceivedDataBuffer[14]=='@' && mainReceivedDataBuffer[15]=='@'){
            ReceivingIndicationLedBlinking();
                copyReceivedDataBuffer();
                
                applianceControl(tempReceivedDataBuffer[0],
                        tempReceivedDataBuffer[1],
                        tempReceivedDataBuffer[2],
                        tempReceivedDataBuffer[3],
                        tempReceivedDataBuffer[4],
                        tempReceivedDataBuffer[5],
                        tempReceivedDataBuffer[6]);
                                
            }   // End of all buffer data check
            else
            {

                errorsMain("ERLS");
                RC1STAbits.SPEN=0;  // Serial port disabled 
                RC1STAbits.CREN = 0; // countinuous Recieve Disable                
                for(int dataBufferCounter = 0; dataBufferCounter< 15; dataBufferCounter++)
                {
                    mainReceivedDataBuffer[dataBufferCounter] = '#'; // clean received data buffer
                }
                RC1STAbits.CREN = 1; // countinuous Recieve Enable
                RC1STAbits.SPEN=1;  // Serial port enabled (configures RXx/DTx and TXx/CKx pins as serial port pins)
            }
        } // End of mainDataReceived condition

        /******************** MANUAL RESPONE STARTS HERE************ */
        
        //check switch one status
        //off condition
//        OUTPUT_RELAY1=OFF;__delay_ms(100);OUTPUT_RELAY1=ON;
//        OUTPUT_RELAY2=OFF;__delay_ms(100);OUTPUT_RELAY2=ON;
//        OUTPUT_RELAY3=OFF;__delay_ms(100);OUTPUT_RELAY3=ON;
//        OUTPUT_RELAY4=OFF;__delay_ms(100);OUTPUT_RELAY4=ON;
//        OUTPUT_RELAY5=OFF;__delay_ms(100);OUTPUT_RELAY5=ON;
//        OUTPUT_RELAY6=OFF;__delay_ms(100);OUTPUT_RELAY6=ON;
//        OUTPUT_RELAY7=OFF;__delay_ms(100);OUTPUT_RELAY7=ON;
//        OUTPUT_RELAY8=OFF;__delay_ms(100);OUTPUT_RELAY8=ON;
       int man = 1;
        if(copy_parentalLockBuffer[1] == CHAR_OFF  && INPUTSWITCH1 == SWITCH_PRESSED && M1 == OFF)//switch is pressed
        {
            if(man == 1)
            {
                SwitchOnStatustToGatway('1');
            
            TransmissionIndicationLedBlinking();
            OUTPUT_RELAY1=ON;
            
            }
            man=0;
            M1=1;
            
        }
        //on condition
        if(copy_parentalLockBuffer[1] == CHAR_OFF && INPUTSWITCH1 == SWITCH_RELEASED && M1 == ON)////switch is released
        {
            //TX1REG='C';
            if(man==1)
            {
            SwitchOffStatustToGatway('1');
            TransmissionIndicationLedBlinking();
            OUTPUT_RELAY1=OFF;
            }
            man=0;
            M1=0;
        }
        
       // //check switch second status 
        //off condition
        if(copy_parentalLockBuffer[2] == CHAR_OFF && INPUTSWITCH2 == SWITCH_PRESSED && M2 == OFF)
        {
            if(man==1)
            {
            SwitchOnStatustToGatway('2');           
            TransmissionIndicationLedBlinking();
            OUTPUT_RELAY2=ON;
            }
            man=0;
            M2=1;
        }
        //on condtion
        if(copy_parentalLockBuffer[2] == CHAR_OFF && INPUTSWITCH2 == SWITCH_RELEASED && M2 == ON)
        {
            if(man==1)
            {
            SwitchOffStatustToGatway('2');
            TransmissionIndicationLedBlinking();
            OUTPUT_RELAY2=OFF;
            }
            man=0;
            M2=0;
        }
        
        
       // //check switch third status 
        //off condition
        if(copy_parentalLockBuffer[3] == CHAR_OFF && INPUTSWITCH3 == SWITCH_PRESSED && M3 == OFF)
        {
            if(man == 1)
            {
            SwitchOnStatustToGatway('3');
            TransmissionIndicationLedBlinking();
            OUTPUT_RELAY3=ON;
            }
            man=0;
            M3=1;
          
        }
        //on condtion
        if(copy_parentalLockBuffer[3] == CHAR_OFF && INPUTSWITCH3 == SWITCH_RELEASED && M3 == ON)
        {
            if(man==1)
            {
            SwitchOffStatustToGatway('3');
            TransmissionIndicationLedBlinking();
            OUTPUT_RELAY3=OFF;
            }
            man=0;
            M3=0;
            
        }
        
     
       // //check switch fourth status 
        //off condition
        if(copy_parentalLockBuffer[4] == CHAR_OFF && INPUTSWITCH4 == SWITCH_PRESSED && M4 == OFF)
        {
            if(man==1)
            {
            SwitchOnStatustToGatway('4');
            TransmissionIndicationLedBlinking();
            OUTPUT_RELAY4=ON;
            }
            man=0;
            M4=1;
            
        }
        //on condtion
        if(copy_parentalLockBuffer[4] == CHAR_OFF && INPUTSWITCH4 == SWITCH_RELEASED && M4 == ON)
        {
            if(man==1)
            {
            
            SwitchOffStatustToGatway('4');
            TransmissionIndicationLedBlinking();
            OUTPUT_RELAY4=OFF;
            }
            man=0;
            M4=0;
           
        }
               //off condition
        if(copy_parentalLockBuffer[5] == CHAR_OFF && INPUTSWITCH5 == SWITCH_PRESSED && M5 == OFF)
        {
            if(man==1)
            {
            SwitchOnStatustToGatway('5');
            TransmissionIndicationLedBlinking();
            OUTPUT_RELAY5=ON;
            }
            man=0;
            M5=1;
            
        }
        //on condtion
        if(copy_parentalLockBuffer[5] == CHAR_OFF && INPUTSWITCH5 == SWITCH_RELEASED && M5 == ON)
        {
            if(man==1)
            {
            
            SwitchOffStatustToGatway('5');
            TransmissionIndicationLedBlinking();
            OUTPUT_RELAY5=OFF;
            }
            man=0;
            M5=0;
           
        }
       
               //off condition
        if(copy_parentalLockBuffer[6] == CHAR_OFF && INPUTSWITCH6 == SWITCH_PRESSED && M6 == OFF)
        {
            if(man==1)
            {
            SwitchOnStatustToGatway('6');
            TransmissionIndicationLedBlinking();
            OUTPUT_RELAY6=ON;
            }
            man=0;
            M6=1;
            
        }
        //on condtion
        if(copy_parentalLockBuffer[6] == CHAR_OFF && INPUTSWITCH6 == SWITCH_RELEASED && M6 == ON)
        {
            if(man==1)
            {
            
            SwitchOffStatustToGatway('6');
            TransmissionIndicationLedBlinking();
            OUTPUT_RELAY6=OFF;
            }
            man=0;
            M6=0;
           
        }
       

               //off condition
        if(copy_parentalLockBuffer[7] == CHAR_OFF && INPUTSWITCH7 == SWITCH_PRESSED && M7 == OFF)
        {
            if(man==1)
            {
            SwitchOnStatustToGatway('7');
            TransmissionIndicationLedBlinking();
            OUTPUT_RELAY7=ON;
            }
            man=0;
            M7=1;
            
        }
        //on condtion
        if(copy_parentalLockBuffer[7] == CHAR_OFF && INPUTSWITCH7 == SWITCH_RELEASED && M7 == ON)
        {
            if(man==1)
            {
            
            SwitchOffStatustToGatway('7');
            TransmissionIndicationLedBlinking();
            OUTPUT_RELAY7=OFF;
            }
            man=0;
            M7=0;
           
        }
 
       

       //off condition
         if(copy_parentalLockBuffer[8] == CHAR_OFF && INPUTSWITCH8 == SWITCH_PRESSED && M8 == OFF)
         {
             if(man==1)
             {
              SwitchOnStatustToGatway('8');  
            TransmissionIndicationLedBlinking();
             OUTPUT_RELAY8=ON;
             }
             man=0;
             M8=1;
            
         }
         //on condtion
         if(copy_parentalLockBuffer[8] == CHAR_OFF && INPUTSWITCH8 == SWITCH_RELEASED && M8 == ON)
         {
             if(man==1)
             {
            
            SwitchOffStatustToGatway('8');
            TransmissionIndicationLedBlinking();
             OUTPUT_RELAY8=OFF;
             }
             man=0;
             M8=0;
           
         }
              //off condition
         if(copy_parentalLockBuffer[9] == CHAR_OFF && INPUTSWITCH9 == SWITCH_PRESSED && M9 == OFF)
         {
             if(man==1)
             {
            
               start_PWM_Generation_in_ISR_FLAG = 0;
           SwitchOnStatustToGatway('9');    
           TransmissionIndicationLedBlinking();
            OUTPUT_DIMMER=OFF;
             }
             man=0;
             M9=1;
            
         }
         //on condtion
         if(copy_parentalLockBuffer[9] == CHAR_OFF && INPUTSWITCH9 == SWITCH_RELEASED && M9 == ON)
         {
             if(man==1)
             {
            start_PWM_Generation_in_ISR_FLAG = 0;
            SwitchOffStatustToGatway('9');
            TransmissionIndicationLedBlinking();
             OUTPUT_DIMMER=ON;
             }
             man=0;
             M9=0;
           
         }
       
 
    }    
}

void applianceControl(char charSwitchMSB, char charSwitchLSB, char charSwitchSTATE, char chDimmerSpeedMSB, char chDimmerSpeedLSB,
        char charParentalControl, char charFinalFrameState){
    
    //define used variables and initilize it with zero
    int integerSwitchNumber = 0;
    int integerSwitchState = 0;
    int integerSpeed = 0;
    int currentStateBufferPositions=0;
    // Get switch Number in Integer format 
    //define all used character data types and initlize it with "#"
    char switchNumberStringBuffer[2]="#";
    char dimmerSpeedStringBuffer[2]="#";
    
    switchNumberStringBuffer[0]=charSwitchMSB;
    switchNumberStringBuffer[1]=charSwitchLSB;    
    integerSwitchNumber = atoi(switchNumberStringBuffer);//convert string into integer
    
    // Get switch State in Integer Format
    
    integerSwitchState = charSwitchSTATE-'0';
    
    // Get speed of Fan or level of dimmer    
    dimmerSpeedStringBuffer[0]=chDimmerSpeedMSB;
    dimmerSpeedStringBuffer[1]=chDimmerSpeedLSB;    
    integerSpeed = atoi(dimmerSpeedStringBuffer);
    
    // save Parental lock state of each switch into parental lock buffer
//    int integerParentalControl=charParentalControl-'0';
    parentalLockBuffer[integerSwitchNumber] = charParentalControl;
    copy_parentalLockBuffer[integerSwitchNumber]=parentalLockBuffer[integerSwitchNumber];
    // ACKNOWLEDGMENT data Format :->> (Gateway+SwitchState+SwitchMSB+SwitchLSB)
    
    currentStateBufferPositions = ((1+4*(integerSwitchNumber))-5);
    currentStateBuffer[currentStateBufferPositions++] = 'G';
    currentStateBuffer[currentStateBufferPositions++] = charSwitchSTATE;
    currentStateBuffer[currentStateBufferPositions++] = charSwitchMSB;
    currentStateBuffer[currentStateBufferPositions] = charSwitchLSB;    
    
    currentStateBufferPositions-=3;     // since we have come forward by 3 address in current state buffer
    if(charFinalFrameState=='1')    // until 
    {
        sendAcknowledgment(currentStateBuffer+currentStateBufferPositions);    
    }
    
    switch(integerSwitchNumber){
        case 1:
        {
            OUTPUT_RELAY1 = integerSwitchState;
        }break;
            
        case 2:
            {
            OUTPUT_RELAY2 = integerSwitchState;

            } break;
        case 3:
        {
            OUTPUT_RELAY3 = integerSwitchState;

        } break;
          
        case 4:
        {
            OUTPUT_RELAY4 = integerSwitchState;
        }break;
        case 5:
        {
            OUTPUT_RELAY5 = integerSwitchState;
        }break;  
        case 6:
        {
            OUTPUT_RELAY6 = integerSwitchState;
        }break;

        case 7:
        {
            OUTPUT_RELAY7 = integerSwitchState;
        }break; 
      
        case 8:
        {
            OUTPUT_RELAY8 = integerSwitchState;
        }break; 
        case 9:{
                start_PWM_Generation_in_ISR_FLAG = integerSwitchState;
               switch(integerSwitchState){
                case 0:
                    OUTPUT_DIMMER=1;  // For Triac --> inverted condition for off
                    break;
                case 1:
                    levelofDimmer_MSB = chDimmerSpeedMSB;
                    levelofDimmer_LSB = chDimmerSpeedLSB;
                    break;
                default:
                    break;
               }
        }break;  
            default:
            break;
        }
    
}


/*
 * All input output pin initialization
 */
void GPIO_pin_Initialize(){
    clearAllPorts();
    pinINIT_extra();
    INPUT_SWITCH_DIR_1 = 1;
    INPUT_SWITCH_DIR_2 = 1;
    INPUT_SWITCH_DIR_3 = 1;
    INPUT_SWITCH_DIR_4 = 1;
    INPUT_SWITCH_DIR_5 = 1;
    INPUT_SWITCH_DIR_6 = 1;
    INPUT_SWITCH_DIR_7 = 1;
    INPUT_SWITCH_DIR_8 = 1;
    INPUT_SWITCH_DIR_9 = 1;

   
    
    OUTPUT_RELAY_DIR_1 = 0;
    OUTPUT_RELAY_DIR_2 = 0;
    OUTPUT_RELAY_DIR_3 = 0;
    OUTPUT_RELAY_DIR_4 = 0;
    OUTPUT_RELAY_DIR_5 = 0;
    OUTPUT_RELAY_DIR_6 = 0;
    OUTPUT_RELAY_DIR_7 = 0;
    OUTPUT_RELAY_DIR_8 = 0;
    OUTPUT_DIMMER_DIR = 0;
    
    
    ZCD_CCP10_DIR = 1;
    ZCD_CCP9_DIR = 1;
    ZCD_CCP7_DIR = 1;
    ZCD_CCP8_DIR = 1;
   OUTPUT_TRANMIT_INDICATION_LED_DIR = 0;
    OUTPUT_RECEIVE_INDICATION_LED_DIR = 0;
    USER_RECEIVE_INDICATION_LED_DIR = 0;
    
    
    OUTPUT_RELAY_RED_LED_DIR = 0;
    OUTPUT_RELAY_GREEN_LED_DIR = 0;
    OUTPUT_RELAY_BLUE_LED_DIR = 0;
    // peripherals directions
    
    // USART DIRECTIONS
    USART_1_TRANSMIT_OUTPUT_DIR = 0;
    USART_1_RECIEVE_INPUT_DIR = 1;
    
    clearAllPorts();
}

/*
 * ALL Peripheral Initialization
 */
void allPeripheralInit(){
    EUSART_Initialize();
    TMR1_Initialize();
    TMR2_Initialize();
    TMR3_Initialize();
    TMR4_Initialize();
    TMR5_Initialize();
    TMR6_Initialize();
    
    TMR8_Initialize();
    TMR10_Initialize();
    CCP9_Initialize();
    CCP8_Initialize();
    CCP7_Initialize();
    CCP10_Initialize();
}

/*
 * USART Control Registers initialization
 */
void EUSART_Initialize(){
    PIE1bits.RC1IE = 0;
    PIE1bits.TX1IE = 0;

    // Set the EUSART module to the options selected in the user interface.

    // ABDOVF no_overflow; SCKP Non-Inverted; BRG16 16bit_generator; WUE enabled; ABDEN disabled;
    BAUD1CON = 0x0A;

    // SPEN enabled; RX9 8-bit; CREN enabled; ADDEN disabled; SREN disabled;
    RC1STA = 0x90;

    // TX9 8-bit; TX9D 0; SENDB sync_break_complete; TXEN enabled; SYNC asynchronous; BRGH hi_speed; CSRC slave;
    TX1STA = 0x24;

    // Baud Rate = 9600; SP1BRGL 12;
    //SPBRGL = 0x0C;
    //SPBRGL = 0x19;                  // SP1BRGL is 25 (hex value=0x19) for 9600 baud on 16 MHz crystal frequency
    SP1BRGL = 0xA0;                  // SYNC =0 ; BRGH = 1 ; BRG16=1;
    // Baud Rate = 9600; SP1BRGH 1;
    SP1BRGH = 0x01;

    // Enable all active interrupts ---> INTCON reg .... bit 7            page 105
    GIE = 1;

    // Enables all active peripheral interrupts -----> INTCON reg .... bit 6         page 105
    PEIE = 1;

    // enable receive interrupt
    PIE1bits.RC1IE = 1;                    // handled into INTERRUPT_Initialize()

    // Transmit Enabled
    TX1STAbits.TXEN = 1;

    // Serial Port Enabled
    RC1STAbits.SPEN = 1;
}

void TMR1_Initialize(void)
{
   
    T1CON = 0x00;

    //T1GSS T1G; TMR1GE disabled; T1GTM disabled; T1GPOL low; T1GGO_nDONE done; T1GSPM disabled;
    T1GCON = 0x00;

        //TMR1H 29;
    TMR1H = 0x00;

    //TMR1L 112;
    TMR1L = 0x00;

    // Clearing IF flag before enabling the interrupt.
    PIR1bits.TMR1IF = 0;

    // Enabling TMR1 interrupt.
    PIE1bits.TMR1IE = 1;

    // Start TMR1
   // T1CONbits.TMR1ON = 1;

    // Enable all active interrupts ---> INTCON reg .... bit 7            page 105
    GIE = 1;

    // Enables all active peripheral interrupts -----> INTCON reg .... bit 6         page 105
    PEIE = 1;

}

void TMR3_Initialize(void)
{

    T3CON = 0x00;

    //T1GSS T1G; TMR1GE disabled; T1GTM disabled; T1GPOL low; T1GGO_nDONE done; T1GSPM disabled;
    T3GCON = 0x00;

        //TMR1H 29;
    TMR3H = 0x00;
 
    //TMR1L 112;
    TMR3L = 0x00;

    // Clearing IF flag before enabling the interrupt.
    PIR3bits.TMR3IF = 0;

    // Enabling TMR1 interrupt.
    PIE3bits.TMR3IE = 1;

    // Start TMR1
   // T1CONbits.TMR1ON = 1;

    // Enable all active interrupts ---> INTCON reg .... bit 7            page 105
    GIE = 1;

    // Enables all active peripheral interrupts -----> INTCON reg .... bit 6         page 105
    PEIE = 1;

}
void TMR5_Initialize(void)
{
    //Set the Timer to the options selected in the GUI

    //T5CKPS 1:1; T5OSCEN disabled; nT5SYNC synchronize; TMR5CS FOSC/4; TMR5ON off; 
    T5CON = 0x00;

    //T5GSS T5G; TMR5GE disabled; T5GTM disabled; T5GPOL low; T5GGO_nDONE done; T5GSPM disabled; 
    T5GCON = 0x00;

    //TMR5H 123; 
    TMR5H = 0x00;

    //TMR5L 48; 
    TMR5L = 0x00;

    // Clearing IF flag.
    PIR3bits.TMR5IF = 0;    
    
    // Enabling TMR5 interrupt.
    PIE3bits.TMR5IE = 1;
}

void TMR2_Initialize(void)
{
//     Set TMR2 to the options selected in the User Interface

//     T2CKPS 1:1; T2OUTPS 1:1; TMR2ON off; 
    T2CON = 0x08;
//
//     PR2 39; 
//    PR2 = 0x00;
//
//     TMR2 10; 
    TMR2 = 0x00;

//     Clearing IF flag before enabling the interrupt.
    PIR1bits.TMR2IF = 0;

//     Enabling TMR2 interrupt.
    PIE1bits.TMR2IE = 1;
         GIE = 1;

//     Enables all active peripheral interrupts -----> INTCON reg .... bit 6         page 105
    PEIE = 1;
}


void TMR4_Initialize(void)
{
    // Set TMR2 to the options selected in the User Interface

    // T2CKPS 1:2; T2OUTPS 1:1; TMR2ON off; 
    T4CON = 0x08;

    // PR2 39; 
//    PR2 = 0x00;

    // TMR2 10; 
    TMR4 = 0x00;

    // Clearing IF flag before enabling the interrupt.
    PIR3bits.TMR4IF = 0;

    // Enabling TMR2 interrupt.
    PIE3bits.TMR4IE = 1;
            GIE = 1;

//     Enables all active peripheral interrupts -----> INTCON reg .... bit 6         page 105
    PEIE = 1;
}

void TMR6_Initialize(void)
{
    // Set TMR6 to the options selected in the User Interface

    // T6CKPS 1:2; T6OUTPS 1:1; TMR6ON off; 
    T6CON = 0x08;

    // PR6 39; 
//    PR6 = 0x27;

    // TMR6 0; 
    TMR6 = 0x00;

    // Clearing IF flag before enabling the interrupt.
    PIR3bits.TMR6IF = 0;

    // Enabling TMR6 interrupt.
    PIE3bits.TMR6IE = 1;
}
void TMR8_Initialize(void)
{
    // Set TMR6 to the options selected in the User Interface

    // T6CKPS 1:2; T6OUTPS 1:1; TMR6ON off; 
    T8CON = 0x08;

    // PR6 39; 
//    PR6 = 0x27;

    // TMR6 0; 
    TMR8 = 0x00;

    // Clearing IF flag before enabling the interrupt.
    PIR2bits.TMR8IF = 0;

    // Enabling TMR6 interrupt.
    PIE2bits.TMR8IE = 1;
}
void TMR10_Initialize(void)
{
    // Set TMR6 to the options selected in the User Interface

    // T6CKPS 1:2; T6OUTPS 1:1; TMR6ON off; 
    T10CON = 0x08;

    // PR6 39; 
//    PR6 = 0x27;

    // TMR6 0; 
    TMR10 = 0x00;

    // Clearing IF flag before enabling the interrupt.
    PIR2bits.TMR10IF = 0;

    // Enabling TMR6 interrupt.
    PIE2bits.TMR10IE = 1;
}
void CCP9_Initialize(){
    // Set the CCP1 to the options selected in the User Interface

    // MODE Every edge; EN enabled; FMT right_aligned;
    CCP9CON = 0x84;

    // RH 0;
    CCPR9H = 0x00;

    // RL 0;
    CCPR9L = 0x00;
    
//    CCPTMRS2bits.C9TSEL0=0;
//    CCPTMRS2bits.C9TSEL1=0;

    // Clear the CCP1 interrupt flag
    PIR4bits.CCP9IF = 0;

    // Enable the CCP1 interrupt
    PIE4bits.CCP9IE = 1;
}
void CCP10_Initialize(){
    // Set the CCP1 to the options selected in the User Interface

    // MODE Every edge; EN enabled; FMT right_aligned;
    CCP10CON = 0x84;

    // RH 0;
    CCPR10H = 0x00;

    // RL 0;
    CCPR10L = 0x00;
    
//    CCPTMRS2bits.C9TSEL0=0;
//    CCPTMRS2bits.C9TSEL1=0;

    // Clear the CCP1 interrupt flag
    PIR4bits.CCP10IF = 0;

    // Enable the CCP1 interrupt
    PIE4bits.CCP10IE = 1;
}
void CCP7_Initialize(){
    // Set the CCP1 to the options selected in the User Interface

    // MODE Every edge; EN enabled; FMT right_aligned;
    CCP7CON = 0x84;

    // RH 0;
    CCPR7H = 0x00;

    // RL 0;
    CCPR7L = 0x00;
    
//    CCPTMRS2bits.C9TSEL0=0;
//    CCPTMRS2bits.C9TSEL1=0;

    // Clear the CCP1 interrupt flag
    PIR4bits.CCP7IF = 0;

    // Enable the CCP1 interrupt
    PIE4bits.CCP7IE = 1;
}
void CCP8_Initialize(){
    // Set the CCP1 to the options selected in the User Interface

    // MODE Every edge; EN enabled; FMT right_aligned;
    CCP9CON = 0x84;

    // RH 0;
    CCPR8H = 0x00;

    // RL 0;
    CCPR8L = 0x00;
    
//    CCPTMRS2bits.C9TSEL0=0;
//    CCPTMRS2bits.C9TSEL1=0;

    // Clear the CCP1 interrupt flag
    PIR4bits.CCP8IF = 0;

    // Enable the CCP1 interrupt
    PIE4bits.CCP8IE = 1;
}
void errorsISR(const char* errNum){
   
  	while(*errNum != NULL)
 	{ 
        while (!TX1STAbits.TRMT);
 		TX1REG = *errNum;
 		*errNum++;
        
 	}
}
void errorsMain(const char* errNum){
   
  	while(*errNum != NULL)
 	{ 
        while (!TX1STAbits.TRMT);
 		TX1REG = *errNum;
 		*errNum++;
       
 	}
}

void TransmissionIndicationLedBlinking(){
            OUTPUT_TRANMIT_INDICATION_LED = 0;
            __delay_ms(100);
            OUTPUT_TRANMIT_INDICATION_LED = 1;
}
void ReceivingIndicationLedBlinking(){
                  OUTPUT_RECEIVE_INDICATION_LED = 0;
                __delay_ms(100);
                OUTPUT_RECEIVE_INDICATION_LED = 1;
 
}
void sendAcknowledgment( char* currentStateBuffer){
  int Tx_count=0;
  	while(Tx_count!=4)
 	{ 
        while (!TX1STAbits.TRMT);
 		TX1REG = *currentStateBuffer;
 		*currentStateBuffer++;
        Tx_count++;
 	}
}
void SwitchOffStatustToGatway(const char SwitchOffNumber)
{
            TX1REG = 'R';__delay_ms(1);
            TX1REG = '0';__delay_ms(1);
            TX1REG = '0';__delay_ms(1);
            TX1REG = SwitchOffNumber;__delay_ms(1);
}
void SwitchOnStatustToGatway(const char SwitchOnNumber)
{
            TX1REG = 'R';__delay_ms(1);
            TX1REG = '1';__delay_ms(1);
            TX1REG = '0';__delay_ms(1);
            TX1REG = SwitchOnNumber;__delay_ms(1);
}
void copyReceivedDataBuffer(){
    int dataBufferCounter=2;
    for(dataBufferCounter=2;dataBufferCounter<9;dataBufferCounter++){
        tempReceivedDataBuffer[dataBufferCounter-2]=mainReceivedDataBuffer[dataBufferCounter]; // copy data buffer from main
        mainReceivedDataBuffer[dataBufferCounter]='#';  // clean data buffer
    }
}
/*
 * AANALOG and PULL up REGISTERS related initialization
 */
void pinINIT_extra(){
    ANSELG=0x00;    WPUG = 0;
    
    ANSELF=0x00;
    
    ANSELE=0x00;    WPUE=0x00;
    
    ANSELD=0x00;    WPUD=0x00;
    
    ANSELB=0x00;    WPUB=0x00;
    
    ANSELA=0x00;     
} 

/*
 * always clear all the ports before initialization
 */
void clearAllPorts(){
    
    //this is wituout transistor
    //when used transistor use as opposite
    OUTPUT_RELAY1=0;
    OUTPUT_RELAY2=0;
    OUTPUT_RELAY3=0;
    OUTPUT_RELAY4=0;
    OUTPUT_RELAY5=0;
    OUTPUT_RELAY6=0;
    OUTPUT_RELAY7=0;
    OUTPUT_RELAY8=0;
    OUTPUT_TRANMIT_INDICATION_LED = 1;
    OUTPUT_RECEIVE_INDICATION_LED = 1;
    USER_RECEIVE_INDICATION_LED = 1;//off condition--->>>leds are high by default
    
    OUTPUT_FOR_RED_LED=0;
    OUTPUT_FOR_GREEN_LED=0;
    OUTPUT_FOR_BLUE_LED=0;
    OUTPUT_DIMMER = 0;
   
}
