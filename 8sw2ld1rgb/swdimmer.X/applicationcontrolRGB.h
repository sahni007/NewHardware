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
        
//while(*ModuleNameString != NULL)//convert string into buffer to concatenate the string>>>Not possible through string
//{
       // sendAcknowledgment(ModuleNamestr);
      //  sendAcknowledgment(SwitchNumberstr);
        
     //  sendAcknowledgment(SwitchStatusstr);
      //  sendAcknowledgment(RedColorIntensitystr);
      //  sendAcknowledgment(GreenColorIntensitystr);
      //  sendAcknowledgment(BlueColorIntensitystr);
    //   sendAcknowledgment(ChildLockstr);//>>issue
        //sendAcknowledgment(Finalframebitstr);
        
        
//    int i=0;
//    for(i=0;i<MODULENAMELENGTH;i++)
//    {
//        ModuleNameBuffer[i]=*ModuleNamestr;
//        *ModuleNamestr++;
//    }
//}
        //frame>>SW.
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
     //   strcat(sendFinalBufferToGAteway,".");
//        strcat(sendFinalBufferToGAteway,"|");
        sendAcknowledgment(sendFinalBufferToGAteway);
          
        parentalLockBuffer[IntegerSwitchNumber] = charchildLock;
                    //**********************//
    	int ConvertStringIntoInt=0;
	float ConvertIntToTimeInMilisec=0;
	unsigned long long int Pulse=0,NeedPulse=0,CompleteClock =65535;
	float deno = 20.0;
	float clockPerCycle=0.25;//microsecinds
	int remainder=0; 
	char HexlevelBuffer[5];
	int start=0;
    int end = strlen(HexlevelBuffer)-1;
    char strRedH[3],strRedL[3];
    char strGreenH[3],strGreenL[3];
    char strBlueH[3],strBlueL[3];
     //   TX1REG = copyparentalLockBuffer[IntegerSwitchNumber];
       //    sendAcknowledgment(sendFinalBufferToGAteway);  
        
        
       // sendAcknowledgment(sendFinalBufferToGAteway);//ok

//while(*SwitchNumberString != NULL)
//{
//int     i=0;
//    for(i=0;i<SWITCHNUMBERLENGTH;i++)
//    {
//        SwitchNumberBuffer[i]=*SwitchNumberstr;
//        *SwitchNumberstr++;
//    }
//}
        //>>frame SW.01.
//        strcat(sendFinalBufferToGAteway,SwitchNumberstr);
//        strcat(sendFinalBufferToGAteway,".");
      //  sendAcknowledgment(sendFinalBufferToGAteway);
//while(*AcknowledgementString != NULL)
//{
//    i=0;
//    for(i=0;i<ACKNOWLEDGEMENTLENGTH;i++)
//    {
//        AcknowledgementBuffer[i]=*AcknowledgementString;
//        *AcknowledgementString++;
//    }
//}
        //frame SW.01.ACTACK
//        strcat(sendFinalBufferToGAteway,"ACTACK");
//        strcat(sendFinalBufferToGAteway,".");//ok
       // sendAcknowledgment(sendFinalBufferToGAteway);//ok
//while(*SwitchStatusString != NULL)
//{
//     int i=0;
//    for(i=0;i<SWITCHSTATUS;i++)
//    {
//        SwitchStatusBuffer[i]=*SwitchStatusstr;
//        *SwitchStatusstr++;
//    }
//}
        //frame SW.01.ACTACK.1
//        strcat(sendFinalBufferToGAteway,SwitchStatusstr);
//        strcat(sendFinalBufferToGAteway,".");
       // sendAcknowledgment(sendFinalBufferToGAteway);//ok
