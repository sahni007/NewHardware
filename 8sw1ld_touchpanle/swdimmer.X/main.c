/* 
 * File:   only_on_off.c
 * Author: Varun sahni
 * client: test
 * module: this is working code for 7 switches and one dimmer with spinal touchpanel
 * Note: for switching the led 
 * ON>>give low
 * OFF>>give 
 * Tip: use function as much possible for memory consumption(when i was using the R101 with normal TX1REG it use 39% of memory while using function it use 31% of memory(8% of memory relesed))
 * Note: LEVEL Shifter is used to supply 3.3 voltage to xbee.
 * #RA3 pin is used , keep RA3 pin to operate in 5 voltade mode, keep RA3 LOW to operate in 3.3 voltage
 * #In this hardware, input switch works diffrently then our original---->>input switch work as the keypad...all input switch initlize as high(pull-up) and when we press it it will give low output
 * operation              Pin Status
 * switch ON                0
 * Switch OFF               1
 * so work opposite to the original one but output is same original -->>load is operating with transistor
 * # In this hardware we also debug the transmitting and received frame--->>>after receiveing the frame glow Blue Led
 *                                                                     ---->>>after transmitting the frame glow Green Led
 * NOTE: CCP8 is not working, so don't use CCP8 at all
 *  
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include<pic16f1526.h>
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<math.h>
#include<float.h>
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
#define OUTPUT_RELAY1 PORTEbits.RE1
#define OUTPUT_RELAY2 PORTEbits.RE0
#define OUTPUT_RELAY3 PORTGbits.RG0
#define OUTPUT_RELAY4 PORTGbits.RG3
#define OUTPUT_RELAY5 PORTGbits.RG4
#define OUTPUT_RELAY6 PORTFbits.RF6
#define OUTPUT_RELAY7 PORTFbits.RF5
#define OUTPUT_RELAY8 PORTFbits.RF4
#define OUTPUT_DIMMER PORTFbits.RF3 //MOC


#define OUTPUT_RELAY_DIR_1 TRISEbits.TRISE1
#define OUTPUT_RELAY_DIR_2 TRISEbits.TRISE0
#define OUTPUT_RELAY_DIR_3 TRISGbits.TRISG0
#define OUTPUT_RELAY_DIR_4 TRISGbits.TRISG3
#define OUTPUT_RELAY_DIR_5 TRISGbits.TRISG4
#define OUTPUT_RELAY_DIR_6 TRISFbits.TRISF6
#define OUTPUT_RELAY_DIR_7 TRISFbits.TRISF5
#define OUTPUT_RELAY_DIR_8 TRISFbits.TRISF4
#define OUTPUT_DIMMER_DIR TRISFbits.TRISF3




#define INPUTSWITCH1 PORTDbits.RD6
#define INPUTSWITCH2 PORTDbits.RD7
#define INPUTSWITCH3 PORTBbits.RB0
#define INPUTSWITCH4 PORTBbits.RB1
#define INPUTSWITCH5 PORTBbits.RB2
#define INPUTSWITCH6 PORTBbits.RB3
#define INPUTSWITCH7 PORTBbits.RB4
#define INPUTSWITCH8 PORTBbits.RB5


#define INPUT_SWITCH_DIR_1 TRISDbits.TRISD6
#define INPUT_SWITCH_DIR_2 TRISDbits.TRISD7
#define INPUT_SWITCH_DIR_3 TRISBbits.TRISB0
#define INPUT_SWITCH_DIR_4 TRISBbits.TRISB1
#define INPUT_SWITCH_DIR_5 TRISBbits.TRISB2
#define INPUT_SWITCH_DIR_6 TRISBbits.TRISB3
#define INPUT_SWITCH_DIR_7 TRISBbits.TRISB4
#define INPUT_SWITCH_DIR_8 TRISBbits.TRISB5




//macros for leds

#define OUTPUT_TRANMIT_INDICATION_LED PORTFbits.RF0 //green
#define OUTPUT_RECEIVE_INDICATION_LED PORTFbits.RF1 //blue led
#define USER_RECEIVE_INDICATION_LED PORTFbits.RF2 //orange led

#define OUTPUT_TRANMIT_INDICATION_LED_DIR TRISFbits.TRISF0
#define OUTPUT_RECEIVE_INDICATION_LED_DIR TRISFbits.TRISF1
#define USER_RECEIVE_INDICATION_LED_DIR TRISFbits.TRISF2

#define LEVEL_SHIFTER_OUTPUT_PIN PORTAbits.RA3
#define LEVEL_SHIFTER_OUTPUT_PIN_DIR TRISAbits.TRISA3
// direction of PWM OUTPUT to MOC3021


#define ZCD_CCP10_DIR TRISEbits.TRISE3//for dimmer
#define ZCD_CCP8_DIR TRISEbits.TRISE4
/*
 * Extra Periferals Direction and PORT
 */
