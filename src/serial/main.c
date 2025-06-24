#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "task.h"

#define THRESHOLD __INT8_MAX__

#define LED_PIN 25  // GPIO of the integrated LED on the Pico

struct SharedResource {
    __int8_t data;
} shared_resource;

void init_shared_resource(struct SharedResource *res) {
    res->data = 0;
}

//#define SEQ_MODE
#define CONC_MODE

#ifdef SEQ_MODE
// Sequential mode: Tasks run one after another without concurrency

void checker_task(struct SharedResource *res){
    if (res->data >= THRESHOLD){
        printf("Checker Task: Data exceeds threshold (%d >= %d)\n", res->data, THRESHOLD);
    } else {
        printf("Checker Task: Data is within threshold (%d < %d)\n", res->data, THRESHOLD);
    }
}
void incrementer_task(struct SharedResource *res){
    res->data++;
    printf("Incrementer Task: Data = %d\n", res->data);
}

void resetter_task(struct SharedResource *res){
    res->data = 0;
    printf("Resetter Task: Data reset to 0\n");
}

void random_fault_task(struct SharedResource *res){
    gpio_put(LED_PIN, 1); // Turn on LED to signal fault
    while(true){
        printf("Random Fault Task: Simulating random fault...\n");
        sleep_ms(500); // Avoid terminal flooding
    }
}

int main() {
    // Initialize serial communication
    stdio_init_all();

    // Initialize the LED pin
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0); // Turn off LED at start

    // Infinite loop to simulate task execution
    while (true) {
        init_shared_resource(&shared_resource);
        checker_task(&shared_resource);
        incrementer_task(&shared_resource);

        // Randomly (0.1% chance) trigger the random fault task
        if (rand() % 1000 == 10) {
            random_fault_task(&shared_resource);
        }
        resetter_task(&shared_resource);
    }
}
#endif

#ifdef CONC_MODE

#define PERIOD_CHECKER 50 // ms
#define PERIOD_INCREMENTER 100 // ms
#define PERIOD_RESETTER 200 // ms

#define EXECUTION_TIME_CHECKER 10 // ms
#define EXECUTION_TIME_INCREMENTER 20 // ms
#define EXECUTION_TIME_RESETTER 40 // ms

// Global handle for the fault task
TaskHandle_t faultHandle = NULL;

// Mutex for shared resource access
SemaphoreHandle_t xMutex;

void checker_task(void *pvParameters);
void incrementer_task(void *pvParameters);
void resetter_task(void *pvParameters);
void random_fault_task(void *pvParameters);

int main() {
    // Initialization of serial communication
    stdio_init_all();

    sleep_ms(5000); // Allow time for serial to initialize

    // Initialize the LED pin
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0); // Turn off LED at start

    // Initialization of the shared resource
    init_shared_resource(&shared_resource);

    // Mutex initialization
    xMutex = xSemaphoreCreateMutex();

    // Error handling in mutex creation
    if (xMutex == NULL) {
        // Error creating the mutex
        printf("Error creating mutex!\n");
        while (1);
    }

    // Task creation
    xTaskCreate(checker_task,       "Checker Task",         256, &shared_resource, 4, NULL);
    xTaskCreate(incrementer_task,   "Incrementer Task",     256, &shared_resource, 3, NULL);
    xTaskCreate(random_fault_task,  "Random Fault Task",    256, &shared_resource, 2, &faultHandle);
    xTaskCreate(resetter_task,      "Resetter Task",        256, &shared_resource, 1, NULL);

    // Start the FreeRTOS scheduler
    vTaskStartScheduler();

    // Should never reach here unless heap/memory config is wrong
    while (1);
}

