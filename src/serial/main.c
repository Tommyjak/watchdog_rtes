#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"

//#define SEQ_MODE
#define CONC_MODE

#ifdef SEQ_MODE
// Sequential mode: Tasks run one after another without concurrency

#define THRESHOLD __INT16_MAX__
#define LED_PIN 25  // GPIO del LED integrato del Pico


struct SharedResource {
    __int16_t data;
} shared_resource;

void init_shared_resource(struct SharedResource *res) {
    res->data = 0;
}

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
    gpio_put(LED_PIN, 1); // Accende LED per segnalare fault
    while(true){
        printf("Random Fault Task: Simulating random fault...\n");
        sleep_ms(500); // Non saturare il terminale
    }
}

int main() {
    // Initialization of serial communication
    stdio_init_all();

    // Initialization of the LED pin
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0); // Spegne LED all'inizio

    // Infinite loop to simulate task execution
    while (true) {
        init_shared_resource(&shared_resource);
        checker_task(&shared_resource);
        incrementer_task(&shared_resource);

        // Randomly (.1% chanche) trigger of random fault task
        if (rand() % 1000 == 10) {
            random_fault_task(&shared_resource);
        }
        resetter_task(&shared_resource);
    }
}
#endif

#ifdef CONC_MODE
int main() {
    stdio_init_all();
    while (true) {
        printf("Concurrent mode is not implemented in this example.\n");
    }
}
#endif