//while(*RedColorIntensityString != NULL)
//{
        
        //***********************RED COLOR STRING************************************// 
        if(IntergerRedColorIntensity == 0)
        {
            Timer3H=0;
            Timer3L=0;
        }
        else if(IntergerRedColorIntensity == 255)
        {
            Timer3H=252;
            Timer3L=224;
        }
        
       
        int i=0;
        if(((IntergerRedColorIntensity & GIVES_OUTPUT_ZERO_IF_EVEN) == 1) && IntergerRedColorIntensity != 255 && IntergerRedColorIntensity != 0 )//ckeck for even
        {
            //odd - convert into even by adding one
            IntergerRedColorIntensity++;
           // TX1REG = 'O';
        }
        if(((IntergerRedColorIntensity & GIVES_OUTPUT_ZERO_IF_EVEN) == 0) && IntergerRedColorIntensity != 255 && IntergerRedColorIntensity != 0 )
        {
          //  TX1REG = 'E';
            IntergerRedColorIntensity = (IntergerRedColorIntensity/2);
            IntergerRedColorIntensity = round(IntergerRedColorIntensity*0.78);//to convert 0-123 to 0-99 eg: 123*0.8048 = 98
            itoa(ConvertIntegertoStringRedColorBuffer,IntergerRedColorIntensity,10);   // here 10 means decimal convert integer to string/buffer
           ///>>>>>>This loop is used to send two digit number to ISR.
            // for example, fro speed 30, after calibration it will give value less then 10, that means it ISR will read 90 instead 09, so that will create problem.
            if(IntergerRedColorIntensity > 0 && IntergerRedColorIntensity < 25)
            {
                 Send_Acknowlde_To_RedPWM(ConvertIntegertoStringRedColorBuffer[0]);
            }
            else
            {
            //even 
           for(i=0;i<RedColorIntensityStringLength;i++)
           {
              // TX1REG = ConvertIntegertoStringRedColorBuffer[i];//ok
               __delay_ms(0.5);
               RedColorIntensityBuffer[i]=ConvertIntegertoStringRedColorBuffer[i];
           //    TX1REG = RedColorIntensityBuffer[i];//ok

           }
            }
        }
        
 
  
   //********************GREEN COLOR STRING****************************************??  
        
    if(IntegerGreenColorIntensity == 0)
        {
            GreenColorIntensityBuffer[0] = '0';
            GreenColorIntensityBuffer[1] = '0';
            //RedColorIntensityBuffer[0] = '0';
           // TX1REG = 'Z';
        }
        else if(IntegerGreenColorIntensity == 255)
        {
            GreenColorIntensityBuffer[0] = '9';
            GreenColorIntensityBuffer[1] = '9';
           // RedColorIntensityBuffer[0] = '3';
          
        }
        
        int i=0;
        if(((IntegerGreenColorIntensity & GIVES_OUTPUT_ZERO_IF_EVEN) == 1) && IntegerGreenColorIntensity != 255 && IntegerGreenColorIntensity != 0 )//ckeck for even
        {
            //odd
            IntegerGreenColorIntensity++;
           // TX1REG = 'O';
        }
        if(((IntegerGreenColorIntensity & GIVES_OUTPUT_ZERO_IF_EVEN) == 0) && IntegerGreenColorIntensity != 255 && IntegerGreenColorIntensity != 0 )
        {
          //  TX1REG = 'E';
            IntegerGreenColorIntensity = (IntegerGreenColorIntensity/2);
            IntegerGreenColorIntensity = round(IntegerGreenColorIntensity*0.78);//ok//to convert 0-128 to 0-99 eg: 127*0.78 = 99
            itoa(ConvertIntegertoStringGreenColorBuffer,IntegerGreenColorIntensity,10);   // here 10 means decimal convert integer to string
            if(IntegerGreenColorIntensity > 0 && IntegerGreenColorIntensity < 25)
            {
                Send_Acknowlde_To_GreenPWM(ConvertIntegertoStringGreenColorBuffer[0]);
            }
            else
            {
            //even 
           for(i=0;i<GreenColorIntensityStringLength;i++)
           {
              // TX1REG = ConvertIntegertoStringRedColorBuffer[i];//ok
               __delay_ms(0.5);
               GreenColorIntensityBuffer[i]=ConvertIntegertoStringGreenColorBuffer[i];
 //              TX1REG = GreenColorIntensityBuffer[i];//ok

           }
           }
        }
  ////*******************BLUE COLOR*********************//
        
        if(IntegerBlueColorIntensity == 0)
        {
            BlueColorIntensityBuffer[0] = '0';
            BlueColorIntensityBuffer[1] = '0';
            //RedColorIntensityBuffer[0] = '0';
           // TX1REG = 'Z';
        }
        else if(IntegerBlueColorIntensity == 255)
        {
            BlueColorIntensityBuffer[0] = '9';
            BlueColorIntensityBuffer[1] = '9';
           // RedColorIntensityBuffer[0] = '3';
          //  TX1REG = 'F';
        }
        
 
        
        
        int i=0;
        if(((IntegerBlueColorIntensity & GIVES_OUTPUT_ZERO_IF_EVEN) == 1) && IntegerBlueColorIntensity != 255 && IntegerBlueColorIntensity != 0 )//ckeck for even
        {
            //odd
            IntergerRedColorIntensity++;
           // TX1REG = 'O';
        }
        if(((IntegerBlueColorIntensity & GIVES_OUTPUT_ZERO_IF_EVEN) == 0) && IntegerBlueColorIntensity != 255 && IntegerBlueColorIntensity != 0 )
        {
          //  TX1REG = 'E';
            IntegerBlueColorIntensity = (IntegerBlueColorIntensity/2);
            IntegerBlueColorIntensity = round(IntegerBlueColorIntensity*0.78);//to convert 0-123 to 0-99 eg: 123*0.78 = 98
            itoa(ConvertIntegertoStringBlueColorBuffer,IntegerBlueColorIntensity,10);   // here 10 means decimal convert integer to string
          if(IntegerBlueColorIntensity > 0 && IntegerBlueColorIntensity < 25)
            {
                Send_Acknowlde_To_BluePWM(ConvertIntegertoStringBlueColorBuffer[0]);
            }
          else{
            //even 
           for(i=0;i<BlueColorIntensityStringLength;i++)
           {
              // TX1REG = ConvertIntegertoStringRedColorBuffer[i];//ok
               __delay_ms(0.5);
               
               BlueColorIntensityBuffer[i]=ConvertIntegertoStringBlueColorBuffer[i];
           //    TX1REG = RedColorIntensityBuffer[i];//ok

           }
          }
        }
        ///////////////END*************************
