#include "mbed.h"
#include "DHT.h" 
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include "Grove_LCD_RGB_Backlight.h"
#include <iostream>
// main() runs in its own thread in the OS
DHT SensorR1 (D5,SEN51035P); //https://os.mbed.com/users/Wimpie/code/DHT/
DHT SensorR2 (D6,SEN51035P); //https://os.mbed.com/users/Wimpie/code/DHT/
Grove_LCD_RGB_Backlight mydisplay (D14, D15); //Backlight display skal simulere en større skærm
DigitalOut red(D2);
DigitalOut blue(D8);
DigitalOut buzzer(D3);
AnalogIn lightsensor(A2);
AnalogIn loudness(A1);
DigitalIn Restart (D7); //button for rest time
DigitalIn countButton(USER_BUTTON);
static BufferedSerial pc(USBTX,USBRX);
float val;
struct room{
    char nameofroom[25];
};
Thread larm;
Thread R1;
Thread R2;
Thread Pts;
Thread Tt;
void Larm();
void room1();
void room2();
void pts();
void tjekTempatur();
void tjekTempaturr2();
void timer();
void counter();
void Buzzer();
int getTxtInp(int Max);
int getTxt(char line[],int MAX);
char Room1name[25];
char Room2name[25];
char Rum1[25];
char Rum2[25];
    int NoR=3;
    int Room=0;
    int antal = 0;//count how  there is ind the hale
    int forsøg=0;
    int stop1=0;
    int stop2=0;
    int v1=0;
    int sekunder=0;
    int PreesCount=0;
    uint8_t prev_nb_touches = 0;
    uint8_t idx;
    uint16_t x, y;
    float r1=0.0f;
    float r2=0.0f;
    float f1=0.0f;
    float f2=0.0f;
    float light;
    int err;
    char text[25];
    char ROOM1[25];
    char Humidity[25];
    char Humidity2[25];
    char txtbuffer[25];
    int DoN=1;
    bool bsound =true;
    char *buff = new char[1];
    int *number;
    int Max=2;
    int X=0;
    int textantal =0;
    int antalrum=2;
    char Text[8];
    char UserText[8];
