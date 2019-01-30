/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>

#include "esp_system.h"

#include "driver/gpio.h"
#include <dht/dht.h>

// LED_POWER_PIN    D1 GPIO_5
// LED_WIFI_PIN     D2 GPIO_4
// MOTOR_PIN        D4 GPIO_2

// BUTTON_PIN       D5 GPIO_14
// DHT11_PIN        D6 GPIO_12


void initIO(void)
{
	gpio_config_t configIO;

    // Configuration OUT (LED | Motor)
    configIO.mode=GPIO_MODE_OUTPUT;
    configIO.pin_bit_mask = GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5;
    configIO.intr_type = GPIO_INTR_DISABLE;
    configIO.pull_down_en = configIO.pull_up_en = 0;
    gpio_config(&configIO);

    // Configuration Button
	configIO.mode=GPIO_MODE_INPUT;
	configIO.pin_bit_mask = GPIO_Pin_14;
    configIO.intr_type = GPIO_INTR_POSEDGE;
    configIO.pull_up_en = 1;
	gpio_config(&configIO);

    // Configuration Temperature
	configIO.mode=GPIO_MODE_INPUT;
	configIO.pin_bit_mask = GPIO_Pin_12;
    configIO.intr_type = GPIO_INTR_ANYEDGE; //?
    configIO.pull_down_en = 1; //?
	gpio_config(&configIO);
}



/******************************************************************************
 * FunctionName : app_main
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void app_main(void)
{
    // Informations
    printf("Bienvenue sur le Tempauto!");
    printf("SDK version:%s\n", esp_get_idf_version());

    //Initialisation des IO
    initIO();

    // Allumer la LED ON/OFF
    gpio_set_level(GPIO_NUM_0, 1);
}
