/*
 * File:   curtain_shutter_control.c
 * Author: Alfaone
 * PCB used: STD.8SW.1D.RGB
 *@ Note: This code is for shutter (up,stop,down) and curtain control
 * TImer1 is used for shutter working only
 * Timer3 and TImer5 is used for curtain controlling
 * Created on 19 March, 2019, 2:46 PM 
 * @ Description: Here we are using one shutter(up,stop and down) and one curtain 
 * when we up>live >OUTPUT_RELAY1
 * stop>LOW>OUTPUT_RELY2
 * down>OUTPUT_RELAY3
 * for curtain operation: 15 seconds delay
 * last used: 25/03/2019: akhilesh
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
//************Shutter control********************//
#define OUTPUT_RELAY1 PORTEbits.RE0
#define OUTPUT_RELAY2 PORTEbits.RE1
#define OUTPUT_RELAY3 PORTGbits.RG0
//*********************************************//


//*********curtain control***********************//
#define OUTPUT_RELAY4 PORTGbits.RG3
#define OUTPUT_RELAY5 PORTGbits.RG4
//************************************//
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


//RGB MACORS
#define OUTPUT_FOR_RED_LED PORTDbits.RD4 ///pwm for red
#define OUTPUT_FOR_GREEN_LED PORTDbits.RD3 //pwm for green
#define OUTPUT_FOR_BLUE_LED PORTDbits.RD2   // PWM for blue

#define OUTPUT_RELAY_RED_LED_DIR TRISDbits.TRISD4
#define OUTPUT_RELAY_GREEN_LED_DIR TRISDbits.TRISD3
#define OUTPUT_RELAY_BLUE_LED_DIR TRISDbits.TRISD2        // direction of PWM OUTPUT to MOC3021


#define ZCD_CCP10_DIR TRISEbits.TRISE3//for dimmer


#define INPUTSWITCH1 PORTDbits.RD5 //Instead of RD6 we are using RD5 because of wrong wiring connection
#define INPUTSWITCH2 PORTDbits.RD6
#define INPUTSWITCH3 PORTDbits.RD7
#define INPUTSWITCH4 PORTBbits.RB0
#define INPUTSWITCH5 PORTBbits.RB1
#define INPUTSWITCH6 PORTBbits.RB2
#define INPUTSWITCH7 PORTBbits.RB3
#define INPUTSWITCH8 PORTBbits.RB4



#define INPUT_SWITCH_DIR_1 TRISDbits.TRISD5
#define INPUT_SWITCH_DIR_2 TRISDbits.TRISD6
#define INPUT_SWITCH_DIR_3 TRISDbits.TRISD7
#define INPUT_SWITCH_DIR_4 TRISBbits.TRISB0
#define INPUT_SWITCH_DIR_5 TRISBbits.TRISB1
#define INPUT_SWITCH_DIR_6 TRISBbits.TRISB2
#define INPUT_SWITCH_DIR_7 TRISBbits.TRISB3
#define INPUT_SWITCH_DIR_8 TRISBbits.TRISB4
//#define INPUT_SWITCH_DIR_9 TRISBbits.TRISB5




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
#define SWITCHED_PRESSED 0
#define SWITCHED_RELEASED 1


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
int TimerCounter2 =0;
int TimerCounter4 =0;
int CurtFlag1=0;
int CurtFlag2=0;
int ShutterUpCounter=0;
int ShutterStopCounter=0;
int ShutterDownCounter=0;
int ShutterMotorUpFlag=0;
int ShutterMotorStopFlag=0;
int ShutterMotorDownFlag=0;
unsigned char ErrorNames[5]="####";
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
void TMR5_Initialize();
void TMR2_Initialize();
void TMR4_Initialize();
void allPeripheralInit();

void copyReceivedDataBuffer();