int main()
{
    bsound=false;
    buzzer=0;
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
    sprintf(txtbuffer,"antal larm %i ",antal); // putter sekunderne ind i charen
    mydisplay.print(txtbuffer);//udskriver charen om hvor mange sekunder der er 
     //char tempatur(25);
    larm.start(&Larm);
    R1.start(&room1);
    R2.start(&room2);
    Tt.start(&timer);
    char Roomename[]="Romsnavn:";
    printf("giv dine rum et navn\n");
    room rooms[antalrum];
    for(X=0;X<antalrum;X++){
        pc.write(Roomename,sizeof(Roomename));
        getTxt(rooms[X].nameofroom,20);
        printf("\n");
    }
    for(X=0;X<antalrum;X++){
        printf("%s\t\n",rooms[X].nameofroom);
    }
    sprintf(Rum1,"%s",rooms[0].nameofroom);
    sprintf(Rum2,"%s",rooms[1].nameofroom);
    Pts.start(&pts);
    while (true) {
    printf("vælg Dit Room:");
        char tal =pc.read(text,sizeof(text));
        textantal = int(tal);
        getTxtInp(1);
        //pc.read(number);
        //pc.write(number,sizeof(number));
        //printf("Sensor reading: %2.f  ", light*100);
        
            //tempatur=
           // printf("Temperature is %4.2f C \r\n",c);
            /*/sprintf(text, "Tempatur %4.2f", r1);
            sprintf(ROOM1,"Humidity: %4.2f",r2);
            sprintf(Humidity,"Tempatur: %4.2f",f1);
            sprintf(Humidity2,"Humidity %4.2f",f2);       
            BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)"       room 1" , LEFT_MODE); // tilføjer text på skærm
            BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)text , LEFT_MODE); // tilføjer text på skærm
            BSP_LCD_DisplayStringAt(0, LINE(7), (uint8_t *)Humidity , LEFT_MODE); // tilføjer text på skærm
            BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)"Room 2       " , RIGHT_MODE); // tilføjer text på skærm
            BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)ROOM1 , RIGHT_MODE); // tilføjer text på skærm
            BSP_LCD_DisplayStringAt(0, LINE(7), (uint8_t *)Humidity2 , RIGHT_MODE); // tilføjer text på skærm*/
            
            /*printf("Temperature is %4.2f F \r\n",Sensor.ReadTemperature(FARENHEIT));
            printf("Temperature is %4.2f K \r\n",Sensor.ReadTemperature(KELVIN));
            printf("Humidity is %4.2f \r\n",Sensor.ReadHumidity());
            printf("Dew point is %4.2f  \r\n",Sensor.CalcdewPoint(Sensor.ReadTemperature(CELCIUS), Sensor.ReadHumidity()));
            printf("Dew point (fast) is %4.2f  \r\n",Sensor.CalcdewPointFast(Sensor.ReadTemperature(CELCIUS), Sensor.ReadHumidity()));*/

        //wait_us(1000000);
       // printf("Sensor reading: %f \r\n", val);


        

    }
}
int getTxt(char line[],int MAX){
    int i=0, ascii=0;
    char buf[1]={0};
    while((i<MAX)&&(ascii !=13)&& (ascii !=27)){
        pc.read(buf,sizeof(buf));
        pc.write(buf,sizeof(buf));
        ascii=int(buf[0]);

        if(ascii){
           line[i]=buf[0];
            i++;
        }
    }
    line[i]='\0';
    return i;
}
int getTxtInp(int MAX){
    int i=0, ascii=0;
    char buf[1]={0};
    while((i<MAX)&&(ascii !=13)&& (ascii !=27)){
        pc.read(buf,sizeof(buf));
        pc.write(buf,sizeof(buf));
        ascii=int(buf[0]);

        if(ascii){
            UserText[i]=buf[0];
            i++;
        }
    }
    UserText[i]='\0';
    return i;
}
void Larm(){
    while(true){ 
            if(light*100<22){
                if(Restart){
                    antal=0; //resætter sekunder til 0
                    printf("virker");
                }
                else if (val>0.17)
                {
                    antal ++;
                    
                    counter();
                }
            }
            else
            {        
                if(Restart){
                    antal=0; //resætter sekunder til 0
                }
                else if (val>0.20) {
                    antal ++;
                }            
            }
                    sprintf(txtbuffer,"antal larm:%i",antal); // putter sekunderne ind i charen
                    mydisplay.clear(); // sletter det tidliger ting som var på displayet
                    mydisplay.print(txtbuffer);//udskriver charen om hvor mange sekunder der er
                    wait_us(300000); //lader den vente så den passer til skeunder in real time   
    }
}

void counter(){
        if(val>0.17){ //
            forsøg++;
            if(forsøg==5){
                bsound=true;
                Buzzer();
            }
        }
        printf("Sensor reading: %f \r\n", val);
        printf("Forsøg: %i \r sekunder til reset:%i",forsøg,sekunder);
}

