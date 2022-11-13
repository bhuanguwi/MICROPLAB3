# Student ID - 816022781 

## Branching 
Each branch was created to add a new type of testing to the repository such as unit, integration and verification./
The master branch includes all the project files for unit, system and verification testing. In each project folder, contains the output file and images for evidence only
in the verification test.

## Testing
The first question from lab 2 was used for testing purposes. It is stated below.\
Create an example application with three tasks that are created in app_main.\
Two tasks should share a single GPIO pin (you may use the pin connected to the built-in LED for convenience).\
One task will turn the GPIO pin on, actively wait for 0.5 seconds, and task-delay for 1 second.\
The other will turn the GPIO pin off, actively wait for 0.5 seconds, and task-delay for 1 second.\
The GPIO pin should be managed using a semaphore/mutex. The third task will print a status message only, and task-delay for 1 second.\
Each task should have it's own priority level. Build and run your code.

## Unit Testing

The mutex used in the program was chosen to be unit tested. The xSemaphoreCreateMutex, xSemaphoreTake and xSemaphoreGive functions were selected as the functions for unit testing.\
The following stubs were created which took the same inputs as the FreeRTOS API functions. 
1. SemaphoreHandle_t CreateMutex( ) - returns NULL
2. bool TakeKey(SemaphoreHandle_t mutex, TickType_t tick) - waits **tick** time and then returns true
3. bool GiveKey(SemaphoreHandle_t mutex) - returns false

There were three test cases created in the app_main. 
1. Test Case 1 tests to see if the mutex is successfully created when the function is called.
2. Test Case 2 tests to see if the mutex key is successfully taken when the function is called. 
3. Test Case 3 tests to see if the mutex key is successfully released when the function is called. 

<p align="justify"> In order to proceed with the test, a handle for the mutex must be created and a boolean type variable which stores whether the key is taken(True) or not(False).
This was done in line 28-29 in user_main.c in the unit testing project file. On lines 58-61 inside app_main, the mutex was created using the stub CreateMutex( ) and then assert was called
to see if the mutex was "NULL" as expected. Similarly, the TakeKey(SemaphoreHandle_t mutex, TickType_t tick) and GiveKey(SemaphoreHandle_t mutex) was assigned to the boolean
variable named "taken". After each function call, assert was used again to check if "taken" was equal to true and false respectively. This is seen on lines 63-72. If all
tests were successful and no errors are thrown, then the debug messsages at the end of each test case would output to the serial monitor.
This is seen in the output file labelled "unit_test" which shows that all tests passed. </p>

## Reflecting on Unit Testing

A testing framework wasn't utilized here because the complexity and scale of the program did not require such procedures and only a few functions were tested.
However, using a testing framework such as a test harness or Unity's testing framework could provide some benefits for large scale projects such as:
1. Improvement in test efficiency
2. Lower maintenance costs 
3. Less manual intervention
4. Maximum test coverage 
5. Reusability of code 

<p align="justify"> There was a test case that could be added where it tries to take the mutex key that has already been taken. Instead, it was deferred for integration testing but could have been
unit tested if the TakeKey stub function was written differently to return true only if "taken" was false previously. </p>

## Integration Testing 

Integration testing was performed to see if a function that calls the unit tested function operates as expected. All of the task functions (turn on the LED, 
turn off the LED, produce a status message) require the unit tested functions, TakeKey and GiveKey. A stub for creating a task and a fake function to turn on the LED 
was created. 
1. void TaskCreate (char task_function[]) - a stub for creating a task where the only input is the task function. The other parameters such as priority level, 
stack size and task name weren't necessary. 
2. void OnLED () - a fake for turning on LED where a boolean variable named "pin_state" was used to store the state of the pin. If the pin is 0, the LED is on and
if it is 1, the LED is off.

There were three test cases created in the app_main.
1. Test Case 1 tests if pin state goes low when task is running.
2. Test Case 2 tests if task is prevented from running because it does not have mutex key. 
3. Test Case 3 tests if task runs if it wasn't created.

<p align="justify"> To proceed with the tests, we must create a boolean variable to store the state of the pin and a string for the task function. They were named "pin_state" and "task" 
respectively. This is seen on lines 34 and 36 in user_main.c of the integration testing project file. Firstly, TaskCreate is called to create the task "OnLED". This was
done to replicate the task creation process. "task" now stores the string "OnLED". If "task" stores the task function name being called which is "OnLED" then it will
call the fake function "OnLED". Afterwards, an assert is used to check if the pin_state is equal to 0 which would indicate that the LED is on. If it isn't 0, then an error
would be thrown and test case 1 would fail which would be indicated on the serial monitor. Next, "taken" is set to true to produce the situation where another task has
the mutex key. This time OnLED sends a message to the serial monitor saying that the key was not obtained as seen in the output file. The last test requires "task" to be empty
to produce the situation where the task wasn't created but tries to run. OnLED does not get to run and a message is output to the serial monitor which says the task was not created
as seen in the output file. Therefore, all of the test cases passed. </p>


## Reflecting on Integration Testing

<p align="justify"> A driver function was created for the task function which is supposed to turn on a LED. "pin_state" was used to track the pin's state which would determine if the LED 
is on or off. Instead of using the stubs created for the mutex in the unit tests, the actual mutex FreeRTOS API functions should have been called in the driver function.
This would have ensured that the test results are more accurate since they are used in the final version of the program. However, the stubs were written to return the 
same values as the API functions and so it would still achieve the same end result. Therefore, for more complicated API functions, it would be wiser to use them in the
integration test instead of the stubs. </p>

## Verification Testing

The functional requirements of the program were as follows: 
1. One task to turn the LED on and actively wait for 0.5s and then task delay for 1 second.
2. Another task to turn the LED off and actively wait for 0.5s and then task delay for 1 second.
3. The third task will print a status message only, and task-delay for 1 second.

<p align="justify">The functional requirements were verified using debug messages output on the serial monitor. Each message has a time-stamp relative to the start-up time of the program.
This was used to check if the active wait was 0.5 seconds and if the task delay was 1 second. Additionally, a multimeter was used to check the voltage levels on the GPIO2 pin
to check if the state of the pin was toggling between 0(0V) and 1(3.3V). This can be seen in the images provided in the verification testing project folder labelled 
"High Voltage" and "Low Voltage". The video in the link below shows the flashing of the LED on and off: </p>

[LED Flashing Video](https://youtube.com/shorts/s09NqaZRLHs?feature=share)

<p align="justify">  As seen in the video, the LED is off longer than it is on because of the sequence of tasks being run. This is because the LED turns on and actively waits for 0.5 seconds. 
It then turns off, actively waits for 0.5 seconds, produces the status message and then it waits 0.5 seconds until the LED turns on again due to the task delay. 
Hence it is on for 0.5 seconds and off for 1 second. This is also seen in the timestamps provided by the debug messages in the output file labelled "verification_testing". 
Looking at lines 33-39 in the output file, we can see that Task 1 and 2 actively wait for 0.5 seconds. After task 3 completes, the extra 0.5 seconds wait is due to
the task delay of 1 second after the completion of task 1 hence it runs at 1741 ms in line 39. </p>

## Reflecting on Verification Testing

<p align="justify"> Another way to determine whether functional requirements 1,2 and 3 were met was to use an oscilloscope. The oscilloscope would be able to determine the active
wait time as well as the task delay time. It also helps in verifying functional requirements 3 since it can check the pin voltage and hence the pin state , 0 or 1.</p>