void applianceControl(char switchMSB, char switchLSB, char switchSTATE, char dimmerSpeedMSB, char dimmerSpeedLSB, char parentalControl, char finalFrameState);
interrupt void isr(){
 
    // ************************************* UART INTERRUPT *********************************************** //
    if(RC1IF){        
        if(RC1STAbits.OERR){    // If over run error, then reset the receiver
            RC1STAbits.CREN = 0; // countinuous Recieve Disable
            RC1STAbits.CREN = 1; // countinuous Recieve Enable
            
            ErrorNames[0]='E';      ErrorNames[1]='R';      ErrorNames[2]='O';      ErrorNames[3]='V';
            errorsISR(ErrorNames); 
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
            
            ErrorNames[0]='E';      ErrorNames[1]='R';      ErrorNames[2]='R';      ErrorNames[3]='X';
            errorsISR(ErrorNames);            
        }
    }// End of RC1IF 
    
    //***************************************************************************//
   if(PIE1bits.TMR1IE == 1 && PIR1bits.TMR1IF==1)
    {
        PIR1bits.TMR1IF=0;        //TX1REG='T'; 
        
		if(ShutterUpCounter >= 16){
            ShutterUpCounter=0;
        OUTPUT_RELAY1 = 0;
        sendAcknowledgment("G001");
         T1CONbits.TMR1ON = 0;
        }
       else if(ShutterMotorUpFlag){
        ShutterUpCounter=ShutterUpCounter+1;
        TMR1H=0x0B;        TMR1L=0xDC;        T1CONbits.TMR1ON = 1;
        }
    }//end of timer 1
    
   //***************************************************************************//
   if(PIE3bits.TMR3IE == 1 && PIR3bits.TMR3IF==1)
    {
        PIR3bits.TMR3IF=0;        //TX1REG='T';   
		if(ShutterStopCounter >= 16){
        OUTPUT_RELAY2 = 1;
        sendAcknowledgment("G002");
        T3CONbits.TMR3ON = 0;
        }
        else if(ShutterMotorStopFlag){
        ShutterStopCounter=ShutterStopCounter+1;
        TMR3H=0x0B;        TMR3L=0xDC;        T3CONbits.TMR3ON = 1;
        
        }
    }//end of timer 3
    
       //***************************************************************************//
   if(PIE3bits.TMR5IE == 1 && PIR3bits.TMR5IF==1)
    {
        PIR3bits.TMR5IF=0;        //TX1REG='T';   
		if(ShutterDownCounter >= 16){
            ShutterDownCounter=0;
        OUTPUT_RELAY3 = 0;
        sendAcknowledgment("G003");
        T5CONbits.TMR5ON = 0;
        }
        else if(ShutterMotorDownFlag){
        ShutterDownCounter=ShutterDownCounter+1;
        TMR5H=0x0B;        TMR5L=0xDC;        T5CONbits.TMR5ON = 1;
        
        }
    }//end of timer 5
    
           //***************************************************************************//
   if(PIE1bits.TMR2IE == 1 && PIR1bits.TMR2IF==1)
    {
       //4ms*500 == 2000ms = 2sec
        PIR1bits.TMR2IF=0;        //TX1REG='T';   
        //15seconds
		if(TimerCounter2 >= 3750){
        OUTPUT_RELAY4 = 0;
        OUTPUT_RELAY5 = 0;
        sendAcknowledgment("G004");
        T2CONbits.TMR2ON = 0;
        }
        else if(CurtFlag1){
        TimerCounter2=TimerCounter2+1;
        PR2=0xF9;        T2CONbits.TMR2ON = 1;
        
        }
    }//end of timer 2
               //***************************************************************************//
   if(PIE3bits.TMR4IE == 1 && PIR3bits.TMR4IF==1)
    {
       //4ms*37500 == 15000ms = 15sec
        PIR3bits.TMR4IF=0;        //TX1REG='T';   
		if(TimerCounter4 >= 3750){
        OUTPUT_RELAY5 = 0;
        OUTPUT_RELAY4 = 0;
        sendAcknowledgment("G005");
        T4CONbits.TMR4ON = 0;
        }
        else if(CurtFlag2){
        TimerCounter4=TimerCounter4+1;
        PR4=0xF9;        T4CONbits.TMR4ON = 1;
        
        }
    }//end of timer 2
}

/*
 * Alfaone Main code starts here
 * For 4 switches 1 Dimmer
 */