//#define ZCD_CCP9_DIR TRISEbits.TRISE3
// USART Directions
#define USART_1_TRANSMIT_OUTPUT_DIR TRISCbits.TRISC6
#define USART_1_RECIEVE_INPUT_DIR TRISCbits.TRISC7
#define USART_2_TRANSMIT_OUTPUT_DIR TRISGbits.TRISG1
#define USART_2_RECIEVE_INPUT_DIR TRISGbits.TRISG2
#define RECIEVED_DATA_LENGTH (16+2)
#define TOTAL_NUMBER_OF_SWITCH (12+2)
#define TOUCHPANEL_DATA_LENGTH (8*2)
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
#define TouchMatikBoardAddress 'h'

int mainReceivedDataPosition=0, mainDataReceived=FALSE;
unsigned char mainReceivedDataBuffer[RECIEVED_DATA_LENGTH]="#"; 
unsigned char tempReceivedDataBuffer[RECIEVED_DATA_LENGTH-8]="#";
unsigned char parentalLockBuffer[TOTAL_NUMBER_OF_SWITCH]="000000000000";
unsigned char copy_parentalLockBuffer[TOTAL_NUMBER_OF_SWITCH]="000000000000";
unsigned char currentStateBuffer[(TOTAL_NUMBER_OF_SWITCH*4)+2]="#";

int  checkFlag = FALSE;
int touchpanelReceivedataPosition = 0; 
volatile int touchPanelDataReceived = FALSE;
unsigned char touchpanleReceivedDatabuffer[TOUCHPANEL_DATA_LENGTH]="#";
unsigned char tempReceiveTouchpanelDataBuffer[TOUCHPANEL_DATA_LENGTH-8]="#";


unsigned int M1;unsigned int M2;unsigned int M3;unsigned int M4;
unsigned int M5;unsigned int M6;unsigned int M7;unsigned int M8;
int Timer1H=0,Timer1L=0;
int start_PWM_Generation_For_DIMMER = 0;
    void TMR1_Initialize();
    void TMR2_Initialize();
    void CCP10_Initialize();
    void CCP8_Initialize();
