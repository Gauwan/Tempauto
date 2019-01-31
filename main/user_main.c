/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>

#include "esp_system.h"

//#include <Servo.h>

#include "driver/gpio.h"
#include "driver/adc.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/pwm.h"

// LED_POWER_PIN    D1 GPIO_5
// LED_WIFI_PIN     D2 GPIO_4
// MOTOR_PIN        D4 GPIO_2

// BUTTON_PIN       D5 GPIO_14
// DHT11_PIN        D6 GPIO_12

#define PWM_0_OUT_IO_NUM 2

// Variable globale
int16_t temperature = 20;
int16_t selectedTemp;

bool radiatorIsOpen = false;


/******************************************************************************
 * FunctionName : initIO
 * Description  : Initialization of Input and Output
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void initIO(void)
{
    printf("Start initIO() ... ");
	gpio_config_t configIO;

    // Configuration OUT (LED | Motor)
    configIO.mode=GPIO_MODE_OUTPUT;
    configIO.pin_bit_mask = GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5;
    configIO.intr_type = GPIO_INTR_DISABLE;
    configIO.pull_down_en = 0;
    configIO.pull_up_en = 0;
    gpio_config(&configIO);

    // Configuration Button
	configIO.mode=GPIO_MODE_INPUT;
	configIO.pin_bit_mask = GPIO_Pin_14;
    configIO.intr_type = GPIO_INTR_POSEDGE;
    configIO.pull_up_en = 1;
	gpio_config(&configIO);

    // Configuration Temperature
	configIO.mode=GPIO_MODE_OUTPUT_OD;
	configIO.pin_bit_mask = GPIO_NUM_12;
    configIO.intr_type = GPIO_INTR_DISABLE;
    configIO.pull_down_en = 0;
    configIO.pull_up_en = 0;
	gpio_config(&configIO);

    printf("finished!\n");
}


/******************************************************************************
 * FunctionName : app_main
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void handleButton()
{
    //printf("Button pressed.\n");
    radiatorIsOpen = 1-radiatorIsOpen;
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
    printf("Bienvenue sur le Tempauto!\n");
    printf("SDK version:%s\n", esp_get_idf_version());

    // Initialisation des IO
    initIO();
    
    // Intialisation des handler
    gpio_install_isr_service(0);
    gpio_isr_handler_add(GPIO_NUM_14, handleButton, NULL);

    // Initialisation moteur

    const uint32_t pin_num[1] = {
        PWM_0_OUT_IO_NUM
    };

    // dutys table, (duty/PERIOD)*depth
    uint32_t duty[1] = {600};
    int16_t phase[1] = {0};

    pwm_init(20000, duty,1,pin_num);
    pwm_set_phases(phase);

    pwm_start();
    

    // Allumer la LED ON/OFF
    gpio_set_level(GPIO_NUM_5, 1);

    // Boucle principal
    while(1)
    {
        //Choix de la temperature
        if (selectedTemp != adc_read()/25)
        {
            selectedTemp = adc_read()/25;
        }


        if (radiatorIsOpen && ((selectedTemp-temperature) > 0))
        {
            uint32_t nDuty[1] = {600}; // Min Ouverture
            pwm_set_duties(nDuty);
            pwm_start();

            // MAJ
            radiatorIsOpen = 0;
            gpio_set_level(GPIO_NUM_4, 0);
        }
        else if (!radiatorIsOpen && ((selectedTemp-temperature) < 0))
        {
            uint32_t nDuty[1] = {1000}; // Max ouverture
            pwm_set_duties(nDuty);
            pwm_start();

            // MAJ
            radiatorIsOpen = 1;
            gpio_set_level(GPIO_NUM_4, 1);
        }
            

        // Servo-Motor
        //gpio_set_level(GPIO_NUM_2, val);

        
        //Affichage
        printf("Temperature : %i°C / %i°C\n", temperature, selectedTemp);

        // Ralentissement
        int duration = pdMS_TO_TICKS(500);
        vTaskDelay(duration);
    }

    // Arret des handler
    gpio_isr_handler_remove(GPIO_NUM_14);

    // Eteindre la LED ON/OFF
    gpio_set_level(GPIO_NUM_5, 0);
}
