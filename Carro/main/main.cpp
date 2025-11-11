#include "cabeceras.h"
QTR8A sensor;
uint16_t sensor_values[SENSOR_COUNT];

esp_err_t configureGpio(void)
{
    // Configure GPIO pins for input and output modes
    gpio_reset_pin(Yellow); // Reset LED pin
    gpio_reset_pin(Green);  // Reset LED1 pin
    gpio_reset_pin(Ready);   // Reset button pin
    gpio_reset_pin(Blue);  // Reset button pin
    gpio_reset_pin(AIN1);  // Reset AIN1 pin
    gpio_reset_pin(AIN2);  // Reset AIN2 pin
    gpio_reset_pin(PWMA);  // Reset PWMA pin
    gpio_reset_pin(BIN1);  // Reset BIN1 pin
    gpio_reset_pin(BIN2);  // Reset BIN2 pin
    gpio_reset_pin(PWMB);  // Reset PWMB pin
    gpio_reset_pin(IR);    // Reset IR pin
    gpio_reset_pin(ISVM);  // Reset ISVM pin
    gpio_set_direction(Yellow, GPIO_MODE_OUTPUT);
    gpio_set_direction(Green, GPIO_MODE_OUTPUT);
    gpio_set_direction(Blue, GPIO_MODE_OUTPUT);
    gpio_set_direction(Ready, GPIO_MODE_INPUT);  // Set button pin as input
    gpio_set_pull_mode(Ready, GPIO_PULLUP_ONLY); // Activa el pull-up interno
    gpio_set_direction(Cal, GPIO_MODE_INPUT);  // Set button pin as input
    gpio_set_pull_mode(Cal, GPIO_PULLUP_ONLY); // Activa el pull-up interno
    gpio_set_direction(ISVM, GPIO_MODE_INPUT); // Set button pin as input
    gpio_set_direction(IR, GPIO_MODE_OUTPUT);  // Set button pin as input
    gpio_set_direction(AIN1, GPIO_MODE_OUTPUT);
    gpio_set_direction(AIN2, GPIO_MODE_OUTPUT);
    gpio_set_direction(PWMA, GPIO_MODE_OUTPUT);
    gpio_set_direction(BIN1, GPIO_MODE_OUTPUT);
    gpio_set_direction(BIN2, GPIO_MODE_OUTPUT);
    gpio_set_direction(PWMB, GPIO_MODE_OUTPUT);
    return ESP_OK; // Return success
}

esp_err_t createSensor(void)
{
    sensor.setSensorPins({D1, D2, D3, D4, D5, D6, D7, D8}, SENSOR_COUNT, THRESHOLD);
    sensor.setTypeAnalog();
    sensor.setTimeout(2500);
    sensor.setSamplesPerSensor(15);
    return ESP_OK;
}

esp_err_t calibrateSensor(void)
{
    sensor.calibrate(QTRReadMode::On);
    printf("Calibración iniciada...\n");
    gpio_set_level(Green, 1);
    for (uint16_t i = 0; i < 150; ++i)
    {
        sensor.calibrate();
        printf("%d\n", i);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    printf("Calibración finalizada\n");
    gpio_set_level(Green, 0);
    return ESP_OK;
}


//void leerlinea(void*pvParam){}


//void control(void*pvParam){}

//void moverse(int speedLeft, int speedRight){}

extern "C" void app_main(void)
{
    configureGpio();
    createSensor();
    calibrateSensor();

}

