    void applicationControlRGB(char *ModuleNamestr,char *SwitchNumberstr,char *SwitchStatusstr,
                char  *RedColorIntensitystr,char *GreenColorIntensitystr,char *BlueColorIntensitystr,char *ChildLockstr,char *Allcolorstr ){
   

      //  sendAcknowledgment(Allcolorstr);
        
        IntegerSwitchNumber = atoi(SwitchNumberstr);
        IntergerSwitchStatus = atoi(SwitchStatusstr);
        IntergerRedColorIntensity = atoi(RedColorIntensitystr);
        IntegerBlueColorIntensity = atoi(BlueColorIntensitystr);
        IntegerGreenColorIntensity = atoi(GreenColorIntensitystr);
        IntegerAllColorIntensity = atoi(Allcolorstr);
        IntegerChildLock = atoi(ChildLockstr);
       
        
        charchildLock = IntegerChildLock + '0';
        charSwitchState = IntergerSwitchStatus + '0';
        

        strcpy(sendFinalBufferToGAteway,ModuleNamestr);//in firststep we need to ccreate a new buffer so copy is best option rather than concatenation   
        strcat(sendFinalBufferToGAteway,".");//now add delimiter with all frames in ;last
        strcat(sendFinalBufferToGAteway,SwitchNumberstr);
        strcat(sendFinalBufferToGAteway,".");
        strcat(sendFinalBufferToGAteway,"ACTACK");
        strcat(sendFinalBufferToGAteway,".");//ok
        strcat(sendFinalBufferToGAteway,SwitchStatusstr);
        strcat(sendFinalBufferToGAteway,".");
        strcat(sendFinalBufferToGAteway,RedColorIntensitystr);
        strcat(sendFinalBufferToGAteway,".");
        strcat(sendFinalBufferToGAteway,GreenColorIntensitystr);
        strcat(sendFinalBufferToGAteway,".");
        strcat(sendFinalBufferToGAteway,BlueColorIntensitystr);
        strcat(sendFinalBufferToGAteway,".");
        strcat(sendFinalBufferToGAteway,ChildLockstr);
        strcat(sendFinalBufferToGAteway,".");
         strcat(sendFinalBufferToGAteway,Allcolorstr);
        sendAcknowledgment(sendFinalBufferToGAteway);
          
        parentalLockBuffer[IntegerSwitchNumber] = charchildLock;
            //**********************//
   double ConvertIntToTimeInMicrosecRed;
   double ConvertIntToTimeInMicrosecGreen;
   double ConvertIntToTimeInMicrosecBlue;
	unsigned long long int RedPulse=0,GreenPulse=0,BluePulse=0,NeedRedPulse=0,NeedGreenPulse=0,NeedBluePulse=0,CompleteClock = 65535;
	float Convert255RangeintoHundred = 2.57;//****255/2.57=99******//
    int ConvertIntoTenMsRangeFactor = 10; //convert into 0-10ms range
	float clockPerCycle=0.25;//microsecinds
	int remainder=0; 
	char HexlevelBuffer[5];
	int start=0;
    int end = strlen(HexlevelBuffer)-1;
    char strRedH[3],strRedL[3];
    char strGreenH[3],strGreenL[3];
    char strBlueH[3],strBlueL[3];
    
        IntegerSwitchNumber = atoi(SwitchNumberstr);
        IntergerSwitchStatus = atoi(SwitchStatusstr);
        IntergerRedColorIntensity = atoi(RedColorIntensitystr);
        IntegerBlueColorIntensity = atoi(BlueColorIntensitystr);
        IntegerGreenColorIntensity = atoi(GreenColorIntensitystr);
        IntegerAllColorIntensity = atoi(Allcolorstr);
        IntegerChildLock = atoi(ChildLockstr);
   
       //**************Calculation for Red Pulses************************//   ConvertIntoTenMsRangeFactor
   //Range of ConvertIntToTimeInMilisecRed should be between 0ms to 10ms
    ConvertIntToTimeInMicrosecRed = (IntergerRedColorIntensity/Convert255RangeintoHundred);//2.57>>>>>>>>convert 0-255 range into 0-99 range
    ConvertIntToTimeInMicrosecRed = (ConvertIntToTimeInMicrosecRed/ConvertIntoTenMsRangeFactor);//>>>convert 0-99 rangr into 0-10ms
    ConvertIntToTimeInMicrosecRed = (ConvertIntToTimeInMicrosecRed*1000);//convert into microseconds
    RedPulse = (int)(ConvertIntToTimeInMicrosecRed/clockPerCycle);//0.25
    NeedRedPulse = abs(CompleteClock - RedPulse);//65535-pulse
    //*****************************************************************//
    
    //*****************Calculation for green pulses***************//
    ConvertIntToTimeInMicrosecGreen = (IntegerGreenColorIntensity/Convert255RangeintoHundred);//2.52
    ConvertIntToTimeInMicrosecGreen = (ConvertIntToTimeInMicrosecGreen/ConvertIntoTenMsRangeFactor);//2.52
    ConvertIntToTimeInMicrosecGreen = (ConvertIntToTimeInMicrosecGreen*1000); //convert into microseconds
    GreenPulse = (int)(ConvertIntToTimeInMicrosecGreen/clockPerCycle);//0.25
    NeedGreenPulse = abs(CompleteClock - GreenPulse);//65535-pulse
    //**********************************************************//
    
    //******************Calculation for blues pulse*****************//
    ConvertIntToTimeInMicrosecBlue = (IntegerBlueColorIntensity/Convert255RangeintoHundred);//2.52
    ConvertIntToTimeInMicrosecBlue = (ConvertIntToTimeInMicrosecBlue/ConvertIntoTenMsRangeFactor);//2.52
    ConvertIntToTimeInMicrosecBlue = (ConvertIntToTimeInMicrosecBlue*1000);//convert into microseconds
    BluePulse = (int)(ConvertIntToTimeInMicrosecBlue/clockPerCycle);//0.25
    NeedBluePulse = abs(CompleteClock - BluePulse);//65535-pulse
    //**************************************************************//
    
    
    sprintf(HexlevelBuffer,"%X",NeedRedPulse);
    strncpy(strRedH,HexlevelBuffer,2);
	strRedH[2]='\0';
	strncpy(strRedL,HexlevelBuffer+2,2);
	strRedL[2]='\0';
    memset(HexlevelBuffer,'0',sizeof(HexlevelBuffer));
    //************************************//
    sprintf(HexlevelBuffer,"%X",NeedGreenPulse);
    strncpy(strGreenH,HexlevelBuffer,2);
	strGreenH[2]='\0';
	strncpy(strGreenL,HexlevelBuffer+2,2);
	strGreenL[2]='\0'; 
    memset(HexlevelBuffer,'0',sizeof(HexlevelBuffer));
    
  //************************************//
    sprintf(HexlevelBuffer,"%X",NeedBluePulse);
    strncpy(strBlueH,HexlevelBuffer,2);
	strBlueH[2]='\0';
	strncpy(strBlueL,HexlevelBuffer+2,2);
	strBlueL[2]='\0';  
    memset(HexlevelBuffer,'0',sizeof(HexlevelBuffer));
//**************************************//
    
         
        switch(IntegerSwitchNumber)
        {
            case 1:{
                switch(IntergerSwitchStatus){
                    case 0:
                        start_PWM_Generation_For_BlueLed = start_PWM_Generation_For_GreenLed = start_PWM_Generation_For_RedLed = 0;
                        OUTPUT_FOR_RED_LED = OFF;
                        OUTPUT_FOR_GREEN_LED = OFF;
                        OUTPUT_FOR_BLUE_LED = OFF;
                        break;
                    case 1:
                        if(IntergerRedColorIntensity == 255)
                            OUTPUT_FOR_RED_LED=1;
                        else if(IntergerRedColorIntensity == 0)
                                OUTPUT_FOR_RED_LED=0;
                        else
                        {
                        start_PWM_Generation_For_RedLed = 1;
                        Timer1H = hexadecimalToDecimal(strRedH);
                        Timer1L = hexadecimalToDecimal(strRedL);
                            
                        }
                        if(IntegerGreenColorIntensity == 255)
                            OUTPUT_FOR_GREEN_LED=1;
                        else if(IntegerGreenColorIntensity == 0)
                        OUTPUT_FOR_GREEN_LED=0;
                        else
                        {
                        start_PWM_Generation_For_GreenLed =1;
                        Timer3H = hexadecimalToDecimal(strGreenH);
                        Timer3L = hexadecimalToDecimal(strGreenL);
                        }
                        if(IntegerBlueColorIntensity == 255)
                            OUTPUT_FOR_BLUE_LED=1;
                        else if(IntegerBlueColorIntensity == 0)
                        OUTPUT_FOR_BLUE_LED=0;
                        else{
                       start_PWM_Generation_For_BlueLed =1;
                        Timer5H = hexadecimalToDecimal(strBlueH);
                        Timer5L = hexadecimalToDecimal(strBlueL);
                        }
//                     TX1REG = levelofRedLed_LSB;__delay_ms(0.5);
//                      TX1REG = levelofRedLed_MSB;__delay_ms(0.5);
//                      TX1REG = levelofGreenLed_LSB;__delay_ms(0.5);
//                      TX1REG = levelofGreenLed_MSB;__delay_ms(0.5); 
//                      TX1REG = levelofBlueLed_LSB;__delay_ms(0.5);
//                      TX1REG = levelofBlueLed_MSB;__delay_ms(0.5);
                      
                      break;
                    default:
                        break;          
                }
                break;
                default:
                    break;
             }
//        memset(strRedH,'0',sizeof(strRedH));
//         memset(strRedL,'0',sizeof(strRedL));
//          memset(strGreenH,'0',sizeof(strGreenH));
//           memset(strGreenL,'0',sizeof(strGreenL));
//            memset(strBlueH,'0',sizeof(strBlueH));
//             memset(strBlueL,'0',sizeof(strBlueL));
        }
    
    
    
}