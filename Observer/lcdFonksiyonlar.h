#ifndef _LCD_H
#define _LCD_H

#define RS P2OUT_bit.P6 // LCD'nin RS pini MCU'nun P2.6 pinine bağlandı.
#define E P2OUT_bit.P7 // LCD'nin E pini MCU'nun P2.7 pinine bağlandı.



extern void lcdBekle(int ms);
extern void lcdHucreyeGit(unsigned int satir, unsigned int sutun);
extern void lcdDegistir(void);
extern void lcdNeYazilacak(char ne);
extern void lcdGonder(char ne, int veri);
extern void lcdYaziYaz (char* yazi); 
extern void lcdHarfYaz (char harf);
extern void lcdTemizle(void);
extern void lcdHazirla(void);



#endif
