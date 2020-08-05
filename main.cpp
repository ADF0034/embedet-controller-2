#include "mbed.h"
#include "DHT.h" 
// main() runs in its own thread in the OS
DHT Sensor (A0,SEN51035P); //https://os.mbed.com/users/Wimpie/code/DHT/
DigitalOut red(D2);
DigitalOut blue(D2);
AnalogIn lightsensor(A2);

int main()
{
     float val;
    while (true) {
        int err;
        red =1;
        err = Sensor.readData();
        val = lightsensor.read();
         if (err == 0) {
            printf("Temperature is %4.2f C \r\n",Sensor.ReadTemperature(CELCIUS));
            printf("Temperature is %4.2f F \r\n",Sensor.ReadTemperature(FARENHEIT));
            printf("Temperature is %4.2f K \r\n",Sensor.ReadTemperature(KELVIN));
            printf("Humidity is %4.2f \r\n",Sensor.ReadHumidity());
            printf("Dew point is %4.2f  \r\n",Sensor.CalcdewPoint(Sensor.ReadTemperature(CELCIUS), Sensor.ReadHumidity()));
            printf("Dew point (fast) is %4.2f  \r\n",Sensor.CalcdewPointFast(Sensor.ReadTemperature(CELCIUS), Sensor.ReadHumidity()));
        } else
        red= 0;
        wait_us(100000);
        printf("Sensor reading: %2.2f \r", val);
        blue = !blue;
        wait_us(500000);
    }
}