void timer(){
    if(val>0.15&&light*100<22&&forsøg>0){
    
        while(sekunder<=300||forsøg!=5){
            sekunder ++;
            if(sekunder==300){
                sekunder=0;
                forsøg=0;
            }
            wait_us(100000); //lader den vente så den passer til skeunder in real time
            printf("Sensor reading: %i \r",sekunder);
        }
    }
}
void room1(){
    while(true){
        err = SensorR1.readData();
        r1=SensorR1.ReadTemperature(CELCIUS);
        f1=SensorR1.ReadHumidity();
        light = lightsensor.read();
        wait_us(100000);
    }
}
void room2(){
    while(true){
        err=SensorR2.readData();
        r2=SensorR2.ReadTemperature(CELCIUS);
        f2=SensorR2.ReadHumidity();    
        val = loudness.read();
        wait_us(100000);
    }
}
void tjekTempaturr1(){
        if(r1>=29){
            switch(Room){
                case 0:
                    BSP_LCD_Clear(LCD_COLOR_RED);
                    BSP_LCD_DisplayStringAt(0, LINE(9), (uint8_t *)"Room 1 forvarmt" , LEFT_MODE); // tilføjer text på skærm
                    stop1=1;
                    break;
                 case 1:
                    BSP_LCD_Clear(LCD_COLOR_RED);
                    BSP_LCD_DisplayStringAt(0, LINE(9), (uint8_t *)"Room 1 forvarmt" , CENTER_MODE); // tilføjer text på skærm
                    stop1=1;
                    break;
                case 2:
                    BSP_LCD_Clear(LCD_COLOR_RED);
                    BSP_LCD_DisplayStringAt(0, LINE(9), (uint8_t *)"Room 1 forvarmt" , CENTER_MODE); // tilføjer text på skærm
                    stop1=1;
                    break;
            }
        }
        else if(r1<23){
            switch(Room){
                case 0:
                    BSP_LCD_Clear(LCD_COLOR_RED);
                    BSP_LCD_DisplayStringAt(0, LINE(9), (uint8_t *)"Room 1 forkoldt" , LEFT_MODE); // tilføjer text på skærm
                    stop1=1;
                    break;
                 case 1:
                    BSP_LCD_Clear(LCD_COLOR_RED);
                    BSP_LCD_DisplayStringAt(0, LINE(9), (uint8_t *)"Room 1 forkoldt" , CENTER_MODE); // tilføjer text på skærm
                    stop1=1;
                    break;
                case 2:
                    BSP_LCD_Clear(LCD_COLOR_RED);
                    BSP_LCD_DisplayStringAt(0, LINE(9), (uint8_t *)"Room 1 forkoldt" , CENTER_MODE); // tilføjer text på skærm
                    stop1=1;
                    break;
            }
        }
        else {
        blue=0;
        red=0 ;
            if(stop1==1){
                BSP_LCD_Clear(LCD_COLOR_RED);
                stop1=0;
            }

        }
         if(r2>=29){
            switch(Room){
                case 0:
                    BSP_LCD_DisplayStringAt(0, LINE(9), (uint8_t *)"Room 2 forvarmt" , RIGHT_MODE); // tilføjer text på skærm
                    stop2=1;
                    break;
                 case 1:
                    BSP_LCD_DisplayStringAt(0, LINE(10), (uint8_t *)"Room 2 forvarmt" , CENTER_MODE); // tilføjer text på skærm
                    stop2=1;
                    break;
                case 2:
                    BSP_LCD_DisplayStringAt(0, LINE(10), (uint8_t *)"Room 2 forvarmt" , CENTER_MODE); // tilføjer text på skærm
                    stop2=1;
                    break;
            }
        }
        else if(r2<23){
            switch(Room){
                case 0:
                    BSP_LCD_DisplayStringAt(0, LINE(9), (uint8_t *)"Room 2 forkoldt" , RIGHT_MODE); // tilføjer text på skærm
                    stop2=1;
                    break;
                 case 1:
                    BSP_LCD_DisplayStringAt(0, LINE(10), (uint8_t *)"Room 2 forkoldt" , CENTER_MODE); // tilføjer text på skærm
                    
                    stop2=1;
                    break;
                case 2:
                    BSP_LCD_DisplayStringAt(0, LINE(10), (uint8_t *)"Room 2 forkoldt" , CENTER_MODE); // tilføjer text på skærm
                    stop2=1;
                    break;
            }
        }
        else {
            blue=0;
            red=0 ;
                if(stop2==1){
                    BSP_LCD_Clear(LCD_COLOR_RED);
                    stop2=0;
                }
            }
            if(r1>=29||r2>=29){
                red=1;
                blue=0;
            }
            else if (r1<=23||r2<=23) {
                red=0;
                blue=1;
            }
            else {
                red=0;
                blue=0;
            }
    
}
/*void tjekTempaturr2(){
        if(r2>26){
            switch(Room){
                case 0:
                    BSP_LCD_DisplayStringAt(0, LINE(9), (uint8_t *)"Room 2 er forvarmt" , RIGHT_MODE); // tilføjer text på skærm
                    red =1;
                    blue =0;
                    stop2=1;
                    break;
                 case 1:
                    BSP_LCD_DisplayStringAt(0, LINE(10), (uint8_t *)"Room 2 er forvarmt" , CENTER_MODE); // tilføjer text på skærm
                    red =1;
                    blue =0;
                    stop2=1;
                    break;
                case 2:
                    BSP_LCD_DisplayStringAt(0, LINE(10), (uint8_t *)"Room 2 er forvarmt" , CENTER_MODE); // tilføjer text på skærm
                    red =1;
                    blue =0;
                    stop2=1;
                    break;
            }
        }
        else if(r2<23){
            switch(Room){
                case 0:
                    BSP_LCD_DisplayStringAt(0, LINE(9), (uint8_t *)"Room 2 er forkoldt" , RIGHT_MODE); // tilføjer text på skærm
                    blue =1;
                    red=0;
                    stop2=1;
                    break;
                 case 1:
                    BSP_LCD_DisplayStringAt(0, LINE(10), (uint8_t *)"Room 2 er forkoldt" , CENTER_MODE); // tilføjer text på skærm
                    blue =1;
                    red=0;
                    stop2=1;
                    break;
                case 2:
                    BSP_LCD_DisplayStringAt(0, LINE(10), (uint8_t *)"Room 2 er forkoldt" , CENTER_MODE); // tilføjer text på skærm
                    blue =1;
                    red=0;
                    stop2=1;
                    break;
            }
        }
        else {
        blue=0;
        red=0 ;
            if(stop2==1){
                BSP_LCD_Clear(LCD_COLOR_RED);
                stop2=0;
            }

        }
    
}*/
void pts(){
    while(true){
        sprintf(text, "Tempatur %4.2f", r1);
        sprintf(ROOM1,"Humidity: %4.2f",f1);
        sprintf(Humidity,"Tempatur: %4.2f",r2);
        sprintf(Humidity2,"Humidity %4.2f",f2);   
        wait_us(50000);
        if(*UserText=='1'){        
            if(Room!=1){
                BSP_LCD_Clear(LCD_COLOR_RED);
            }
                Room=1;
            BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)Rum1 , CENTER_MODE); // tilføjer text på skærm
            BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)text , CENTER_MODE); // tilføjer text på skærm
            BSP_LCD_DisplayStringAt(0, LINE(7), (uint8_t *)ROOM1 , CENTER_MODE); // tilføjer text på skærm
        }
        else if (*UserText=='2') {
        if(Room!=2){
            BSP_LCD_Clear(LCD_COLOR_RED);
        }
        Room=2;
        BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)Rum2 , CENTER_MODE); // tilføjer text på skærm
        BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)Humidity , CENTER_MODE); // tilføjer text på skærm
        BSP_LCD_DisplayStringAt(0, LINE(7), (uint8_t *)Humidity2 , CENTER_MODE); // tilføjer text på skærm
        }
        else {
            if(Room!=3){
            BSP_LCD_Clear(LCD_COLOR_RED);
            }
        Room=3;
        BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)Rum1 , LEFT_MODE); // tilføjer text på skærm
        BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)text , LEFT_MODE); // tilføjer text på skærm
        BSP_LCD_DisplayStringAt(0, LINE(7), (uint8_t *)ROOM1, LEFT_MODE); // tilføjer text på skærm
        BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)Rum2, RIGHT_MODE); // tilføjer text på skærm
        BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)Humidity , RIGHT_MODE); // tilføjer text på skærm
        BSP_LCD_DisplayStringAt(0, LINE(7), (uint8_t *)Humidity2 , RIGHT_MODE); // tilføjer text på skærm
        }
        
        wait_us(50000);
        tjekTempaturr1();
    }
}
void Buzzer(){
    bsound=true;
    while(bsound){
        if(countButton){
            PreesCount++;
            if(PreesCount==2){
                bsound=false;
                PreesCount=0;
                buzzer=0;
                forsøg=0;
            }
            printf("virker");
            wait_us(10000); // gør at den vendter 
        }
        buzzer=1;
        wait_us(10000); // gør at den vendter 
    }
    PreesCount=0;
    buzzer=0;
    forsøg=0;
}