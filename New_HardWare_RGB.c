/*
 * File:   New_HardWare_RGB.c
 * Author:varun sahni
 * new hardware
 * This is test code for New hardware with RGB strip
 * Created on 2 January, 2019, 6:58 PM
 */


#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include<pic16f1526.h>
#include<string.h>
#include <math.h>
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

///RGB MACROS
#define OUTPUT_FOR_RED_LED PORTAbits.RA3 ///pwm for red
#define OUTPUT_FOR_GREEN_LED PORTAbits.RA2 //pwm for green
#define OUTPUT_FOR_BLUE_LED PORTEbits.RE5   // PWM for blue

#define OUTPUT_RELAY_DIR_1 TRISEbits.TRISE0
#define OUTPUT_RELAY_DIR_2 TRISEbits.TRISE1
#define OUTPUT_RELAY_DIR_3 TRISGbits.TRISG0
#define OUTPUT_RELAY_DIR_4 TRISGbits.TRISG3
#define OUTPUT_RELAY_DIR_5 TRISGbits.TRISG4
#define OUTPUT_RELAY_DIR_6 TRISFbits.TRISF6
#define OUTPUT_RELAY_DIR_7 TRISFbits.TRISF5
#define OUTPUT_RELAY_DIR_8 TRISFbits.TRISF4
#define OUTPUT_RELAY_RED_LED_DIR TRISAbits.TRISA3
#define OUTPUT_RELAY_GREEN_LED_DIR TRISAbits.TRISA2
#define OUTPUT_RELAY_BLUE_LED_DIR TRISEbits.TRISE5        // direction of PWM OUTPUT to MOC3021

#define INPUTSWITCH1 PORTBbits.RB0
#define INPUTSWITCH2 PORTBbits.RB1
#define INPUTSWITCH3 PORTBbits.RB2
#define INPUTSWITCH4 PORTBbits.RB3
#define INPUTSWITCH5 PORTBbits.RB4
#define INPUTSWITCH6 PORTBbits.RB5
#define INPUTSWITCH7 PORTDbits.RD6
#define INPUTSWITCH8 PORTDbits.RD7

#define INPUT_SWITCH_DIR_1 TRISBbits.TRISB0
#define INPUT_SWITCH_DIR_2 TRISBbits.TRISB1
#define INPUT_SWITCH_DIR_3 TRISBbits.TRISB2
#define INPUT_SWITCH_DIR_4 TRISBbits.TRISB3
#define INPUT_SWITCH_DIR_5 TRISBbits.TRISB4
#define INPUT_SWITCH_DIR_6 TRISBbits.TRISB5
#define INPUT_SWITCH_DIR_7 TRISDbits.TRISD6
#define INPUT_SWITCH_DIR_8 TRISDbits.TRISD7

#define ZCD_CCP9_DIR TRISEbits.TRISE3
#define ZCD_CCP1_DIR TRISCbits.TRISC2
#define ZCD_CCP3_DIR TRISGbits.TRISG0
#define ZCD_CCP10_DIR TRIS



        // direction of PWM OUTPUT to MOC3021

/*
 * Extra Periferals Direction and PORT
 */
//#define ZCD_CCP9_DIR TRISEbits.TRISE3
// USART Directions
#define USART_1_TRANSMIT_OUTPUT_DIR TRISCbits.TRISC6
#define USART_1_RECIEVE_INPUT_DIR TRISCbits.TRISC7

#define RECIEVED_DATA_LENGTH (30+2)
#define TOTAL_NUMBER_OF_SWITCH (8+2)

#define TRUE 1
#define FALSE 0



// Conditional compilation
//#define DEBUG
//#define RELEASE



// Conditional compilation
//#define DEBUG
#define MODULENAMELENGTH 3
#define SWITCHNUMBERLENGTH 2
//#define ACKNOWLEDGEMENTLENGTH 6
#define SWITCHSTATUS 1
#define REDCOLOINTENSITYLENGTH 2
#define GREENCOLORINTENSITYLENGHT 2
#define BLUECOLOINTENSITYLENGHT 2
#define CHILDLOCKLENGTH 1
#define FINALFRAMEBITLENGTH 1

#define FINALFRAMELENGTH ((3+2+6+1+3+3+3+1+1)*2)

unsigned char ModuleNameBuffer[MODULENAMELENGTH]=NULL;
unsigned char SwitchNumberBuffer[SWITCHNUMBERLENGTH]=NULL;
//unsigned char AcknowledgementBuffer[ACKNOWLEDGEMENTLENGTH]=NULL;
unsigned char SwitchStatusBuffer[SWITCHSTATUS]=NULL;
unsigned char RedColorIntensityBuffer[REDCOLOINTENSITYLENGTH]=NULL;
unsigned char GreenColorIntensityBuffer[GREENCOLORINTENSITYLENGHT]=NULL;
unsigned char BlueColorIntensityBuffer[BLUECOLOINTENSITYLENGHT]=NULL;
unsigned char ChildLockBuffer[CHILDLOCKLENGTH]=NULL;
unsigned char FinalFramebitBuffer[FINALFRAMEBITLENGTH]=NULL;

unsigned char sendFinalBufferToGAteway[FINALFRAMELENGTH]=NULL;

// ALL error Definitions
/* 
 * #define WRONG_DATA_RECIEVED_ERROR_CODE ERRX
 * #define RECIVING_OVERRUN_ERROR EROV
 * #define RECEIVING_DATA_LOST_IN_MAIN ERLS
 */
/* DATA USED IN MANUAL  STARTS HERE*/
unsigned int M1;unsigned int M2;unsigned int M3;unsigned int M4;unsigned int M5;
unsigned int M6;unsigned int M7;unsigned int M8;
//#define DEBUG
#define ON 1
#define OFF 0
#define CHAR_OFF '0'
#define CHAR_ON '1'
#define GIVES_OUTPUT_ZERO_IF_EVEN 1
/* DATA USED IN MANUAL END HERE*/


unsigned int FrameLegthCounter =0;
int mainReceivedDataPosition=0, mainDataReceived=FALSE;
int mainReceivedDataFlag=0;
int mainDatacopyPosition=0;
unsigned char mainReceivedDataBuffer[RECIEVED_DATA_LENGTH]="#"; 
unsigned char copymainReceivedDataBuffer[RECIEVED_DATA_LENGTH]="#";
unsigned char tempReceivedDataBuffer[RECIEVED_DATA_LENGTH-8]="#";
unsigned char parentalLockBuffer[TOTAL_NUMBER_OF_SWITCH]="0000000";
unsigned char copyparentalLockBuffer[TOTAL_NUMBER_OF_SWITCH]="0000000";
unsigned char currentStateBuffer[(TOTAL_NUMBER_OF_SWITCH*4)+2]="#";
int start_PWM_Generation_For_RedLed=FALSE;
int start_PWM_Generation_For_GreenLed = FALSE;
int start_PWM_Generation_For_BlueLed = FALSE;
char levelofBlueLed_MSB = '0',levelofBlueLed_MID = '0',levelofBlueLed_LSB = '0';
char levelofRedLed_MSB = '0',levelofRedLed_MID = '0',levelofRedLed_LSB = '0';
char levelofGreenLed_MSB = '0',levelofGreenLed_MID = '0',levelofGreenLed_LSB = '0';

        
        unsigned char ConvertIntegertoStringRedColorBuffer[REDCOLOINTENSITYLENGTH] = NULL;
        unsigned char ConvertIntegertoStringGreenColorBuffer[GREENCOLORINTENSITYLENGHT] = NULL;
        unsigned char ConvertIntegertoStringBlueColorBuffer[BLUECOLOINTENSITYLENGHT] = NULL;
        
 char *ConvertmaindataReceiveIntoString = NULL;
  int mainDataReceiveStringLength=0;
 int RedColorIntensityStringLength =0;
 int GreenColorIntensityStringLength=0;
 int BlueColorIntensityStringLength =0;
const char delimiter[2] = ".";
char *token=NULL;
int partCounter=0;


char *ModuleNameString=NULL;
char *SwitchNumberString = NULL;
char switchNumberBuffer[3]=NULL;
char *AcknowledgementString =NULL;
char *SwitchStatusString = NULL;
char switchstatusbuffer[2]=NULL;
char *RedColorIntensityString = NULL;
char *GreenColorIntensityString = NULL;
char *BlueColorIntensityString = NULL;
char *ChildLockString = NULL;
char ChildLockbuffer[2]=NULL;
char *FinalframebitString = NULL;
char Finalframebitbuffer[2] = NULL;
char *StopBitString=NULL;
char StopBitStringbufer=NULL;





