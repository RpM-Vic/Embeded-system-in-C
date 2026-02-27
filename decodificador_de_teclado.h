
int16 teclado(void)
///////////////////////////Prende y apaga del pin b0 a pin b3
//Mami, vamos a hacerlo  //Detecta el 1 lógico del pin b4 a pin b7
//fácil, deja que te coma//Así detectas dónde aprieta
//       enterita        //Repite hasta detectar una tecla apretada
///////////////////////////Devuelve un valor (preguntar cuál) Se hace 1 vez
/******************************************/
/******************************************/
/*                                        */
/*     Hecho por:                         */ 
/*     Humpty Alexander Dumpty            */
/*     DORA Te quiero muaaaaaaaaaaaaaaaa  */
/******************************************/
/******************************************/
//Update:cambio de pines y mamadas así
//Victor es gay pero lo respeto
//No se crean, es hombre, está soltero y busca chica de 18 a 30 años

{
    int validacion=0;
    int tecla;
    output_bit(pin_d0,0);
    output_bit(pin_d1,0);
    output_bit(pin_d2,0);
    output_bit(pin_d3,0);
    while(validacion!=2)
    {
        output_bit(pin_d0,1);
        if(input(pin_d4))
        {
            tecla=1;
            validacion =2;

        }
        if(input(pin_d5))
        {
            tecla=2;
            validacion =2;

        }
        if(input(pin_d6))
        {
            tecla=3;
            validacion =2;

        }
        if(input(pin_d7))
        {
            tecla=10;
            validacion =2;

        }
        output_bit(pin_d0,0);
        output_bit(pin_d1,1);
        if(input(pin_d4))
        {
            tecla=4;
            validacion =2;

        }
        if(input(pin_d5))
        {
            tecla=5;
            validacion =2;

        }
        if(input(pin_d6))
        {
            tecla=6;
            validacion =2;

        }
        if(input(pin_d7))
        {
            tecla=11;
            validacion =2;

        }
        output_bit(pin_d1,0);
        output_bit(pin_d2,1);
        if(input(pin_d4))
        {
            tecla=7;
            validacion =2;

        }
        if(input(pin_d5))
        {
            tecla=8;
            validacion =2;

        }
        if(input(pin_d6))
        {
            tecla=9;
            validacion =2;
        }
        if(input(pin_d7))
        {
            tecla=12;
            validacion =2;
        }
        output_bit(pin_d2,0);
        output_bit(pin_d3,1);
        if(input(pin_d4))
        {
            tecla=13;
            validacion =2;

        }
        if(input(pin_d5))
        {
            tecla=0;
            validacion =2;

        }
        if(input(pin_d6))
        {
            tecla=14;
            validacion =2;

        }
        if(input(pin_d7))
        {
            tecla=15;
            validacion =2;
        }
         output_bit(pin_d3,0);
    }
    
       return tecla;
}


