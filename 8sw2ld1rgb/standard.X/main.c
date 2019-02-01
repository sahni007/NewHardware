/* 
 * File:   varun_4_1.c
 * Author: VARUNS SAHNI
 *New hardware
 * Created on 8 April, 2018, 8:40 PM
 * this is is used in AI digest
 * this is test code of single rgb on new hardware 
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include<pic16f1526.h>
#include<string.h>
#include <math.h>

// PIC16F1526 Configuration Bit Settings

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

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


// Since we have used 16 MHz crystal
#define _XTAL_FREQ 16000000  

// Pin MACROS
#define OUTPUT_RELAY1 PORTEbits.RE0
#define OUTPUT_RELAY2 PORTEbits.RE1
#define OUTPUT_FOR_RED_LED PORTDbits.RD4 ///pwm for red
#define OUTPUT_FOR_GREEN_LED PORTDbits.RD3 //pwm for green
#define OUTPUT_FOR_BLUE_LED PORTDbits.RD2   // PWM for blue

#define OUTPUT_RELAY_DIR_1 TRISEbits.TRISE0
#define OUTPUT_RELAY_DIR_2 TRISEbits.TRISE1
//output pwm for rgb
#define OUTPUT_RELAY_RED_LED_DIR TRISDbits.TRISD4
#define OUTPUT_RELAY_GREEN_LED_DIR TRISDbits.TRISD3
#define OUTPUT_RELAY_BLUE_LED_DIR TRISDbits.TRISD2        // direction of PWM OUTPUT to MOC3021

#define INPUTSWITCH1 PORTDbits.RD6
#define INPUTSWITCH2 PORTDbits.RD7
#define INPUTSWITCH3 PORTBbits.RB0
#define INPUTSWITCH4 PORTBbits.RB1
#define INPUTSWITCH5 PORTBbits.RB2
#define INPUT_SWITCH_DIR_1 TRISDbits.TRISD6
#define INPUT_SWITCH_DIR_2 TRISDbits.TRISD7
#define INPUT_SWITCH_DIR_3 TRISBbits.TRISB0
#define INPUT_SWITCH_DIR_4 TRISBbits.TRISB1
#define INPUT_SWITCH_DIR_5 TRISBbits.TRISB2

//ccp for rgb
#define ZCD_CCP7_DIR TRISEbits.TRISE5
#define ZCD_CCP8_DIR TRISEbits.TRISE4
#define ZCD_CCP9_DIR TRISEbits.TRISE3



#define OUTPUT_TRANMIT_INDICATION_LED PORTFbits.RF0 //green
#define OUTPUT_RECEIVE_INDICATION_LED PORTFbits.RF1 //blue led
#define USER_RECEIVE_INDICATION_LED PORTFbits.RF2 //blue led

#define OUTPUT_TRANMIT_INDICATION_LED_DIR TRISFbits.TRISF0
#define OUTPUT_RECEIVE_INDICATION_LED_DIR TRISFbits.TRISF1
#define USER_RECEIVE_INDICATION_LED_DIR TRISFbits.TRISF2

        // direction of PWM OUTPUT to MOC3021

/*
 * Extra Periferals Direction and PORT
 */
//#define ZCD_CCP9_DIR TRISEbits.TRISE3
// USART Directions
#define USART_1_TRANSMIT_OUTPUT_DIR TRISCbits.TRISC6
#define USART_1_RECIEVE_INPUT_DIR TRISCbits.TRISC7

#define RECIEVED_DATA_LENGTH (30+2)
#define TOTAL_NUMBER_OF_SWITCH (5+2)

#define TRUE 1
#define FALSE 0



// Conditional compilation
//#define DEBUG
//#define RELEASE
#define SWITCH_1_RELAY
//#define SWITCH_1_DIMMER

#define SWITCH_2_RELAY
//#define SWITCH_2_DIMMER

#define SWITCH_3_RELAY
//#define SWITCH_3_DIMMER

#define SWITCH_4_RELAY


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

//#define DEBUG
#define ON 1
#define OFF 0
#define CHAR_OFF '0'
#define CHAR_ON '1'
#define GIVES_OUTPUT_ZERO_IF_EVEN 1
//#define USEMANUAL_SWITCH1_REDLIGHT_DIMMABLE
//#define USEMANUAL_SWITCH2_GREENLIGHT_DIMMABLE
//#define USEMANUAL_SWITCH3_BLUELIGHT_DIMMABLE
#define USEMANUAL_SWITCH1_REDLIGHT_SWITCH 
#define USEMANUAL_SWITCH1_GREENLIGHT_SWITCH
#define USEMANUAL_SWITCH1_BLUELIGHT_SWITCH
/* DATA USED IN MANUAL END HERE*/

unsigned char ErrorNames[5]="####";
unsigned int FrameLegthCounter =0;
int mainReceivedDataPosition=0, mainDataReceived=FALSE;
int mainReceivedDataFlag=0;
int mainDatacopyPosition=0;
unsigned char mainReceivedDataBuffer[RECIEVED_DATA_LENGTH]="#"; 
unsigned char copymainReceivedDataBuffer[RECIEVED_DATA_LENGTH]="#";
unsigned char tempReceivedDataBuffer[RECIEVED_DATA_LENGTH-8]="#";
unsigned char parentalLockBuffer[6]="00000";
//unsigned char copyparentalLockBuffer[6]="00000";
unsigned char currentStateBuffer[(TOTAL_NUMBER_OF_SWITCH*4)+2]="#";
int start_PWM_Generation_For_RedLed=FALSE;
int start_PWM_Generation_For_GreenLed = FALSE;
int start_PWM_Generation_For_BlueLed = FALSE;
char levelofBlueLed_MSB = '0',levelofBlueLed_LSB = '0';
char levelofRedLed_MSB = '0',levelofRedLed_LSB = '0';
char levelofGreenLed_MSB = '0',levelofGreenLed_LSB = '0';

        
        unsigned char ConvertIntegertoStringRedColorBuffer[REDCOLOINTENSITYLENGTH] = NULL;
        unsigned char ConvertIntegertoStringGreenColorBuffer[GREENCOLORINTENSITYLENGHT] = NULL;
        unsigned char ConvertIntegertoStringBlueColorBuffer[BLUECOLOINTENSITYLENGHT] = NULL;
        
 char *ConvertmaindataReceiveIntoString = NULL;
 int mainDataReceiveStringLength=0;
 int RedColorIntensityStringLength =0;
 int GreenColorIntensityStringLength=0;
 int AllColorIntensityStringLength=0;
 int BlueColorIntensityStringLength =0;

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
char *AllColorIntensityString = NULL;
char *ChildLockString = NULL;
char ChildLockbuffer[2]=NULL;
char *FinalframebitString = NULL;
char Finalframebitbuffer[2] = NULL;
char *StopBitString=NULL;
char StopBitStringbufer=NULL;

int RedLedSwitchCounter=0;
int GreenLedSwitchCounter =0 ;
int BlueLedSwitchCounter =0;
        int IntegerSwitchNumber=0;
        int IntergerSwitchStatus = 0;
        unsigned int IntergerRedColorIntensity = 0;
        unsigned int IntegerGreenColorIntensity = 0;
        unsigned int IntegerBlueColorIntensity = 0;
        unsigned int IntegerAllColorIntensity = 0;
        int IntegerChildLock = 0;
        char charchildLock = '0';
        char charSwitchState = '0';


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
void TransmissionIndicationLedBlinking();
void ReceivingIndicationLedBlinking();
void TMR5_Initialize();
void TMR2_Initialize();
void TMR4_Initialize();
void TMR6_Initialize();
void TMR3_Initialize();
void TMR1_Initialize();
void CCP9_Initialize();
void CCP7_Initialize();
void CCP8_Initialize();
void InternalOscInit();
void allPeripheralInit();




