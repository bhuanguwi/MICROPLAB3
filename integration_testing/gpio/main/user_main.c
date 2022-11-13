/*
Integration testing utilizing a driver function the various test cases using a fake task function to turn on a LED and  
the mutex stub functions.

Test Case 1 tests if pin state goes low when task is running
Test Case 2 tests if task is prevented from running because it does not have mutex key
Test Case 3 tests if task runs if it wasn't created
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
// Include semaphore header file 
#include "freertos/semphr.h"

#include "driver/gpio.h"

#include "esp_log.h"
#include "esp_system.h"
//#include "mutex.h"

#define HALF_SECOND_DELAY	2650000

static const char* TAG = "main";
SemaphoreHandle_t CreateMutex();
bool TakeKey(SemaphoreHandle_t mutex, TickType_t tick);
bool GiveKey(SemaphoreHandle_t mutex);
bool taken;
bool pin_state;
unsigned int delay_value = 0;
char task[];
SemaphoreHandle_t Mutex;

//Stub for Creating mutex
SemaphoreHandle_t CreateMutex() 
{
	return NULL;
}

//Stub for Taking Mutex Key
bool TakeKey(SemaphoreHandle_t mutex, TickType_t tick) 
{
	while (tick != 0){
		tick--;
	}
	return true;
}

//Stub for Releasing the Mutex Key
bool GiveKey(SemaphoreHandle_t mutex)
{
	return false;
}

//Stub for creating the task 

void TaskCreate (char task_function[])
{
	strcpy(task,task_function);
	return;
}

//A fake function created for task 1 which turns on LED
void OnLED () 
{
	if(taken == false)
	{
		taken = TakeKey(Mutex,10);
			if (taken == true)
			{
				pin_state = 0; // set pin to 0 which turns on LED
				ESP_LOGI(TAG, "Pin State : 0");
				while(delay_value != HALF_SECOND_DELAY)
				{
					delay_value++;
				}
				ESP_LOGI(TAG, "Task 1 to turn LED on is Completed");
				delay_value = 0;
				taken = GiveKey(Mutex);
			}
	}
	else
	{
		ESP_LOGI(TAG, "Task 1 : Key was not obtained");
	}

}


// App_main tests to see how driver function for task works with mutex stubs
void app_main(void)
{
	Mutex = CreateMutex();
	ESP_LOGI(TAG, "Starting Test");
	while(1)
	{
		//Test Case 1 checks if pin state goes low when task is running
		ESP_LOGI(TAG, "Test Case 1, checks if pin goes low");
		TaskCreate("OnLED");
		for (int i = 0; i < 10; i++)
		{
			if(strcmp(task,"OnLED") == 0)
			{
				OnLED();
				// test if pin goes low which turns/keeps on LED
				assert(pin_state == 0); 
			}
			else 
			{
				ESP_LOGI(TAG, "Task not created");
			}
		}
		
		// set taken to true to see what happens if another task has the key
		taken = true; 
		//Test Case 2 checks if task is prevented from running because it does not have mutex key
		ESP_LOGI(TAG, "Test Case 2, checks if current task is prevented from running due to other task having the key for mutex");
		for (int i = 0; i < 10; i++)
		{
			if(strcmp(task,"OnLED") == 0)
			{
				OnLED();
			}
			else 
			{
				ESP_LOGI(TAG, "Task not created");
			}
		}
		// make key available again
		taken = false;
		// change task to null char, "\0" to check scenario where the task was not created.
		task [0] = "\0";
		//Test Case 3 checks if task runs if it wasn't created
		ESP_LOGI(TAG, "Test Case 3, checks if task runs even if task wasn't created");
		for (int i = 0; i < 10; i++)
		{
			if(strcmp(task,"OnLED") == 0)
			{
				OnLED();
			}
			else 
			{
				ESP_LOGI(TAG, "Task not created");
			}
		}
	}

}









