//Agregar en include

//#include "tecladin.h"

//Para usarlo

//Variable_donde_se_guardara=teclado();
//ejemplo:
//numero=teclado();
int teclado(void)
{
   int16 numero;
   int16 valido=0;
   while(valido==0)
   {
      if(input(pin_B0)==0&&input(pin_B1)==0&&input(pin_B2)==0&&input(pin_B3)==0)
      {
         numero=0;
         valido=0;
      }
      if(input(pin_B0)==1&&input(pin_B1)==0&&input(pin_B2)==0&&input(pin_B3)==0)
      {
         numero=14;
         valido=1;
      }
      if(input(pin_B0)==0&&input(pin_B1)==1&&input(pin_B2)==0&&input(pin_B3)==0)
      {
         numero=13;
         valido=1;
      }
      if(input(pin_B0)==1&&input(pin_B1)==1&&input(pin_B2)==0&&input(pin_B3)==0)
      {
         numero=12;
         valido=1;
      }
      if(input(pin_B0)==0&&input(pin_B1)==0&&input(pin_B2)==1&&input(pin_B3)==0)
      {
         numero=15;
         valido=1;
      }
      if(input(pin_B0)==1&&input(pin_B1)==0&&input(pin_B2)==1&&input(pin_B3)==0)
      {
         numero=9;
         valido=1;
      }
      if(input(pin_B0)==0&&input(pin_B1)==1&&input(pin_B2)==1&&input(pin_B3)==0)
      {
         numero=6;
         valido=1;
      }
      if(input(pin_B0)==1&&input(pin_B1)==1&&input(pin_B2)==1&&input(pin_B3)==0)
      {
         numero=0;
         valido=1;
      }
      if(input(pin_B0)==0&&input(pin_B1)==0&&input(pin_B2)==0&&input(pin_B3)==1)
      {
         numero=8;
         valido=1;
      }
      if(input(pin_B0)==1&&input(pin_B1)==0&&input(pin_B2)==0&&input(pin_B3)==1)
      {
         numero=5;
         valido=1;
      }
      if(input(pin_B0)==0&&input(pin_B1)==1&&input(pin_B2)==0&&input(pin_B3)==1)
      {
         numero=7;
         valido=1;
      }
      if(input(pin_B0)==1&&input(pin_B1)==1&&input(pin_B2)==0&&input(pin_B3)==1)
      {
         numero=11;
         valido=1;
      }
      if(input(pin_B0)==0&&input(pin_B1)==0&&input(pin_B2)==1&&input(pin_B3)==1)
      {
         numero=3;
         valido=1;
      }
      if(input(pin_B0)==1&&input(pin_B1)==0&&input(pin_B2)==1&&input(pin_B3)==1)
      {
         numero=2;
         valido=1;
      }
      if(input(pin_B0)==0&&input(pin_B1)==1&&input(pin_B2)==1&&input(pin_B3)==1)
      {
         numero=1;
         valido=1;
      }
      if(input(pin_B0)==1&&input(pin_B1)==1&&input(pin_B2)==1&&input(pin_B3)==1)
      {
         numero=4;
         valido=1;
      }
   }
   return numero;
}
