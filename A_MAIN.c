#include <main.h>
#include "tecladin.h"
#INCLUDE <string.h>
#include <stdlib.h>

#use standard_io(A)
#use standard_io(B)
#use standard_io(C)
#use standard_io(D)
#use standard_io(E)

int32 contador=0;                                                              //variables globales donde almacenamos el temporizador
int16 msegundos=0;                                                             //variable que utilizaremos para medir las milisegundos
int16 msegundos_alternativos1=0;
int16 msegundos_alternativos2=0;
int16 contador_alternativo1=0;
int16 contador_alternativo2=0;
#INT_TIMER0
void  TIMER0_isr(void) 
{ 
    if(contador<=8)                                                            //Como nuestra resolucion es de 128us entonces (1000us)/(128us)= 7.9
    {                                                                          //veces que ocupamos que se desborde nuestro timer
        contador++;
        contador_alternativo1++;
        contador_alternativo2++;
    }
    else
    {
        contador_alternativo1=0;
        contador_alternativo2=0;
        contador=0;
        msegundos_alternativos1++;
        msegundos_alternativos2++;
        msegundos++;                                                             //cuando se alcanza el minuto, se reinicia el contador de minutos
    }
    if(contador==7)               
    {
        set_timer0(27457);                                                      //variable utilizada para evitar la perdida de informacion
    }  
    
}

int32 contador1=0;                                                             //variables globales donde almacenamos el temporizador
int16 segundo1=0;                                                              //variable que utilizaremos para medir las milisegundos
#INT_TIMER1
void  TIMER1_isr(void)                                                          //32.7 ms overflow 
{
    if(contador1<=31)                                                           //Como nuestra resolucion es de 32.7ms entonces (500ms)/(32.7ms)= 
    {                                                                          //veces que ocupamos que se desborde nuestro timer
        contador1++;
    }
    else
    {
        contador1=0;
        segundo1++;                                                       //cuando se alcanza el minuto, se reinicia el contador de minutos
    }
    if(contador1==7)               
    {
        set_timer1(27457);                                                      //variable utilizada para evitar la perdida de informacion
    }
}

int32 contador2=0;
int segundo2=0;
#INT_TIMER2                                                                     //0.5 us overflow, 0.5 us interrupt
void  TIMER2_isr(void) 
{
    if(contador2<=1000)                                                           //Como nuestra resolucion es de 32.7ms entonces (500ms)/(32.7ms)= 
    {                                                                          //veces que ocupamos que se desborde nuestro timer
        contador2++;
    }
    else
    {
        contador2=0;
        segundo2++;                                                       //cuando se alcanza el minuto, se reinicia el contador de minutos
    }
}

#include <lcd.c>

