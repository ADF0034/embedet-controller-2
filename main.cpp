#include "mbed.h"
#include "DHT.h" 
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include "Grove_LCD_RGB_Backlight.h"
#include <iostream>
// main() runs in its own thread in the OS
DHT SensorR1 (A0,SEN51035P); //https://os.mbed.com/users/Wimpie/code/DHT/
DHT SensorR2 (D6,SEN51035P); //https://os.mbed.com/users/Wimpie/code/DHT/
Grove_LCD_RGB_Backlight mydisplay (D14, D15); //Backlight display skal simulere en større skærm
DigitalOut red(D2);
DigitalOut blue(D8);
AnalogIn lightsensor(A2);
AnalogIn loudness(A2);
DigitalIn Restart (D7); //button for rest time

float val;
struct room{
    float tempatur;
    char nameofroom[50];
}rooms[2];
Thread larm;
Thread R1;
Thread R2;
void Larm();
void room1();
void room2();

    int antal = 0;//count how  there is ind the hale
    float r1=0.0f;
    float r2=0.0f;
    float f1=0.0f;
    float f2=0.0f;
    int err;
    char text[25];
    char ROOM1[25];
    char Humidity[25];
    char Humidity2[25];
int main()
{
    /*
    while(true){

        printf("Loudness: %f\r\n", loudness.read());
        wait_us(5000000);
    }*/
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
    BSP_LCD_Clear(LCD_COLOR_RED);
    BSP_LCD_SetFont(&Font20);
    BSP_LCD_SetBackColor(LCD_COLOR_RED);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

     //char tempatur(25);

     larm.start(&Larm);
     R1.start(&room1);
     R2.start(&room2);
    while (true) {
        int err;
        red =1;
        blue=1;
            //tempatur=
           // printf("Temperature is %4.2f C \r\n",c);
            sprintf(text, "Tempatur %4.2f", r1);
            sprintf(ROOM1,"Humidity: %4.2f",r2);
            sprintf(Humidity,"Tempatur: %4.2f",f1);
            sprintf(Humidity2,"Humidity %4.2f",f2);       
            BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)"       room 1" , LEFT_MODE); // tilføjer text på skærm
            BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)text , LEFT_MODE); // tilføjer text på skærm
            BSP_LCD_DisplayStringAt(0, LINE(7), (uint8_t *)Humidity , LEFT_MODE); // tilføjer text på skærm
            BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)"Room 2       " , RIGHT_MODE); // tilføjer text på skærm
            BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)ROOM1 , RIGHT_MODE); // tilføjer text på skærm
            BSP_LCD_DisplayStringAt(0, LINE(7), (uint8_t *)Humidity2 , RIGHT_MODE); // tilføjer text på skærm
            
            /*printf("Temperature is %4.2f F \r\n",Sensor.ReadTemperature(FARENHEIT));
            printf("Temperature is %4.2f K \r\n",Sensor.ReadTemperature(KELVIN));
            printf("Humidity is %4.2f \r\n",Sensor.ReadHumidity());
            printf("Dew point is %4.2f  \r\n",Sensor.CalcdewPoint(Sensor.ReadTemperature(CELCIUS), Sensor.ReadHumidity()));
            printf("Dew point (fast) is %4.2f  \r\n",Sensor.CalcdewPointFast(Sensor.ReadTemperature(CELCIUS), Sensor.ReadHumidity()));*/

        red= 0;
        wait_us(1000000);
        printf("Sensor reading: %f \r", val);
        printf( "tempatur t");
        if(val>0.40){
            blue=0;
        }
        wait_us(500000);
        

    }
}
void Larm(){
 char txtbuffer[25]; 
    while(true){
        val = loudness.read();
        if(Restart){
            antal=0; //resætter sekunder til 0
        }
        else if (val>0.22)
        {
            antal ++;
        }
        mydisplay.clear(); // sletter det tidliger ting som var på displayet
        sprintf(txtbuffer,"antal larm %i ",antal); // putter sekunderne ind i charen
        mydisplay.print(txtbuffer);//udskriver charen om hvor mange sekunder der er 
        wait_us(100000); //lader den vente så den passer til skeunder in real time
    }
}
void room1(){
    while(true){
        err = SensorR1.readData();
        r1=SensorR1.ReadTemperature(CELCIUS);
        f1=SensorR1.ReadHumidity();
        wait_us(100000);
    }
}
void room2(){
    while(true){
        err=SensorR2.readData();
        r2=SensorR2.ReadTemperature(CELCIUS);
        f2=SensorR2.ReadHumidity();
        wait_us(100000);
    }
}
