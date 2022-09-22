#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_adc_cal.h"
#include 

#define PINO_LED 2

//adc
#define DEFAULT_VREF 1100 //use adc3_vref_gpio() to obtain a better estimate
#define NO_OF_SAMPLES 50 //multisampling, dá uma estabilidade um pouco melhor

// control LED Brightness With a Potentiometer (PWM)

SemaphoreHandle_t semaphore = NULL;

//configuração do ADC
static const adc_channel_t channel = ADC_CHANNEL_6; //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_bits_width_t width = ADC_WIDTH_BIT_12;
static const adc_atten_t atten = ADC_ATTEN_DB_11; //dá 3,9v, é o mais perto de 3,3v

void Sensor1Task(void *parameters)
{

}

void Sensor2Task(void *parameters)
{

}

void app_main(void)
{
    semaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(semaphore);

    xTaskCreate(Sensor1Task, "sensor1", 2048, NULL, 0, NULL);
    xTaskCreate(Sensor2Task, "sensor2", 2048, NULL, 0, NULL);
}