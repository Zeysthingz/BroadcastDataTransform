#include "lcdFonksiyonlar.h"
#include "io430.h"
#include "in430.h"



void lcdBekle(int ms)             // Bekleme fonksiyonu
{
  unsigned int i, j;
  i = ms;
  for(j = 0 ; j < 250 ; j++)
  {
     for( ; i > 0 ; i--);
     i = ms;
  }
}


void lcdEDegistir(void)           // E pini düşen kenar tetiklemeli.
{                                 // Bu yüzden gönderdiğimiz bilginin LCD üzerinde
  E = 1;                          // işlem görmesi için E ucuna düşen kenar uyguluyoruz.
  E = 0;                          // Bunu da E'yi önce "1", sonra "0" yaparak hallediyoruz.
}


void lcdNeYazilacak(char ne)      // LCD'ye giden bilgimiz veri ya da komut olabilir.
{
  if(ne == 'k')                   // Komut ise;
    RS = 0;                       // RS pini "0" yapılır.
  else if(ne == 'v')              // Veri ise;
    RS = 1;                       // RS pini "1" yapılır.
}


void lcdGonder(char ne, int veri) // Gidecek bilginin türünü ve bilgiyi aldık.
{
  P1OUT = veri;                   // Bilgiyi P1OUT'a bastık. Böylece LCD ile bağlantılı kısım en değerlikli dört bit oluyor.
  lcdNeYazilacak(ne);             // Fonksiyona gelen bilgi türünü diğer fonksiyona aktarıp RS pinini ayarladık.
  lcdEDegistir();                 // Bilginin aktif olması için E pinine düşen kenar uyguladık.
 
  veri = (veri << 4);             // Şimdi değerliksiz dört biti göndereceğiz. Veriyi dört bit sola kaydırdık.
                                  // Bu şekilde verinin değerliksiz bitleri, artık değerlikli kısımda.
  P1OUT = veri;                   // Değişen veriyi P1OUT'a bastık.
  lcdNeYazilacak(ne);             // RS pinini ayarladık.
  lcdEDegistir();                 // E pinine düşen kenar uyguladık.
}

void lcdHucreyeGit(unsigned int satir, unsigned int sutun)
{
  if(satir == 1)
  {
    sutun--;                      // 1-16 arasında giriş için 0-15 arası değer gelecek.
    lcdGonder('k', 0x80 + sutun); // Komut tablosuna göre 80-8F arasında değer gönderiliyor.
  }
 
  else if(satir == 2)
  {
    sutun--;                      // 0-15 arasında bir değer elde etmek için.
    lcdGonder('k', 0xC0 + sutun); // C0 - CF arası değer gönderiliyor.
  }
}

void lcdHazirla(void)
{
  lcdBekle(15);                   // Bir süre beklendi.
  lcdGonder('k', 0x02);           // "00000010" yani imleç en başa gidecek.
  lcdGonder('k', 0x28);           // "00101000" yani 4 pin, 2 satır, 5x7 karakter kullanılacak.
  lcdGonder('k', 0x0C);           // "00001000" yani imleç gizlenecek.
  lcdGonder('k', 0x06);           // "00000110" yani imleç yazılan her karakterden sonra sağa doğru kayacak.
  lcdGonder('k', 0x01);           // "00000001" yani LCD temizlenecek.
  lcdBekle(10);                   // Bir süre beklendi.
}



void lcdYaziYaz (char* yazi)      // Bir string bastırmak için fonksiyon.
{
  lcdBekle(1);                    // Bir süre beklendi.
  int i = 0;
  while(yazi[i])                  // Yazının sonuna gelene kadar,
  lcdGonder('v', yazi[i++]);      // Veriyi gönder ve bir sonraki veriye geç.
}

void lcdHarfYaz (char harf)       // Tek harf bastırmak için fonksiyon.
{
  lcdBekle(1);                    // Bir süre beklendi.
  lcdGonder('v', harf);           // Veriyi gönder.
}

void lcdTemizle(void)
{
  lcdGonder('k', 0x01);           // LCD temizlendi.
  lcdHucreyeGit(1, 1);            // 1. satır, 1. sütuna gidildi.
  lcdBekle(10);                   // Bir süre beklendi.
}