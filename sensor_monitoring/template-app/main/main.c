#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include <ultrasonic.h>

SemaphoreHandle_t bmp_mutex = NULL;
SemaphoreHandle_t ultrassom_mutex = NULL;

#define DELAY 1000 //ms

#define MAX_DISTANCE_CM 500 // 5m max

#define GPIO_TRIGGER 15
#define GPIO_ECHO 2

/*
* Leitura BMP280
*/

struct bmp280 {
    float tmp;
    float press;
};

struct bmp280 valor_bmp280;

struct bmp280 leitura_bmp280() {
    struct bmp280 sensor;
    // Inserir leitura
    sensor.tmp = 1;
    sensor.press = 2;
    return sensor;
}

void bmp280_task(void * parametros) {
    struct bmp280 leitura;
    while(1) {
        leitura = leitura_bmp280();
        while (xSemaphoreTake(bmp_mutex, 10)) {
            valor_bmp280 = leitura;
        }
        xSemaphoreGive(bmp_mutex);
        vTaskDelay(DELAY / portTICK_PERIOD_MS);
    }
}

/*
* Leitura ultrassom
*/

float valor_ultrassom = 84;

float leitura_ultrassom() {
    // Inserir leitura
    uint32_t distance = 0;

	ultrasonic_sensor_t sensor = {
		.trigger_pin = GPIO_TRIGGER,
		.echo_pin = GPIO_ECHO
	};

    /*
	ultrasonic_init(&sensor);
    
    while (true) {
		esp_err_t res = ultrasonic_measure_cm(&sensor, MAX_DISTANCE_CM, &distance);
		if (res != ESP_OK) {
			printf("Error: ");
			switch (res) {
				case ESP_ERR_ULTRASONIC_PING:
					printf("Cannot ping (device is in invalid state)\n");
					break;
				case ESP_ERR_ULTRASONIC_PING_TIMEOUT:
					printf("Ping timeout (no device found)\n");
					break;
				case ESP_ERR_ULTRASONIC_ECHO_TIMEOUT:
					printf("Echo timeout (i.e. distance too big)\n");
					break;
				default:
					printf("%d\n", res);
			}
		}
        
		vTaskDelay(500 / portTICK_PERIOD_MS);
	}*/

    return *((float*)&distance);
}

void ultrassom_task(void * parametros) {
    float leitura;
    while(1) {
        leitura = leitura_ultrassom();
        while (xSemaphoreTake(ultrassom_mutex, 10)) {
            valor_ultrassom = leitura;
        }
        xSemaphoreGive(ultrassom_mutex);
        vTaskDelay(DELAY / portTICK_PERIOD_MS);
    }
}

/*
* Comunicação
*/

void comunicacao_task(void * parametros) {
    struct bmp280 bmp280_lido;
    bmp280_lido.tmp = 0;
    bmp280_lido.press = 0;
    float ultrassom_lido = 0;
    while(1) {
        if (xSemaphoreTake(bmp_mutex, 10)) {
            bmp280_lido = valor_bmp280;
        }
        xSemaphoreGive(bmp_mutex);
        if (xSemaphoreTake(ultrassom_mutex, 10)) {
            ultrassom_lido = valor_ultrassom;
        }
        xSemaphoreGive(ultrassom_mutex);
    
        printf("Enviando leituras...\n");
        printf("Temperatura: %f\n", bmp280_lido.tmp);
        printf("Pressao: %f\n", bmp280_lido.press);
        printf("Distancia: %f\n", ultrassom_lido);
        printf("Finalizado envio!\n");
        vTaskDelay(DELAY / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    bmp_mutex = xSemaphoreCreateMutex();
    xSemaphoreGive(bmp_mutex);
    ultrassom_mutex = xSemaphoreCreateMutex();
    xSemaphoreGive(ultrassom_mutex);

    xTaskCreate(bmp280_task, "BMP280", 2048, NULL, 1, NULL);
    xTaskCreate(ultrassom_task, "UltrassomArduino", 2048, NULL, 1, NULL);
    xTaskCreate(comunicacao_task, "Comunicacao", 2048, NULL, 1, NULL);
}