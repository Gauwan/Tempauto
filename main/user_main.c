/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>

#include "esp_system.h"

#include "driver/gpio.h"


void initIO(void)
{
	gpio_config_t in;
	in.mode=GPIO_MODE_INPUT;
	in.pin_bit_mask = GPIO_Pin_4 | GPIO_Pin_5;
	gpio_config(&in);

	gpio_config_t out;
        out.mode=GPIO_MODE_OUTPUT;
       	out.pin_bit_mask = GPIO_Pin_4 | GPIO_Pin_5;
        gpio_config(&out);
}



/******************************************************************************
 * FunctionName : app_main
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void app_main(void)
{
    printf("SDK version:%s\n", esp_get_idf_version());
    printf("Hello world!");
}
