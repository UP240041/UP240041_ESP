#include <stdio.h> // Incluye la biblioteca estándar de entrada/salida
#include "freertos/FreeRTOS.h" // Incluye la biblioteca principal de FreeRTOS

//Sensor infrarojo
#define D1 GPIO_NUM_36
#define D2 GPIO_NUM_39
#define D3 GPIO_NUM_34
#define D4 GPIO_NUM_35
#define D5 GPIO_NUM_32
#define D6 GPIO_NUM_33
#define D7 GPIO_NUM_25
#define D8 GPIO_NUM_26

//Motor derecho-B
#define PWMB GPIO_NUM_19
#define BIN2 GPIO_NUM_18
#define BIN1 GPIO_NUM_5

//Motor izquierdo-A
#define PWMA GPIO_NUM_4
#define AIN2 GPIO_NUM_16
#define AIN1 GPIO_NUM_17

//Botones
#define Cal GPIO_NUM_15 
#define Ready GPIO_NUM_13

//LEDs
#define Yellow GPIO_NUM_27
#define Green GPIO_NUM_14
#define Blue GPIO_NUM_2