int hexadecimalToDecimal(char hexVal[]) ;
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
void EUSART2_Initialize();
void sendFeedback_TO_Gateway(char Switch_Num, char sw_status);
void allPeripheralInit();
void copyTouchpanelReceiveDataBuffer();
void copyReceivedDataBuffer();
void actiontouchPanel(char Switch_Num, char sw_status );//, char speeds
void applianceControl(char switchMSB, char switchLSB, char switchSTATE, char dimmerSpeedMSB, char dimmerSpeedLSB, char parentalControl, char finalFrameState);
#include "include.h"
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
    
        /**************************************TOUCH_PANEL INTERRUPT*******************************************/
    if(RC2IF){        
        if(RC2STAbits.OERR){    // If over run error, then reset the receiver
            RC2STAbits.CREN = 0; // countinuous Recieve Disable
            RC2STAbits.CREN = 1; // countinuous Recieve Enable
            
          
            errorsISR("ERROV"); 
        }   
       if(RC2STAbits.FERR){    // If over run error, then reset the receiver
            RC2STAbits.CREN = 0; // countinuous Recieve Disable
            RC2STAbits.CREN = 1; // countinuous Recieve Enable
            
          
            errorsISR("RERR"); 
        }  
        
        touchpanleReceivedDatabuffer[touchpanelReceivedataPosition] = RC2REG;
        #ifdef DEBUG
        TX1REG= touchpanleReceivedDatabuffer[touchpanelReceivedataPosition];
        #endif
        if(touchpanleReceivedDatabuffer[0] == '(')
        {
            touchpanelReceivedataPosition++;
            if(touchpanelReceivedataPosition > 7)
            {
                touchPanelDataReceived = TRUE;
            
                touchpanelReceivedataPosition=0;
                 RC2IF = 0;
            }
        }
        else{
            RC2STAbits.CREN = 0; // countinuous Recieve Disable
            RC2STAbits.CREN = 1; // countinuous Recieve Enable
            touchpanelReceivedataPosition=0; // Reinitiate buffer counter
          
            errorsISR("ERRT");            
        }
    }//End of RC2IF
    
       //*******************************DIMMER 11111 *************************************
    
     if(PIE1bits.TMR2IE==1 && PIR1bits.TMR2IF==1)
    {           
        PIR1bits.TMR2IF=0;
        OUTPUT_DIMMER = FALSE;//for dimmer
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
    if(PIR4bits.CCP10IF == 1){
    if(CCP10IF){
        if(CCP10IF == 1){
             CCP10IF=0;
         if(start_PWM_Generation_For_DIMMER == 1)
                                    TMR1H = Timer1H;
                                    TMR1L = Timer1L;
                                    T1CONbits.TMR1ON = 1;
       
                            }//end of ccp10
                }
        }//end of CCP
}




/*
 * Alfaone Main code starts here
 * For 8 switches
 */