void errorsISR(char* errNum);
void errorsMain(char* errNum);
void sendAcknowledgment(char* currentStateBuffer);
void Send_Acknowlde_To_RedPWM(char charlevelofRedLed_LSB);
void Send_Acknowlde_To_GreenPWM(char charlevelofGreenLed_LSB);
void Send_Acknowlde_To_BluePWM(char charlevelofBlueLed_LSB);

void clearAllPorts();
void pinINIT_extra();
void GPIO_pin_Initialize();
void peripheralsEnable();
void AllInterruptEnable();
void EUSART_Initialize();

void TMR5_Initialize();
void TMR2_Initialize();
void TMR4_Initialize();
void TMR6_Initialize();
void TMR3_Initialize();
void TMR1_Initialize();
void CCP9_Initialize();
void CCP3_Initialize();
void CCP1_Initialize();

void allPeripheralInit();




void applicationControl(char *ModuleName,char *SwitchNumber,char *SwitchStatus,
                char  *RedColorIntensity,char *GreenColorIntensity,char *BlueColorIntensity,char *ChildLock,char *Finalframe);
#include"include.h"
interrupt void isr(){
 

        // ************************************* UART INTERRUPT *********************************************** //
    if(RC1IF){        
        if(RC1STAbits.OERR){    // If over run error, then reset the receiver
            RC1STAbits.CREN = 0; // countinuous Recieve Disable
            RC1STAbits.CREN = 1; // countinuous Recieve Enable
            errorsISR("EROV"); 
        } 
        
        mainReceivedDataBuffer[mainReceivedDataPosition]=RC1REG;
         copymainReceivedDataBuffer[mainReceivedDataPosition] = mainReceivedDataBuffer[mainReceivedDataPosition];
        #ifdef DEBUG
        TX1REG=copymainReceivedDataBuffer[mainReceivedDataPosition];
        #endif
        

           //if(mainReceivedDataPosition>29){
            if(copymainReceivedDataBuffer[mainReceivedDataPosition] == '|'){
                mainDataReceived=TRUE;
                mainReceivedDataPosition=0;
               
                RC1IF=0;                
            }
            else if((copymainReceivedDataBuffer[0]=='R' || copymainReceivedDataBuffer[0]=='S') && copymainReceivedDataBuffer[0] != NULL){
                            mainReceivedDataPosition++;
                            FrameLegthCounter++;
                            
            }
        

        else{
             TX1REG = copymainReceivedDataBuffer[mainReceivedDataPosition];
            RC1STAbits.CREN = 0; // countinuous Recieve Disable
            RC1STAbits.CREN = 1; // countinuous Recieve Enable
            mainReceivedDataPosition=0; // Reinitiate buffer counter
            errorsISR("ERRX");           
        }
    }// End of RC1IF
    
    
    
    //**************BLUE COLOR**************************//
   //*******************TIMER2 INTERRUPT**************************//
       if(PIE1bits.TMR2IE==1 && PIR1bits.TMR2IF==1)
    {        

        
        PIR1bits.TMR2IF=0;
        OUTPUT_FOR_RED_LED = FALSE;//for rgb
        T2CONbits.TMR2ON=0;
    } 
     
    //*********************TIMER1 INTERRUPT**************************//
     if(PIE1bits.TMR1IE == 1 && PIR1bits.TMR1IF==1)
    {
        PIR1bits.TMR1IF=0;  
        OUTPUT_FOR_RED_LED = TRUE;
        PR2=0x9F;
         T2CONbits.TMR2ON = 1;
        T1CONbits.TMR1ON = 0; 
        
    }
       
     //*******************************GREEN COLOR 22222 *************************************
 
    if(PIE3bits.TMR4IE==1 && PIR3bits.TMR4IF==1)
    {           
        PIR3bits.TMR4IF=0;
        OUTPUT_FOR_GREEN_LED = FALSE;
        T4CONbits.TMR4ON=0;

    }
    
    if(PIE3bits.TMR3IE == 1 && PIR3bits.TMR3IF==1)
    {
        PIR3bits.TMR3IF=0;   
        OUTPUT_FOR_GREEN_LED = TRUE;
        PR4=0x9F;
        T4CONbits.TMR4ON = 1;
        T3CONbits.TMR3ON = 0;        
    }
       //*****************************blue color*************************************    
    
    if(PIE3bits.TMR6IE == 1 && PIR3bits.TMR6IF == 1)
    {           
        PIR3bits.TMR6IF=0;
       OUTPUT_FOR_BLUE_LED = FALSE;
        T6CONbits.TMR6ON=0;
    } 
    
    if(PIE3bits.TMR5IE == 1 && PIR3bits.TMR5IF==1)
    {
         PIR3bits.TMR5IF=0;        
        OUTPUT_FOR_BLUE_LED = TRUE;
        PR6=0x9F;
        T6CONbits.TMR6ON=1;
        T5CONbits.TMR5ON=0;        
    }
    
    //*************************ZCD INTERRRUPT****************************//
    if(PIR1bits.CCP1IF==1 || PIR3bits.CCP3IF == 1 || PIR4bits.CCP9IF==1){    
   if(CCP9IF){
        if(CCP9IF == 1){
             CCP9IF=0;
         if(start_PWM_Generation_For_RedLed == 1){
          switch(levelofRedLed_MSB){
                case '9':           // 8.5
                    /**/
                        switch(levelofRedLed_LSB)
                             {
                             case '9':           // 8.5
                             {
//                                    TMR1H=0x7B;
//                                    TMR1L=0x30;
                                    OUTPUT_FOR_RED_LED=1;
//                                    T1CONbits.TMR1ON = 1;
//                                    PIE1bits.TMR1IE = 1;
//                                    PIR1bits.TMR1IF=0;
                                    
                             }
                                     break;
                             case '8':           // 8.4
                             {
  										TMR1H=0x86;
                                         TMR1L=0xE8;
                                         T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                   
                                     break;
                             case '7':           // 8.35
                             {
                                      TMR1H=0x87;
                                         TMR1L=0xB0;
                                         T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '6':           // 8.25
                             {
                                      TMR1H=0x87;
                                         TMR1L=0xB0;
                                         T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '5':          // 8.15
                             {
                                     TMR1H=0x88;
                                         TMR1L=0x78;
                                         T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '4':               // 8.1
                             {
                                     TMR1H=0x88;
                                         TMR1L=0x78;
                                         T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '3':               // 8.0  
                             {
                                     TMR1H=0x89;
                                         TMR1L=0x40;
                                         T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '2':            //7.95
                             {
                                     TMR1H=0x89;
                                         TMR1L=0x40;
                                         T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '1':           //7.9
                             {
                                    TMR1H=0x8A;
                                         TMR1L=0x08;
                                         T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '0':           // 7.85
                             {
                                     TMR1H=0x8A;
                                         TMR1L=0x08;
                                         T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;

                             default:
                                 break;
                         }                    
                        break;
                case '8':           // 7.8-7.3

                            switch(levelofRedLed_LSB)
                                 {
                                 case '9':           // 7.8
                                 {
                                         TMR1H=0x8A;
                                         TMR1L=0xD0;
                                         T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                                 }
                                         break;
                                 case '8':           // 7.75
                                 {
                                         TMR1H=0x8A;
                                         TMR1L=0xD0;
                                         T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                                 }
                                         break;
                                 case '7':           // 7.7
                                 {
                                           TMR1H=0x8B;
                                         TMR1L=0x98;
                                         T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                                 }
                                         break;
                                 case '6':           // 7.65
                                 {
                                           TMR1H=0x8B;
                                         TMR1L=0x98;
                                         T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                                 }
                                         break;
                                 case '5':            // 7.6
                                 {
                                          TMR1H=0x8C;
                                         TMR1L=0x60;
                                         T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                                 }
                                         break;
                                 case '4':               // 7.55
                                 {
                                         TMR1H=0x8C;
                                         TMR1L=0x60;
                                         T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                                 }
                                                                          

                                         break;
                                 case '3':               // 7.5 
                                 {
                                         TMR1H=0x8D;
                                         TMR1L=0x28;
                                         T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                                 }
                                         break;
                                 case '2':            //7.45
                                 {
                                        TMR1H=0x8D;
                                         TMR1L=0x28;
                                         T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                                 }
                                         break;
                                 case '1':           //7.4
                                 {
                                         TMR1H=0x8D;
                                     TMR1L=0xF0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                                 }
                                         break;
                                 case '0':           // 7.35
                                 {
                                         TMR1H=0x8D;
                                     TMR1L=0xF0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                                 }
                                         break;

                                 default:
                                     break;
                                }
                        break;
                case '7':           // 7.3-
/**/
                        switch(levelofRedLed_LSB)
                             {
                             case '9':           // 7.3-6.85
                             {
                                    TMR1H=0x8E;
                                     TMR1L=0xB8;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '8':           // 7.25
                             {
                                     TMR1H=0x8E;
                                     TMR1L=0xB8;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '7':           // 7.20
                             {
                                      TMR1H=0x8F;
                                     TMR1L=0x80;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '6':           // 7.15
                             {
                                     TMR1H=0x8F;
                                     TMR1L=0x80;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 7.1
                             {
                                    TMR1H=0x90;
                                     TMR1L=0x48;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '4':               // 7.05
                             {
                                     TMR1H=0x90;
                                     TMR1L=0x48;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '3':               // 7.0   
                             {
                                    TMR1H=0x91;
                                     TMR1L=0x10;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '2':            //6.95
                             {
                                     TMR1H=0x91;
                                     TMR1L=0x10;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '1':           //6.9
                             {
                                    TMR1H=0x91;
                                     TMR1L=0xD8;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '0':           // 6.85
                             {
                                     TMR1H=0x91;
                                     TMR1L=0xD8;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;

                             default:
                                 break;
                         }                    
                        break;
                case '6':           // 6.8-5.9                
/**/
                        switch(levelofRedLed_LSB)
                             {
                             case '9':           // 6.8
                             {
                                     TMR1H=0x92;
                                     TMR1L=0xA0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '8':           // 6.7
                             {
                                    TMR1H=0x92;
                                     TMR1L=0xA0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '7':           // 6.6
                             {
                                    TMR1H=0x93;
                                     TMR1L=0x68;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '6':           // 6.5
                             {
                                     TMR1H=0x93;
                                     TMR1L=0x68;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 6.4
                             {
                                     TMR1H=0x94;
                                     TMR1L=0x30;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '4':               // 6.3
                             {
                                    TMR1H=0x94;
                                     TMR1L=0x30;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '3':               // 6.2  
                             {
                                     TMR1H=0x94;
                                     TMR1L=0xF8;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '2':            //6.1
                             {
                                     TMR1H=0x94;
                                     TMR1L=0xF8;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '1':           //6.0
                             {
                                    TMR1H=0x95;
                                     TMR1L=0xC0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '0':           // 5.9
                             {
                                  TMR1H=0x95;
                                     TMR1L=0xC0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             default:
                                     break;
                            }
                        break;
                case '5'://TX1REG='n';      // 5.8-4.9                    
/**/
                        switch(levelofRedLed_LSB)
                             {
                             case '9':           // 5.8
                             {
                                   TMR1H=0x97;
                                     TMR1L=0x50;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '8':           // 5.7
                             {
                                    TMR1H=0x97;
                                     TMR1L=0x50;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '7':           // 5.6
                             {
                                     TMR1H=0x98;
                                     TMR1L=0xE0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '6':           // 5.5
                             {
                                      TMR1H=0x98;
                                     TMR1L=0xE0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '5':           // 5.4
                             {
                                     TMR1H=0x9A;
                                     TMR1L=0x70;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '4':               // 5.3
                             {
                                    TMR1H=0x9A;
                                     TMR1L=0x70;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '3':               // 5.2   
                             {
                                    TMR1H=0x9C;
                                     TMR1L=0x00;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '2':              // 5.1
                             {
                                   TMR1H=0x9C;
                                     TMR1L=0x00;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '1':              // 5.0
                             {
                                     TMR1H=0x9D;
                                     TMR1L=0x90;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '0':              // 4.9
                             {
                                   TMR1H=0x9D;
                                     TMR1L=0x90;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             default:
                                     break;
                         }
                        break;
                case '4':               // 4.8-3.9
/**/
                        switch(levelofRedLed_LSB)
                             {
                             case '9':           // 4.8
                             {
                                     TMR1H=0x9F;
                                     TMR1L=0x20;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '8':           // 4.7
                             {
                                    TMR1H=0x9F;
                                     TMR1L=0x20;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '7':           // 4.6
                             {
                                     TMR1H=0xA0;
                                     TMR1L=0xB0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '6':           // 4.5
                             {
                                      TMR1H=0xA0;
                                     TMR1L=0xB0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 4.4
                             {
                                   TMR1H=0xA2;
                                     TMR1L=0x40;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '4':               // 4.3
                             {
                                    TMR1H=0xA2;
                                     TMR1L=0x40;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '3':               // 4.2   
                             {
                                   TMR1H=0xA3;
                                     TMR1L=0xD0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '2':            //4.1
                             {
                                    TMR1H=0xA3;
                                     TMR1L=0xD0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '1':           //4.0
                             {
                                     TMR1H=0xA5;
                                     TMR1L=0x60;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '0':           // 3.9
                             {
                                     TMR1H=0xA5;
                                     TMR1L=0x60;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;

                             default:
                                 break;
                            }                    
                        break;
                case '3':               // 3.8-2.9 
/**/
                        switch(levelofRedLed_LSB)
                             {
                             case '9':           // 3.8
                             {
                                    TMR1H=0xA6;
                                     TMR1L=0xF0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '8':           // 3.7
                             {
                                    TMR1H=0xA6;
                                     TMR1L=0xF0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '7':           // 3.6
                             {
                                      TMR1H=0xA8;
                                     TMR1L=0x80;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '6':           // 3.5
                             {
                                    TMR1H=0xA8;
                                     TMR1L=0x80;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 3.4
                             {
                                     TMR1H=0xAA;
                                     TMR1L=0x10;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '4':               // 3.3
                             {
                                     TMR1H=0xAA;
                                     TMR1L=0x10;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '3':               // 3.2   
                             {
                                      TMR1H=0xAB;
                                     TMR1L=0xA0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '2':            //3.1
                             {
                                       TMR1H=0xAB;
                                     TMR1L=0xA0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '1':           // 3.0
                             {
                                      TMR1H=0xAD;
                                     TMR1L=0x30;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '0':           // 2.9
                             {
                                     TMR1H=0xAD;
                                     TMR1L=0x30;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             default:
                                     break;
                            }                    
                        break;
                case '2':            //2.8-1.9
/**/
                        switch(levelofRedLed_LSB)
                             {
                             case '9':           // 2.8
                             {
                                      TMR1H=0xAE;
                                     TMR1L=0xC0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '8':           // 2.7
                             {
                                      TMR1H=0xAE;
                                     TMR1L=0xC0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '7':           // 2.6
                             {
                                     TMR1H=0xB0;
                                     TMR1L=0x50;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '6':           // 2.5
                             {
                                    TMR1H=0xB0;
                                     TMR1L=0x50;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 2.4
                             {
                                     TMR1H=0xB1;
                                     TMR1L=0xE0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '4':               // 2.3
                             {
                                    TMR1H=0xB1;
                                     TMR1L=0xE0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '3':               // 2.2  
                             {
                                    TMR1H=0xB3;
                                     TMR1L=0x70;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '2':            // 2.1
                             {
                                     TMR1H=0xB3;
                                     TMR1L=0x70;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '1':           // 2.0
                             {
                                     TMR1H=0xB5;
                                     TMR1L=0x00;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '0':           // 1.9
                             {
                                     TMR1H=0xB5;
                                     TMR1L=0x00;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             default:
                                     break;
                            }
                        break;
                case '1':           //1.8-1.2
/**/
                        switch(levelofRedLed_LSB)
                             {
                             case '9':           // 1.8
                             {
                                     TMR1H=0xB6;
                                     TMR1L=0x90;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '8':           // 1.75
                             {
                                    TMR1H=0xB6;
                                     TMR1L=0x90;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '7':           // 1.7
                             {
                                     TMR1H=0xB8;
                                     TMR1L=0x20;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '6':           // 1.65
                             {
                                    TMR1H=0xB8;
                                     TMR1L=0x20;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 1.6
                             {
                                    TMR1H=0xB9;
                                     TMR1L=0xB0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '4':               // 1.5
                             {
                                     TMR1H=0xB9;
                                     TMR1L=0xB0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '3':               // 1.4   
                             {
                                      TMR1H=0xBB;
                                     TMR1L=0x40;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '2':            //1.3
                             {
                                     TMR1H=0xBB;
                                     TMR1L=0x40;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     
                                     break;
                             case '1':           //1.25
                             {
                                    TMR1H=0xBC;
                                     TMR1L=0xD0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '0':           // 1.2
                             {
                                     TMR1H=0xBC;
                                     TMR1L=0xD0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             default:
                                     break;
                            }
                        break;
                case '0':           // 1.1-0.2
/**/
                        switch(levelofRedLed_LSB)
                             {
                             case '9':           // 1.1
                             {
                                     TMR1H=0xBE;
                                     TMR1L=0x60;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '8':           // 1.0
                             {
                                    TMR1H=0xBE;
                                     TMR1L=0x60;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '7':           // 0.9
                             {
                                      TMR1H=0xBF;
                                     TMR1L=0xF0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '6':           // 0.8
                             {
                                      TMR1H=0xBF;
                                     TMR1L=0xF0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 0.7
                             {
                                     TMR1H=0xC1;
                                     TMR1L=0x80;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '4':               // 0.6
                             {
                                     TMR1H=0xC1;
                                     TMR1L=0x80;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '3':               // 0.5    
                             {
                                     TMR1H=0xC3;
                                     TMR1L=0x10;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '2':            //0.4
                             {
									TMR1H=0xC3;
                                     TMR1L=0x10;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '1':           //0.3
                             {
//                                     TMR1H=0xFB;
//                                     TMR1L=0x50;
//                                    T1CONbits.TMR1ON = 1;
                                    OUTPUT_FOR_RED_LED=0;
//                                    PIE1bits.TMR1IE = 1;
//                                    PIR1bits.TMR1IF=0;
                             }
                                    
                                     break;
                             case '0':           // 0.2
                             {
//                                     TMR1H=0xFC;
//                                    TMR1L=0xE0;
//                                    T1CONbits.TMR1ON = 1;
                                      
                                     OUTPUT_FOR_RED_LED=0;
//                                    PIE1bits.TMR1IE = 1;
//                                    PIR1bits.TMR1IF=0;
                             }
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
       
    }//end of ccp9
    //**********************************green******************************//
         if(CCP1IF){
        if(CCP1IF == 1){
             CCP1IF=0;
         if(start_PWM_Generation_For_GreenLed == 1){
          switch(levelofGreenLed_MSB){
                case '9':           // 8.5
                    /**/
                        switch(levelofGreenLed_LSB)
                             {
                             case '9':           // 8.5
                             {
//                                    TMR3H=0x7B;
//                                    TMR3L=0x30;
                                    OUTPUT_FOR_GREEN_LED=1;
//                                    T3CONbits.TMR3ON = 1;
//                                    PIE3bits.TMR3IE = 1;
//                                    PIR3bits.TMR3IF=0;
                                    
                             }
                                     break;
                             case '8':           // 8.4
                             {
  										TMR3H=0x86;
                                         TMR3L=0xE8;
                                         T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                   
                                     break;
                             case '7':           // 8.35
                             {
                                      TMR3H=0x87;
                                         TMR3L=0xB0;
                                         T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '6':           // 8.25
                             {
                                      TMR3H=0x87;
                                         TMR3L=0xB0;
                                         T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '5':          // 8.15
                             {
                                     TMR3H=0x88;
                                         TMR3L=0x78;
                                         T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '4':               // 8.1
                             {
                                     TMR3H=0x88;
                                         TMR3L=0x78;
                                         T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '3':               // 8.0  
                             {
                                     TMR3H=0x89;
                                         TMR3L=0x40;
                                         T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '2':            //7.95
                             {
                                     TMR3H=0x89;
                                         TMR3L=0x40;
                                         T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '1':           //7.9
                             {
                                    TMR3H=0x8A;
                                         TMR3L=0x08;
                                         T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '0':           // 7.85
                             {
                                     TMR3H=0x8A;
                                         TMR3L=0x08;
                                         T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;

                             default:
                                 break;
                         }                    
                        break;
                case '8':           // 7.8-7.3

                            switch(levelofGreenLed_LSB)
                                 {
                                 case '9':           // 7.8
                                 {
                                         TMR3H=0x8A;
                                         TMR3L=0xD0;
                                         T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                                 }
                                         break;
                                 case '8':           // 7.75
                                 {
                                         TMR3H=0x8A;
                                         TMR3L=0xD0;
                                         T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                                 }
                                         break;
                                 case '7':           // 7.7
                                 {
                                           TMR3H=0x8B;
                                         TMR3L=0x98;
                                         T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                                 }
                                         break;
                                 case '6':           // 7.65
                                 {
                                           TMR3H=0x8B;
                                         TMR3L=0x98;
                                         T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                                 }
                                         break;
                                 case '5':            // 7.6
                                 {
                                          TMR3H=0x8C;
                                         TMR3L=0x60;
                                         T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                                 }
                                         break;
                                 case '4':               // 7.55
                                 {
                                         TMR3H=0x8C;
                                         TMR3L=0x60;
                                         T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                                 }
                                                                          

                                         break;
                                 case '3':               // 7.5 
                                 {
                                         TMR3H=0x8D;
                                         TMR3L=0x28;
                                         T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                                 }
                                         break;
                                 case '2':            //7.45
                                 {
                                        TMR3H=0x8D;
                                         TMR3L=0x28;
                                         T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                                 }
                                         break;
                                 case '1':           //7.4
                                 {
                                         TMR3H=0x8D;
                                     TMR3L=0xF0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                                 }
                                         break;
                                 case '0':           // 7.35
                                 {
                                         TMR3H=0x8D;
                                     TMR3L=0xF0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                                 }
                                         break;

                                 default:
                                     break;
                                }
                        break;
                case '7':           // 7.3-
/**/
                        switch(levelofGreenLed_LSB)
                             {
                             case '9':           // 7.3-6.85
                             {
                                    TMR3H=0x8E;
                                     TMR3L=0xB8;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '8':           // 7.25
                             {
                                     TMR3H=0x8E;
                                     TMR3L=0xB8;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '7':           // 7.20
                             {
                                      TMR3H=0x8F;
                                     TMR3L=0x80;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '6':           // 7.15
                             {
                                     TMR3H=0x8F;
                                     TMR3L=0x80;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 7.1
                             {
                                    TMR3H=0x90;
                                     TMR3L=0x48;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '4':               // 7.05
                             {
                                     TMR3H=0x90;
                                     TMR3L=0x48;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '3':               // 7.0   
                             {
                                    TMR3H=0x91;
                                     TMR3L=0x10;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '2':            //6.95
                             {
                                     TMR3H=0x91;
                                     TMR3L=0x10;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '1':           //6.9
                             {
                                    TMR3H=0x91;
                                     TMR3L=0xD8;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '0':           // 6.85
                             {
                                     TMR3H=0x91;
                                     TMR3L=0xD8;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;

                             default:
                                 break;
                         }                    
                        break;
                case '6':           // 6.8-5.9                
/**/
                        switch(levelofGreenLed_LSB)
                             {
                             case '9':           // 6.8
                             {
                                     TMR3H=0x92;
                                     TMR3L=0xA0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '8':           // 6.7
                             {
                                    TMR3H=0x92;
                                     TMR3L=0xA0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '7':           // 6.6
                             {
                                    TMR3H=0x93;
                                     TMR3L=0x68;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '6':           // 6.5
                             {
                                     TMR3H=0x93;
                                     TMR3L=0x68;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 6.4
                             {
                                     TMR3H=0x94;
                                     TMR3L=0x30;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '4':               // 6.3
                             {
                                    TMR3H=0x94;
                                     TMR3L=0x30;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '3':               // 6.2  
                             {
                                     TMR3H=0x94;
                                     TMR3L=0xF8;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '2':            //6.1
                             {
                                     TMR3H=0x94;
                                     TMR3L=0xF8;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '1':           //6.0
                             {
                                    TMR3H=0x95;
                                     TMR3L=0xC0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '0':           // 5.9
                             {
                                  TMR3H=0x95;
                                     TMR3L=0xC0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             default:
                                     break;
                            }
                        break;
                case '5'://TX1REG='n';      // 5.8-4.9                    
/**/
                        switch(levelofGreenLed_LSB)
                             {
                             case '9':           // 5.8
                             {
                                   TMR3H=0x97;
                                     TMR3L=0x50;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '8':           // 5.7
                             {
                                    TMR3H=0x97;
                                     TMR3L=0x50;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '7':           // 5.6
                             {
                                     TMR3H=0x98;
                                     TMR3L=0xE0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '6':           // 5.5
                             {
                                      TMR3H=0x98;
                                     TMR3L=0xE0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '5':           // 5.4
                             {
                                     TMR3H=0x9A;
                                     TMR3L=0x70;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '4':               // 5.3
                             {
                                    TMR3H=0x9A;
                                     TMR3L=0x70;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '3':               // 5.2   
                             {
                                    TMR3H=0x9C;
                                     TMR3L=0x00;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '2':              // 5.1
                             {
                                   TMR3H=0x9C;
                                     TMR3L=0x00;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '1':              // 5.0
                             {
                                     TMR3H=0x9D;
                                     TMR3L=0x90;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '0':              // 4.9
                             {
                                   TMR3H=0x9D;
                                     TMR3L=0x90;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             default:
                                     break;
                         }
                        break;
                case '4':               // 4.8-3.9
/**/
                        switch(levelofGreenLed_LSB)
                             {
                             case '9':           // 4.8
                             {
                                     TMR3H=0x9F;
                                     TMR3L=0x20;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '8':           // 4.7
                             {
                                    TMR3H=0x9F;
                                     TMR3L=0x20;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '7':           // 4.6
                             {
                                     TMR3H=0xA0;
                                     TMR3L=0xB0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '6':           // 4.5
                             {
                                      TMR3H=0xA0;
                                     TMR3L=0xB0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 4.4
                             {
                                   TMR3H=0xA2;
                                     TMR3L=0x40;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '4':               // 4.3
                             {
                                    TMR3H=0xA2;
                                     TMR3L=0x40;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '3':               // 4.2   
                             {
                                   TMR3H=0xA3;
                                     TMR3L=0xD0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '2':            //4.1
                             {
                                    TMR3H=0xA3;
                                     TMR3L=0xD0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '1':           //4.0
                             {
                                     TMR3H=0xA5;
                                     TMR3L=0x60;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '0':           // 3.9
                             {
                                     TMR3H=0xA5;
                                     TMR3L=0x60;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;

                             default:
                                 break;
                            }                    
                        break;
                case '3':               // 3.8-2.9 
/**/
                        switch(levelofGreenLed_LSB)
                             {
                             case '9':           // 3.8
                             {
                                    TMR3H=0xA6;
                                     TMR3L=0xF0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '8':           // 3.7
                             {
                                    TMR3H=0xA6;
                                     TMR3L=0xF0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '7':           // 3.6
                             {
                                      TMR3H=0xA8;
                                     TMR3L=0x80;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '6':           // 3.5
                             {
                                    TMR3H=0xA8;
                                     TMR3L=0x80;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 3.4
                             {
                                     TMR3H=0xAA;
                                     TMR3L=0x10;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '4':               // 3.3
                             {
                                     TMR3H=0xAA;
                                     TMR3L=0x10;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '3':               // 3.2   
                             {
                                      TMR3H=0xAB;
                                     TMR3L=0xA0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '2':            //3.1
                             {
                                       TMR3H=0xAB;
                                     TMR3L=0xA0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '1':           // 3.0
                             {
                                      TMR3H=0xAD;
                                     TMR3L=0x30;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '0':           // 2.9
                             {
                                     TMR3H=0xAD;
                                     TMR3L=0x30;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             default:
                                     break;
                            }                    
                        break;
                case '2':            //2.8-1.9
/**/
                        switch(levelofGreenLed_LSB)
                             {
                             case '9':           // 2.8
                             {
                                      TMR3H=0xAE;
                                     TMR3L=0xC0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '8':           // 2.7
                             {
                                      TMR3H=0xAE;
                                     TMR3L=0xC0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '7':           // 2.6
                             {
                                     TMR3H=0xB0;
                                     TMR3L=0x50;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '6':           // 2.5
                             {
                                    TMR3H=0xB0;
                                     TMR3L=0x50;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 2.4
                             {
                                     TMR3H=0xB1;
                                     TMR3L=0xE0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '4':               // 2.3
                             {
                                    TMR3H=0xB1;
                                     TMR3L=0xE0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '3':               // 2.2  
                             {
                                    TMR3H=0xB3;
                                     TMR3L=0x70;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '2':            // 2.1
                             {
                                     TMR3H=0xB3;
                                     TMR3L=0x70;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '1':           // 2.0
                             {
                                     TMR3H=0xB5;
                                     TMR3L=0x00;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '0':           // 1.9
                             {
                                     TMR3H=0xB5;
                                     TMR3L=0x00;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             default:
                                     break;
                            }
                        break;
                case '1':           //1.8-1.2
/**/
                        switch(levelofGreenLed_LSB)
                             {
                             case '9':           // 1.8
                             {
                                     TMR3H=0xB6;
                                     TMR3L=0x90;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '8':           // 1.75
                             {
                                    TMR3H=0xB6;
                                     TMR3L=0x90;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '7':           // 1.7
                             {
                                     TMR3H=0xB8;
                                     TMR3L=0x20;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '6':           // 1.65
                             {
                                    TMR3H=0xB8;
                                     TMR3L=0x20;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 1.6
                             {
                                    TMR3H=0xB9;
                                     TMR3L=0xB0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '4':               // 1.5
                             {
                                     TMR3H=0xB9;
                                     TMR3L=0xB0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '3':               // 1.4   
                             {
                                      TMR3H=0xBB;
                                     TMR3L=0x40;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '2':            //1.3
                             {
                                     TMR3H=0xBB;
                                     TMR3L=0x40;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     
                                     break;
                             case '1':           //1.25
                             {
                                    TMR3H=0xBC;
                                     TMR3L=0xD0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '0':           // 1.2
                             {
                                     TMR3H=0xBC;
                                     TMR3L=0xD0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             default:
                                     break;
                            }
                        break;
                case '0':           // 1.1-0.2
/**/
                        switch(levelofGreenLed_LSB)
                             {
                             case '9':           // 1.1
                             {
                                     TMR3H=0xBE;
                                     TMR3L=0x60;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '8':           // 1.0
                             {
                                    TMR3H=0xBE;
                                     TMR3L=0x60;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '7':           // 0.9
                             {
                                      TMR3H=0xBF;
                                     TMR3L=0xF0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '6':           // 0.8
                             {
                                      TMR3H=0xBF;
                                     TMR3L=0xF0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 0.7
                             {
                                     TMR3H=0xC1;
                                     TMR3L=0x80;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '4':               // 0.6
                             {
                                     TMR3H=0xC1;
                                     TMR3L=0x80;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '3':               // 0.5    
                             {
                                     TMR3H=0xC3;
                                     TMR3L=0x10;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '2':            //0.4
                             {
									TMR3H=0xC3;
                                     TMR3L=0x10;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '1':           //0.3
                             {
//                                     TMR3H=0xFB;
//                                     TMR3L=0x50;
//                                    T3CONbits.TMR3ON = 1;
                                    OUTPUT_FOR_GREEN_LED=0;
//                                    PIE3bits.TMR3IE = 1;
//                                    PIR3bits.TMR3IF=0;
                             }
                                    
                                     break;
                             case '0':           // 0.2
                             {
//                                     TMR3H=0xFC;
//                                    TMR3L=0xE0;
//                                    T3CONbits.TMR3ON = 1;
                                      
                                     OUTPUT_FOR_GREEN_LED=0;
//                                    PIE3bits.TMR3IE = 1;
//                                    PIR3bits.TMR3IF=0;
                             }
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
       
    }//end of ccp1
         if(CCP3IF){
        if(CCP3IF == 1){
             CCP3IF=0;
         if(start_PWM_Generation_For_BlueLed == 1){
          switch(levelofBlueLed_MSB){
                case '9':           // 8.5
                    /**/
                        switch(levelofBlueLed_LSB)
                             {
                             case '9':           // 8.5
                             {
//                                    TMR5H=0x7B;
//                                    TMR5L=0x30;
                                    OUTPUT_FOR_BLUE_LED=1;
//                                    T5CONbits.TMR5ON = 1;
//                                    PIE3bits.TMR5IE = 1;
//                                    PIR3bits.TMR5IF=0;
                                    
                             }
                                     break;
                             case '8':           // 8.4
                             {
  										TMR5H=0x86;
                                         TMR5L=0xE8;
                                         T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                   
                                     break;
                             case '7':           // 8.35
                             {
                                      TMR5H=0x87;
                                         TMR5L=0xB0;
                                         T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '6':           // 8.25
                             {
                                      TMR5H=0x87;
                                         TMR5L=0xB0;
                                         T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '5':          // 8.15
                             {
                                     TMR5H=0x88;
                                         TMR5L=0x78;
                                         T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '4':               // 8.1
                             {
                                     TMR5H=0x88;
                                         TMR5L=0x78;
                                         T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '3':               // 8.0  
                             {
                                     TMR5H=0x89;
                                         TMR5L=0x40;
                                         T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '2':            //7.95
                             {
                                     TMR5H=0x89;
                                         TMR5L=0x40;
                                         T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '1':           //7.9
                             {
                                    TMR5H=0x8A;
                                         TMR5L=0x08;
                                         T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '0':           // 7.85
                             {
                                     TMR5H=0x8A;
                                         TMR5L=0x08;
                                         T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;

                             default:
                                 break;
                         }                    
                        break;
                case '8':           // 7.8-7.3

                            switch(levelofBlueLed_LSB)
                                 {
                                 case '9':           // 7.8
                                 {
                                         TMR5H=0x8A;
                                         TMR5L=0xD0;
                                         T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                                 }
                                         break;
                                 case '8':           // 7.75
                                 {
                                         TMR5H=0x8A;
                                         TMR5L=0xD0;
                                         T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                                 }
                                         break;
                                 case '7':           // 7.7
                                 {
                                           TMR5H=0x8B;
                                         TMR5L=0x98;
                                         T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                                 }
                                         break;
                                 case '6':           // 7.65
                                 {
                                           TMR5H=0x8B;
                                         TMR5L=0x98;
                                         T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                                 }
                                         break;
                                 case '5':            // 7.6
                                 {
                                          TMR5H=0x8C;
                                         TMR5L=0x60;
                                         T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                                 }
                                         break;
                                 case '4':               // 7.55
                                 {
                                         TMR5H=0x8C;
                                         TMR5L=0x60;
                                         T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                                 }
                                                                          

                                         break;
                                 case '3':               // 7.5 
                                 {
                                         TMR5H=0x8D;
                                         TMR5L=0x28;
                                         T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                                 }
                                         break;
                                 case '2':            //7.45
                                 {
                                        TMR5H=0x8D;
                                         TMR5L=0x28;
                                         T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                                 }
                                         break;
                                 case '1':           //7.4
                                 {
                                         TMR5H=0x8D;
                                     TMR5L=0xF0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                                 }
                                         break;
                                 case '0':           // 7.35
                                 {
                                         TMR5H=0x8D;
                                     TMR5L=0xF0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                                 }
                                         break;

                                 default:
                                     break;
                                }
                        break;
                case '7':           // 7.3-
/**/
                        switch(levelofBlueLed_LSB)
                             {
                             case '9':           // 7.3-6.85
                             {
                                    TMR5H=0x8E;
                                     TMR5L=0xB8;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '8':           // 7.25
                             {
                                     TMR5H=0x8E;
                                     TMR5L=0xB8;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '7':           // 7.20
                             {
                                      TMR5H=0x8F;
                                     TMR5L=0x80;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '6':           // 7.15
                             {
                                     TMR5H=0x8F;
                                     TMR5L=0x80;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 7.1
                             {
                                    TMR5H=0x90;
                                     TMR5L=0x48;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '4':               // 7.05
                             {
                                     TMR5H=0x90;
                                     TMR5L=0x48;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '3':               // 7.0   
                             {
                                    TMR5H=0x91;
                                     TMR5L=0x10;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '2':            //6.95
                             {
                                     TMR5H=0x91;
                                     TMR5L=0x10;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '1':           //6.9
                             {
                                    TMR5H=0x91;
                                     TMR5L=0xD8;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '0':           // 6.85
                             {
                                     TMR5H=0x91;
                                     TMR5L=0xD8;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;

                             default:
                                 break;
                         }                    
                        break;
                case '6':           // 6.8-5.9                
/**/
                        switch(levelofBlueLed_LSB)
                             {
                             case '9':           // 6.8
                             {
                                     TMR5H=0x92;
                                     TMR5L=0xA0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '8':           // 6.7
                             {
                                    TMR5H=0x92;
                                     TMR5L=0xA0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '7':           // 6.6
                             {
                                    TMR5H=0x93;
                                     TMR5L=0x68;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '6':           // 6.5
                             {
                                     TMR5H=0x93;
                                     TMR5L=0x68;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 6.4
                             {
                                     TMR5H=0x94;
                                     TMR5L=0x30;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '4':               // 6.3
                             {
                                    TMR5H=0x94;
                                     TMR5L=0x30;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '3':               // 6.2  
                             {
                                     TMR5H=0x94;
                                     TMR5L=0xF8;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '2':            //6.1
                             {
                                     TMR5H=0x94;
                                     TMR5L=0xF8;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '1':           //6.0
                             {
                                    TMR5H=0x95;
                                     TMR5L=0xC0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '0':           // 5.9
                             {
                                  TMR5H=0x95;
                                     TMR5L=0xC0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             default:
                                     break;
                            }
                        break;
                case '5'://TX1REG='n';      // 5.8-4.9                    
/**/
                        switch(levelofBlueLed_LSB)
                             {
                             case '9':           // 5.8
                             {
                                   TMR5H=0x97;
                                     TMR5L=0x50;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '8':           // 5.7
                             {
                                    TMR5H=0x97;
                                     TMR5L=0x50;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '7':           // 5.6
                             {
                                     TMR5H=0x98;
                                     TMR5L=0xE0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '6':           // 5.5
                             {
                                      TMR5H=0x98;
                                     TMR5L=0xE0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '5':           // 5.4
                             {
                                     TMR5H=0x9A;
                                     TMR5L=0x70;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '4':               // 5.3
                             {
                                    TMR5H=0x9A;
                                     TMR5L=0x70;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '3':               // 5.2   
                             {
                                    TMR5H=0x9C;
                                     TMR5L=0x00;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '2':              // 5.1
                             {
                                   TMR5H=0x9C;
                                     TMR5L=0x00;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '1':              // 5.0
                             {
                                     TMR5H=0x9D;
                                     TMR5L=0x90;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '0':              // 4.9
                             {
                                   TMR5H=0x9D;
                                     TMR5L=0x90;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             default:
                                     break;
                         }
                        break;
                case '4':               // 4.8-3.9
/**/
                        switch(levelofBlueLed_LSB)
                             {
                             case '9':           // 4.8
                             {
                                     TMR5H=0x9F;
                                     TMR5L=0x20;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '8':           // 4.7
                             {
                                    TMR5H=0x9F;
                                     TMR5L=0x20;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '7':           // 4.6
                             {
                                     TMR5H=0xA0;
                                     TMR5L=0xB0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '6':           // 4.5
                             {
                                      TMR5H=0xA0;
                                     TMR5L=0xB0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 4.4
                             {
                                   TMR5H=0xA2;
                                     TMR5L=0x40;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '4':               // 4.3
                             {
                                    TMR5H=0xA2;
                                     TMR5L=0x40;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '3':               // 4.2   
                             {
                                   TMR5H=0xA3;
                                     TMR5L=0xD0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '2':            //4.1
                             {
                                    TMR5H=0xA3;
                                     TMR5L=0xD0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '1':           //4.0
                             {
                                     TMR5H=0xA5;
                                     TMR5L=0x60;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '0':           // 3.9
                             {
                                     TMR5H=0xA5;
                                     TMR5L=0x60;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;

                             default:
                                 break;
                            }                    
                        break;
                case '3':               // 3.8-2.9 
/**/
                        switch(levelofBlueLed_LSB)
                             {
                             case '9':           // 3.8
                             {
                                    TMR5H=0xA6;
                                     TMR5L=0xF0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '8':           // 3.7
                             {
                                    TMR5H=0xA6;
                                     TMR5L=0xF0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '7':           // 3.6
                             {
                                      TMR5H=0xA8;
                                     TMR5L=0x80;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '6':           // 3.5
                             {
                                    TMR5H=0xA8;
                                     TMR5L=0x80;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 3.4
                             {
                                     TMR5H=0xAA;
                                     TMR5L=0x10;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '4':               // 3.3
                             {
                                     TMR5H=0xAA;
                                     TMR5L=0x10;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '3':               // 3.2   
                             {
                                      TMR5H=0xAB;
                                     TMR5L=0xA0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '2':            //3.1
                             {
                                       TMR5H=0xAB;
                                     TMR5L=0xA0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '1':           // 3.0
                             {
                                      TMR5H=0xAD;
                                     TMR5L=0x30;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '0':           // 2.9
                             {
                                     TMR5H=0xAD;
                                     TMR5L=0x30;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             default:
                                     break;
                            }                    
                        break;
                case '2':            //2.8-1.9
/**/
                        switch(levelofBlueLed_LSB)
                             {
                             case '9':           // 2.8
                             {
                                      TMR5H=0xAE;
                                     TMR5L=0xC0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '8':           // 2.7
                             {
                                      TMR5H=0xAE;
                                     TMR5L=0xC0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '7':           // 2.6
                             {
                                     TMR5H=0xB0;
                                     TMR5L=0x50;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '6':           // 2.5
                             {
                                    TMR5H=0xB0;
                                     TMR5L=0x50;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 2.4
                             {
                                     TMR5H=0xB1;
                                     TMR5L=0xE0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '4':               // 2.3
                             {
                                    TMR5H=0xB1;
                                     TMR5L=0xE0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '3':               // 2.2  
                             {
                                    TMR5H=0xB3;
                                     TMR5L=0x70;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '2':            // 2.1
                             {
                                     TMR5H=0xB3;
                                     TMR5L=0x70;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '1':           // 2.0
                             {
                                     TMR5H=0xB5;
                                     TMR5L=0x00;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '0':           // 1.9
                             {
                                     TMR5H=0xB5;
                                     TMR5L=0x00;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             default:
                                     break;
                            }
                        break;
                case '1':           //1.8-1.2
/**/
                        switch(levelofBlueLed_LSB)
                             {
                             case '9':           // 1.8
                             {
                                     TMR5H=0xB6;
                                     TMR5L=0x90;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '8':           // 1.75
                             {
                                    TMR5H=0xB6;
                                     TMR5L=0x90;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '7':           // 1.7
                             {
                                     TMR5H=0xB8;
                                     TMR5L=0x20;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '6':           // 1.65
                             {
                                    TMR5H=0xB8;
                                     TMR5L=0x20;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 1.6
                             {
                                    TMR5H=0xB9;
                                     TMR5L=0xB0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '4':               // 1.5
                             {
                                     TMR5H=0xB9;
                                     TMR5L=0xB0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '3':               // 1.4   
                             {
                                      TMR5H=0xBB;
                                     TMR5L=0x40;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '2':            //1.3
                             {
                                     TMR5H=0xBB;
                                     TMR5L=0x40;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     
                                     break;
                             case '1':           //1.25
                             {
                                    TMR5H=0xBC;
                                     TMR5L=0xD0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '0':           // 1.2
                             {
                                     TMR5H=0xBC;
                                     TMR5L=0xD0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             default:
                                     break;
                            }
                        break;
                case '0':           // 1.1-0.2
/**/
                        switch(levelofBlueLed_LSB)
                             {
                             case '9':           // 1.1
                             {
                                     TMR5H=0xBE;
                                     TMR5L=0x60;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '8':           // 1.0
                             {
                                    TMR5H=0xBE;
                                     TMR5L=0x60;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '7':           // 0.9
                             {
                                      TMR5H=0xBF;
                                     TMR5L=0xF0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '6':           // 0.8
                             {
                                      TMR5H=0xBF;
                                     TMR5L=0xF0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 0.7
                             {
                                     TMR5H=0xC1;
                                     TMR5L=0x80;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '4':               // 0.6
                             {
                                     TMR5H=0xC1;
                                     TMR5L=0x80;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '3':               // 0.5    
                             {
                                     TMR5H=0xC3;
                                     TMR5L=0x10;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '2':            //0.4
                             {
									TMR5H=0xC3;
                                     TMR5L=0x10;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '1':           //0.3
                             {
//                                     TMR5H=0xFB;
//                                     TMR5L=0x50;
//                                    T5CONbits.TMR5ON = 1;
                                    OUTPUT_FOR_BLUE_LED=0;
//                                    PIE3bits.TMR5IE = 1;
//                                    PIR3bits.TMR5IF=0;
                             }
                                    
                                     break;
                             case '0':           // 0.2
                             {
//                                     TMR5H=0xFC;
//                                    TMR5L=0xE0;
//                                    T5CONbits.TMR5ON = 1;
                                      
                                     OUTPUT_FOR_BLUE_LED=0;
//                                    PIE3bits.TMR5IE = 1;
//                                    PIR3bits.TMR5IF=0;
                             }
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
       
    }//end of ccp3
    
   }//END OF CCP 
}
   




/*
 * Alfaone Main code starts here
 * For 4 switches 1 Dimmer
 */
int main() {
    __delay_ms(2000);
        M1=ON;    M2=ON;    M3=ON;    M4=ON;    M5=ON;
   //     OUTPUT_RELAY1 = OFF;  OUTPUT_RELAY2 = OFF;    OUTPUT_FOR_RED_LED = OFF;  OUTPUT_FOR_GREEN_LED = OFF;OUTPUT_FOR_BLUE_LED=OFF;

            GPIO_pin_Initialize();
            allPeripheralInit();
    
    while(1){       
        if(mainDataReceived==TRUE){
        //   errorsMain("MAIN");
            mainDataReceived=FALSE;
            if(copymainReceivedDataBuffer[0]=='S' && copymainReceivedDataBuffer[1]=='W' || (copymainReceivedDataBuffer[0]=='R' && copymainReceivedDataBuffer[1]=='G')){
          //     errorsMain("OK");
               ConvertmaindataReceiveIntoString = copymainReceivedDataBuffer;
               
           //    sendAcknowledgment(ConvertmaindataReceiveIntoString);
            //**************strat breaking the string**************//
              
           //  token = strtok(ConvertmaindataReceiveIntoString,delimiter);
               token = strtok(ConvertmaindataReceiveIntoString,".");
             ModuleNameString = token;
          //   sendAcknowledgment(ModuleNameString);//ok
             partCounter=0;
             if((strcmp(ModuleNameString,"RGB") == 0) || (strcmp(ModuleNameString,"SWW") == 0)){  
             //  errorsMain("KILO");//OK
             // errorsMain(ModuleName);//ok
               while(token != NULL)
                {
                    partCounter++;
                    token = strtok(NULL,".");                 
                    switch(partCounter){
                        case 1:
                        {   
                            int j=0;
                            for(j=0;j<2;j++)
                            {
                              switchNumberBuffer[j]= *token;
                              *token++;
                            }
                            SwitchNumberString = switchNumberBuffer;
                        //    sendAcknowledgment(SwitchNumberString);
                            
                        }break;
                        
                        case 2:
                        {
                            AcknowledgementString = token;
                         //  errorsMain(AcknowledgementString); //ok                     
   
                        }break;
                        case 3:
                        {
                            int k=0;
                            for(k=0;k<1;k++)
                            {
                              switchstatusbuffer[k]= *token;
                              *token++;
                            }
                            SwitchStatusString = switchstatusbuffer;
                           // sendAcknowledgment(SwitchStatusString);
                        
                           
                        } break;
                        case 4:
                        {
                            RedColorIntensityString = token;
                          //  errorsMain(RedColorIntensityString);//255
                           
                        } break;
                        case 5:
                        {
                            GreenColorIntensityString = token;
                        //    errorsMain(GreenColorIntensityString);//ok-255
                           
                        } break;
                        case 6:
                        {
                            BlueColorIntensityString = token;
                        //    errorsMain(BlueColorIntensityString);//ok-255
                            
                        }  break; 
                        case 7:
                        {
                            
                            int k=0;
                            for(k=0;k<1;k++)
                            {
                              ChildLockString[k]= *token;
                              *token++;
                            }
                            ChildLockString = ChildLockbuffer;
                            //sendAcknowledgment(ChildLockString);//ok
                           
                        } break;
                        case 8:
                        {
                            int k=0;
                            for(k=0;k<1;k++)
                            {
                              Finalframebitbuffer[k]= *token;
                              *token++;
                            }
                            FinalframebitString = Finalframebitbuffer;
                           // sendAcknowledgment(FinalframebitString);

                            
                        }break;
                        case 9:
                        {
                            StopBitString = token;
                        //    errorsMain(StopBitString);
                            
                        }break;
                        default:
                            break;
                         
                            
                    }
                }//end of while

             } 
             
                   applicationControl(ModuleNameString,SwitchNumberString,SwitchStatusString,
                 RedColorIntensityString,GreenColorIntensityString,BlueColorIntensityString,ChildLockString,FinalframebitString ); 
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
       int man = 1;
        if(copyparentalLockBuffer[1] == CHAR_OFF  && INPUTSWITCH1 == OFF && M1 == OFF)
        {
            if(man == 1)
            {
            
            OUTPUT_RELAY1=ON;
            }
            man=0;
            M1=1;
            
        }
        //on condition
        if(copyparentalLockBuffer[1] == CHAR_OFF && INPUTSWITCH1 == ON && M1 == ON)
        {
            //TX1REG='C';
            if(man==1)
            {
                
            OUTPUT_RELAY1=OFF;
            }
            man=0;
            M1=0;
        }
        
       // //check switch second status 
        //off condition
        if(copyparentalLockBuffer[2] == CHAR_OFF && INPUTSWITCH2 == OFF && M2 == OFF)
        {
            if(man==1)
            {
             
            OUTPUT_RELAY2=ON;
            }
            man=0;
            M2=1;
        }
        //on condtion
        if(copyparentalLockBuffer[2] == CHAR_OFF && INPUTSWITCH2 == ON && M2 == ON)
        {
            if(man==1)
            {
          
            OUTPUT_RELAY2=OFF;
            }
            man=0;
            M2=0;
        }
       
       
        //check switch third status 
        //off condition
        if(copyparentalLockBuffer[3] == CHAR_OFF && INPUTSWITCH3 == OFF && M3 == OFF)
        {
            if(man==1)
            {
             
            OUTPUT_RELAY3=ON;
            }
            man=0;
            M3=1;
        }
        //on condtion
        if(copyparentalLockBuffer[3] == CHAR_OFF && INPUTSWITCH3 == ON && M3 == ON)
        {
            if(man==1)
            {
            
            OUTPUT_RELAY3=OFF;
            }
            man=0;
            M3=0;
        }
       
       //check switch fourth status 
        //off condition
        if(copyparentalLockBuffer[4] == CHAR_OFF && INPUTSWITCH4 == OFF && M4 == OFF)
        {
            if(man==1)
            {
              
            OUTPUT_RELAY4=ON;
            }
            man=0;
            M4=1;
        }
        //on condtion
        if(copyparentalLockBuffer[4] == CHAR_OFF && INPUTSWITCH4 == ON && M4 == ON)
        {
            if(man==1)
            {
            
            OUTPUT_RELAY4=OFF;
            }
            man=0;
            M4=0;
        }
       
      //check switch Fifth status 
        //off condition
        if(copyparentalLockBuffer[5] == CHAR_OFF && INPUTSWITCH5 == OFF && M5 == OFF)
        {
            if(man==1)
            {
            
            OUTPUT_RELAY5=ON;
            }
            man=0;
            M5=1;
        }
        //on condtion
        if(copyparentalLockBuffer[5] == CHAR_OFF && INPUTSWITCH5 == ON && M5 == ON)
        {
            if(man==1)
            {
            
            OUTPUT_RELAY5=OFF;
            }
            man=0;
            M5=0;
        }
       
        //check switch Sixth status 
        //off condition
        if(copyparentalLockBuffer[6] == CHAR_OFF && INPUTSWITCH6 == OFF && M6 == OFF)
        {
            if(man==1)
            {
            
            OUTPUT_RELAY6=ON;
            }
            man=0;
            M6=1;
        }
        //on condtion
        if(copyparentalLockBuffer[6] == CHAR_OFF && INPUTSWITCH6 == ON && M6 == ON)
        {
            if(man==1)
            {
            
            OUTPUT_RELAY6=OFF;
            }
            man=0;
            M6=0;
        }
       //check switch Seventh status 
        //off condition
        if(copyparentalLockBuffer[7] == CHAR_OFF && INPUTSWITCH7 == OFF && M7 == OFF)
        {
            if(man==1)
            {
            
            OUTPUT_RELAY7=ON;
            }
            man=0;
            M7=1;
        }
        //on condtion
        if(copyparentalLockBuffer[7] == CHAR_OFF && INPUTSWITCH7 == ON && M7 == ON)
        {
            if(man==1)
            {
            
            OUTPUT_RELAY7=OFF;
            }
            man=0;
            M7=0;
        }
       
       //check switch Eigth status 
        //off condition
        if(copyparentalLockBuffer[8] == CHAR_OFF && INPUTSWITCH8 == OFF && M8 == OFF)
        {
            if(man==1)
            {
           
            OUTPUT_RELAY8=ON;
            }
            man=0;
            M8=1;
        }
        //on condtion
        if(copyparentalLockBuffer[8] == CHAR_OFF && INPUTSWITCH8 == ON && M8 == ON)
        {
            if(man==1)
            {
            
            OUTPUT_RELAY8=OFF;
            }
            man=0;
            M8=0;
        }
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
    
    OUTPUT_RELAY_DIR_1 = 0;
    OUTPUT_RELAY_DIR_2 = 0;
    OUTPUT_RELAY_DIR_3 = 0;
    OUTPUT_RELAY_DIR_4 = 0;
    OUTPUT_RELAY_DIR_5 = 0;
    OUTPUT_RELAY_DIR_6 = 0;
    OUTPUT_RELAY_DIR_7 = 0;
    OUTPUT_RELAY_DIR_8 = 0;
    OUTPUT_RELAY_RED_LED_DIR = 0;
    OUTPUT_RELAY_GREEN_LED_DIR = 0;
    OUTPUT_RELAY_BLUE_LED_DIR = 0; 
    
    // peripherals directions
    ZCD_CCP9_DIR = 1;
    ZCD_CCP1_DIR = 1;
    ZCD_CCP3_DIR = 1;
    
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
    TMR3_Initialize();
    TMR5_Initialize();
    TMR2_Initialize();
    TMR4_Initialize();
    TMR6_Initialize();
    CCP9_Initialize();
    CCP3_Initialize();
    CCP1_Initialize();

}
void Send_Acknowlde_To_RedPWM(char charlevelofRedLed_LSB)
{
    levelofRedLed_LSB = charlevelofRedLed_LSB;
    levelofRedLed_MSB = '0';
}
void Send_Acknowlde_To_BluePWM(char charlevelofBlueLed_LSB)
{
    levelofBlueLed_LSB = charlevelofBlueLed_LSB;
    levelofBlueLed_MSB = '0';
}
void Send_Acknowlde_To_GreenPWM(char charlevelofGreenLed_LSB)
{
    levelofBlueLed_LSB = charlevelofGreenLed_LSB;
    levelofBlueLed_MSB = '0';
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
void CCP1_Initialize()
{
    // Set the CCP1 to the options selected in the User Interface

    // MODE Every edge; EN enabled; FMT right_aligned;
    CCP1CON = 0x04;
 //   CCP1CON = 0x84;

    // RH 0;
    CCPR1H = 0x00;

    // RL 0;
    CCPR1L = 0x00;

    // Clear the CCP1 interrupt flag
    PIR1bits.CCP1IF = 0;

    // Enable the CCP1 interrupt
    PIE1bits.CCP1IE = 1;
   GIE = 1;

    // Enables all active peripheral interrupts -----> INTCON reg .... bit 6         page 105
    PEIE = 1;
}
void CCP9_Initialize(){
    // Set the CCP1 to the options selected in the User Interface

    // MODE Every edge; EN enabled; FMT right_aligned;
    CCP9CON = 0x04;

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
    
      GIE = 1;

    // Enables all active peripheral interrupts -----> INTCON reg .... bit 6         page 105
    PEIE = 1;
}

void CCP3_Initialize(void)
{
    // Set the CCP3 to the options selected in the User Interface

    // MODE Every edge; EN enabled; FMT right_aligned;
    CCP3CON = 0x04;    

    // CCPR3L 0; 
    CCPR3L = 0x00;    

    // CCPR3H 0; 
    CCPR3H = 0x00;    
    
    // Selecting Timer 3
//    CCPTMRS0bits.C3TSEL = 0x1;

    // Clear the CCP3 interrupt flag
    PIR3bits.CCP3IF = 0;

    // Enable the CCP3 interrupt
    PIE3bits.CCP3IE = 1;
      GIE = 1;

    // Enables all active peripheral interrupts -----> INTCON reg .... bit 6         page 105
    PEIE = 1;
}

void peripheralsEnable(){
    // Transmit Enabled
    TX1STAbits.TXEN = 1;

    // Serial Port Enabled
    RC1STAbits.SPEN = 1;
}
void AllInterruptEnable(){
    // Enable all active interrupts ---> INTCON reg .... bit 7            page 105
    GIE = 1;

    // Enables all active peripheral interrupts -----> INTCON reg .... bit 6         page 105
    PEIE = 1;
    
    // enable receive interrupt
    PIE1bits.RC1IE = 1;                    // handled into INTERRUPT_Initialize()

}

void errorsISR(char* errNum){
    int Tx_count=0;
  	while(Tx_count!=4)
 	{ 
        while (!TX1STAbits.TRMT);
 		TX1REG = *errNum;
 		*errNum++;
        Tx_count++;
 	}
}
void errorsMain(char* errNum){
   int Tx_count=0;
  	while(Tx_count!=4)
 	{ 
        while (!TX1STAbits.TRMT);
 		TX1REG = *errNum;
 		*errNum++;
        Tx_count++;
 	}
}
void sendAcknowledgment(char *currentStateBuffer){
  int Tx_count=0;
  //	while(Tx_count!=29)
  while(*currentStateBuffer != NULL)
 	{ 
        while (!TX1STAbits.TRMT);
//        TX1REG='S';
 		TX1REG = *currentStateBuffer;
 		*currentStateBuffer++;
        Tx_count++;
 	}
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
    OUTPUT_RELAY1=0;
    OUTPUT_RELAY2=0;
    OUTPUT_RELAY3=0;
    OUTPUT_RELAY4=0;
    OUTPUT_RELAY5=0;
    OUTPUT_RELAY6=0;
    OUTPUT_RELAY7=0;
    OUTPUT_FOR_RED_LED=0;
    OUTPUT_FOR_GREEN_LED=0;
    OUTPUT_FOR_BLUE_LED=0;
    
}
