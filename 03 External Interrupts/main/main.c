/*
 * Este programa para ESP32 utiliza una interrupción externa para contar el número de veces
 * que se presiona un botón conectado al pin GPIO5. Cuando se detecta un flanco de subida
 * en el pin, se incrementa el contador y se actualiza el estado del botón.
 *
 * Principales componentes:
 * - INT_PIN: Pin GPIO configurado para la interrupción externa.
 * - int_count: Variable que almacena el número de interrupciones detectadas.
 * - button_state: Bandera que indica si se ha detectado una interrupción.
 *
 * Funciones:
 * - gpio_isr_handler: Manejador de la interrupción, incrementa el contador y actualiza el estado.
 *   // Esta función es llamada automáticamente cuando ocurre una interrupción en el pin INT_PIN.
 *   // Incrementa el contador de interrupciones y actualiza la bandera de estado del botón.
 *
 * - app_main: Configura el pin, la interrupción y muestra el contador por consola cada vez que se detecta una pulsación.
 *   // Función principal del programa. Inicializa el GPIO, configura la interrupción externa,
 *   // y en un bucle principal muestra el número de veces que se ha presionado el botón.
 *
 * Notas:
 * - El manejador de interrupción vuelve a registrar la ISR y habilita la interrupción, lo cual no es necesario y puede causar problemas.
 * - Se utiliza FreeRTOS para la gestión de tareas y retardos.
 */
#include <stdio.h> // Incluye funciones de entrada/salida estándar
#include "freertos/FreeRTOS.h" // Incluye FreeRTOS para gestión de tareas
#include "driver/gpio.h" // Incluye funciones para manejar GPIO
#include "inttypes.h" // Incluye definiciones de tipos enteros
#include "freertos/task.h"
#define LED    GPIO_NUM_26
#define INT_PIN GPIO_NUM_5 // Define el pin GPIO5 como pin de interrupción



uint16_t int_count = 0; // Variable para contar interrupciones
bool button_state = false; // Bandera para indicar si el botón fue presionado
const TickType_t debounceDelay = pdMS_TO_TICKS(50);
const TickType_t doubleClickTimeout = pdMS_TO_TICKS(400);
TickType_t startTime;

void punto(){
    gpio_set_level(LED, 1); // Encendemos el LED
    vTaskDelay(pdMS_TO_TICKS(200)); // Esperamos 1 segundo
    gpio_set_level(LED, 0); // Apagamos el LED
    vTaskDelay(pdMS_TO_TICKS(200)); // Esperamos 1 segundo
}

void raya(){
    gpio_set_level(LED, 1); // Encendemos el LED
    vTaskDelay(pdMS_TO_TICKS(500)); // Esperamos 1 segundo
    gpio_set_level(LED, 0); // Apagamos el LED
    vTaskDelay(pdMS_TO_TICKS(500)); // Esperamos 1 segundo
}

void sos()
    {
    for(int i =0; i < 3; i++){
        punto();
    }
    for (int i = 0; i < 3; i++){
        raya();
    }
     for(int i =0; i < 3; i++){
        punto();
    }
}


// Manejador de la interrupción externa
bool tripleclick(void)
{
    // Reinicia la configuración de los pines LED y botón
    gpio_reset_pin(LED);
    gpio_reset_pin(INT_PIN);

    // Configura el pin del LED como salida
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);

    // Configura el pin del botón con resistencia pull-up
    gpio_set_pull_mode(INT_PIN, GPIO_PULLUP_ONLY);
    // Configura el pin del botón como entrada
    gpio_set_direction(INT_PIN, GPIO_MODE_INPUT);
    vTaskDelay(50);
    // Verifica si se soltó el botón
    while (gpio_get_level(INT_PIN) == 0)
    {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    // Marca el tiempo de la primera pulsación
    int_count = 0;
    int_count ++;
    vTaskDelay(50);
    startTime = xTaskGetTickCount(); // Incrementa el contador de interrupciones
    // Espera la segunda pulsación dentro del tiempo permitido
    while ((xTaskGetTickCount() - startTime) < doubleClickTimeout)
    {
        if (gpio_get_level(INT_PIN) == 0)
        {
            // Espera a que se suelte el botón nuevamente
            vTaskDelay(debounceDelay);
            while (gpio_get_level(INT_PIN) == 0)
            {
                vTaskDelay(pdMS_TO_TICKS(10));
            }
            vTaskDelay(50);
            int_count ++; // Incrementa el contador de interrupciones
            startTime = xTaskGetTickCount();
            while ((xTaskGetTickCount() - startTime) < doubleClickTimeout)
            {
                if (gpio_get_level(INT_PIN) == 0)
                {
                    // Espera a que se suelte el botón nuevamente
                    vTaskDelay(debounceDelay);
                    while (gpio_get_level(INT_PIN) == 0)
                    {
                        vTaskDelay(pdMS_TO_TICKS(10));
                    }
                    vTaskDelay(50);
                    int_count ++; // Incrementa el contador de interrupciones
                    return 1;
                }
            }
        }
    }
    return 0;   
}

// Función principal del programa
void app_main(void)
{
    while(1){
    bool state=tripleclick();
    if (state == 1)
    {
        sos();
    }
    else
    {
        gpio_set_level(LED,0);
    }
}
}