int main() {
    __delay_ms(2000);
        M1=OFF;    M2=OFF;    M3=OFF;    M4=OFF;    M5=OFF; M6=OFF; M7=OFF;M8=OFF;
        OUTPUT_DIMMER = 0;
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
   ///STARTING OF TOUCHPANEL DATA RECEIVE
        if(touchPanelDataReceived == TRUE)
        {
            touchPanelDataReceived = FALSE;
            int start_flag = 0;
            int end_flag = 0;
            if(touchpanleReceivedDatabuffer[0] == '(' && touchpanleReceivedDatabuffer[7] == ')')
            {
                
                if(touchpanleReceivedDatabuffer[0] == '('  && start_flag == 0)
                {
                    end_flag =1;

                }
                if(touchpanleReceivedDatabuffer[7] == ')' && end_flag ==1)
                {
                        copyTouchpanelReceiveDataBuffer();
                        if(tempReceiveTouchpanelDataBuffer[0] != '@')
                        {
                           actiontouchPanel(tempReceiveTouchpanelDataBuffer[0],tempReceiveTouchpanelDataBuffer[1]); //,tempReceiveTouchpanelDataBuffer[2]
                            start_flag = 0;
                            end_flag = 0;
                         }
                                
                }
               
            }
                else
                {
                 
                    errorsMain("ERLS");
                    RC2STAbits.SPEN = 0;  // Serial port disabled  
                    RC2STAbits.CREN = 0; // countinuous Recieve Disable                
                    for(int dataBufferCounter = 0; dataBufferCounter< 8; dataBufferCounter++)
                          {
                                      touchpanleReceivedDatabuffer[dataBufferCounter] = '#'; // clean received data buffer
                            }
                    RC2STAbits.CREN = 1; // countinuous Recieve Enable
                    RC2STAbits.SPEN=1;  // Serial port enabled (configures RXx/DTx and TXx/CKx pins as serial port pins)
                 }
            
        }
        /******************** MANUAL RESPONE STARTS HERE************ */
//        
//       int man = 1;
//        if(copy_parentalLockBuffer[1] == CHAR_OFF  && INPUTSWITCH1 == SWITCH_PRESSED && M1 == OFF)//switch is pressed
//        {
//            if(man == 1)
//            {
//                SwitchOnStatustToGatway('1');
//            
//            TransmissionIndicationLedBlinking();
//            OUTPUT_RELAY1=ON;
//            
//            }
//            man=0;
//            M1=1;
//            
//        }
//        //on condition
//        if(copy_parentalLockBuffer[1] == CHAR_OFF && INPUTSWITCH1 == SWITCH_RELEASED && M1 == ON)////switch is released
//        {
//            //TX1REG='C';
//            if(man==1)
//            {
//            SwitchOffStatustToGatway('1');
//            TransmissionIndicationLedBlinking();
//            OUTPUT_RELAY1=OFF;
//            }
//            man=0;
//            M1=0;
//        }
//        
//       // //check switch second status 
//        //off condition
//        if(copy_parentalLockBuffer[2] == CHAR_OFF && INPUTSWITCH2 == SWITCH_PRESSED && M2 == OFF)
//        {
//            if(man==1)
//            {
//            SwitchOnStatustToGatway('2');           
//            TransmissionIndicationLedBlinking();
//            OUTPUT_RELAY2=ON;
//            }
//            man=0;
//            M2=1;
//        }
//        //on condtion
//        if(copy_parentalLockBuffer[2] == CHAR_OFF && INPUTSWITCH2 == SWITCH_RELEASED && M2 == ON)
//        {
//            if(man==1)
//            {
//            SwitchOffStatustToGatway('2');
//            TransmissionIndicationLedBlinking();
//            OUTPUT_RELAY2=OFF;
//            }
//            man=0;
//            M2=0;
//        }
//        
//        
//       // //check switch third status 
//        //off condition
//        if(copy_parentalLockBuffer[3] == CHAR_OFF && INPUTSWITCH3 == SWITCH_PRESSED && M3 == OFF)
//        {
//            if(man == 1)
//            {
//            SwitchOnStatustToGatway('3');
//            TransmissionIndicationLedBlinking();
//            OUTPUT_RELAY3=ON;
//            }
//            man=0;
//            M3=1;
//          
//        }
//        //on condtion
//        if(copy_parentalLockBuffer[3] == CHAR_OFF && INPUTSWITCH3 == SWITCH_RELEASED && M3 == ON)
//        {
//            if(man==1)
//            {
//            SwitchOffStatustToGatway('3');
//            TransmissionIndicationLedBlinking();
//            OUTPUT_RELAY3=OFF;
//            }
//            man=0;
//            M3=0;
//            
//        }
//        
//     
//       // //check switch fourth status 
//        //off condition
//        if(copy_parentalLockBuffer[4] == CHAR_OFF && INPUTSWITCH4 == SWITCH_PRESSED && M4 == OFF)
//        {
//            if(man==1)
//            {
//            SwitchOnStatustToGatway('4');
//            TransmissionIndicationLedBlinking();
//            OUTPUT_RELAY4=ON;
//            }
//            man=0;
//            M4=1;
//            
//        }
//        //on condtion
//        if(copy_parentalLockBuffer[4] == CHAR_OFF && INPUTSWITCH4 == SWITCH_RELEASED && M4 == ON)
//        {
//            if(man==1)
//            {
//            
//            SwitchOffStatustToGatway('4');
//            TransmissionIndicationLedBlinking();
//            OUTPUT_RELAY4=OFF;
//            }
//            man=0;
//            M4=0;
//           
//        }
//               //off condition
//        if(copy_parentalLockBuffer[5] == CHAR_OFF && INPUTSWITCH5 == SWITCH_PRESSED && M5 == OFF)
//        {
//            if(man==1)
//            {
//            SwitchOnStatustToGatway('5');
//            TransmissionIndicationLedBlinking();
//            OUTPUT_RELAY5=ON;
//            }
//            man=0;
//            M5=1;
//            
//        }
//        //on condtion
//        if(copy_parentalLockBuffer[5] == CHAR_OFF && INPUTSWITCH5 == SWITCH_RELEASED && M5 == ON)
//        {
//            if(man==1)
//            {
//            
//            SwitchOffStatustToGatway('5');
//            TransmissionIndicationLedBlinking();
//            OUTPUT_RELAY5=OFF;
//            }
//            man=0;
//            M5=0;
//           
//        }
//       
//               //off condition
//        if(copy_parentalLockBuffer[6] == CHAR_OFF && INPUTSWITCH6 == SWITCH_PRESSED && M6 == OFF)
//        {
//            if(man==1)
//            {
//            SwitchOnStatustToGatway('6');
//            TransmissionIndicationLedBlinking();
//            OUTPUT_RELAY6=ON;
//            }
//            man=0;
//            M6=1;
//            
//        }
//        //on condtion
//        if(copy_parentalLockBuffer[6] == CHAR_OFF && INPUTSWITCH6 == SWITCH_RELEASED && M6 == ON)
//        {
//            if(man==1)
//            {
//            
//            SwitchOffStatustToGatway('6');
//            TransmissionIndicationLedBlinking();
//            OUTPUT_RELAY6=OFF;
//            }
//            man=0;
//            M6=0;
//           
//        }
//       
//
//               //off condition
//        if(copy_parentalLockBuffer[7] == CHAR_OFF && INPUTSWITCH7 == SWITCH_PRESSED && M7 == OFF)
//        {
//            if(man==1)
//            {
//            SwitchOnStatustToGatway('7');
//            TransmissionIndicationLedBlinking();
//            OUTPUT_RELAY7=ON;
//            }
//            man=0;
//            M7=1;
//            
//        }
//        //on condtion
//        if(copy_parentalLockBuffer[7] == CHAR_OFF && INPUTSWITCH7 == SWITCH_RELEASED && M7 == ON)
//        {
//            if(man==1)
//            {
//            
//            SwitchOffStatustToGatway('7');
//            TransmissionIndicationLedBlinking();
//            OUTPUT_RELAY7=OFF;
//            }
//            man=0;
//            M7=0;
//           
//        }
// 
//       
//
//       //off condition
//         if(copy_parentalLockBuffer[8] == CHAR_OFF && INPUTSWITCH8 == SWITCH_PRESSED && M8 == OFF)
//         {
//             if(man==1)
//             {
//              SwitchOnStatustToGatway('8');  
//            TransmissionIndicationLedBlinking();
//             OUTPUT_DIMMER=ON;
//             }
//             man=0;
//             M8=1;
//            
//         }
//         //on condtion
//         if(copy_parentalLockBuffer[8] == CHAR_OFF && INPUTSWITCH8 == SWITCH_RELEASED && M8 == ON)
//         {
//             if(man==1)
//             {
//            
//            SwitchOffStatustToGatway('8');
//            TransmissionIndicationLedBlinking();
//             OUTPUT_DIMMER=OFF;
//             }
//             man=0;
//             M8=0;
//           
//         }
     
 
    }    
}


