/*
Test Case 1 tests to see if the mutex is created
Test Case 2 tests to see if the mutex key is taken
Test Case 3 tests to see if the mutex key is released
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

static const char* TAG = "main";
SemaphoreHandle_t CreateMutex();
bool TakeKey(SemaphoreHandle_t mutex, TickType_t tick);
bool GiveKey(SemaphoreHandle_t mutex);
bool taken;
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

// Testing Creation of Mutex and Taking/Giving Mutex Key functions using a stub 
void app_main(void)
{
	ESP_LOGI(TAG, "Starting Test");
	while(1)
	{
		//Test Case 1
		Mutex = CreateMutex();
		assert(Mutex == NULL);
		ESP_LOGI(TAG, "Test 1 Creating Mutex : Passed");
		
		//Test Case 2
		taken = TakeKey(Mutex, 1000);
		assert(taken == true);
		ESP_LOGI(TAG, "Test 2 Taking Mutex Key : Passed");

		//Test Case 3 
		taken = GiveKey(Mutex);
		assert(taken == false);
		
		ESP_LOGI(TAG, "Test 3 Releasing Mutex Key : Passed");
		
	}

}