//           for(i=0;i<REDCOLOINTENSITYLENGTH;i++)
//           {
//
//               RedColorIntensityBuffer[i]=*RedColorIntensitystr;
//               *RedColorIntensitystr++;
//
//           }
//}
        //frame SW.01.ACTACK.1.0
//        strcat(sendFinalBufferToGAteway,RedColorIntensitystr);
//        strcat(sendFinalBufferToGAteway,".");
        //sendAcknowledgment(sendFinalBufferToGAteway);//ok
////while(*GreenColorIntensityString != NULL)
////{
//    int i=0;
//    for(i=0;i<GREENCOLORINTENSITYLENGHT;i++)
//    {
//        GreenColorIntensityBuffer[i]=*GreenColorIntensitystr;
//        *GreenColorIntensitystr++;
//    }
//}
        //frame SW.01.ACTACK.1.0.0
//        strcat(sendFinalBufferToGAteway,GreenColorIntensitystr);
//        strcat(sendFinalBufferToGAteway,".");
       // sendAcknowledgment(sendFinalBufferToGAteway);//ok
//while(*BlueColorIntensityBuffer != NULL)
//{
//    int i=0;
//    for(i=0;i<BLUECOLOINTENSITYLENGHT;i++)
//    {
//        BlueColorIntensityBuffer[i]=*BlueColorIntensitystr;
//        *BlueColorIntensitystr++;
//    }
//}
        ////frame SW.01.ACTACK.1.0.0.0
//        strcat(sendFinalBufferToGAteway,BlueColorIntensitystr);
//        strcat(sendFinalBufferToGAteway,".");
      //  sendAcknowledgment(sendFinalBufferToGAteway);//ok
