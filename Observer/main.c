#include<msp430g2553.h>
#include "lcdFonksiyonlar.h"
 
//foksiyon prototipleri
void UARTInit(void);
void TXData(unsigned char c);
void sendCommand(char string[]);
void Delay (int delaytime);
 // boyutu tanimlayinca icerik yazmama gerek yok.
int j = 0;
int respondstate = 0;
char response[300];
char response_temp[6]; 

void main(void){
  
        char string []={"AT+DISA?"};
        
        int a;
        int firstState;
	WDTCTL = WDTPW + WDTHOLD;// watchdog timer kapali
	//DCO clock frekansi ayarlari
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
         
             
	UARTInit();//Uart ayarlari cagiri
        P1DIR |= 0x01; // onceden set edilmis pin eski degerini koruyor yenside set ediliyor.
        
       /* LCD CONFIGURATION*/
        P1DIR |= 0xF0;                         // P1.4 - P1.7 arasi çikis yapildi.
        P2DIR = 0xFF;                         // Port 2 çikis yapildi.
        P2SEL = 0x00;    
        
	 
	__bis_SR_register(GIE);//kesmeler aktif
         
         
         lcdHazirla(); 
          
         while(1)
         {
           if (respondstate==1)
           {
             respondstate = 0;
             
             
             
             for(a=0; a<j; a++)
             {
               if(response[a]=='H')
               {
                response_temp[0] = response[a+10];
                response_temp[1] = response[a+11];
                response_temp[2] = '.';
                response_temp[3] = response[a+12];
                response_temp[4] = response[a+13];
                response_temp[5] = '\0';
                
                break;
               }
             }
             j = 0;

             lcdBekle(20); 
             lcdHucreyeGit(1, 2);                      
             lcdYaziYaz(response_temp);                        
             __no_operation();  
           }
           else
           {
             
             IE2 |= UCA0RXIE; 
             sendCommand(string);  
             Delay(700); 
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
 


#pragma vector=USCIAB0RX_VECTOR      
__interrupt void RX_ISR(void)  // ALICI KISIM INTERRUPTU 
{
       while (!(IFG2&UCA0TXIFG));
     //  UCA0TXBUF = UCA0RXBUF;  karsi  taraftan ilk karakter geldigi an interrupta girerim.interrupta her seferinde yeniden tanimlamalar yapilir.burdaki degiskeni
                                 // baska yerde kullanamam.
      response[j] = UCA0RXBUF;
      if ( response[j]== '\r')
      {
        respondstate = 1;
        //j = 0;
        IE2 &= ~UCA0RXIE;
      }
      j++;
      IFG2 &= ~UCA0RXIFG; //RXi temizler
}    

void TXData(unsigned char c)
{
	while(!(IFG2&UCA0TXIFG));//UCA0TXIFG UCA0TXBUF bos iken set edilir.
	UCA0TXBUF = c; // data gonderiliyor
}

void sendCommand(char string [])
{
    int i=0;
    while(string[i]!='\0')
    { 
     TXData(string[i]);
     i++;
    }
}


/* Delay Function */ 
void Delay (int delaytime)
{
  while(delaytime--)
    __delay_cycles(1000);

}