void main()
{
    //port_B_pullups(0xFF);
    setup_timer_0(RTCC_INTERNAL|RTCC_DIV_1|RTCC_8_bit);                         //128 us overflow
    setup_timer_1(T1_INTERNAL|T1_DIV_BY_1);                                     //32.7 ms overflow
    setup_timer_2(T2_DIV_BY_4,255,1);                                             //0.5 us overflow, 0.5 us interrupt
    //setup_timer_3(T3_INTERNAL | T3_DIV_BY_1);
    

    setup_oscillator(OSC_8MHZ|OSC_INTRC);
    
    
    enable_interrupts(INT_TIMER0);
    enable_interrupts(INT_TIMER1);
    enable_interrupts(INT_TIMER2);
    //enable_interrupts(INT_TIMER3);
    enable_interrupts(GLOBAL);
    
    set_tris_a(0b00001111);                                                     //purto utilzado para la entrada de variables diversas
    set_tris_b(0b11111111);                                                     //puerto utilizado para  la recepcion de datos desde el teclado
    set_tris_c(0b10110000);                                                     //puesrto utilizado para entrada y salida de variables diversas
    set_tris_e(0b00000000);                                                     //puerto para visualizar bombas encendidads
    
    char cadena[100],*string;                                                   //cadena para la recepcion de datos del puesrto serial
    char delimiter[2];
    char *ptr;
    char nvc[6],lec[6],ve2[6],ve1[6],vea[6],sptma[6],sptmi[6],vct[6],ta[6],ln2[6],ln1[6],ln0[6],let[6];
    int16 kbhitt;
    
    /******************************************************************************/   
    /******************PUNTOS DE CONSIGNA******************************************/
    /******************************************************************************/             
    
    int32 n0=100;                                                               //niveles de taanke guardados en eeprom
    int32 n1=400;
    int32 n2=1000;
    
    //direcciones//
    
    long int en00=0xf00000;
    long int en01=0xf00001;                                                     //direcciones donde se guardan los niveles de tanke
    long int en10=0xf00002;
    long int en11=0xf00003;
    long int en20=0xf00004;
    long int en21=0xf00005;
                                                                               //escribir variables en eeprom y despues reasignadas en la misma variable
    write_eeprom(en00,n0);                                                      
    write_eeprom(en01,n0>>8);
    int16 n01=read_eeprom(en01)<<8;                                            //n0
    n0=n01+read_eeprom(en00);
    
    write_eeprom(en10,n1);
    write_eeprom(en11,n1>>8);
    int16 n11=read_eeprom(en11)<<8;                                            //n1
    n1=n11+read_eeprom(en10);
    
    write_eeprom(en20,n2);
    write_eeprom(en21,n2>>8);
    int16 n21=read_eeprom(en21)<<8;                                            //n3
    n2=n21+read_eeprom(en20);
    
    ///////////////////////////////////////////
    int32 TEMPmax=80;                                                             
    int32 TEMPmin=40;
    int32 cambio_de_temp=5;
    
    //direcciones//
    long int eTEMPmax=0xf00006;
    long int eTEMPmin=0xf00007;
    long int ecambio_de_temp=0xf00008;                                          //variables de temperatura
    
    write_eeprom(eTEMPmax,TEMPmax); 
    write_eeprom(eTEMPmin,TEMPmin);
    write_eeprom(ecambio_de_temp,cambio_de_temp);
    
    TEMPmax=read_eeprom(eTEMPmax);
    TEMPmin=read_eeprom(eTEMPmin);
    cambio_de_temp=read_eeprom(ecambio_de_temp);
    
    /////////////////////////////////////////////////  
    
    int16 max_veces_bombas=15;
    
    int32 sis_llena=40000;
    
    int32 sis_vacia=500;
    
    int32 variacion_de_consumo_de_tanke=5;
    int32 consumo_maximo_de_tanke=60;
    
    //direcciones
    
    int16 emax_veces_bombas=0xf00009;
    
    int16 esis_llena1=0xf0000A;
    int16 esis_llenap2=0xf0000B;
    int16 esis_vacia=0xf0000C;
    int16 esis_vaciap2=0xf0000D;
    
    
    write_eeprom(esis_llena1,sis_llena);
    write_eeprom(esis_llenap2,sis_llena>>8);
    int16 sis_llenap2=read_eeprom(esis_llenap2)<<8;                            //sisterna llena             
    sis_llena=sis_llenap2+read_eeprom(esis_llena1);
    
    
    write_eeprom(esis_vacia,sis_vacia);
    write_eeprom(esis_vaciap2,sis_vacia>>8);
    int16 sis_vaciap2=read_eeprom(esis_vaciap2)<<8;                            //sis_vacia
    sis_vacia=sis_vaciap2+read_eeprom(esis_vacia);
    
    int16 evariacion_de_consumo_de_tanke=0xf0000D;
    int16 econsumo_maximo_de_tanke=0xf0000E;                               
    
    write_eeprom(emax_veces_bombas,max_veces_bombas);
    
    
    write_eeprom(evariacion_de_consumo_de_tanke,variacion_de_consumo_de_tanke);
    write_eeprom(econsumo_maximo_de_tanke,consumo_maximo_de_tanke);
    
    //variables en eprom que no se modifican desde teclado
    int16 ecount_B1=0xf0000F;
    int16 ecount_B2=0xf00010;
    int16 ecount_b=0xf00011;
    
    int32 count_B1=15;
    int32 count_B2=6;
    int32 count_b=3;
    
    write_eeprom(ecount_B1,count_B1);                                           //contadores de bombas
    write_eeprom(ecount_B2,count_B2);
    write_eeprom(ecount_b,count_b);
    
    count_B1=read_eeprom(ecount_B1);
    count_B2=read_eeprom(ecount_B2);
    count_b=read_eeprom(ecount_b);
    
    int32 temperatura=45;
    int16 etemperatura=0xf00012;
    write_eeprom(etemperatura,temperatura);
    temperatura=read_eeprom(etemperatura);                                      //temperatura
    
    int32 consumo_tanke=15;
    int16 econsumo_tanke=0xf00015;
    write_eeprom(econsumo_tanke,consumo_tanke);
    consumo_tanke=read_eeprom(econsumo_tanke);
    
    int32 consumo_sis;
    int16 econsumo_sis=0xf00016;
    write_eeprom(econsumo_sis,consumo_sis);
    consumo_sis=read_eeprom(econsumo_sis);
    
    
    int32 litros_actuales_tanke=50;
    int16 elitros_actuales_tankep1=0xf00013;
    int16 elitros_actuales_tankep2=0xf00014;    
    
    write_eeprom(elitros_actuales_tankep1,litros_actuales_tanke);
    write_eeprom(elitros_actuales_tankep2,litros_actuales_tanke>>8);    
    int16 litros_actuales_tankep2=read_eeprom(elitros_actuales_tankep2)<<8;                                            
    litros_actuales_tanke=litros_actuales_tankep2+read_eeprom(elitros_actuales_tankep1);
    
    int32 nivel_sis=300;
    
    int16 enivel_sis1=0xf00017;
    int16 enivel_sis2=0xf00018;
    //invel_sis
    write_eeprom(enivel_sis1,nivel_sis);
    write_eeprom(enivel_sis2,nivel_sis>>8);
    int16 nivel_sis2=read_eeprom(enivel_sis2)<<8;
    nivel_sis=nivel_sis2+read_eeprom(enivel_sis1);
    
    /*write_eeprom(en20,n2);
    write_eeprom(en21,n2>>8);
    int16 n21=read_eeprom(en21)<<8;                                            //n3
    n2=n21+read_eeprom(en20);*/
    
    /******************************************************************************/
    /*********************MAS VARIABLES  SIN EEPROM    ****************************/
    /******************************************************************************/
    
    
    short validar_pin_c2=0;
    short validar_pin_b5=0;
    //short validar_pin_c5=0;
    short validar_pin_c1=0;
    
    short validar_pin_d3=0;
    
    short validar_pin_a0=0;
    short validar_pin_a2=0;
    short validar_pin_a3=0;
    
    short validar_pin_b6=0;
    short validar_pin_b4=0;
    
    short validar_pin_a1=0;
    int vaciador_de_tanke=0;
        
    
    int lectura;
    int16 lectura_aux;
    int i=1;
    
    int alternar=0;
    int validar_alternar=0;
    
    int  pantalla_actual=1;
    short validar_contador_b=1;
    short validar_contador_B1=1;
    short validar_contador_B2=1;
    int validar_aumento_tanke=0;
    
    short encendedor_b=0;
    short encendedor_Bs=0;
    /////////////////// variables on/off de leds en labview ///////////////////
    int16 B1on=0;
    int16 B2on=0;
    int16 bon=0;
    int16 n0on=0;
    int16 n1on=0;
    int16 n2on=0;
    int16 rojo=0;
    int16 amarillo=0;
    int16 verde=0;
    
    
    //read_eeprom();
    lcd_init();
    
    
    
    /****************************************************************************************************/    
    while(TRUE)
    {
        /******************Rutina para cambiar valores ********************************/
        /******************      a las consignas       ********************************/
        if(input(pin_d3))
            validar_pin_d3=1;
        if(validar_pin_d3 && input(pin_d3)==0)
        {
            validar_pin_d3=0;
            printf(LCD_PUTC,"\fpress a key to\nchange consignas");
            lectura=teclado();
            delay_ms(500);
            if(lectura)
            {  /////////////////////////////////////////////////////////////////
                lectura_aux=0;
                printf(LCD_PUTC,"\fnivel 0 de bomba\n%04Li",lectura_aux);
                for(i=0;i<=3;i++)               
                {
                    lectura=teclado();
                    delay_ms(500);
                    lectura_aux=lectura_aux*10.0;
                    lectura_aux=lectura_aux+lectura;
                    lcd_gotoxy(1,1);                                              //n0
                    printf(LCD_PUTC,"nivel 0 de bomba\n%04Li",lectura_aux);
                }
                n0=lectura_aux;
                write_eeprom(en00,n0);
                write_eeprom(en01,n0>>8);
                n01=read_eeprom(en01)<<8;                                           
                n0=n01+read_eeprom(en00);              
                printf(LCD_PUTC,"\fnivel 0 de bomba\n=%Li",n0);
                delay_ms(2000);
                
                ///////////////////////////////////////////////////////////////////
                lectura_aux=0;
                printf(LCD_PUTC,"\fnivel 1 de bomba\n%04Li",lectura_aux);
                for(i=0;i<=3;i++)               
                {
                    lectura=teclado();
                    delay_ms(500);
                    lectura_aux=lectura_aux*10.0;
                    lectura_aux=lectura_aux+lectura;
                    lcd_gotoxy(1,1);                                              //n1
                    printf(LCD_PUTC,"nivel 1 de bomba\n%04Li",lectura_aux);
                }
                n1=lectura_aux;
                write_eeprom(en10,n1);
                write_eeprom(en11,n1>>8);
                n01=read_eeprom(en11)<<8;                                           
                n0=n01+read_eeprom(en10);              
                printf(LCD_PUTC,"\fnivel 1 de bomba\n=%Li",n1);
                delay_ms(2000);
                
                ///////////////////////////////////////////////////////////////////
                lectura_aux=0;
                printf(LCD_PUTC,"\fnivel 2 de bomba\n%04Li",lectura_aux);
                for(i=0;i<=3;i++)               
                {
                    lectura=teclado();
                    delay_ms(500);
                    lectura_aux=lectura_aux*10.0;
                    lectura_aux=lectura_aux+lectura;
                    lcd_gotoxy(1,1);                                              //n2
                    printf(LCD_PUTC,"nivel 2 de bomba\n%04Li",lectura_aux);
                }
                n2=lectura_aux;
                write_eeprom(en20,n2);
                write_eeprom(en21,n2>>8);
                n01=read_eeprom(en21)<<8;                                           
                n0=n01+read_eeprom(en20);              
                printf(LCD_PUTC,"\fnivel 2 de bomba\n=%Li",n2);
                delay_ms(2000);     
                
                ///////////////////////////////////////////////////////////////////
                lectura_aux=0;
                printf(LCD_PUTC,"\fTemperatura max\n%Li",lectura_aux);
                for(i=0;i<=1;i++)               
                {
                    lectura=teclado();
                    delay_ms(500);
                    lectura_aux=lectura_aux*10.0;
                    lectura_aux=lectura_aux+lectura;
                    lcd_gotoxy(1,1);                                              //TEMPmax
                    printf(LCD_PUTC,"\fTemperatura max\n%Li",lectura_aux);
                }
                TEMPmax=lectura_aux;
                write_eeprom(eTEMPmax,TEMPmax);                                         
                TEMPmax=read_eeprom(eTEMPmax);              
                printf(LCD_PUTC,"\fTemperatura max\n=%Li",TEMPmax);
                delay_ms(2000);  
                
                ///////////////////////////////////////////////////////////////////
                lectura_aux=0;
                printf(LCD_PUTC,"\fTemperatura min\n%Li",lectura_aux);
                for(i=0;i<=1;i++)               
                {
                    lectura=teclado();
                    delay_ms(500);
                    lectura_aux=lectura_aux*10.0;
                    lectura_aux=lectura_aux+lectura;
                    lcd_gotoxy(1,1);                                              //TEMPmin
                    printf(LCD_PUTC,"\fTemperatura min\n%Li",lectura_aux);
                }
                TEMPmin=lectura_aux;
                write_eeprom(eTEMPmin,TEMPmin);                                         
                TEMPmin=read_eeprom(eTEMPmin);              
                printf(LCD_PUTC,"\fTemperatura min\n=%Li",TEMPmin);
                delay_ms(2000);                  
                
                
                ///////////////////////////////////////////////////////////////////
                lectura_aux=0;
                printf(LCD_PUTC,"\fPaso de temperat\n%Li",lectura_aux);
                for(i=0;i<=1;i++)               
                {
                    lectura=teclado();
                    delay_ms(500);
                    lectura_aux=lectura_aux*10.0;
                    lectura_aux=lectura_aux+lectura;
                    lcd_gotoxy(1,1);                                              //cambio_de_temp
                    printf(LCD_PUTC,"\fPaso de temperat\n%Li",lectura_aux);
                }
                cambio_de_temp=lectura_aux;
                write_eeprom(ecambio_de_temp,cambio_de_temp);                                         
                cambio_de_temp=read_eeprom(ecambio_de_temp);              
                printf(LCD_PUTC,"\fPaso de temperat\n=%Li",cambio_de_temp);
                delay_ms(2000);   
                
                ///////////////////////////////////////////////////////////////////
                lectura_aux=0;
                printf(LCD_PUTC,"\fcisterna vacia\n%Li",lectura_aux);
                for(i=0;i<=3;i++)               
                {
                    lectura=teclado();
                    delay_ms(500);
                    lectura_aux=lectura_aux*10.0;
                    lectura_aux=lectura_aux+lectura;
                    lcd_gotoxy(1,1);                                              //sis_vacia
                    printf(LCD_PUTC,"\fcisterna vacia\n%Li",lectura_aux);
                }
                sis_vacia=lectura_aux;
                write_eeprom(esis_vacia,sis_vacia);
                write_eeprom(esis_vaciap2,sis_vacia>>8);
                sis_vaciap2=read_eeprom(esis_vaciap2)<<8;                        
                sis_vacia=sis_vaciap2+read_eeprom(esis_vacia);            
                printf(LCD_PUTC,"\fcisterna vacia\n=%Li",sis_vacia);
                delay_ms(2000);   
                
                
                ///////////////////////////////////////////////////////////////////
                lectura_aux=0;
                printf(LCD_PUTC,"\fcisterna llena\n%Li",lectura_aux);
                for(i=0;i<=4;i++)               
                {
                    lectura=teclado();
                    delay_ms(500);
                    lectura_aux=lectura_aux*10.0;
                    lectura_aux=lectura_aux+lectura;
                    lcd_gotoxy(1,1);                                              //sis_llena
                    printf(LCD_PUTC,"\fcisterna llena\n%Li",lectura_aux);
                }
                sis_llena=lectura_aux;
                write_eeprom(esis_llena1,sis_llena);
                write_eeprom(esis_llenap2,sis_llena>>8);
                sis_llenap2=read_eeprom(esis_llenap2)<<8;                            //sisterna llena             
                sis_llena=sis_llenap2+read_eeprom(esis_llena1);          
                printf(LCD_PUTC,"\fcisterna llena\n=%Li",sis_llena);
                delay_ms(2000);  
                
                ///////////////////////////////////////////////////////////////////
                lectura_aux=0;
                printf(LCD_PUTC,"\fpaso de consumo\n%Li",lectura_aux);
                for(i=0;i<=1;i++)               
                {
                    lectura=teclado();
                    delay_ms(500);
                    lectura_aux=lectura_aux*10.0;
                    lectura_aux=lectura_aux+lectura;
                    lcd_gotoxy(1,1);                                              //variacion_de_consumo_de_tenke
                    printf(LCD_PUTC,"\fpaso de consumo\n%Li",lectura_aux);
                }
                variacion_de_consumo_de_tanke=lectura_aux;
                write_eeprom(evariacion_de_consumo_de_tanke,variacion_de_consumo_de_tanke);                                         
                variacion_de_consumo_de_tanke=read_eeprom(evariacion_de_consumo_de_tanke);              
                printf(LCD_PUTC,"\fpaso de consumo\n=%Li",variacion_de_consumo_de_tanke);
                delay_ms(2000);     
                
                
                ///////////////////////////////////////////////////////////////////
                lectura_aux=0;
                printf(LCD_PUTC,"\fconsumo maximo\n%Li",lectura_aux);
                for(i=0;i<=1;i++)               
                {
                    lectura=teclado();
                    delay_ms(500);
                    lectura_aux=lectura_aux*10.0;
                    lectura_aux=lectura_aux+lectura;
                    lcd_gotoxy(1,1);                                              //consumo_maximo_de_tanke
                    printf(LCD_PUTC,"\fmaximo\n%Li",lectura_aux);
                }
                consumo_maximo_de_tanke=lectura_aux;
                write_eeprom(econsumo_maximo_de_tanke,consumo_maximo_de_tanke);                                        
                consumo_maximo_de_tanke=read_eeprom(econsumo_maximo_de_tanke);              
                printf(LCD_PUTC,"\fconsumo maximo\n=%Li",consumo_maximo_de_tanke);
                delay_ms(2000); 
                
            }        
        }
        
        /************ rutina para mostrar datos en pantalla ***************************/   
        
        
        if(input(pin_b6))
        {
            validar_pin_b6=1;
        }
        if(validar_pin_b6 && input(pin_b6)==0)
        {
            pantalla_actual++;
            validar_pin_b6=0;
            printf(LCD_PUTC,"\f");
        }
        if(pantalla_actual>=4)
        {
            pantalla_actual=1;
        }
        
        
        switch (pantalla_actual) 
        {
        
        case 1:                                                               //pantalla 1
            lcd_gotoxy(1,1);
            printf(LCD_PUTC, "T=%02LiC Tanke=%04Li\nB1=%02Li B2=%02Li b=%02Li"temperatura,litros_actuales_tanke,count_B1,count_B2,count_b);
            break;
            
        case 2:                                                               //pantalla 2
            lcd_gotoxy(1,1);
            printf(LCD_PUTC,"Consumo tanke=%02Li\nConsumo cis=%02Li",consumo_tanke,consumo_sis);
            break;
            
        case 3:                                                               //pantalla 3
            lcd_gotoxy(1,1);
            printf(lcd_putc,"Nivel cisterna=\n%Li               ",nivel_sis);
            
            break; 
        }
        /***************Rutina de histeresis para la temperatura**********************/
        if(temperatura>=read_eeprom(eTEMPmax))
        {
            validar_pin_c1=1;
        }
        
        if(validar_pin_c1)
        {
            amarillo=1;
            if(msegundos%400<=100 ) 
            {
                output_high(pin_c1);
            }
            else
                output_low(pin_c1);
            if(msegundos_alternativos2>500)
            {  
               temperatura-=5; 
               write_eeprom(etemperatura,temperatura);
               temperatura=read_eeprom(etemperatura);
               
               msegundos_alternativos2=0;
            } 
        }
        
        if(temperatura<=read_eeprom(eTEMPmin))
        {
            validar_pin_c1=0;
            output_low(pin_c1);
            amarillo=0;
        }
        
        /********* rutina para encender alarma de cisterna vacia   ************/
        if(nivel_sis<=sis_vacia)                                                 //alarma visible e intermitente
        {
            validar_pin_c2=1;
        }
        
        if(validar_pin_c2)
        {
            rojo=1;
            if(msegundos%400<=100 ) 
            {
                output_high(pin_c2);
            }
            else
                output_low(pin_c2);
            
        }
        if(nivel_sis>sis_vacia)
        {
            validar_pin_c2=0;
            output_low(pin_c2);
            rojo=0;
        }
        
        
        /***************   Incrementar temperatura ***********************************/
        if(input(pin_a2))
        {
            validar_pin_a2=1;
        }
        
        if(validar_pin_a2 && input(pin_a2)==0)
        {
            temperatura+=read_eeprom(ecambio_de_temp);
            write_eeprom(etemperatura,temperatura);
            validar_pin_a2=0;
        }
        /***************   menguar temperatura ***********************************/
        if(input(pin_a3))
        {
            validar_pin_a3=1;
        }
        
        if(validar_pin_a3 && input(pin_a3)==0)
        {
            temperatura=temperatura-read_eeprom(ecambio_de_temp);
            write_eeprom(etemperatura,temperatura);
            validar_pin_a3=0;
        }
        /**************   rutina para llenar sisterna  ****************************/
        if(input(pin_b5))
        {
            validar_pin_b5=1;
        }
        if(validar_pin_b5 && input(pin_b5)==0)
        {
            validar_pin_b5=0;
            nivel_sis=sis_llena;
            
            write_eeprom(enivel_sis1,nivel_sis);
            write_eeprom(enivel_sis2,nivel_sis>>8);
            nivel_sis2=read_eeprom(enivel_sis2)<<8;
            nivel_sis=nivel_sis2+read_eeprom(enivel_sis1);
        }
        /***************     rutina para alternar entre las bombas ***********/

        if(litros_actuales_tanke <n0  && nivel_sis>sis_vacia && litros_actuales_tanke<=n2)                   //encender bomba auxiliar
        {
            
            output_high(pin_e2);                       
            output_high(pin_C0);                                               //encender torreta
            bon=1;
            verde=1;
            write_eeprom(econsumo_sis,65);
            consumo_sis=read_eeprom(econsumo_sis);
            if(validar_contador_b)
            {
               validar_contador_b=0;
               count_b++;
               write_eeprom(ecount_b,count_b);
               encendedor_b=1;
            }
        }
        
        if(litros_actuales_tanke >=n1 && litros_actuales_tanke < n2 && nivel_sis>sis_vacia)                   //apagar bomba auxiliar
        {
            validar_contador_b=1;
            output_low(pin_E2); 
            bon=0;
            write_eeprom(econsumo_sis,40);
            consumo_sis=read_eeprom(econsumo_sis);
            encendedor_b=0;  
        }
        
        if(litros_actuales_tanke >=n2)                                           //apagar bombas 1 y 2
        {
            validar_contador_B1=1;
            validar_contador_B2=1;
            validar_alternar=1;
            
            output_low(pin_e0);
            output_low(pin_e1);
            output_low(pin_c0);
            
            B1on=0;
            B2on=0;
            verde=0;
            
            write_eeprom(econsumo_sis,0);
            consumo_sis==read_eeprom(econsumo_sis);
            encendedor_Bs=0;
        }
        if(nivel_sis <= sis_vacia)
        {
            output_low(pin_e0);
            output_low(pin_e1);
            output_low(pin_c0);
            
            B1on=0;
            B2on=0;
            verde=0;
            
            write_eeprom(econsumo_sis,0);
            consumo_sis==read_eeprom(econsumo_sis);
            encendedor_Bs=0;
        }
        
        if(validar_alternar)
        {
            alternar++;
            validar_alternar=0;
        }
        
        if(litros_actuales_tanke <=n1 && alternar%2==0 && nivel_sis>sis_vacia)
        {
            if(validar_contador_B1 && count_B1< max_veces_bombas)
            {
               validar_contador_B1=0;
               count_B1++;
               write_eeprom(ecount_B1,count_B1);
            }
            encendedor_Bs=1;
            output_high(pin_E0);
            output_high(pin_c0);           
            
            B1on=1;
            verde=1;
        
        }
        
        if(litros_actuales_tanke <=n1 && alternar%2==1 &&  nivel_sis>sis_vacia)
        {
            if(validar_contador_B2)
            {
               validar_contador_B2=0;
               count_B2++;
               write_eeprom(ecount_B2,count_B2);
            }
            encendedor_Bs=1;
            output_high(pin_E1);
            output_high(pin_c0);
            
            B2on=1;
            verde=1;
        }
                    
            
        if(msegundos_alternativos1>1000)
        {  
            if(encendedor_Bs || encendedor_b)
            {
            litros_actuales_tanke+=consumo_sis;
            nivel_sis-=consumo_sis;     
               
            write_eeprom(elitros_actuales_tankep1,litros_actuales_tanke);
            write_eeprom(elitros_actuales_tankep2,litros_actuales_tanke>>8);    
            litros_actuales_tankep2=read_eeprom(elitros_actuales_tankep2)<<8;                                            
            litros_actuales_tanke=litros_actuales_tankep2+read_eeprom(elitros_actuales_tankep1);
    
            write_eeprom(enivel_sis1,nivel_sis);
            write_eeprom(enivel_sis2,nivel_sis>>8);
            nivel_sis2=read_eeprom(enivel_sis2)<<8;
            nivel_sis=nivel_sis2+read_eeprom(enivel_sis1);
            msegundos_alternativos1=0;
            }                         
               
        } 
        /****************    rutina para vaciar tanke   **********************/
        if(input(pin_a1))
            validar_pin_a1=1;
        if(validar_pin_a1 && input(pin_a1)==0)
        {
            validar_pin_a1=0;
            vaciador_de_tanke++;        
        }
        
        
        if(vaciador_de_tanke%2 && litros_actuales_tanke>0)
        {
            if(segundo1>=1)
            {
               validar_aumento_tanke=0;
               litros_actuales_tanke-=consumo_tanke;
       
               write_eeprom(elitros_actuales_tankep1,litros_actuales_tanke);
               write_eeprom(elitros_actuales_tankep2,litros_actuales_tanke>>8);    
               litros_actuales_tankep2=read_eeprom(elitros_actuales_tankep2)<<8;                                            
               litros_actuales_tanke=litros_actuales_tankep2+read_eeprom(elitros_actuales_tankep1);              
               segundo1=0;
            }
        }

        /************** rutina para variar consumo del tanke *****************/
        if(input(pin_a0))
            validar_pin_a0=1;
        if(validar_pin_a0 && input(pin_a0)==0 && consumo_tanke<=consumo_maximo_de_tanke)
        {
            validar_pin_a0=0;
            consumo_tanke+=variacion_de_consumo_de_tanke;
        }
        if(consumo_tanke>consumo_maximo_de_tanke)
           consumo_tanke=consumo_maximo_de_tanke;
        
        
        
        if(input(pin_b4))
            validar_pin_b4=1;
        if(validar_pin_b4 && input(pin_b4)==0 && consumo_tanke>0)
        {
            validar_pin_b4=0;
            consumo_tanke-=variacion_de_consumo_de_tanke;
        }
           
        /****************    rutina para encender niveles de tanque **********/
        if(litros_actuales_tanke<=n0)
        {
            output_low(pin_a4);
            output_low(pin_a5);
            output_low(pin_a6);
            
            n2on=0;
            n1on=0;
            n0on=0;
        }
        if(litros_actuales_tanke>n0 && litros_actuales_tanke<=n1)
        {
            output_low(pin_a4);
            output_low(pin_a5);
            output_high(pin_a6);
            n2on=0;
            n1on=0;
            n0on=1;
            
        }
        if(litros_actuales_tanke>n1 && litros_actuales_tanke<=n2)
        {
            output_low(pin_a4);
            output_high(pin_a5);
            output_high(pin_a6);
            n2on=0;
            n1on=1;
            n0on=1;
        }
        if(litros_actuales_tanke>=n2)
        {
            output_high(pin_a4);
            output_high(pin_a5);
            output_high(pin_a6);
            n2on=1;
            n1on=1;
            n0on=1;            
        }
        
        /*****************  rutina para reiniciar contador de bombas  ***************/
        count_B1=read_eeprom(ecount_B1);
        count_B2=read_eeprom(ecount_B2);
        count_b=read_eeprom(ecount_b);
        max_veces_bombas=read_eeprom(emax_veces_bombas);
        
        
        write_eeprom(ecount_B1,count_B1);
        write_eeprom(ecount_B2,count_B2);
        write_eeprom(ecount_b,count_b);
        write_eeprom(emax_veces_bombas,max_veces_bombas);
        
        if(count_B1>=max_veces_bombas && input(pin_B7))
        {
            count_B1=0;
            write_eeprom(ecount_B1,count_B1);
        }
        if(count_B2>=max_veces_bombas && input(pin_B7))
        {
            count_B2=0;
            write_eeprom(ecount_B2,count_B2);
        }
        if(count_b>=max_veces_bombas && input(pin_B7))
        {
            count_b=0;
            write_eeprom(ecount_b,count_b);
        }

        
        
        /****************  rutina para enviar datos rs 232 ***********************/
        if(segundo1>=1)
            {
               printf("@%05Li/%05Li/%03Li/%03Li/%03Li/%03Li/%03Li/%02Li/%03Li/%05Li/%05Li/%05Li/%04Li/%1Li/%1Li/%1Li/%1Li/%1Li/%1Li/%1Li/%1Li/%1Li",sis_vacia,nivel_sis,count_B2,count_B1,count_b,TEMPmax,TEMPmin,variacion_de_consumo_de_tanke,temperatura,n2,n1,n0,litros_actuales_tanke,B1on,B2on,bon,n0on,n1on,n2on,rojo,amarillo,verde);             
               segundo1=0;
            }
        
        /*if(input(pin_a0))
            validar_pin_a0=1;
        if(validar_pin_a0 && input(pin_a0)==0)
        {
            printf("@%05Li/%05Li/%03Li/%03Li/%03Li/%03Li/%03Li/%02Li/%03Li/%05Li/%05Li/%05Li/%04Li",sis_vacia,nivel_sis,count_B2,count_B1,count_b,TEMPmax,TEMPmin,variacion_de_consumo_de_tanke,temperatura,n2,n1,n0,litros_actuales_tanke);
            validar_pin_a0=0;
        }*/   
        
        /****************  rutina para leer datos rs 232  ************************/
        kbhitt=kbhit();//Si no funciona le pones un cero entre el paréntesis
        if(kbhitt)
        {
            string=getc();
            strcpy(cadena,string);
            strcpy(delimiter,",");
            ptr = strtok(cadena, delimiter); 
            strcpy(nvc, ptr);
            ptr = strtok(0, delimiter);
            strcpy(lec, ptr); 
            ptr = strtok(0, delimiter);
            strcpy(ve2, ptr);
            ptr = strtok(0, delimiter);
            strcpy(ve1, ptr);
            ptr = strtok(0, delimiter);
            strcpy(vea, ptr);
            ptr = strtok(0, delimiter);
            strcpy(sptma, ptr);
            ptr = strtok(0, delimiter);
            strcpy(sptmi, ptr);
            ptr = strtok(0, delimiter);
            strcpy(vct, ptr);
            ptr = strtok(0, delimiter);
            strcpy(ta, ptr);
            ptr = strtok(0, delimiter);
            strcpy(ln2, ptr);
            ptr = strtok(0, delimiter);
            strcpy(ln1, ptr);
            ptr = strtok(0, delimiter);
            strcpy(ln0, ptr);
            ptr = strtok(0, delimiter);
            strcpy(let, ptr);
            
            sis_vacia=atoi32(nvc);
            nivel_sis=atoi32(lec);
            count_b2=atoi32(ve2);
            count_b1=atoi32(ve1);
            count_b=atoi32(vea);
            TEMPmax=atoi32(sptma);
            TEMPmin=atoi32(sptmi);
            variacion_de_consumo_de_tanke=atoi32(vct);
            temperatura=atoi32(ta);
            n2=atoi32(ln2);
            n1=atoi32(ln1);
            n0=atoi32(ln0);
            litros_actuales_tanke=atoi32(let);
            
            write_eeprom(en00,n0);
            write_eeprom(en01,n0>>8);
            n01=read_eeprom(en01)<<8;                                            //n0
            n0=n01+read_eeprom(en00);
        
            write_eeprom(en10,n1);
            write_eeprom(en11,n1>>8);
            n11=read_eeprom(en11)<<8;                                            //n1
            n1=n11+read_eeprom(en10);
        
            write_eeprom(en20,n2);
            write_eeprom(en21,n2>>8);
            n21=read_eeprom(en21)<<8;                                            //n3
            n2=n21+read_eeprom(en20);
        
        ///////////////////////////////////////////
            write_eeprom(eTEMPmax,TEMPmax); 
            write_eeprom(eTEMPmin,TEMPmin);
            write_eeprom(ecambio_de_temp,cambio_de_temp);
        
            TEMPmax=read_eeprom(eTEMPmax);
            TEMPmin=read_eeprom(eTEMPmin);
            cambio_de_temp=read_eeprom(ecambio_de_temp);
        
        /////////////////////////////////////////////////  
        
            write_eeprom(esis_llena1,sis_llena);
            write_eeprom(esis_llenap2,sis_llena>>8);
            sis_llenap2=read_eeprom(esis_llenap2)<<8;                            //sisterna llena             
            sis_llena=sis_llenap2+read_eeprom(esis_llena1);
        
        
            write_eeprom(esis_vacia,sis_vacia);
            write_eeprom(esis_vaciap2,sis_vacia>>8);
            sis_vaciap2=read_eeprom(esis_vaciap2)<<8;                            //sis_vacia
            sis_vacia=sis_vaciap2+read_eeprom(esis_vacia);                           
        
            write_eeprom(emax_veces_bombas,max_veces_bombas);
        
        
            write_eeprom(evariacion_de_consumo_de_tanke,variacion_de_consumo_de_tanke);
            write_eeprom(econsumo_maximo_de_tanke,consumo_maximo_de_tanke);
        
        /*********   rutina para actualizar valores eeprom         ************/
        
        //variables en eprom que no se modifican desde teclado
        
            write_eeprom(ecount_B1,count_B1);
            write_eeprom(ecount_B2,count_B2);                                       //ccontadores b1,b2,b
            write_eeprom(ecount_b,count_b);
        
            count_B1=read_eeprom(ecount_B1);
            count_B2=read_eeprom(ecount_B2);
            count_b=read_eeprom(ecount_b);
        
            write_eeprom(etemperatura,temperatura);
            temperatura=read_eeprom(etemperatura);
        
            write_eeprom(econsumo_tanke,consumo_tanke);
            consumo_tanke=read_eeprom(econsumo_tanke);
        
            write_eeprom(econsumo_sis,consumo_sis);
            consumo_sis=read_eeprom(econsumo_sis);
        
            write_eeprom(elitros_actuales_tankep1,litros_actuales_tanke);
            write_eeprom(elitros_actuales_tankep2,litros_actuales_tanke>>8);    
            litros_actuales_tankep2=read_eeprom(elitros_actuales_tankep2)<<8;                                            
            litros_actuales_tanke=litros_actuales_tankep2+read_eeprom(elitros_actuales_tankep1);
        
            write_eeprom(enivel_sis1,nivel_sis);
            write_eeprom(enivel_sis2,nivel_sis>>8);
            nivel_sis2=read_eeprom(enivel_sis2)<<8;
            nivel_sis=nivel_sis2+read_eeprom(enivel_sis1); 
        } 
    }
}
