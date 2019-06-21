

void applianceControl(char charSwitchMSB, char charSwitchLSB, char charSwitchSTATE, char chDimmerSpeedMSB, char chDimmerSpeedLSB,
        char charParentalControl, char charFinalFrameState){
    
    //define used variables and initilize it with zero
    int integerSwitchNumber = 0;
    int integerSwitchState = 0;
    int integerSpeed = 0;
    int currentStateBufferPositions=0;
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
    char strH[3],strL[3];
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
    
        // Get speed of Fan or level of dimmer    
    dimmerSpeedStringBuffer[0]=chDimmerSpeedMSB;
    dimmerSpeedStringBuffer[1]=chDimmerSpeedLSB;    
    integerSpeed = atoi(dimmerSpeedStringBuffer);
    integerSpeed = 99-integerSpeed;
    ConvertIntToTimeInMilisec = (integerSpeed/deno);
    ConvertIntToTimeInMilisec = (ConvertIntToTimeInMilisec*1000);//convert into microseconds
    Pulse = (ConvertIntToTimeInMilisec/clockPerCycle);
    NeedPulse = CompleteClock - Pulse;//65535-pulse
    sprintf(HexlevelBuffer,"%X",NeedPulse);
    strncpy(strH,HexlevelBuffer,2);
	strH[2]='\0';
//	printf("Higer Nibble: %s",strH);

	strncpy(strL,HexlevelBuffer+2,2);
	strL[2]='\0';
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
      
//        case 8:
//        {
//            OUTPUT_RELAY8 = integerSwitchState;
//        }break; 
        case 8:{
                start_PWM_Generation_For_DIMMER = integerSwitchState;
               switch(integerSwitchState){
                case 0:
                    OUTPUT_DIMMER=1;  // For Triac --> inverted condition for off
                    break;
                case 1:
                    Timer1H = hexadecimalToDecimal(strH);
                    Timer1L = hexadecimalToDecimal(strL);
                    break;
                default:
                    break;
               }
        }break;  
            default:
            break;
        }
    
}