//while(*ChildLockString != NULL)
//{
//   int i=0;
//    for(i=0;i<CHILDLOCKLENGTH;i++)
//    {
//        ChildLockBuffer[i]=*ChildLockstr;
//        *ChildLockstr++;
//    }
//}
         ////frame SW.01.ACTACK.1.0.0.0.0
//        strcat(sendFinalBufferToGAteway,ChildLockstr);
//        strcat(sendFinalBufferToGAteway,".");
   //     sendAcknowledgment(sendFinalBufferToGAteway);
//while(*FinalframebitString != NULL)
//{
//     i=0;
//    for(i=0;i<FINALFRAMEBITLENGTH;i++)
//    {
//        FinalFramebitBuffer[i]=*Finalframebitstr;
//        *Finalframebitstr++;
//    }
//}
        //INPUT FRAME >> SW/,.01.ACT.1.0.0.0.0.1.|
        //>>Final frame be like>>>>SW/RGB.01.ACTACK .1.0.0.0.0.1.|
        
//        strcat(sendFinalBufferToGAteway,Finalframebitstr);
//        strcat(sendFinalBufferToGAteway,".");
//        strcat(sendFinalBufferToGAteway,"|");
       //sendAcknowledgment(sendFinalBufferToGAteway);
        
        
        switch(IntegerSwitchNumber)
        {
            

            case 1:
            
                start_PWM_Generation_For_BlueLed = start_PWM_Generation_For_GreenLed = start_PWM_Generation_For_RedLed = IntergerSwitchStatus;
                switch(IntergerSwitchStatus){
                    case 0:
                        
                        OUTPUT_FOR_RED_LED = OFF;
                        OUTPUT_FOR_GREEN_LED = OFF;
                        OUTPUT_FOR_BLUE_LED = OFF;
                        break;
                    case 1:
                       
                      levelofRedLed_MSB =  RedColorIntensityBuffer[0];__delay_ms(0.5);
                      levelofRedLed_LSB =  RedColorIntensityBuffer[1];__delay_ms(0.5);
                       levelofGreenLed_MSB =  GreenColorIntensityBuffer[0];__delay_ms(0.5);
                      levelofGreenLed_LSB =  GreenColorIntensityBuffer[1];__delay_ms(0.5);
                      levelofBlueLed_LSB =  BlueColorIntensityBuffer[0];__delay_ms(0.5);
                      levelofBlueLed_MSB =  BlueColorIntensityBuffer[1];__delay_ms(0.5);                      
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

//                switch(IntergerSwitchStatus){
//                    case 0:
//                        
//                        OUTPUT_FOR_GREEN_LED = OFF;
//                        break;
//                    case 1:
//                      
//                      levelofGreenLed_MSB =  GreenColorIntensityBuffer[0];__delay_ms(0.5);
//                      levelofGreenLed_LSB =  GreenColorIntensityBuffer[1];__delay_ms(0.5);
////                    TX1REG = levelofGreenLed_LSB;__delay_ms(0.5);
////                      TX1REG = levelofGreenLed_MID;__delay_ms(0.5);
// //                   TX1REG = levelofGreenLed_MSB;__delay_ms(0.5);
//                      break;
//                    default:
//                        break;
//                        
//                }
//
//                switch(IntergerSwitchStatus){
//                    case 0:
//                     
//                        OUTPUT_FOR_BLUE_LED = OFF;
//                        break;
//                    case 1:
//                       
//                      levelofBlueLed_LSB =  BlueColorIntensityBuffer[0];__delay_ms(0.5);
//                      levelofBlueLed_MSB =  BlueColorIntensityBuffer[1];__delay_ms(0.5);
////                      TX1REG = levelofBlueLed_LSB;__delay_ms(0.5);
////                      TX1REG = levelofBlueLed_MID;__delay_ms(0.5);
////                      TX1REG = levelofBlueLed_MSB;__delay_ms(0.5);
//                      break;
//                    default:
//                        break;
//                        
//                }
//
//            break;
//            default:
//                break;
//            
//        }
//        
        
    }
}