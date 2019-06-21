       int man = 1;
        if(copy_parentalLockBuffer[1] == CHAR_OFF  && INPUTSWITCH1 == SWITCH_PRESSED && M1 == OFF)//switch is pressed
        {
            if(man == 1)
            {
                SwitchOnStatustToGatway(1);
            
            TransmissionIndicationLedBlinking();
          ///  OUTPUT_RELAY1=ON;
              sendAcknowledgmentRGB("RGB.1.ACTACK.1.0.0.0.0.100");
              OUTPUT_FOR_BLUE_LED=1;
              OUTPUT_FOR_RED_LED=0;
              OUTPUT_FOR_GREEN_LED=0;
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
            SwitchOffStatustToGatway(1);
            TransmissionIndicationLedBlinking();
         //   OUTPUT_RELAY1=OFF;
            
             sendAcknowledgmentRGB("RGB.1.ACTACK.0.0.0.0.0.100");
              OUTPUT_FOR_BLUE_LED=0;
              OUTPUT_FOR_RED_LED=0;
              OUTPUT_FOR_GREEN_LED=0;
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
            SwitchOnStatustToGatway(2);           
            TransmissionIndicationLedBlinking();
          //  OUTPUT_RELAY2=ON;
              sendAcknowledgmentRGB("RGB.2.ACTACK.1.0.0.0.0.100");
              OUTPUT_FOR_BLUE_LED=1;
              OUTPUT_FOR_RED_LED=1;
              OUTPUT_FOR_GREEN_LED=0;
            }
            man=0;
            M2=1;
        }
        //on condtion
        if(copy_parentalLockBuffer[2] == CHAR_OFF && INPUTSWITCH2 == SWITCH_RELEASED && M2 == ON)
        {
            if(man==1)
            {
            SwitchOffStatustToGatway(2);
            TransmissionIndicationLedBlinking();
          //  OUTPUT_RELAY2=OFF;
              sendAcknowledgmentRGB("RGB.2.ACTACK.0.0.0.0.0.100");
              OUTPUT_FOR_BLUE_LED=0;
              OUTPUT_FOR_RED_LED=0;
              OUTPUT_FOR_GREEN_LED=0;
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
            SwitchOnStatustToGatway(3);
            TransmissionIndicationLedBlinking();
           // OUTPUT_RELAY3=ON;
             sendAcknowledgmentRGB("RGB.3.ACTACK.1.0.0.0.0.100");
              OUTPUT_FOR_BLUE_LED=0;
              OUTPUT_FOR_RED_LED=1;
              OUTPUT_FOR_GREEN_LED=1;            
            }
            man=0;
            M3=1;
          
        }
        //on condtion
        if(copy_parentalLockBuffer[3] == CHAR_OFF && INPUTSWITCH3 == SWITCH_RELEASED && M3 == ON)
        {
            if(man==1)
            {
            SwitchOffStatustToGatway(3);
            TransmissionIndicationLedBlinking();
          //  OUTPUT_RELAY3=OFF;
              sendAcknowledgmentRGB("RGB.3.ACTACK.0.0.0.0.0.100");
              OUTPUT_FOR_BLUE_LED=0;
              OUTPUT_FOR_RED_LED=0;
              OUTPUT_FOR_GREEN_LED=0;
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
            SwitchOnStatustToGatway(4);
            TransmissionIndicationLedBlinking();
          //  OUTPUT_RELAY4=ON;
              sendAcknowledgmentRGB("RGB.4.ACTACK.1.0.0.0.0.100");
              OUTPUT_FOR_BLUE_LED=1;
              OUTPUT_FOR_RED_LED=0;
              OUTPUT_FOR_GREEN_LED=1;           
            }
            man=0;
            M4=1;
            
        }
        //on condtion
        if(copy_parentalLockBuffer[4] == CHAR_OFF && INPUTSWITCH4 == SWITCH_RELEASED && M4 == ON)
        {
            if(man==1)
            {
            
            SwitchOffStatustToGatway(4);
            TransmissionIndicationLedBlinking();
          //  OUTPUT_RELAY4=OFF;
              sendAcknowledgmentRGB("RGB.4.ACTACK.0.0.0.0.0.100");
              OUTPUT_FOR_BLUE_LED=0;
              OUTPUT_FOR_RED_LED=0;
              OUTPUT_FOR_GREEN_LED=0;
            }
            man=0;
            M4=0;
           
        }
               //off condition
        if(copy_parentalLockBuffer[5] == CHAR_OFF && INPUTSWITCH5 == SWITCH_PRESSED && M5 == OFF)
        {
            if(man==1)
            {
            SwitchOnStatustToGatway(5);
            TransmissionIndicationLedBlinking();
          //  OUTPUT_RELAY5=ON;
             sendAcknowledgmentRGB("RGB.5.ACTACK.1.0.0.0.0.100");
              OUTPUT_FOR_BLUE_LED=0;
              OUTPUT_FOR_RED_LED=1;
              OUTPUT_FOR_GREEN_LED=0;            
            }
            man=0;
            M5=1;
            
        }
        //on condtion
        if(copy_parentalLockBuffer[5] == CHAR_OFF && INPUTSWITCH5 == SWITCH_RELEASED && M5 == ON)
        {
            if(man==1)
            {
            
            SwitchOffStatustToGatway(5);
            TransmissionIndicationLedBlinking();
          //  OUTPUT_RELAY5=OFF;
              sendAcknowledgmentRGB("RGB.5.ACTACK.0.0.0.0.0.100");
              OUTPUT_FOR_BLUE_LED=0;
              OUTPUT_FOR_RED_LED=0;
              OUTPUT_FOR_GREEN_LED=0;           
            }
            man=0;
            M5=0;
           
        }
       
               //off condition
        if(copy_parentalLockBuffer[6] == CHAR_OFF && INPUTSWITCH6 == SWITCH_PRESSED && M6 == OFF)
        {
            if(man==1)
            {
            SwitchOnStatustToGatway(6);
            TransmissionIndicationLedBlinking();
          //  OUTPUT_RELAY6=ON;
            sendAcknowledgmentRGB("RGB.6.ACTACK.1.0.0.0.0.100");
              OUTPUT_FOR_BLUE_LED=0;
              OUTPUT_FOR_RED_LED=0;
              OUTPUT_FOR_GREEN_LED=1;
            }
            man=0;
            M6=1;
            
        }
        //on condtion
        if(copy_parentalLockBuffer[6] == CHAR_OFF && INPUTSWITCH6 == SWITCH_RELEASED && M6 == ON)
        {
            if(man==1)
            {
            
            SwitchOffStatustToGatway(6);
            TransmissionIndicationLedBlinking();
        //    OUTPUT_RELAY6=OFF;
             sendAcknowledgmentRGB("RGB.6.ACTACK.0.0.0.0.0.100");
              OUTPUT_FOR_BLUE_LED=0;
              OUTPUT_FOR_RED_LED=0;
              OUTPUT_FOR_GREEN_LED=0;
            }
            man=0;
            M6=0;
           
        }
       

               //off condition
        if(copy_parentalLockBuffer[7] == CHAR_OFF && INPUTSWITCH7 == SWITCH_PRESSED && M7 == OFF)
        {
            if(man==1)
            {
            SwitchOnStatustToGatway(7);
            TransmissionIndicationLedBlinking();
          //  OUTPUT_RELAY7=ON;
              sendAcknowledgmentRGB("RGB.7.ACTACK.1.0.0.0.0.100");
              OUTPUT_FOR_BLUE_LED=1;
              OUTPUT_FOR_RED_LED=0;
              OUTPUT_FOR_GREEN_LED=0;
            }
            man=0;
            M7=1;
            
        }
        //on condtion
        if(copy_parentalLockBuffer[7] == CHAR_OFF && INPUTSWITCH7 == SWITCH_RELEASED && M7 == ON)
        {
            if(man==1)
            {
            
            SwitchOffStatustToGatway(7);
            TransmissionIndicationLedBlinking();
          //  OUTPUT_RELAY7=OFF;
              sendAcknowledgmentRGB("RGB.7.ACTACK.0.0.0.0.0.100");
              OUTPUT_FOR_BLUE_LED=0;
              OUTPUT_FOR_RED_LED=0;
              OUTPUT_FOR_GREEN_LED=0;
            }
            man=0;
            M7=0;
           
        }
 
       

       //off condition
         if(copy_parentalLockBuffer[8] == CHAR_OFF && INPUTSWITCH8 == SWITCH_PRESSED && M8 == OFF)
         {
             if(man==1)
             {
              SwitchOnStatustToGatway(8);  
            TransmissionIndicationLedBlinking();
           //  OUTPUT_RELAY8=ON;
              sendAcknowledgmentRGB("RGB.8.ACTACK.1.0.0.0.0.100");
              OUTPUT_FOR_BLUE_LED=0;
              OUTPUT_FOR_RED_LED=1;
              OUTPUT_FOR_GREEN_LED=0;
             }
             man=0;
             M8=1;
            
         }
         //on condtion
         if(copy_parentalLockBuffer[8] == CHAR_OFF && INPUTSWITCH8 == SWITCH_RELEASED && M8 == ON)
         {
             if(man==1)
             {
            
            SwitchOffStatustToGatway(8);
            TransmissionIndicationLedBlinking();
        //     OUTPUT_RELAY8=OFF;
              sendAcknowledgmentRGB("RGB.8.ACTACK.0.0.0.0.0.100");
              OUTPUT_FOR_BLUE_LED=0;
              OUTPUT_FOR_RED_LED=0;
              OUTPUT_FOR_GREEN_LED=0;
             }
             man=0;
             M8=0;
           
         }
              //off condition
         if(copy_parentalLockBuffer[9] == CHAR_OFF && INPUTSWITCH9 == SWITCH_PRESSED && M9 == OFF)
         {
             if(man==1)
             {
             sendAcknowledgmentRGB("RGB.9.ACTACK.1.0.0.0.0.100");
              OUTPUT_FOR_BLUE_LED=1;
              OUTPUT_FOR_RED_LED=0;
              OUTPUT_FOR_GREEN_LED=0;

             }
             man=0;
             M9=1;
            
         }
         //on condtion
         if(copy_parentalLockBuffer[9] == CHAR_OFF && INPUTSWITCH9 == SWITCH_RELEASED && M9 == ON)
         {
             if(man==1)
             {
               sendAcknowledgment("RGB.9.ACTACK.0.0.0.0.0.100");
               OUTPUT_FOR_BLUE_LED=0;
               OUTPUT_FOR_RED_LED=0;
               OUTPUT_FOR_GREEN_LED=0;
             }
             man=0;
             M9=0;
           
         }
