#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<float.h>

void reverseString(char str[5]);
void main()
{
	char InputBuffer[3],lsb,msb;
	int ConvertStringIntoInt=0;
	float ConvertIntToTimeInMilisec=0;
	unsigned long long int Pulse=0,NeedPulse=0,CompleteClock =65535;
	float deno =10;
	float clockPerCycle=0.25;//microsecinds
	int remainder=0; 
	char HexlevelBuffer[5];
	int i=0;
	char a,b;
	int hex = 3400; 
	printf("origin hex is : %X\n",hex);
	printf("swapped hex is: %X",((hex & 0x000F)<<12 | (hex & 0x00F0)<<4 | (hex & 0x0F00)>>4 | (hex & 0xF000)>>12));
//	printf(" ***********Exapmle************\n");
//	printf("*    input 10                 *\n");
//	printf("*    MSB = 1 and LSB = 0      *\n");
//	printf("********************************\n\n");
//	printf("Enter the msb and then lsb\n");
	scanf("%c%c",&msb,&lsb);
//	printf("MSB:%c and LSB : %c\n",msb,lsb);
	InputBuffer[1]=lsb;
	InputBuffer[0]=msb;	
	
	ConvertStringIntoInt = atoi(InputBuffer);
//	printf("Output in interger is %d\n",ConvertStringIntoInt);
	ConvertIntToTimeInMilisec = (ConvertStringIntoInt/deno);
//	printf("time in miliseconds is %.1f\n",ConvertIntToTimeInMilisec);
	
//	printf("********calculate the required pu6lses**************\n");
	ConvertIntToTimeInMilisec = (ConvertIntToTimeInMilisec*1000);//convert into microseconds
	Pulse = (ConvertIntToTimeInMilisec/clockPerCycle);
////	printf("pulses are: %llu\n",Pulse);
	NeedPulse = CompleteClock - Pulse;
//	printf("Need pulses are %llu\n",NeedPulse);
	
	while(NeedPulse != 0)
	{
		
		remainder = (NeedPulse % 16);
		//printf("%d",remainder);//ok
		if(remainder < 10){
			HexlevelBuffer[i]=remainder + 48;
			
		}
		else{
			HexlevelBuffer[i]=remainder + 55;		
		}
		i++;	
		NeedPulse = NeedPulse/16;		
		
	}
//	printf("Hex value is %s\n",HexlevelBuffer);
	reverseString(HexlevelBuffer);
	
}

void reverseString(char str[5]){
	int i,j;
	int start=0;
	int end = strlen(str)-1;
	char temp,strH[5];
	//printf("reverse string is %s",str);
	while(start <= end)
	{
		temp = str[start];
		str[start]=str[end];
		str[end]=temp;
		start++;
	}
	printf("Hex value is %s\n",str);
	 
	 for(i=0,j=0;i<strlen(str);i++,j+=2)
    { 
        sprintf((char*)strH+j,"%X",str[i]);
    }
    strH[j]='\0'; /*adding NULL in the end*/
     printf("Hexadecimal converted string is: \n");
    printf("%s\n",strH);
}

