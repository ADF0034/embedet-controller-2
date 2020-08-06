#include "mbed.h"
#include "DHT.h" 
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include <iostream>
// main() runs in its own thread in the OS
DHT Sensor (A0,SEN51035P); //https://os.mbed.com/users/Wimpie/code/DHT/
DigitalOut red(D2);
DigitalOut blue(D8);
AnalogIn lightsensor(A2);
AnalogIn loudness(A2);
struct room{
    float tempatur;
    char nameofroom[50];
}rooms[2];
Thread t;
void Larm();
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
     float val;
     float f=0.0f;
     //char tempatur(25);
     char text[25];
     t.start(&Larm);
    while (true) {
        int err;
        red =1;
        blue=1;
        err = Sensor.readData();
        f=Sensor.ReadTemperature(CELCIUS);
        val = lightsensor.read();
         if (err == 0) {
            //tempatur=
            printf("Temperature is %4.2f C \r\n",f);
            sprintf(text, "Showet starter om lidt %4.2f", f);
            BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)text , CENTER_MODE); // tilføjer text på skærm
            /*printf("Temperature is %4.2f F \r\n",Sensor.ReadTemperature(FARENHEIT));
            printf("Temperature is %4.2f K \r\n",Sensor.ReadTemperature(KELVIN));
            printf("Humidity is %4.2f \r\n",Sensor.ReadHumidity());
            printf("Dew point is %4.2f  \r\n",Sensor.CalcdewPoint(Sensor.ReadTemperature(CELCIUS), Sensor.ReadHumidity()));
            printf("Dew point (fast) is %4.2f  \r\n",Sensor.CalcdewPointFast(Sensor.ReadTemperature(CELCIUS), Sensor.ReadHumidity()));*/
        } else
        red= 0;
        wait_us(1000000);
        printf("Sensor reading: %2.2f \r", val);
        printf("Sensor reading: %1.0f \r",val);
        printf( "tempatur t");
        if(val>0.60){
            blue=0;
        }
        wait_us(500000);
        

    }
}
void Larm(){

}
