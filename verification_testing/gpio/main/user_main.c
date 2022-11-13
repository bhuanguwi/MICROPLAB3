/*
Cooperative scheduling is used so each task gets to run until it yields 
#define configUSE_PREEMPTION 0, to enable cooperative scheduling
*/

/* Task 1 runs and turns on LED for 0.5 seconds and then task delays for 1 second at t = 0.5s
Task 2 runs at t = 0.5 seconds for another 0.5 seconds and task delays for 1 second at t = 1s
Task 3 runs at t = 1s and task delays for 1 second.
The next sequence would start at t = 1.5s hence the system was idle between t = 1s and 1.5s. 
The task sequence repeats continuously. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
// Include semaphore header file 
#include "freertos/semphr.h"

#include "driver/gpio.h"

#include "esp_log.h"
#include "esp_system.h"

static const char* TAG = "main";


#define configUSE_PREEMPTION 0
#define GPIO_OUTPUT_IO_0    2
#define GPIO_OUTPUT_PIN_SEL (1ULL<<GPIO_OUTPUT_IO_0) 
#define TASK1_PRIORITY		3
#define TASK2_PRIORITY		2
#define TASK3_PRIORITY		1
//HALF_SECOND_DELAY was calculated to be 2650000
#define HALF_SECOND_DELAY	2650000

SemaphoreHandle_t xSemaphore = NULL;
unsigned int delay_value;
static void Task1(void * pvParameters);
static void Task2(void * pvParameters);
static void Task3(void * pvParameters);

/* A task that uses the semaphore. */
static void Task1(void * pvParameters)
{
	
	/* ... Do other things. */
	while (1) {
		if (xSemaphore != NULL)
		{
			
			/* See if we can obtain the semaphore.  If the semaphore is not
			available wait 1 ms to see if it becomes free. */
			if (xSemaphoreTake(xSemaphore, (TickType_t) 1/portTICK_RATE_MS ) == pdTRUE)
			{
				/* We were able to obtain the semaphore and can now access the
				shared resource. */

				/* ... */
				gpio_set_level(GPIO_OUTPUT_IO_0, 0);
				ESP_LOGI(TAG, "GPIO2 pin set to %d, LED ON", gpio_get_level(GPIO_OUTPUT_IO_0));
				// active wait for 0.5 seconds
				while (delay_value != HALF_SECOND_DELAY) {
					delay_value++;
				}
				ESP_LOGI(TAG, "Task 1 Completed");
				/* We have finished accessing the shared resource.  Release the
				semaphore. */
				delay_value = 0;
				xSemaphoreGive(xSemaphore);
				vTaskDelay(1000 / portTICK_RATE_MS);
			}
			else
			{
				ESP_LOGI(TAG, "Task 1 : Semaphore was not obtained");
				/* We could not obtain the semaphore and can therefore not access
				the shared resource safely. */
			}
		}
	}
}

/* A task that uses the semaphore. */
static void Task2(void * pvParameters)
{
	
	/* ... Do other things. */
	while (1) {
		if (xSemaphore != NULL)
		{
			/* See if we can obtain the semaphore.  If the semaphore is not
			available wait 1 ms to see if it becomes free. */
			if (xSemaphoreTake(xSemaphore, (TickType_t) 1/portTICK_RATE_MS ) == pdTRUE)
			{
				/* We were able to obtain the semaphore and can now access the
				shared resource. */

				/* ... */
				gpio_set_level(GPIO_OUTPUT_IO_0, 1);
				ESP_LOGI(TAG, "GPIO2 pin set to %d, LED OFF", gpio_get_level(GPIO_OUTPUT_IO_0));
				while (delay_value != HALF_SECOND_DELAY) {
					delay_value++;
				}
				ESP_LOGI(TAG, "Task 2 Completed");
				/* We have finished accessing the shared resource.  Release the
				semaphore. */
				delay_value = 0;
				xSemaphoreGive(xSemaphore);
				vTaskDelay(1000 / portTICK_RATE_MS);
			}
			else
			{
				ESP_LOGI(TAG, "Task 2 : Semaphore was not obtained");
				/* We could not obtain the semaphore and can therefore not access
				the shared resource safely. */
			}
		}
	}
}

/* A task that uses the semaphore. */
static void Task3(void * pvParameters)
{
	/* ... Do other things. */
	while (1) {
		if (xSemaphore != NULL)
		{
			/* See if we can obtain the semaphore.  If the semaphore is not
			available wait 1 ms to see if it becomes free. */
			if (xSemaphoreTake(xSemaphore, (TickType_t) 1/portTICK_RATE_MS ) == pdTRUE)
			{
				/* We were able to obtain the semaphore and can now access the
				shared resource. */

				/* ... */
				ESP_LOGI(TAG, "Status Update, Pin State : %d", gpio_get_level(GPIO_OUTPUT_IO_0));
				ESP_LOGI(TAG, "Task 3 Completed");
				/* We have finished accessing the shared resource.  Release the
				semaphore. */
				xSemaphoreGive(xSemaphore);
				vTaskDelay(1000 / portTICK_RATE_MS);
			}
			else
			{
				ESP_LOGI(TAG, "Task 3 : Semaphore was not obtained");
				/* We could not obtain the semaphore and can therefore not access
				the shared resource safely. */
			}
		}
	}
}

// Task scheduler in Freertos is automatically started in app_main , therefore it not necessary to start it manually
void app_main(void)
{
  gpio_config_t io_conf;
  //disable interrupt
  io_conf.intr_type = GPIO_INTR_DISABLE;
  //set as output mode
  io_conf.mode = GPIO_MODE_OUTPUT;
  //bit mask of the pins that you want to set,e.g.GP2
  io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
  //disable pull-down mode
  io_conf.pull_down_en = 0;
  //disable pull-up mode
  io_conf.pull_up_en = 0;
  //configure GPIO with the given settings
  gpio_config(&io_conf);


  delay_value = 0;
  TaskHandle_t xTask1, xTask2, xTask3;
  xSemaphore = xSemaphoreCreateMutex();
  

xTaskCreate(Task1, "LED On", 4096, NULL, TASK1_PRIORITY, &xTask1);
xTaskCreate(Task2, "LED OFF", 4096, NULL, TASK2_PRIORITY, &xTask2);
xTaskCreate(Task3, "Status", 4096, NULL, TASK3_PRIORITY, &xTask3);

//vTaskStartScheduler();
//ESP_LOGI(TAG, "Task Scheduler initialized");

}









