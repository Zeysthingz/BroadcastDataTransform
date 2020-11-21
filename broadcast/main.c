
#include <msp430g2553.h>
#include <string.h>
#include <stdio.h>
 
/* Functions */
void UARTInit(void);
void sendCommand(char string[]); 
void TXData(unsigned char c);
void Delay (int);
void string2hexString(char*, char*);


/* Global Variables*/
char response [13];
int responseCounter = 0;
int responseState = 0;




int main(void){

        /* AT COMMANDS */
        char hex_temp_data[9]; 
        char temp [] ={"AT+TEMP?"}; 
        char advert[] = {"AT+PACK000000000000"}; 
        char temparatureData[5];
        
        
      

	int i  = 0;
        int j = 0;
      
        WDTCTL = WDTPW + WDTHOLD;       // watchdog timer kapalifr
	DCOCTL = 0;                     //DCO clock frekansi ayarlari
	BCSCTL1 = CALBC1_1MHZ;          // basic clock control system u
	DCOCTL = CALDCO_1MHZ;	
 
	UARTInit();                     //Uart ayarlari cagirildi
       
        
        __bis_SR_register(GIE);      // kesmeler aktif bundan 
     
        
        
        
        while(1)
        {
          
          if (responseState == 1) // Check all AT response is received.
          {
            
            for (i=8; i<13;i++)
            {
             if(response[i] == '.') //ham data geliyor .
               continue ;
             temparatureData[j] = response[i];
             j++; 
            }        
            
            j = 0;
            temparatureData[4] = '\0'; 
            
            /*Convert temperature data from char to hexadecimal format*/
            string2hexString(temparatureData, hex_temp_data); 
            
            
            
            advert[14] =hex_temp_data[7];
            advert[13] =hex_temp_data[6];
            advert[12] =hex_temp_data[5];
            advert[11] =hex_temp_data[4];
            advert[10] = hex_temp_data[3];
            advert[9] = hex_temp_data[2]; 
            advert[8] = hex_temp_data[1];  
            advert[7] = hex_temp_data[0]; 
            
            
           Delay(1000);
           sendCommand(advert);
           Delay(1000);
            
           responseState = 0;          
           responseCounter = 0;    

        }
        else
        {
          Delay(1000);
          sendCommand(temp);
          Delay(1000);
        }
          
    }
    
}



//Uart Ayarlari
void UARTInit(void){
	P1SEL |= (BIT1 + BIT2);//P1.1 = RXD P1.2 = TXD
	P1SEL2 |= (BIT1 + BIT2);//P1.1 = RXD P1.2 = TXD
	UCA0CTL1 = UCSSEL_2; // clock source SMCLK
	//UCOS16 = 1 icin Baudrate ayarlari
	UCA0BR0 = 104; // 9600 baundrate ayari 
	UCA0BR1 = 0;
	//Modulator ayari
	UCA0MCTL = UCBRS0;//formule gore bkz.
 
	UCA0CTL1 &= ~UCSWRST; // USCI islemleri icin UCSWRST registeri resetten çikti.
 
	IE2 |= UCA0RXIE; // RXD kesmesi aktif edildi
}


/*Tek bir karakter yollama Fonksiyonu*/
void TXData(unsigned char c){       // string parametre olarak buraya geliyor 
	while(!(IFG2&UCA0TXIFG));   // UCA0TXIFG UCA0TXBUF bos iken set edilir.
	UCA0TXBUF = c;              // data gonderiliyor burda da kacinci indekse onu yolluyorum buffera
}


#pragma vector=USCIAB0RX_VECTOR      
__interrupt void Zeynep_ISR(void)
{
  while (!(IFG2&UCA0TXIFG)); // TX Bufferi set ediliyor
  response[responseCounter++] = UCA0RXBUF; 
  
  if (responseCounter == 13)
  {
    responseCounter = 0;
    responseState = 1;
    //__bic_SR_register_on_exit(LPM0_bits);
  }
  
  UCA0TXBUF = UCA0RXBUF; // Rx bufferi doluysa bu interrupta gireblirim.burada tx e veri yazildiginda rx e yolluyor interrupta dusup geri veri aliyor.
  IFG2 &= ~UCA0RXIFG; //Xi temizler
 
}


/* Delay Function */ 
void Delay (int delaytime)
{
  while(delaytime--)
    __delay_cycles(1000);

}


/* Sensöre Komut Yollama Fonksiyonu*/
void sendCommand(char string[])
{ 
  int e=0;          
  while(string[e] != '\0') 
  {
     TXData(string[e]);
     e++;
  }
}

//function to convert ascii char[] to hex-string (char[])
void string2hexString(char* input, char* output)
{
    int loop = 0;
    int i = 0; 
    
    while(input[loop] != '\0')
    {
        sprintf((char*)(output+i),"%02X", input[loop]);
        loop+=1;
        i+=2;
    }
    
    //insert NULL at the end of the output string
    //output[i++] = '\0';

}