void applicationControl(char *ModuleName,char *SwitchNumber,char *SwitchStatus,
                char  *RedColorIntensity,char *GreenColorIntensity,char *BlueColorIntensity,char *ChildLock,char *AllColorStringlength);
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
            else if((copymainReceivedDataBuffer[0]=='R') && copymainReceivedDataBuffer[0] != NULL){
                            mainReceivedDataPosition++;
                            FrameLegthCounter++;
                            
            }
        

        else{
          //   TX1REG = copymainReceivedDataBuffer[mainReceivedDataPosition];
            RC1STAbits.CREN = 0; // countinuous Recieve Disable
            RC1STAbits.CREN = 1; // countinuous Recieve Enable
            mainReceivedDataPosition=0; // Reinitiate buffer counter
            errorsISR("ERRX");           
        }
    }// End of RC1IF
    
    
    
    //**************RED COLOR**************************//
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
    if(PIR4bits.CCP8IF==1 || PIR4bits.CCP7IF == 1 || PIR4bits.CCP9IF==1){    
   if(CCP9IF){
        if(CCP9IF == 1){
             CCP9IF=0;
         if(start_PWM_Generation_For_RedLed == 1){
          //   errorsISR("ISR");
          switch(levelofRedLed_MSB){
                case '9':{           // 8.5
                    /**/
                        switch(levelofRedLed_LSB)
                             {
                             case '9':           // 8.5
                             {
 //                                    TMR1H=0x7B;
  //     /                             TMR1L=0x30;
                                    OUTPUT_FOR_RED_LED=1;
  //                                 T1CONbits.TMR1ON = 1;
  ///                                  PIE1bits.TMR1IE = 1;
   //                                 PIR1bits.TMR1IF=0;
                                    
                             }
                                     break;
                             case '8':           // 8.4
                             {
    //                                 TMR1H=0x7C;
    ////                                 TMR1L=0xC0;
    //                                     T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
     //                               PIE1bits.TMR1IE = 1;
      //                              PIR1bits.TMR1IF=0;
                             }
                                   
                                     break;
                             case '7':           // 8.35
                             {
      //                               TMR1H=0x7D;
      //                               TMR1L=0x88;
      //                                   T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
        //                            PIE1bits.TMR1IE = 1;
        //                            PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '6':           // 8.25
                             {
          ///                           TMR1H=0x7F;
          //                           TMR1L=0x18;
          //                               T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
           //                         PIE1bits.TMR1IE = 1;
            //                        PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '5':          // 8.15
                             {
                                     TMR1H=0x80;
                                     TMR1L=0xA8;
                                         T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '4':               // 8.1
                             {
                                     TMR1H=0x81;
                                     TMR1L=0x70;
                                         T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '3':               // 8.0  
                             {
                                     TMR1H=0x83;
                                     TMR1L=0x00;
                                         T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '2':            //7.95
                             {
                                     TMR1H=0x83;
                                     TMR1L=0xC8;
                                         T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '1':           //7.9
                             {
                                     TMR1H=0x87;
                                     TMR1L=0xB0;
                                         T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '0':           // 7.85
                             {
                                     TMR1H=0x85;
                                     TMR1L=0x58;
                                         T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;

                             default:
                                 break;
                         }
//                                   T1CONbits.TMR1ON = 1;
//                                   OUTPUT_FOR_RED_LED=1;
//                                    PIE1bits.TMR1IE = 1;
//                                    PIR1bits.TMR1IF=0;
                    }
                        break;
                case '8':{           // 7.8-7.3

                            switch(levelofRedLed_LSB)
                                 {
                                 case '9':           // 7.8
                                 {
                                         TMR1H=0x86;
                                         TMR1L=0x20;
                                         T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                                 }
                                         break;
                                 case '8':           // 7.75
                                 {
                                         TMR1H=0x86;
                                         TMR1L=0xE8;
                                         T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                                 }
                                         break;
                                 case '7':           // 7.7
                                 {
                                         TMR1H=0x87;
                                         TMR1L=0xB0;
                                         T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                                 }
                                         break;
                                 case '6':           // 7.65
                                 {
                                         TMR1H=0x88;
                                         TMR1L=0x78;
                                         T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                                 }
                                         break;
                                 case '5':            // 7.6
                                 {
                                         TMR1H=0x89;
                                         TMR1L=0x40;
                                         T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                                 }
                                         break;
                                 case '4':               // 7.55
                                 {
                                         TMR1H=0x8A;
                                         TMR1L=0x08;
                                         T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                                 }
                                                                          

                                         break;
                                 case '3':               // 7.5 
                                 {
                                         TMR1H=0x8A;
                                         TMR1L=0xD0;
                                         T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                                 }
                                         break;
                                 case '2':            //7.45
                                 {
                                          TMR1H=0x8B;
                                         TMR1L=0x98;
                                         T1CONbits.TMR1ON = 1;
                                         OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                                 }
                                         break;
                                 case '1':           //7.4
                                 {
                                         TMR1H=0x8C;
                                         TMR1L=0x60;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                                 }
                                         break;
                                 case '0':           // 7.35
                                 {
                                        TMR1H=0x8D;
                                         TMR1L=0x28;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                                 }
                                         break;

                                 default:
                                     break;
                                }
//                                     T1CONbits.TMR1ON = 1;
//                                     OUTPUT_FOR_RED_LED=1;
//                                    PIE1bits.TMR1IE = 1;
//                                    PIR1bits.TMR1IF=0;
                           }
                        break;
                case '7':{           // 7.3-
/**/
                        switch(levelofRedLed_LSB)
                             {
                             case '9':           // 7.3-6.85
                             {
                                     TMR1H=0x8D;
                                     TMR1L=0xF0;
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
                                   TMR1H=0x90;
                                     TMR1L=0x48;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 7.1
                             {
                                     TMR1H=0x91;
                                     TMR1L=0x10;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '4':               // 7.05
                             {
                                     TMR1H=0x91;
                                     TMR1L=0xD8;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '3':               // 7.0   
                             {
                                     TMR1H=0x92;
                                     TMR1L=0xA0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '2':            //6.95
                             {
                                    TMR1H=0x93;
                                     TMR1L=0x68;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '1':           //6.9
                             {
                                      TMR1H=0x94;
                                     TMR1L=0x30;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '0':           // 6.85
                             {
                                     TMR1H=0x94;
                                     TMR1L=0xF8;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;

                             default:
                                 break;
                         }
//                                    T1CONbits.TMR1ON = 1;
//                                    OUTPUT_FOR_RED_LED=1;
//                                    PIE1bits.TMR1IE = 1;
//                                    PIR1bits.TMR1IF=0;
                     }
                        break;
                case '6':{           // 6.8-5.9                
/**/
                        switch(levelofRedLed_LSB)
                             {
                             case '9':           // 6.8
                             {
                                     TMR1H=0x95;
                                     TMR1L=0xC0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '8':           // 6.7
                             {
                                     TMR1H=0x97;
                                     TMR1L=0x50;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '7':           // 6.6
                             {
                                     TMR1H=0x98;
                                     TMR1L=0xE0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '6':           // 6.5
                             {
                                     TMR1H=0x9A;
                                     TMR1L=0x70;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 6.4
                             {
                                     TMR1H=0x9C;
                                     TMR1L=0x00;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '4':               // 6.3
                             {
                                     TMR1H=0x9D;
                                     TMR1L=0x90;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '3':               // 6.2  
                             {
                                     TMR1H=0x9F;
                                     TMR1L=0x20;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '2':            //6.1
                             {
                                     TMR1H=0xA0;
                                     TMR1L=0xB0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '1':           //6.0
                             {
                                      TMR1H=0xA2;
                                     TMR1L=0x40;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '0':           // 5.9
                             {
                                     TMR1H=0xA3;
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
//                                    T1CONbits.TMR1ON = 1;
//                                    OUTPUT_FOR_RED_LED=1;
//                                    PIE1bits.TMR1IE = 1;
//                                    PIR1bits.TMR1IF=0;
                        }
                        break;
                case '5':{//TX1REG='n';      // 5.8-4.9                    
/**/
                        switch(levelofRedLed_LSB)
                             {
                             case '9':           // 5.8
                             {
                                     TMR1H=0xA5;
                                     TMR1L=0x60;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '8':           // 5.7
                             {
                                     TMR1H=0xA6;
                                     TMR1L=0xF0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '7':           // 5.6
                             {
                                     TMR1H=0xA8;
                                     TMR1L=0x80;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '6':           // 5.5
                             {
                                     TMR1H=0xAA;
                                     TMR1L=0x10;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '5':           // 5.4
                             {
                                     TMR1H=0xAB;
                                     TMR1L=0xA0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '4':               // 5.3
                             {
                                     TMR1H=0xAD;
                                     TMR1L=0x30;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '3':               // 5.2   
                             {
                                     TMR1H=0xAE;
                                     TMR1L=0xC0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '2':              // 5.1
                             {
                                     TMR1H=0xB0;
                                     TMR1L=0x50;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '1':              // 5.0
                             {
                                     TMR1H=0xB1;
                                     TMR1L=0xE0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '0':              // 4.9
                             {
                                     TMR1H=0xB3;
                                     TMR1L=0x70;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             default:
                                     break;
                         }
//                                   T1CONbits.TMR1ON = 1;
//                                    OUTPUT_FOR_RED_LED=1;
//                                    PIE1bits.TMR1IE = 1;
//                                    PIR1bits.TMR1IF=0;
                      }
                        break;
                case '4':{               // 4.8-3.9
/**/
                        switch(levelofRedLed_LSB)
                             {
                             case '9':           // 4.8
                             {
                                     TMR1H=0xB5;
                                     TMR1L=0x00;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '8':           // 4.7
                             {
                                     TMR1H=0xB6;
                                     TMR1L=0x90;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '7':           // 4.6
                             {
                                     TMR1H=0xB8;
                                     TMR1L=0x20;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '6':           // 4.5
                             {
                                     TMR1H=0xB8;
                                     TMR1L=0x20;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 4.4
                             {
                                     TMR1H=0xBB;
                                     TMR1L=0x40;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '4':               // 4.3
                             {
                                       TMR1H=0xBC;
                                     TMR1L=0xD0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '3':               // 4.2   
                             {
                                     TMR1H=0xBE;
                                     TMR1L=0x60;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '2':            //4.1
                             {
                                     TMR1H=0xBF;
                                     TMR1L=0xF0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '1':           //4.0
                             {
                                     TMR1H=0xC1;
                                     TMR1L=0x80;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '0':           // 3.9
                             {
                                    TMR1H=0xC3;
                                     TMR1L=0x10;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;

                             default:
                                 break;
                            } 
//                                   T1CONbits.TMR1ON = 1;
//                                    OUTPUT_FOR_RED_LED=1;
//                                    PIE1bits.TMR1IE = 1;
//                                    PIR1bits.TMR1IF=0;
                        }
                        break;
                case '3':{               // 3.8-2.9 
/**/
                        switch(levelofRedLed_LSB)
                             {
                             case '9':           // 3.8
                             {
                                    TMR1H=0xC4;
                                     TMR1L=0xA0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '8':           // 3.7
                             {
                                     TMR1H=0xC6;
                                     TMR1L=0x30;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '7':           // 3.6
                             {
                                    TMR1H=0xC7;
                                     TMR1L=0xC0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '6':           // 3.5
                             {
                                      TMR1H=0xC9;
                                     TMR1L=0x50;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 3.4
                             {
                                    TMR1H=0xCA;
                                     TMR1L=0xE0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '4':               // 3.3
                             {
                                    TMR1H=0xCA;
                                     TMR1L=0xE0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '3':               // 3.2   
                             {
                                     TMR1H=0xCE;
                                     TMR1L=0x00;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '2':            //3.1
                             {
                                     TMR1H=0xCF;
                                     TMR1L=0x90;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '1':           // 3.0
                             {
                                     TMR1H=0xD1;
                                     TMR1L=0x20;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '0':           // 2.9
                             {
                                     TMR1H=0xD2;
                                     TMR1L=0xB0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             default:
                                     break;
                            } 
//                                    T1CONbits.TMR1ON = 1;
//                                    OUTPUT_FOR_RED_LED=1;
//                                    PIE1bits.TMR1IE = 1;
//                                    PIR1bits.TMR1IF=0;
                        }
                        break;
                case '2':{            //2.8-1.9
/**/
                        switch(levelofRedLed_LSB)
                             {
                             case '9':           // 2.8
                             {
                                      TMR1H=0xD4;
                                     TMR1L=0x40;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '8':           // 2.7
                             {
                                     TMR1H=0xD5;
                                     TMR1L=0xD0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '7':           // 2.6
                             {
                                     TMR1H=0xD7;
                                     TMR1L=0x60;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '6':           // 2.5
                             {
                                     TMR1H=0xD8;
                                     TMR1L=0xF0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 2.4
                             {
                                     TMR1H=0xDA;
                                     TMR1L=0x80;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '4':               // 2.3
                             {
                                     TMR1H=0xDC;
                                     TMR1L=0x10;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '3':               // 2.2  
                             {
                                     TMR1H=0xDD;
                                     TMR1L=0xA0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '2':            // 2.1
                             {
                                      TMR1H=0xDF;
                                     TMR1L=0x30;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '1':           // 2.0
                             {
                                     TMR1H=0xE0;
                                     TMR1L=0xC0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '0':           // 1.9
                             {
                                        TMR1H=0xE2;
                                     TMR1L=0x50;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             default:
                                     break;
                            }
//                                     T1CONbits.TMR1ON = 1;
//                                    OUTPUT_FOR_RED_LED=1;
//                                    PIE1bits.TMR1IE = 1;
//                                    PIR1bits.TMR1IF=0;
                        }
                        break;
                case '1':{           //1.8-1.2
/**/
                        switch(levelofRedLed_LSB)
                             {
                             case '9':           // 1.8
                             {
                                                                    								 TMR1H=0xE3;
                                     TMR1L=0xE0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '8':           // 1.75
                             {
                                     TMR1H=0xE4;
                                     TMR1L=0xA8;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '7':           // 1.7
                             {
                                    TMR1H=0xE5;
                                     TMR1L=0x70;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '6':           // 1.65
                             {
                                     TMR1H=0xE6;
                                     TMR1L=0x38;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 1.6
                             {
                                     TMR1H=0xE7;
                                     TMR1L=0x00;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '4':               // 1.5
                             {
                                     TMR1H=0xE8;
                                     TMR1L=0x90;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '3':               // 1.4   
                             {
                                     TMR1H=0xEA;
                                     TMR1L=0x20;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '2':            //1.3
                             {
                                     TMR1H=0xEB;
                                     TMR1L=0xB0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     
                                     break;
                             case '1':           //1.25
                             {
                                     TMR1H=0xEC;
                                     TMR1L=0x78;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '0':           // 1.2
                             {
                                     TMR1H=0xED;
                                     TMR1L=0x40;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             default:
                                     break;
                            }
//                                    T1CONbits.TMR1ON = 1;
//                                    OUTPUT_FOR_RED_LED=1;
//                                    PIE1bits.TMR1IE = 1;
//                                    PIR1bits.TMR1IF=0;
                }
                        break;
                case '0': {          // 1.1-0.2
/**/
                        switch(levelofRedLed_LSB)
                             {
                             case '9':           // 1.1
                             {
                                     TMR1H=0xEE;
                                     TMR1L=0xD0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '8':           // 1.0
                             {
                                     TMR1H=0xF0;
                                     TMR1L=0x60;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '7':           // 0.9
                             {
                                     TMR1H=0xF1;
                                     TMR1L=0xF0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '6':           // 0.8
                             {
                                     TMR1H=0xF3;
                                     TMR1L=0x80;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 0.7
                             {
                                     TMR1H=0xF5;
                                     TMR1L=0x10;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '4':               // 0.6
                             {
                                          TMR1H=0xF6;
                                     TMR1L=0xA0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '3':               // 0.5    
                             {
                                     TMR1H=0xF8;
                                     TMR1L=0x30;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '2':            //0.4
                             {
                                  TMR1H=0xF9;
                                     TMR1L=0xC0;
                                     T1CONbits.TMR1ON = 1;
                                     OUTPUT_FOR_RED_LED=1;
                                    PIE1bits.TMR1IE = 1;
                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                             case '1':           //0.3
                             {
 //                                   TMR1H=0xFB;
 //                                    TMR1L=0x50;
//                                    T1CONbits.TMR1ON = 1;
                                    OUTPUT_FOR_RED_LED=0;
//                                    PIE1bits.TMR1IE = 1;
//                                    PIR1bits.TMR1IF=0;
                             }
                                    
                                     break;
                             case '0':           // 0.2
                             {
 //                                    TMR1H=0xFC;
   //                                 TMR1L=0xE0;
//                                    T1CONbits.TMR1ON = 1;
                                      
                                     OUTPUT_FOR_RED_LED=0;
//                                    PIE1bits.TMR1IE = 1;
//                                    PIR1bits.TMR1IF=0;
                             }
                                     break;
                                     default:
                                     break;
                            

                }
//                                    T1CONbits.TMR1ON = 1;
//                                    OUTPUT_FOR_RED_LED=1;
//                                    PIE1bits.TMR1IE = 1;
//                                    PIR1bits.TMR1IF=0;
                }
                        break;
                default:
                        break;
             
          }
        }
        }
    }//end of ccp9
    //**********************************green******************************//
          if(CCP8IF){
        if(CCP8IF == 1){
             CCP8IF=0;
         if(start_PWM_Generation_For_GreenLed == 1){
          switch(levelofGreenLed_MSB){
                case '9':{           // 8.5
                    /**/
                        switch(levelofGreenLed_LSB)
                             {
                             case '9':           // 8.5
                             {
          //                           TMR3H=0x7B;
           //                         TMR3L=0x30;
                                    OUTPUT_FOR_GREEN_LED=1;
           //                        T3CONbits.TMR3ON = 1;
            //                        PIE3bits.TMR3IE = 1;
            //                        PIR3bits.TMR3IF=0;
                                    
                             }
                                     break;
                             case '8':           // 8.4
                             {
             /////                        TMR3H=0x7C;
             //                        TMR3L=0xC0;
                                         T3CONbits.TMR3ON = 1;
              //                           OUTPUT_FOR_GREEN_LED=1;
              //                      PIE3bits.TMR3IE = 1;
              //                      PIR3bits.TMR3IF=0;
                             }
                                   
                                     break;
                             case '7':           // 8.35
                             {
               ///                      TMR3H=0x7D;
               //                      TMR3L=0x88;
                                         T3CONbits.TMR3ON = 1;
                //                         OUTPUT_FOR_GREEN_LED=1;
                //                    PIE3bits.TMR3IE = 1;
                //                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '6':           // 8.25
                             {
                  //                   TMR3H=0x7F;
                  //                   TMR3L=0x18;
                  //                       T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                  //                  PIE3bits.TMR3IE = 1;
                  //                  PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '5':          // 8.15
                             {
                                     TMR3H=0x80;
                                     TMR3L=0xA8;
                                         T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '4':               // 8.1
                             {
                                     TMR3H=0x81;
                                     TMR3L=0x70;
                                         T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '3':               // 8.0  
                             {
                                     TMR3H=0x83;
                                     TMR3L=0x00;
                                         T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '2':            //7.95
                             {
                                     TMR3H=0x83;
                                     TMR3L=0xC8;
                                         T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '1':           //7.9
                             {
                                     TMR3H=0x87;
                                     TMR3L=0xB0;
                                         T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '0':           // 7.85
                             {
                                     TMR3H=0x85;
                                     TMR3L=0x58;
                                         T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;

                             default:
                                 break;
                         }
//                                   T3CONbits.TMR3ON = 1;
//                                   OUTPUT_FOR_GREEN_LED=1;
//                                    PIE3bits.TMR3IE = 1;
//                                    PIR3bits.TMR3IF=0;
                    }
                        break;
                case '8':{           // 7.8-7.3

                            switch(levelofGreenLed_LSB)
                                 {
                                 case '9':           // 7.8
                                 {
                                         TMR3H=0x86;
                                         TMR3L=0x20;
                                         T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                                 }
                                         break;
                                 case '8':           // 7.75
                                 {
                                         TMR3H=0x86;
                                         TMR3L=0xE8;
                                         T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                                 }
                                         break;
                                 case '7':           // 7.7
                                 {
                                         TMR3H=0x87;
                                         TMR3L=0xB0;
                                         T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                                 }
                                         break;
                                 case '6':           // 7.65
                                 {
                                         TMR3H=0x88;
                                         TMR3L=0x78;
                                         T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                                 }
                                         break;
                                 case '5':            // 7.6
                                 {
                                         TMR3H=0x89;
                                         TMR3L=0x40;
                                         T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                                 }
                                         break;
                                 case '4':               // 7.55
                                 {
                                         TMR3H=0x8A;
                                         TMR3L=0x08;
                                         T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                                 }
                                                                          

                                         break;
                                 case '3':               // 7.5 
                                 {
                                         TMR3H=0x8A;
                                         TMR3L=0xD0;
                                         T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                                 }
                                         break;
                                 case '2':            //7.45
                                 {
                                          TMR3H=0x8B;
                                         TMR3L=0x98;
                                         T3CONbits.TMR3ON = 1;
                                         OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                                 }
                                         break;
                                 case '1':           //7.4
                                 {
                                         TMR3H=0x8C;
                                         TMR3L=0x60;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                                 }
                                         break;
                                 case '0':           // 7.35
                                 {
                                        TMR3H=0x8D;
                                         TMR3L=0x28;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                                 }
                                         break;

                                 default:
                                     break;
                                }
//                                     T3CONbits.TMR3ON = 1;
//                                     OUTPUT_FOR_GREEN_LED=1;
//                                    PIE3bits.TMR3IE = 1;
//                                    PIR3bits.TMR3IF=0;
                           }
                        break;
                case '7':{           // 7.3-
/**/
                        switch(levelofGreenLed_LSB)
                             {
                             case '9':           // 7.3-6.85
                             {
                                     TMR3H=0x8D;
                                     TMR3L=0xF0;
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
                                   TMR3H=0x90;
                                     TMR3L=0x48;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 7.1
                             {
                                     TMR3H=0x91;
                                     TMR3L=0x10;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '4':               // 7.05
                             {
                                     TMR3H=0x91;
                                     TMR3L=0xD8;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '3':               // 7.0   
                             {
                                     TMR3H=0x92;
                                     TMR3L=0xA0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '2':            //6.95
                             {
                                    TMR3H=0x93;
                                     TMR3L=0x68;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '1':           //6.9
                             {
                                      TMR3H=0x94;
                                     TMR3L=0x30;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '0':           // 6.85
                             {
                                     TMR3H=0x94;
                                     TMR3L=0xF8;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;

                             default:
                                 break;
                         }
//                                    T3CONbits.TMR3ON = 1;
//                                    OUTPUT_FOR_GREEN_LED=1;
//                                    PIE3bits.TMR3IE = 1;
//                                    PIR3bits.TMR3IF=0;
                     }
                        break;
                case '6':{           // 6.8-5.9                
/**/
                        switch(levelofGreenLed_LSB)
                             {
                             case '9':           // 6.8
                             {
                                     TMR3H=0x95;
                                     TMR3L=0xC0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '8':           // 6.7
                             {
                                     TMR3H=0x97;
                                     TMR3L=0x50;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '7':           // 6.6
                             {
                                     TMR3H=0x98;
                                     TMR3L=0xE0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '6':           // 6.5
                             {
                                     TMR3H=0x9A;
                                     TMR3L=0x70;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 6.4
                             {
                                     TMR3H=0x9C;
                                     TMR3L=0x00;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '4':               // 6.3
                             {
                                     TMR3H=0x9D;
                                     TMR3L=0x90;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '3':               // 6.2  
                             {
                                     TMR3H=0x9F;
                                     TMR3L=0x20;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '2':            //6.1
                             {
                                     TMR3H=0xA0;
                                     TMR3L=0xB0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '1':           //6.0
                             {
                                      TMR3H=0xA2;
                                     TMR3L=0x40;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '0':           // 5.9
                             {
                                     TMR3H=0xA3;
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
//                                    T3CONbits.TMR3ON = 1;
//                                    OUTPUT_FOR_GREEN_LED=1;
//                                    PIE3bits.TMR3IE = 1;
//                                    PIR3bits.TMR3IF=0;
                        }
                        break;
                case '5':{//TX1REG='n';      // 5.8-4.9                    
/**/
                        switch(levelofGreenLed_LSB)
                             {
                             case '9':           // 5.8
                             {
                                     TMR3H=0xA5;
                                     TMR3L=0x60;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '8':           // 5.7
                             {
                                     TMR3H=0xA6;
                                     TMR3L=0xF0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '7':           // 5.6
                             {
                                     TMR3H=0xA8;
                                     TMR3L=0x80;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '6':           // 5.5
                             {
                                     TMR3H=0xAA;
                                     TMR3L=0x10;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '5':           // 5.4
                             {
                                     TMR3H=0xAB;
                                     TMR3L=0xA0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '4':               // 5.3
                             {
                                     TMR3H=0xAD;
                                     TMR3L=0x30;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '3':               // 5.2   
                             {
                                     TMR3H=0xAE;
                                     TMR3L=0xC0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '2':              // 5.1
                             {
                                     TMR3H=0xB0;
                                     TMR3L=0x50;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '1':              // 5.0
                             {
                                     TMR3H=0xB1;
                                     TMR3L=0xE0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '0':              // 4.9
                             {
                                     TMR3H=0xB3;
                                     TMR3L=0x70;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             default:
                                     break;
                         }
//                                   T3CONbits.TMR3ON = 1;
//                                    OUTPUT_FOR_GREEN_LED=1;
//                                    PIE3bits.TMR3IE = 1;
//                                    PIR3bits.TMR3IF=0;
                      }
                        break;
                case '4':{               // 4.8-3.9
/**/
                        switch(levelofGreenLed_LSB)
                             {
                             case '9':           // 4.8
                             {
                                     TMR3H=0xB5;
                                     TMR3L=0x00;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '8':           // 4.7
                             {
                                     TMR3H=0xB6;
                                     TMR3L=0x90;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '7':           // 4.6
                             {
                                     TMR3H=0xB8;
                                     TMR3L=0x20;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '6':           // 4.5
                             {
                                     TMR3H=0xB8;
                                     TMR3L=0x20;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 4.4
                             {
                                     TMR3H=0xBB;
                                     TMR3L=0x40;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '4':               // 4.3
                             {
                                       TMR3H=0xBC;
                                     TMR3L=0xD0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '3':               // 4.2   
                             {
                                     TMR3H=0xBE;
                                     TMR3L=0x60;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '2':            //4.1
                             {
                                     TMR3H=0xBF;
                                     TMR3L=0xF0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '1':           //4.0
                             {
                                     TMR3H=0xC1;
                                     TMR3L=0x80;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '0':           // 3.9
                             {
                                    TMR3H=0xC3;
                                     TMR3L=0x10;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;

                             default:
                                 break;
                            } 
//                                   T3CONbits.TMR3ON = 1;
//                                    OUTPUT_FOR_GREEN_LED=1;
//                                    PIE3bits.TMR3IE = 1;
//                                    PIR3bits.TMR3IF=0;
                        }
                        break;
                case '3':{               // 3.8-2.9 
/**/
                        switch(levelofGreenLed_LSB)
                             {
                             case '9':           // 3.8
                             {
                                    TMR3H=0xC4;
                                     TMR3L=0xA0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '8':           // 3.7
                             {
                                     TMR3H=0xC6;
                                     TMR3L=0x30;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '7':           // 3.6
                             {
                                    TMR3H=0xC7;
                                     TMR3L=0xC0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '6':           // 3.5
                             {
                                      TMR3H=0xC9;
                                     TMR3L=0x50;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 3.4
                             {
                                    TMR3H=0xCA;
                                     TMR3L=0xE0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '4':               // 3.3
                             {
                                    TMR3H=0xCA;
                                     TMR3L=0xE0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '3':               // 3.2   
                             {
                                     TMR3H=0xCE;
                                     TMR3L=0x00;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '2':            //3.1
                             {
                                     TMR3H=0xCF;
                                     TMR3L=0x90;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '1':           // 3.0
                             {
                                     TMR3H=0xD1;
                                     TMR3L=0x20;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '0':           // 2.9
                             {
                                     TMR3H=0xD2;
                                     TMR3L=0xB0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             default:
                                     break;
                            } 
//                                    T3CONbits.TMR3ON = 1;
//                                    OUTPUT_FOR_GREEN_LED=1;
//                                    PIE3bits.TMR3IE = 1;
//                                    PIR3bits.TMR3IF=0;
                        }
                        break;
                case '2':{            //2.8-1.9
/**/
                        switch(levelofGreenLed_LSB)
                             {
                             case '9':           // 2.8
                             {
                                      TMR3H=0xD4;
                                     TMR3L=0x40;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '8':           // 2.7
                             {
                                     TMR3H=0xD5;
                                     TMR3L=0xD0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '7':           // 2.6
                             {
                                     TMR3H=0xD7;
                                     TMR3L=0x60;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '6':           // 2.5
                             {
                                     TMR3H=0xD8;
                                     TMR3L=0xF0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 2.4
                             {
                                     TMR3H=0xDA;
                                     TMR3L=0x80;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '4':               // 2.3
                             {
                                     TMR3H=0xDC;
                                     TMR3L=0x10;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '3':               // 2.2  
                             {
                                     TMR3H=0xDD;
                                     TMR3L=0xA0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '2':            // 2.1
                             {
                                      TMR3H=0xDF;
                                     TMR3L=0x30;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '1':           // 2.0
                             {
                                     TMR3H=0xE0;
                                     TMR3L=0xC0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '0':           // 1.9
                             {
                                        TMR3H=0xE2;
                                     TMR3L=0x50;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             default:
                                     break;
                            }
//                                     T3CONbits.TMR3ON = 1;
//                                    OUTPUT_FOR_GREEN_LED=1;
//                                    PIE3bits.TMR3IE = 1;
//                                    PIR3bits.TMR3IF=0;
                        }
                        break;
                case '1':{           //1.8-1.2
/**/
                        switch(levelofGreenLed_LSB)
                             {
                             case '9':           // 1.8
                             {
                                                                    								 TMR3H=0xE3;
                                     TMR3L=0xE0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '8':           // 1.75
                             {
                                     TMR3H=0xE4;
                                     TMR3L=0xA8;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '7':           // 1.7
                             {
                                    TMR3H=0xE5;
                                     TMR3L=0x70;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '6':           // 1.65
                             {
                                     TMR3H=0xE6;
                                     TMR3L=0x38;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 1.6
                             {
                                     TMR3H=0xE7;
                                     TMR3L=0x00;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '4':               // 1.5
                             {
                                     TMR3H=0xE8;
                                     TMR3L=0x90;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '3':               // 1.4   
                             {
                                     TMR3H=0xEA;
                                     TMR3L=0x20;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '2':            //1.3
                             {
                                     TMR3H=0xEB;
                                     TMR3L=0xB0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     
                                     break;
                             case '1':           //1.25
                             {
                                     TMR3H=0xEC;
                                     TMR3L=0x78;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '0':           // 1.2
                             {
                                     TMR3H=0xED;
                                     TMR3L=0x40;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             default:
                                     break;
                            }
//                                    T3CONbits.TMR3ON = 1;
//                                    OUTPUT_FOR_GREEN_LED=1;
//                                    PIE3bits.TMR3IE = 1;
//                                    PIR3bits.TMR3IF=0;
                }
                        break;
                case '0': {          // 1.1-0.2
/**/
                        switch(levelofGreenLed_LSB)
                             {
                             case '9':           // 1.1
                             {
                                     TMR3H=0xEE;
                                     TMR3L=0xD0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '8':           // 1.0
                             {
                                     TMR3H=0xF0;
                                     TMR3L=0x60;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '7':           // 0.9
                             {
                                     TMR3H=0xF1;
                                     TMR3L=0xF0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '6':           // 0.8
                             {
                                     TMR3H=0xF3;
                                     TMR3L=0x80;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 0.7
                             {
                                     TMR3H=0xF5;
                                     TMR3L=0x10;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '4':               // 0.6
                             {
                                          TMR3H=0xF6;
                                     TMR3L=0xA0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '3':               // 0.5    
                             {
                                     TMR3H=0xF8;
                                     TMR3L=0x30;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '2':            //0.4
                             {
                                  TMR3H=0xF9;
                                     TMR3L=0xC0;
                                     T3CONbits.TMR3ON = 1;
                                     OUTPUT_FOR_GREEN_LED=1;
                                    PIE3bits.TMR3IE = 1;
                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                             case '1':           //0.3
                             {
 //                                   TMR3H=0xFB;
  //                                   TMR3L=0x50;
//                                    T3CONbits.TMR3ON = 1;
                                    OUTPUT_FOR_GREEN_LED=0;
//                                    PIE3bits.TMR3IE = 1;
//                                    PIR3bits.TMR3IF=0;
                             }
                                    
                                     break;
                             case '0':           // 0.2
                             {
 //                                    TMR3H=0xFC;
 //                                  TMR3L=0xE0;
//                                    T3CONbits.TMR3ON = 1;
                                      
                                     OUTPUT_FOR_GREEN_LED=0;
//                                    PIE3bits.TMR3IE = 1;
//                                    PIR3bits.TMR3IF=0;
                             }
                                     break;
                                     default:
                                     break;
                            

                }
//                                    T3CONbits.TMR3ON = 1;
//                                    OUTPUT_FOR_GREEN_LED=1;
//                                    PIE3bits.TMR3IE = 1;
//                                    PIR3bits.TMR3IF=0;
                }
                        break;
                default:
                        break;
             
          }
        }
        }
    }//end of ccp1
      if(CCP7IF){
        if(CCP7IF == 1){
             CCP7IF=0;
         if(start_PWM_Generation_For_BlueLed == 1){
          switch(levelofBlueLed_MSB){
                case '9':{           // 8.5
                    /**/
                        switch(levelofBlueLed_LSB)
                             {
                             case '9':           // 8.5
                             {
               //                      TMR5H=0x7B;
               //                     TMR5L=0x30;
                                    OUTPUT_FOR_BLUE_LED=1;
                //                   T5CONbits.TMR5ON = 1;
                 //                   PIE3bits.TMR5IE = 1;
                 //                   PIR3bits.TMR5IF=0;
                                    
                             }
                                     break;
                             case '8':           // 8.4
                             {
               //                      TMR5H=0x7C;
               //                      TMR5L=0xC0;
               //                          T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
               /////                     PIE3bits.TMR5IE = 1;
                 //                   PIR3bits.TMR5IF=0;
                             }
                                   
                                     break;
                             case '7':           // 8.35
                             {
                  //                   TMR5H=0x7D;
                  //                   TMR5L=0x88;
                  //                       T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                  //                  PIE3bits.TMR5IE = 1;
                  //                  PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '6':           // 8.25
                             {
                     //                TMR5H=0x7F;
                     //                TMR5L=0x18;
                      //                   T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                      //              PIE3bits.TMR5IE = 1;
                      //              PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '5':          // 8.15
                             {
                                     TMR5H=0x80;
                                     TMR5L=0xA8;
                                         T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '4':               // 8.1
                             {
                                     TMR5H=0x81;
                                     TMR5L=0x70;
                                         T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '3':               // 8.0  
                             {
                                     TMR5H=0x83;
                                     TMR5L=0x00;
                                         T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '2':            //7.95
                             {
                                     TMR5H=0x83;
                                     TMR5L=0xC8;
                                         T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '1':           //7.9
                             {
                                     TMR5H=0x87;
                                     TMR5L=0xB0;
                                         T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '0':           // 7.85
                             {
                                     TMR5H=0x85;
                                     TMR5L=0x58;
                                         T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;

                             default:
                                 break;
                         }
//                                   T5CONbits.TMR5ON = 1;
//                                   OUTPUT_FOR_BLUE_LED=1;
//                                    PIE3bits.TMR5IE = 1;
//                                    PIR3bits.TMR5IF=0;
                    }
                        break;
                case '8':{           // 7.8-7.3

                            switch(levelofBlueLed_LSB)
                                 {
                                 case '9':           // 7.8
                                 {
                                         TMR5H=0x86;
                                         TMR5L=0x20;
                                         T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                                 }
                                         break;
                                 case '8':           // 7.75
                                 {
                                         TMR5H=0x86;
                                         TMR5L=0xE8;
                                         T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                                 }
                                         break;
                                 case '7':           // 7.7
                                 {
                                         TMR5H=0x87;
                                         TMR5L=0xB0;
                                         T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                                 }
                                         break;
                                 case '6':           // 7.65
                                 {
                                         TMR5H=0x88;
                                         TMR5L=0x78;
                                         T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                                 }
                                         break;
                                 case '5':            // 7.6
                                 {
                                         TMR5H=0x89;
                                         TMR5L=0x40;
                                         T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                                 }
                                         break;
                                 case '4':               // 7.55
                                 {
                                         TMR5H=0x8A;
                                         TMR5L=0x08;
                                         T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                                 }
                                                                          

                                         break;
                                 case '3':               // 7.5 
                                 {
                                         TMR5H=0x8A;
                                         TMR5L=0xD0;
                                         T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                                 }
                                         break;
                                 case '2':            //7.45
                                 {
                                          TMR5H=0x8B;
                                         TMR5L=0x98;
                                         T5CONbits.TMR5ON = 1;
                                         OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                                 }
                                         break;
                                 case '1':           //7.4
                                 {
                                         TMR5H=0x8C;
                                         TMR5L=0x60;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                                 }
                                         break;
                                 case '0':           // 7.35
                                 {
                                        TMR5H=0x8D;
                                         TMR5L=0x28;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                                 }
                                         break;

                                 default:
                                     break;
                                }
//                                     T5CONbits.TMR5ON = 1;
//                                     OUTPUT_FOR_BLUE_LED=1;
//                                    PIE3bits.TMR5IE = 1;
//                                    PIR3bits.TMR5IF=0;
                           }
                        break;
                case '7':{           // 7.3-
/**/
                        switch(levelofBlueLed_LSB)
                             {
                             case '9':           // 7.3-6.85
                             {
                                     TMR5H=0x8D;
                                     TMR5L=0xF0;
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
                                   TMR5H=0x90;
                                     TMR5L=0x48;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 7.1
                             {
                                     TMR5H=0x91;
                                     TMR5L=0x10;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '4':               // 7.05
                             {
                                     TMR5H=0x91;
                                     TMR5L=0xD8;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '3':               // 7.0   
                             {
                                     TMR5H=0x92;
                                     TMR5L=0xA0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '2':            //6.95
                             {
                                    TMR5H=0x93;
                                     TMR5L=0x68;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '1':           //6.9
                             {
                                      TMR5H=0x94;
                                     TMR5L=0x30;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '0':           // 6.85
                             {
                                     TMR5H=0x94;
                                     TMR5L=0xF8;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;

                             default:
                                 break;
                         }
//                                    T5CONbits.TMR5ON = 1;
//                                    OUTPUT_FOR_BLUE_LED=1;
//                                    PIE3bits.TMR5IE = 1;
//                                    PIR3bits.TMR5IF=0;
                     }
                        break;
                case '6':{           // 6.8-5.9                
/**/
                        switch(levelofBlueLed_LSB)
                             {
                             case '9':           // 6.8
                             {
                                     TMR5H=0x95;
                                     TMR5L=0xC0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '8':           // 6.7
                             {
                                     TMR5H=0x97;
                                     TMR5L=0x50;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '7':           // 6.6
                             {
                                     TMR5H=0x98;
                                     TMR5L=0xE0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '6':           // 6.5
                             {
                                     TMR5H=0x9A;
                                     TMR5L=0x70;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 6.4
                             {
                                     TMR5H=0x9C;
                                     TMR5L=0x00;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '4':               // 6.3
                             {
                                     TMR5H=0x9D;
                                     TMR5L=0x90;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '3':               // 6.2  
                             {
                                     TMR5H=0x9F;
                                     TMR5L=0x20;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '2':            //6.1
                             {
                                     TMR5H=0xA0;
                                     TMR5L=0xB0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '1':           //6.0
                             {
                                      TMR5H=0xA2;
                                     TMR5L=0x40;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '0':           // 5.9
                             {
                                     TMR5H=0xA3;
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
//                                    T5CONbits.TMR5ON = 1;
//                                    OUTPUT_FOR_BLUE_LED=1;
//                                    PIE3bits.TMR5IE = 1;
//                                    PIR3bits.TMR5IF=0;
                        }
                        break;
                case '5':{//TX1REG='n';      // 5.8-4.9                    
/**/
                        switch(levelofBlueLed_LSB)
                             {
                             case '9':           // 5.8
                             {
                                     TMR5H=0xA5;
                                     TMR5L=0x60;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '8':           // 5.7
                             {
                                     TMR5H=0xA6;
                                     TMR5L=0xF0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '7':           // 5.6
                             {
                                     TMR5H=0xA8;
                                     TMR5L=0x80;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '6':           // 5.5
                             {
                                     TMR5H=0xAA;
                                     TMR5L=0x10;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '5':           // 5.4
                             {
                                     TMR5H=0xAB;
                                     TMR5L=0xA0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '4':               // 5.3
                             {
                                     TMR5H=0xAD;
                                     TMR5L=0x30;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '3':               // 5.2   
                             {
                                     TMR5H=0xAE;
                                     TMR5L=0xC0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '2':              // 5.1
                             {
                                     TMR5H=0xB0;
                                     TMR5L=0x50;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '1':              // 5.0
                             {
                                     TMR5H=0xB1;
                                     TMR5L=0xE0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '0':              // 4.9
                             {
                                     TMR5H=0xB3;
                                     TMR5L=0x70;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             default:
                                     break;
                         }
//                                   T5CONbits.TMR5ON = 1;
//                                    OUTPUT_FOR_BLUE_LED=1;
//                                    PIE3bits.TMR5IE = 1;
//                                    PIR3bits.TMR5IF=0;
                      }
                        break;
                case '4':{               // 4.8-3.9
/**/
                        switch(levelofBlueLed_LSB)
                             {
                             case '9':           // 4.8
                             {
                                     TMR5H=0xB5;
                                     TMR5L=0x00;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '8':           // 4.7
                             {
                                     TMR5H=0xB6;
                                     TMR5L=0x90;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '7':           // 4.6
                             {
                                     TMR5H=0xB8;
                                     TMR5L=0x20;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '6':           // 4.5
                             {
                                     TMR5H=0xB8;
                                     TMR5L=0x20;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 4.4
                             {
                                     TMR5H=0xBB;
                                     TMR5L=0x40;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '4':               // 4.3
                             {
                                       TMR5H=0xBC;
                                     TMR5L=0xD0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '3':               // 4.2   
                             {
                                     TMR5H=0xBE;
                                     TMR5L=0x60;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '2':            //4.1
                             {
                                     TMR5H=0xBF;
                                     TMR5L=0xF0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '1':           //4.0
                             {
                                     TMR5H=0xC1;
                                     TMR5L=0x80;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '0':           // 3.9
                             {
                                    TMR5H=0xC3;
                                     TMR5L=0x10;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;

                             default:
                                 break;
                            } 
//                                   T5CONbits.TMR5ON = 1;
//                                    OUTPUT_FOR_BLUE_LED=1;
//                                    PIE3bits.TMR5IE = 1;
//                                    PIR3bits.TMR5IF=0;
                        }
                        break;
                case '3':{               // 3.8-2.9 
/**/
                        switch(levelofBlueLed_LSB)
                             {
                             case '9':           // 3.8
                             {
                                    TMR5H=0xC4;
                                     TMR5L=0xA0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '8':           // 3.7
                             {
                                     TMR5H=0xC6;
                                     TMR5L=0x30;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '7':           // 3.6
                             {
                                    TMR5H=0xC7;
                                     TMR5L=0xC0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '6':           // 3.5
                             {
                                      TMR5H=0xC9;
                                     TMR5L=0x50;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 3.4
                             {
                                    TMR5H=0xCA;
                                     TMR5L=0xE0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '4':               // 3.3
                             {
                                    TMR5H=0xCA;
                                     TMR5L=0xE0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '3':               // 3.2   
                             {
                                     TMR5H=0xCE;
                                     TMR5L=0x00;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '2':            //3.1
                             {
                                     TMR5H=0xCF;
                                     TMR5L=0x90;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '1':           // 3.0
                             {
                                     TMR5H=0xD1;
                                     TMR5L=0x20;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '0':           // 2.9
                             {
                                     TMR5H=0xD2;
                                     TMR5L=0xB0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             default:
                                     break;
                            } 
//                                    T5CONbits.TMR5ON = 1;
//                                    OUTPUT_FOR_BLUE_LED=1;
//                                    PIE3bits.TMR5IE = 1;
//                                    PIR3bits.TMR5IF=0;
                        }
                        break;
                case '2':{            //2.8-1.9
/**/
                        switch(levelofBlueLed_LSB)
                             {
                             case '9':           // 2.8
                             {
                                      TMR5H=0xD4;
                                     TMR5L=0x40;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '8':           // 2.7
                             {
                                     TMR5H=0xD5;
                                     TMR5L=0xD0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '7':           // 2.6
                             {
                                     TMR5H=0xD7;
                                     TMR5L=0x60;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '6':           // 2.5
                             {
                                     TMR5H=0xD8;
                                     TMR5L=0xF0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 2.4
                             {
                                     TMR5H=0xDA;
                                     TMR5L=0x80;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '4':               // 2.3
                             {
                                     TMR5H=0xDC;
                                     TMR5L=0x10;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '3':               // 2.2  
                             {
                                     TMR5H=0xDD;
                                     TMR5L=0xA0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '2':            // 2.1
                             {
                                      TMR5H=0xDF;
                                     TMR5L=0x30;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '1':           // 2.0
                             {
                                     TMR5H=0xE0;
                                     TMR5L=0xC0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '0':           // 1.9
                             {
                                        TMR5H=0xE2;
                                     TMR5L=0x50;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             default:
                                     break;
                            }
//                                     T5CONbits.TMR5ON = 1;
//                                    OUTPUT_FOR_BLUE_LED=1;
//                                    PIE3bits.TMR5IE = 1;
//                                    PIR3bits.TMR5IF=0;
                        }
                        break;
                case '1':{           //1.8-1.2
/**/
                        switch(levelofBlueLed_LSB)
                             {
                             case '9':           // 1.8
                             {
                                                                    								 TMR5H=0xE3;
                                     TMR5L=0xE0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '8':           // 1.75
                             {
                                     TMR5H=0xE4;
                                     TMR5L=0xA8;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '7':           // 1.7
                             {
                                    TMR5H=0xE5;
                                     TMR5L=0x70;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '6':           // 1.65
                             {
                                     TMR5H=0xE6;
                                     TMR5L=0x38;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 1.6
                             {
                                     TMR5H=0xE7;
                                     TMR5L=0x00;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '4':               // 1.5
                             {
                                     TMR5H=0xE8;
                                     TMR5L=0x90;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '3':               // 1.4   
                             {
                                     TMR5H=0xEA;
                                     TMR5L=0x20;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '2':            //1.3
                             {
                                     TMR5H=0xEB;
                                     TMR5L=0xB0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     
                                     break;
                             case '1':           //1.25
                             {
                                     TMR5H=0xEC;
                                     TMR5L=0x78;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '0':           // 1.2
                             {
                                     TMR5H=0xED;
                                     TMR5L=0x40;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             default:
                                     break;
                            }
//                                    T5CONbits.TMR5ON = 1;
//                                    OUTPUT_FOR_BLUE_LED=1;
//                                    PIE3bits.TMR5IE = 1;
//                                    PIR3bits.TMR5IF=0;
                }
                        break;
                case '0': {          // 1.1-0.2
/**/
                        switch(levelofBlueLed_LSB)
                             {
                             case '9':           // 1.1
                             {
                                     TMR5H=0xEE;
                                     TMR5L=0xD0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '8':           // 1.0
                             {
                                     TMR5H=0xF0;
                                     TMR5L=0x60;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '7':           // 0.9
                             {
                                     TMR5H=0xF1;
                                     TMR5L=0xF0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '6':           // 0.8
                             {
                                     TMR5H=0xF3;
                                     TMR5L=0x80;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '5'://TX1REG='n';      // 0.7
                             {
                                     TMR5H=0xF5;
                                     TMR5L=0x10;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '4':               // 0.6
                             {
                                          TMR5H=0xF6;
                                     TMR5L=0xA0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '3':               // 0.5    
                             {
                                     TMR5H=0xF8;
                                     TMR5L=0x30;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '2':            //0.4
                             {
                                  TMR5H=0xF9;
                                     TMR5L=0xC0;
                                     T5CONbits.TMR5ON = 1;
                                     OUTPUT_FOR_BLUE_LED=1;
                                    PIE3bits.TMR5IE = 1;
                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                             case '1':           //0.3
                             {
 //                                   TMR5H=0xFB;
 //                                    TMR5L=0x50;
//                                    T5CONbits.TMR5ON = 1;
                                    OUTPUT_FOR_BLUE_LED=0;
//                                    PIE3bits.TMR5IE = 1;
//                                    PIR3bits.TMR5IF=0;
                             }
                                    
                                     break;
                             case '0':           // 0.2
                             {
   //                                  TMR5H=0xFC;
   //                                 TMR5L=0xE0;
//                                    T5CONbits.TMR5ON = 1;
                                      
                                     OUTPUT_FOR_BLUE_LED=0;
//                                    PIE3bits.TMR5IE = 1;
//                                    PIR3bits.TMR5IF=0;
                             }
                                     break;
                                     default:
                                     break;
                            

                }
//                                    T5CONbits.TMR5ON = 1;
//                                    OUTPUT_FOR_BLUE_LED=1;
//                                    PIE3bits.TMR5IE = 1;
//                                    PIR3bits.TMR5IF=0;
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
     //   OUTPUT_RELAY1 = OFF;  OUTPUT_RELAY2 = OFF;    OUTPUT_FOR_RED_LED = OFF;  OUTPUT_FOR_GREEN_LED = OFF;OUTPUT_FOR_BLUE_LED=OFF;

            GPIO_pin_Initialize();
            allPeripheralInit();
    
    while(1){       
        if(mainDataReceived==TRUE){
           // errorsMain("MAIN");
            mainDataReceived=FALSE;
            if((copymainReceivedDataBuffer[0]=='R' && copymainReceivedDataBuffer[1]=='G')){
              ReceivingIndicationLedBlinking();
               ConvertmaindataReceiveIntoString = copymainReceivedDataBuffer;
           //    mainDataReceiveStringLength = strlen(ConvertmaindataReceiveIntoString);
           
           //    sendAcknowledgment(ConvertmaindataReceiveIntoString);
            //**************strat breaking the string**************//
              
           //  token = strtok(ConvertmaindataReceiveIntoString,delimiter);
               token = strtok(ConvertmaindataReceiveIntoString,".");
             ModuleNameString = token;
          //  sendAcknowledgment(ModuleNameString);//ok
             partCounter=0;
             if((strcmp(ModuleNameString,"RGB") == 0)){  
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
                            for(j=0;j<1;j++)
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
                            RedColorIntensityStringLength = strlen(RedColorIntensityString);
                          //  errorsMain(RedColorIntensityString);//255
                           
                        } break;
                        case 5:
                        {
                            GreenColorIntensityString = token;
                            GreenColorIntensityStringLength = strlen(GreenColorIntensityString);
                        //   errorsMain(GreenColorIntensityString);//ok-255
                           
                        } break;
                        case 6:
                        {
                            BlueColorIntensityString = token;
                            BlueColorIntensityStringLength = strlen(BlueColorIntensityString);
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
                            AllColorIntensityString = token;
                            AllColorIntensityStringLength = strlen(AllColorIntensityString);

                          //  sendAcknowledgment(AllColorIntensityString);

                            
                        }break;

                        default:
                            break;
                         
                            
                    }
                }//end of while

             } 
             
                   applicationControl(ModuleNameString,SwitchNumberString,SwitchStatusString,
                 RedColorIntensityString,GreenColorIntensityString,BlueColorIntensityString,ChildLockString,AllColorIntensityString ); 
            }   // End of all buffer data check
            else
            {
                ErrorNames[0]='E';      ErrorNames[1]='R';      ErrorNames[2]='L';      ErrorNames[3]='S';
                errorsMain(ErrorNames);
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

        
       // //check switch third status 
        //off condition
        if(parentalLockBuffer[1] == CHAR_OFF && INPUTSWITCH1 == OFF && M1 == OFF)
        {
            if(man == 1)
            {
#ifdef USEMANUAL_SWITCH1_REDLIGHT_DIMMABLE
            start_PWM_Generation_For_RedLed=1;
#endif
  //         start_PWM_Generation_For_GreenLed =0;
  //          start_PWM_Generation_For_BlueLed = 0;
#ifdef USEMANUAL_SWITCH1_REDLIGHT_SWITCH
               start_PWM_Generation_For_RedLed =0;
            sendAcknowledgment("RGB.1.ACTACK.0.0.0.0.0.100");
            OUTPUT_FOR_RED_LED=OFF;
#endif
            }
            man=0;
            M1=1;
          
        }
        //on condtion
        if(parentalLockBuffer[1] == CHAR_OFF && INPUTSWITCH1 == ON && M1 == ON)
        {
            if(man==1)
            {
#ifdef USEMANUAL_SWITCH1_REDLIGHT_DIMMABLE
                start_PWM_Generation_For_RedLed=1;
  //          start_PWM_Generation_For_GreenLed =0;
   //         start_PWM_Generation_For_BlueLed = 0;
           
                RedLedSwitchCounter++;
                 itoa(ConvertIntegertoStringRedColorBuffer,RedLedSwitchCounter,10);   // here 10 means decimal convert integer to string/buffer
                if(RedLedSwitchCounter > 0 && RedLedSwitchCounter < 10)
                {
                     Send_Acknowlde_To_RedPWM(ConvertIntegertoStringRedColorBuffer[0]);
                }
                else if(RedLedSwitchCounter >= 10 && RedLedSwitchCounter < 100 )
                {
                    
                     levelofRedLed_MSB=ConvertIntegertoStringRedColorBuffer[0];
                     levelofRedLed_LSB=ConvertIntegertoStringRedColorBuffer[1];
//                     TX1REG = levelofRedLed_MSB;__delay_ms(0.5);
//                      TX1REG = levelofRedLed_LSB;__delay_ms(0.5);
                }
                else
                {
                     RedLedSwitchCounter=0;   
                }
#endif
 
#ifdef USEMANUAL_SWITCH1_REDLIGHT_SWITCH                 
            start_PWM_Generation_For_RedLed =0;         
           sendAcknowledgment("RGB.1.ACTACK.1.0.0.0.0.100");
            OUTPUT_FOR_RED_LED=ON;
#endif            
            }
            man=0;
            M1=0;
            
        }
        
    
       // //check switch fourth status 
 //       off condition
        if(parentalLockBuffer[1] == CHAR_OFF && INPUTSWITCH2 == OFF && M2 == OFF)
        {
            if(man==1)
            {
#ifdef USEMANUAL_SWITCH1_GREENLIGHT_DIMMABLE
            start_PWM_Generation_For_GreenLed =1;
#endif            
 //           start_PWM_Generation_For_BlueLed = 0;
 //            start_PWM_Generation_For_RedLed=0;
#ifdef USEMANUAL_SWITCH1_GREENLIGHT_SWITCH
            start_PWM_Generation_For_GreenLed =0;
            sendAcknowledgment("RGB.1.ACTACK.0.0.0.0.0.100");
            OUTPUT_FOR_GREEN_LED=OFF;
#endif            

            }
            man=0;
            M2=1;
            
        }
        //on condtion
        if(parentalLockBuffer[1] == CHAR_OFF && INPUTSWITCH2 == ON && M2 == ON)
        {
            if(man==1)
            {
#ifdef USEMANUAL_SWITCH1_GREENLIGHT_DIMMABLE
            start_PWM_Generation_For_GreenLed = 1;
 //            start_PWM_Generation_For_BlueLed = 0;
 //            start_PWM_Generation_For_RedLed=0;
            GreenLedSwitchCounter++;
             itoa(ConvertIntegertoStringGreenColorBuffer,GreenLedSwitchCounter,10);
             if(GreenLedSwitchCounter > 0 && GreenLedSwitchCounter < 10)
             {
                 Send_Acknowlde_To_GreenPWM(ConvertIntegertoStringGreenColorBuffer[0]);
       
             }
             else if(GreenLedSwitchCounter >= 10 && GreenLedSwitchCounter < 100)
            {
               
                     levelofGreenLed_MSB=ConvertIntegertoStringGreenColorBuffer[0];
                     levelofGreenLed_LSB=ConvertIntegertoStringGreenColorBuffer[1];
//                   TX1REG = levelofGreenLed_MSB;__delay_ms(0.5);
//                      TX1REG = levelofGreenLed_LSB;__delay_ms(0.5);
            }
            else
            {
                GreenLedSwitchCounter=0;
            }
#endif
             
#ifdef USEMANUAL_SWITCH1_GREENLIGHT_SWITCH
            sendAcknowledgment("RGB.1.ACTACK.1.0.0.0.0.100");
            OUTPUT_FOR_GREEN_LED=ON;
             start_PWM_Generation_For_GreenLed =0;
#endif
            }
            man=0;
            M2=0;
           
        }
//
    if(parentalLockBuffer[1] == CHAR_OFF && INPUTSWITCH3 == OFF && M3 == OFF)
        {
            if(man==1)
            {
#ifdef USEMANUAL_SWITCH1_BLUELIGHT_DIMMABLE
                start_PWM_Generation_For_BlueLed =1;
#endif
   //          start_PWM_Generation_For_GreenLed = 0;
  //           start_PWM_Generation_For_RedLed=0;
#ifdef USEMANUAL_SWITCH1_BLUELIGHT_SWITCH
                start_PWM_Generation_For_BlueLed = 0;
           sendAcknowledgment("RGB.1.ACTACK.0.0.0.0.0.100");
            OUTPUT_FOR_BLUE_LED=OFF;
#endif
            }
            man=0;
            M3=1;
            
        }
        //on condtion
        if(parentalLockBuffer[1] == CHAR_OFF && INPUTSWITCH3 == ON && M3 == ON)
        {
            if(man==1)
            {
#ifdef USEMANUAL_SWITCH1_BLUELIGHT_DIMMABLE                
            start_PWM_Generation_For_BlueLed = 1;
 //            start_PWM_Generation_For_GreenLed = 0;
 //            start_PWM_Generation_For_RedLed=0;
            BlueLedSwitchCounter++;
             itoa(ConvertIntegertoStringBlueColorBuffer,BlueLedSwitchCounter,10);
             if(BlueLedSwitchCounter > 0 && BlueLedSwitchCounter < 10)
             {
                 Send_Acknowlde_To_BluePWM(ConvertIntegertoStringBlueColorBuffer[0]);
             }
             else if(BlueLedSwitchCounter >= 10 && BlueLedSwitchCounter < 100)
            {
                
                 levelofBlueLed_MSB = ConvertIntegertoStringBlueColorBuffer[0];
                 levelofBlueLed_LSB = ConvertIntegertoStringBlueColorBuffer[1];
//                 TX1REG = levelofBlueLed_MSB;__delay_ms(0.5);
//                      TX1REG = levelofBlueLed_LSB;__delay_ms(0.5);
            }
            else
            {
                BlueLedSwitchCounter =0;
            }
#endif
             
#ifdef USEMANUAL_SWITCH1_BLUELIGHT_SWITCH
            sendAcknowledgment("RGB.1.ACTACK.1.0.0.0.0.100");
            OUTPUT_FOR_BLUE_LED=ON;
            start_PWM_Generation_For_BlueLed = 0;
#endif
            }
            man=0;
            M3=0;
           
        }
       
        if(parentalLockBuffer[1] == CHAR_OFF && INPUTSWITCH4 == OFF && M4 == OFF)
        {
            if(man == 1)
            {
                start_PWM_Generation_For_RedLed = 0;
                start_PWM_Generation_For_BlueLed = 0;
                 OUTPUT_FOR_BLUE_LED=OFF;
                 OUTPUT_FOR_RED_LED=OFF;
            }
            man=0;
            M4=1;
          
        }
        //on condtion
        if(parentalLockBuffer[1] == CHAR_OFF && INPUTSWITCH4 == ON && M4 == ON)
        {
            if(man==1)
            {
                start_PWM_Generation_For_RedLed = 1;
                start_PWM_Generation_For_BlueLed = 1;
                levelofRedLed_MSB = '9';
                levelofRedLed_LSB = '9';
                levelofBlueLed_MSB = '3';
                levelofBlueLed_LSB = '2';

            }
            man=0;
            M4=0;
            
        }
       
       
       
      if(parentalLockBuffer[1] == CHAR_OFF && INPUTSWITCH5 == OFF && M5 == OFF)
        {
            if(man == 1)
            {
                  start_PWM_Generation_For_GreenLed = 0;
                  start_PWM_Generation_For_BlueLed = 0;
                 OUTPUT_FOR_BLUE_LED=OFF;
                 OUTPUT_FOR_GREEN_LED=OFF;
  
            }
            man=0;
            M5=1;
          
        }
        //on condtion
        if(parentalLockBuffer[1] == CHAR_OFF && INPUTSWITCH5 == ON && M5 == ON)
        {
            if(man==1)
            {
                start_PWM_Generation_For_GreenLed = 1;
                start_PWM_Generation_For_BlueLed = 1;
                levelofGreenLed_MSB = '9';
                levelofGreenLed_LSB = '4';
                levelofBlueLed_MSB = '6';
                levelofBlueLed_LSB = '5';
            }
            man=0;
            M5=0;
            
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
    
    OUTPUT_RELAY_DIR_1 = 0;
    OUTPUT_RELAY_DIR_2 = 0;
    OUTPUT_RELAY_RED_LED_DIR = 0;
    OUTPUT_RELAY_GREEN_LED_DIR = 0;
    OUTPUT_RELAY_BLUE_LED_DIR = 0; 
    
    // peripherals directions
    ZCD_CCP9_DIR = 1;
    ZCD_CCP8_DIR = 1;
    ZCD_CCP7_DIR = 1;
    
    // USART DIRECTIONS
    USART_1_TRANSMIT_OUTPUT_DIR = 0;
    USART_1_RECIEVE_INPUT_DIR = 1;
    
   // OUTPUT_TRANMIT_INDICATION_LED_DIR = 0;
    OUTPUT_RECEIVE_INDICATION_LED_DIR = 0;
  //  USER_RECEIVE_INDICATION_LED_DIR = 0;
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
    CCP7_Initialize();
    CCP8_Initialize();

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
void CCP8_Initialize()
{
    // Set the CCP1 to the options selected in the User Interface

    // MODE Every edge; EN enabled; FMT right_aligned;
    CCP8CON = 0x04;
 //   CCP1CON = 0x84;

    // RH 0;
    CCPR8H = 0x00;

    // RL 0;
    CCPR8L = 0x00;

    // Clear the CCP1 interrupt flag
    PIR4bits.CCP8IF = 0;

    // Enable the CCP1 interrupt
    PIE4bits.CCP8IE= 1;
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

void CCP7_Initialize(void)
{
    // Set the CCP3 to the options selected in the User Interface

    // MODE Every edge; EN enabled; FMT right_aligned;
    CCP7CON = 0x04;    

    // CCPR3L 0; 
    CCPR7L = 0x00;    

    // CCPR3H 0; 
    CCPR7H = 0x00;    
    
    // Selecting Timer 3
//    CCPTMRS0bits.C3TSEL = 0x1;

    // Clear the CCP3 interrupt flag
    PIR4bits.CCP7IF = 0;

    // Enable the CCP3 interrupt
    PIE4bits.CCP7IE = 1;
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
            void InternalOscInit()
            {
                OSCCON = 0x7A;//IRCF<3:0>: Internal Oscillator Frequency Select bits,1111 = 16MHz //10 = Internal oscillator block |0111010|

                BORCON = 0x00;
              //  PCONbits.nRWDT = 1;//reset has not occured.
            }

/*
 * AANALOG and PULL up REGISTERS related initialization
 */
void pinINIT_extra(){
    ANSELG=0x00;    WPUG = 0x00;
    
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
    OUTPUT_FOR_RED_LED=0;
    OUTPUT_FOR_GREEN_LED=0;
    OUTPUT_FOR_BLUE_LED=0;
    
}