void actiontouchPanel(char Switch_Num, char sw_status) //, char speeds
{

        M1=ON;           M2=ON;        M3=ON;        M4=ON;          M5=ON;      M6=ON;      M7=ON;      M8=ON;

    int switch_status = sw_status - '0';        
    int SwNum = Switch_Num - '@';//ASCII OF SWITCH NUMBER - ASCII OF @ i.e A>>65, B>>66, C>>67, D>>68 65-64=1 and so on

    char ch_sw_num = SwNum +'0';//send '1' for switch A, '2' for sww2 and so on 
   
 if(checkFlag == TRUE)  
  
 {  
     checkFlag=FALSE;   
 }   
    else
    {
        switch(Switch_Num) {

               case 'A':
               {
               if(M1 == ON && copy_parentalLockBuffer[1] == CHAR_OFF )
                  {
                       sendFeedback_TO_Gateway('1',sw_status);
                       OUTPUT_RELAY1 = switch_status; M1 = OFF;
                      
                    //   switchtorealy.TimerCounterCurrentVlaue++;
                       
                  }
               }

               break;
               case 'B':
               {

                 if(M2 == ON && copy_parentalLockBuffer[2] == CHAR_OFF  )
                  {       
                        sendFeedback_TO_Gateway('2',sw_status);
                        OUTPUT_RELAY2 = switch_status;  M2 = OFF;
//                      TMR5H=0x0B;
//                      TMR5L=0xDC;
//                      T5CONbits.TMR5ON = 1;
//                      switchtorealy.TimerStartflag=1;
//                      switchtorealy.TimerCounter=0;
                  }

               }

               break;
               case 'C':
               {
              if(M3 == ON && copy_parentalLockBuffer[3] == CHAR_OFF )
                 {    
                    sendFeedback_TO_Gateway('3',sw_status);
                     OUTPUT_RELAY3 = switch_status;
                       M3 = OFF;
                  }

               }
               break;
               case 'D':
               {
                   if(M4 == ON && copy_parentalLockBuffer[4] == CHAR_OFF)
                  {
                       sendFeedback_TO_Gateway('4',sw_status);
                        OUTPUT_RELAY4 = switch_status;M4 = OFF;
                 }

               }
               break;
               case 'E':
               {
                if(M5 == ON && copy_parentalLockBuffer[5] == CHAR_OFF)
                   {
                         sendFeedback_TO_Gateway('5',sw_status);         
                        OUTPUT_RELAY5 = switch_status;  M5 = OFF;

                  }
               }
               break;
               case 'F':
               {

                  if(M6 == ON && copy_parentalLockBuffer[6] == CHAR_OFF)
                   {               
                      sendFeedback_TO_Gateway('6',sw_status);  
                      OUTPUT_RELAY6 = switch_status;M6 = OFF;

                  } 
               }
               break;
                case 'G':
               {
                  if(M7 == ON && copy_parentalLockBuffer[7] == CHAR_OFF)
                   {
                      sendFeedback_TO_Gateway('7',sw_status);
                        OUTPUT_RELAY7 = switch_status;   M7 = OFF;             

                  } 
               }
               break;
              case 'H':
               {
//                    if(M8 == ON && copy_parentalLockBuffer[8] == CHAR_OFF  && switch_status == 1 ){
//                   
//                   start_PWM_Generation_For_DIMMER=0;
//                
//                   OUTPUT_DIMMER = switch_status ; 
//                   M8 = OFF;
//                    }
//                    else if(M8 == OFF && copy_parentalLockBuffer[8] == CHAR_OFF  && switch_status == 0 ){
//                   
//                   start_PWM_Generation_For_DIMMER=0;
//                 //  __delay_ms(1000);
//                   OUTPUT_DIMMER = switch_status ; 
//                   M8 = ON;
//                    }
//                  if(M8 == ON && copy_parentalLockBuffer[8] == CHAR_OFF)
//                   {
//                      
//                      sendFeedback_TO_Gateway('8',sw_status);
//                       OUTPUT_DIMMER = switch_status;  M8 = OFF;
//                         
//                        
//                  }
                //  }
//                  if(M8 == ON && copy_parentalLockBuffer[8] == CHAR_OFF && switchtorealy.relayToDimmerSwitchBuffer[0] == 1 )
//                   {
//                      if(switchtorealy.relayToDimmerSwitchBuffer[0] == 1){
//                      switchtorealy.relayToDimmerSwitchBuffer[1]=0;
//                      switchtorealy.TimerCounterCurrentVlaue=0;
//                      sendFeedback_TO_Gateway('8',sw_status);
//                      sendAcknowledgment("ENTR");
//                      OUTPUT_RELAY8 = switch_status;  
//                      M8 = OFF;
//                      }
//                  }
//                  else if(M8 == ON && copy_parentalLockBuffer[8] == CHAR_OFF  )
//                   {
//                     // if(switchtorealy.relayToDimmerSwitchBuffer[1] == 1){
//                   //   switchtorealy.relayToDimmerSwitchBuffer[0]=0;
//                 //     switchtorealy.TimerCounterCurrentVlaue=0;
////                      start_PWM_Generation_in_ISR_FLAG=0;
////                      sendAcknowledgment("ENTD");
////                      sendFeedback_TO_Gateway('8',sw_status);
////                      OUTPUT_DIMMER = ~switch_status;  
////                      M8 = OFF;
//                    //  }
//                  }

               }
               break;
             
               default:
               break;
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
   OUTPUT_DIMMER_DIR = 0;
    
    
  
   OUTPUT_TRANMIT_INDICATION_LED_DIR = 0;
    OUTPUT_RECEIVE_INDICATION_LED_DIR = 0;
    USER_RECEIVE_INDICATION_LED_DIR = 0;
    
        ZCD_CCP10_DIR = 1;
    ZCD_CCP8_DIR = 1;
  
    // peripherals directions
    
    // USART DIRECTIONS
    USART_1_TRANSMIT_OUTPUT_DIR = 0;
    USART_1_RECIEVE_INPUT_DIR = 1;
       USART_2_TRANSMIT_OUTPUT_DIR = 0;
    USART_2_TRANSMIT_OUTPUT_DIR = 1;
    
    clearAllPorts();
}

/*
 * ALL Peripheral Initialization
 */
void allPeripheralInit(){
    EUSART_Initialize();
     EUSART2_Initialize();
    TMR1_Initialize();
    TMR2_Initialize();
    CCP10_Initialize();
    CCP8_Initialize();
}
int hexadecimalToDecimal(char hexVal[]) 
{    
    int len = strlen(hexVal); 
      
    // Initializing base value to 1, i.e 16^0 
    int base = 1; 
      
    int dec_val = 0; 
      
    // Extracting characters as digits from last character 
    for (int i=len-1; i>=0; i--) 
    {    
        // if character lies in '0'-'9', converting  
        // it to integral 0-9 by subtracting 48 from 
        // ASCII value. 
        if (hexVal[i]>='0' && hexVal[i]<='9') 
        { 
            dec_val += (hexVal[i] - 48)*base; 
                  
            // incrementing base by power 
            base = base * 16; 
        } 
  
        // if character lies in 'A'-'F' , converting  
        // it to integral 10 - 15 by subtracting 55  
        // from ASCII value 
        else if (hexVal[i]>='A' && hexVal[i]<='F') 
        { 
            dec_val += (hexVal[i] - 55)*base; 
          
            // incrementing base by power 
            base = base*16; 
        } 
    } 
      
    return dec_val; 
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
void EUSART2_Initialize()
{
    PIE4bits.RC2IE = 0;
    PIE4bits.TX2IE = 0;

    // Set the EUSART module to the options selected in the user interface.

    // ABDOVF no_overflow; SCKP Non-Inverted; BRG16 16bit_generator; WUE enabled; ABDEN disabled;
    BAUD2CON = 0x0A;

    // SPEN enabled; RX9 8-bit; CREN enabled; ADDEN disabled; SREN disabled;
    RC2STA = 0x90;

    // TX9 8-bit; TX9D 0; SENDB sync_break_complete; TXEN enabled; SYNC asynchronous; BRGH hi_speed; CSRC slave;
    TX2STA = 0x24;

    // Baud Rate = 9600; SP1BRGL 12;
    SP2BRGL = 0xA0;                  // SYNC =0 ; BRGH = 1 ; BRG16=1;
    // Baud Rate = 9600; SP1BRGH 1;
    SP2BRGH = 0x01;

    // Enable all active interrupts ---> INTCON reg .... bit 7            page 105
    GIE = 1;

    // Enables all active peripheral interrupts -----> INTCON reg .... bit 6         page 105
    PEIE = 1;

    // enable receive interrupt    
    PIE4bits.RC2IE = 1; // handled into INTERRUPT_Initialize()
    // Transmit Enabled
    TX2STAbits.TXEN = 1;

    // Serial Port Enabled
    RC2STAbits.SPEN = 1;
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

void copyTouchpanelReceiveDataBuffer()
{
     int dataBufferCounter=2;
     for(dataBufferCounter=2; dataBufferCounter<5;dataBufferCounter++)
     {
         tempReceiveTouchpanelDataBuffer[dataBufferCounter-2] = touchpanleReceivedDatabuffer[dataBufferCounter];
         touchpanleReceivedDatabuffer[dataBufferCounter] = "#";
     }
}

void sendFeedback_TO_Gateway(char Switch_Num, char sw_status)
{
     __delay_ms(5);     TX1REG = 'R';
    __delay_ms(1);      TX1REG = sw_status;
    __delay_ms(1);      TX1REG = '0';
    __delay_ms(1);      TX1REG = Switch_Num;
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
    OUTPUT_DIMMER = 1;
    OUTPUT_TRANMIT_INDICATION_LED = 1;
    OUTPUT_RECEIVE_INDICATION_LED = 1;
    USER_RECEIVE_INDICATION_LED = 1;//off condition--->>>leds are high by default

   
}