void checker_task(void *pvParameters) {
    struct SharedResource *res = (struct SharedResource *) pvParameters;

    // Initial tick reference
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for ( ;; ) {

        // Start measuring execution time
        TickType_t start = xTaskGetTickCount();
        printf("Checker Task: Starting execution at tick %lu\n", start);

        //-----------TASK BODY-----------
        
        // Waiting for the mutex to be available
        xSemaphoreTake(xMutex, portMAX_DELAY);
    
        // Check the shared resource data
        if(res->data >= THRESHOLD) {

            printf("Checker Task: Data exceeds threshold (%d >= %d)! Rebooting System.\n", res->data, THRESHOLD);

            // Reset shared resource
            res->data = 0;

            // Stop the fault task
            if (faultHandle != NULL) {
                vTaskDelete(faultHandle);
                faultHandle = NULL;
                printf("Checker Task: Fault task terminated.\n");
            }

            gpio_put(LED_PIN, 0); // Turn on LED to signal fault

            // Recreate it (fresh start)
            xTaskCreate(random_fault_task, "Random Fault Task", 256, res, 2, &faultHandle);
            printf("------------------------------Checker Task: Fault task restarted.------------------------------\n");

        }

        // Simulate execution time
        vTaskDelay(pdMS_TO_TICKS(EXECUTION_TIME_CHECKER));
    
        // Release the mutex
        xSemaphoreGive(xMutex);
        
        //-----------END TASK BODY-----------

        TickType_t end = xTaskGetTickCount();
        printf("Checker Task: Finished execution at tick %lu\n", end);
        TickType_t exec_time = end - start;
        printf("Checker Task: Execution time = %lu ticks\n", exec_time);

        // Check if task missed its deadline
        if (exec_time > pdMS_TO_TICKS(PERIOD_CHECKER)) {
            printf("Deadline MISSED by Checker!\n");
        }

        // Wait until the start of the next period
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(PERIOD_CHECKER));

    }
}

void incrementer_task(void *pvParameters) {
    struct SharedResource *res = (struct SharedResource *) pvParameters;

    // Initial tick reference
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for ( ;; ) {

        // Measure start time
        TickType_t start = xTaskGetTickCount();
        printf("Incrementer Task: Starting execution at tick %lu\n", start);

        //-----------TASK BODY-----------

        // Waiting for the mutex to be available
        xSemaphoreTake(xMutex, portMAX_DELAY);

        // Increment the shared resource data
        res->data++;
        printf("Incrementer Task: Incremented data to %d\n", res->data);

        // Simulate execution time
        vTaskDelay(pdMS_TO_TICKS(EXECUTION_TIME_INCREMENTER));

        // Release the mutex
        xSemaphoreGive(xMutex);

        //-----------END TASK BODY-----------

        // Measure end time
        TickType_t end = xTaskGetTickCount();
        printf("Incrementer Task: Finished execution at tick %lu\n", end);

        // Calculate execution time
        TickType_t exec_time = end - start;
        printf("Incrementer Task: Execution time = %lu ticks\n", exec_time);

        // Check if task missed its deadline
        if (exec_time > pdMS_TO_TICKS(PERIOD_INCREMENTER)) {
            printf("Deadline MISSED by Incrementer!\n");
        }

        // Wait until the start of the next period
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(PERIOD_INCREMENTER));

    }
}

void resetter_task(void *pvParameters) {
    struct SharedResource *res = (struct SharedResource *) pvParameters;

    // Initial tick reference
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for ( ;; ) {
        
        // Measure start time
        TickType_t start = xTaskGetTickCount();
        printf("Resetter Task: Starting execution at tick %lu\n", start);

        //-----------TASK BODY-----------

        // Waiting for the mutex to be available
        xSemaphoreTake(xMutex, portMAX_DELAY);

        // Reset the shared resource data
        res->data = 0;
        printf("Resetter Task: Data reset to 0\n");

        // Simulate execution time
        vTaskDelay(pdMS_TO_TICKS(EXECUTION_TIME_RESETTER));

        // Release the mutex
        xSemaphoreGive(xMutex);

        //-----------END TASK BODY-----------

        // Measure end time
        TickType_t end = xTaskGetTickCount();
        printf("Resetter Task: Finished execution at tick %lu\n", end);

        // Calculate execution time
        TickType_t exec_time = end - start;
        printf("Resetter Task: Execution time = %lu ticks\n", exec_time);

        // Check if task missed its deadline
        if (exec_time > pdMS_TO_TICKS(PERIOD_RESETTER)) {
            printf("Deadline MISSED by Resetter!\n");
        }

        // Wait until the start of the next period
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(PERIOD_RESETTER));

    }
}

void random_fault_task(void *pvParameters) {
    struct SharedResource *res = (struct SharedResource *) pvParameters;

    // Simula un tempo di funzionamento corretto prima del fault

    vTaskDelay(pdMS_TO_TICKS(10000));  // 10 second(s)

    for ( ;; ) {
        // Randomly (0.1% chance) trigger the random fault task
        if (rand() % 100000 == 10) {
            printf("Random Fault Task: Simulating random fault\n");
            gpio_put(LED_PIN, 1); // Turn on LED to signal fault
            for ( ;; ) {}
        }
    }

}

#endif