int main() {
    __delay_ms(2000);
        M1=ON;    M2=ON;    M3=ON;    M4=ON;    M5=ON;
      OUTPUT_RELAY2=ON;

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
        if(parentalLockBuffer[1] == CHAR_OFF  && INPUTSWITCH1 == SWITCH_RELEASED && M1 == OFF)
        {
            if(man == 1)
            {
            SwitchOffStatustToGatway('1');
            TransmissionIndicationLedBlinking();
            OUTPUT_RELAY1=OFF;
            }
            man=0;
            M1=1;
            
        }
        //on condition
        if(parentalLockBuffer[1] == CHAR_OFF && INPUTSWITCH1 == SWITCHED_PRESSED && M1 == ON)
        {
            //TX1REG='C';
            if(man==1)
            {
             SwitchOnStatustToGatway('1');
             TransmissionIndicationLedBlinking();
            OUTPUT_RELAY1=ON;
             ShutterMotorUpFlag =1;
             ShutterUpCounter=0;
             TMR1H=0x0B;               
             TMR1L=0xDC;               
             PIR1bits.TMR1IF=0;        
             T1CONbits.TMR1ON = 1;
            }
            man=0;
            M1=0;
        }
        
       // //check switch second status 
        //off condition
        if(parentalLockBuffer[2] == CHAR_OFF && INPUTSWITCH2 == SWITCH_RELEASED && M2 == OFF)
        {
            if(man==1)
            {
            SwitchOffStatustToGatway('2');
            TransmissionIndicationLedBlinking();
            OUTPUT_RELAY2=1;
            }
            man=0;
            M2=1;
        }
        //on condtion
        if(parentalLockBuffer[2] == CHAR_OFF && INPUTSWITCH2 == SWITCHED_PRESSED && M2 == ON)
        {
            if(man==1)
            {
               SwitchOnStatustToGatway('2');
               TransmissionIndicationLedBlinking();
             OUTPUT_RELAY2 = 0;
             ShutterMotorStopFlag =1;
             ShutterStopCounter=0;
             TMR3H=0x0B;               
             TMR3L=0xDC;               
             PIR3bits.TMR3IF=0;        
             T3CONbits.TMR3ON = 1;
            }
            man=0;
            M2=0;
        }
               // //check switch second status 
        //off condition
        if(parentalLockBuffer[3] == CHAR_OFF && INPUTSWITCH3 == SWITCH_RELEASED && M3 == OFF)
        {
            if(man==1)
            {
             SwitchOffStatustToGatway('3');
             TransmissionIndicationLedBlinking();
            OUTPUT_RELAY3=OFF;
            }
            man=0;
            M3=1;
        }
        //on condtion
        if(parentalLockBuffer[3] == CHAR_OFF && INPUTSWITCH3 == SWITCHED_PRESSED && M3 == ON)
        {
            if(man==1)
            {
                SwitchOnStatustToGatway('3');
                TransmissionIndicationLedBlinking();
             OUTPUT_RELAY3 = 1;
             ShutterMotorDownFlag =1;
             ShutterDownCounter=0;
             TMR5H=0x0B;               
             TMR5L=0xDC;               
             PIR3bits.TMR5IF=0;        
             T5CONbits.TMR5ON = 1;
            }
            man=0;
            M3=0;
        }
        //off condition
        if(parentalLockBuffer[4] == CHAR_OFF && INPUTSWITCH4 == SWITCH_RELEASED && M4 == OFF)
        {
            if(man==1)
            {
            SwitchOffStatustToGatway('4');
            TransmissionIndicationLedBlinking();
            OUTPUT_RELAY4=OFF;
            }
            man=0;
            M4=1;
        }
        //on condtion
        if(parentalLockBuffer[4] == CHAR_OFF && INPUTSWITCH4 == SWITCHED_PRESSED && M4 == ON)
        {
            if(man==1)
            {
             SwitchOffStatustToGatway('5');
             OUTPUT_RELAY5 = 0;
             SwitchOnStatustToGatway('4');
             TransmissionIndicationLedBlinking();
             OUTPUT_RELAY4 = 1;
             CurtFlag1 =1;
             TimerCounter2=0;
             PR2=0xF9;
             PIR1bits.TMR2IF=0;        
             T2CONbits.TMR2ON = 1;
            }
            man=0;
            M4=0;
        }
       
               //off condition
        if(parentalLockBuffer[5] == CHAR_OFF && INPUTSWITCH5 == SWITCH_RELEASED && M5 == OFF)
        {
            if(man==1)
            {
            SwitchOffStatustToGatway('5');
            TransmissionIndicationLedBlinking();
            OUTPUT_RELAY5=OFF;
            }
            man=0;
            M5=1;
        }
        //on condtion
        if(parentalLockBuffer[5] == CHAR_OFF && INPUTSWITCH5 == SWITCHED_PRESSED && M5 == ON)
        {
            if(man==1)
            {
             SwitchOffStatustToGatway('4');
               OUTPUT_RELAY4 = 0;
              SwitchOnStatustToGatway('5');
              TransmissionIndicationLedBlinking();
             OUTPUT_RELAY5 = 1;
             CurtFlag2 = 1;
             TimerCounter4=0;
             PR4=0xF9;
             PIR3bits.TMR4IF=0;        
             T4CONbits.TMR4ON = 1;
            }
            man=0;
            M5=0;
        }
               //off condition
        if(parentalLockBuffer[6] == CHAR_OFF && INPUTSWITCH6 == SWITCH_RELEASED && M6 == OFF)
        {
            if(man==1)
            {
             SwitchOffStatustToGatway('6');
             TransmissionIndicationLedBlinking();
            OUTPUT_RELAY6=OFF;
            }
            man=0;
            M6=1;
        }
               //on condtion
        if(parentalLockBuffer[6] == CHAR_OFF && INPUTSWITCH6 == SWITCHED_PRESSED && M6 == ON)
        {
            if(man==1)
            {
                SwitchOnStatustToGatway('6');
                TransmissionIndicationLedBlinking();
             OUTPUT_RELAY6 = 1;

            }
            man=0;
            M6=0;
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
                    if(integerSwitchState == 1){
                    OUTPUT_RELAY1 = 1;
                    ShutterMotorUpFlag =1;
                     ShutterUpCounter=0;
                     TMR1H=0x0B;               
                     TMR1L=0xDC;               
                     PIR1bits.TMR1IF=0;        
                     T1CONbits.TMR1ON = 1;
            }
            else
            {
                    OUTPUT_RELAY1 = 0;
                    ShutterMotorUpFlag = 0;
            }
            break;
        case 2:
            if(integerSwitchState == 1)
            {
                    OUTPUT_RELAY2 = 0;
                    ShutterMotorStopFlag =1;
                    ShutterStopCounter=0;
                    TMR3H=0x0B;               
                    TMR3L=0xDC;               
                    PIR3bits.TMR3IF=0;        
                    T3CONbits.TMR3ON = 1; 
            }
            else
            {
                    OUTPUT_RELAY2 = 1;
                    ShutterMotorStopFlag = 0;
            }
            break;
        case 3:
            if(integerSwitchState == 1)
            {
                    
                    OUTPUT_RELAY3 = 1;
                    ShutterMotorDownFlag =1;
                    ShutterDownCounter=0;
                    TMR5H=0x0B;               
                    TMR5L=0xDC;               
                    PIR3bits.TMR5IF=0;        
                    T5CONbits.TMR5ON = 1; 
            }
            else
            {
                        OUTPUT_RELAY3 = 0;
                        ShutterMotorDownFlag = 0;
            }
            break;
            
        case 4:
            if(integerSwitchState == 1)
            {
                    OUTPUT_RELAY5 = 0;
                    OUTPUT_RELAY4 = 1;
                    CurtFlag1 =1;
                    TimerCounter2=0;
                    PR2=0xF9;
                    PIR1bits.TMR2IF=0;        
                    T2CONbits.TMR2ON = 1; 
            }
            else
            {
                    OUTPUT_RELAY4 = 0;
                    CurtFlag1 = 0;
            }
            break;
        case 5:
            if(integerSwitchState == 1)
            {
                    OUTPUT_RELAY4 = 0;
                    OUTPUT_RELAY5 = 1;
                    CurtFlag2 =1;
                    TimerCounter4=0;
                    PR4=0xF9;
                    PIR3bits.TMR4IF=0;        
                    T4CONbits.TMR4ON = 1; 
            }
            else
            {
                    OUTPUT_RELAY5 = 0;
                    CurtFlag2 = 0;
            }
            break; 
        case 6:
            OUTPUT_RELAY6 = integerSwitchState;
            break;
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


   
    
    OUTPUT_RELAY_DIR_1 = 0;
    OUTPUT_RELAY_DIR_2 = 0;
    OUTPUT_RELAY_DIR_3 = 0;
    OUTPUT_RELAY_DIR_4 = 0;
    OUTPUT_RELAY_DIR_5 = 0;
    OUTPUT_RELAY_DIR_6 = 0;
    OUTPUT_RELAY_DIR_7 = 0;
    OUTPUT_RELAY_DIR_8 = 0;
    
    
    ZCD_CCP10_DIR = 1;

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
    TMR3_Initialize();
    TMR5_Initialize();
    TMR2_Initialize();
    TMR4_Initialize();
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
//prescaling: 1:64
//     T2CKPS 1:1; T2OUTPS 1:1; TMR2ON off; 
    T2CON = 0x03;
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
    T4CON = 0x03;

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
void sendAcknowledgment(char* currentStateBuffer){
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