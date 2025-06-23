# RTES PROJECT
This project is a simple implementation of a Real-Time Embedded System (RTES) using C programming language. It aims to develop a watchdog mechanism to monitor two systems health by monitoring their correct synchronous access to a sared resource and its modification (increasing it or resetting it).

# How to use the project
0. I suggest you download VSCode, it's very useful and easy to use.
1. It's mandatory to be using a Linux environement, preferably Ubuntu (I'm using Ubuntu 22.04).
2. Clone this repository to you local machine:
   ```bash
   git clone https://github.com/Tommyjak/watchdog_rtes.git
   ```

   N.B: I suggest you clone it in /home/your_username/Documents/

3. Clone the following repository to your local machine:
   ```bash
   git clone https://github.com/raspberrypi/pico-sdk.git
    ```

   N.B: I suggest you clone it in /home/your_username/Documents/

4. Clone the following repository to your local machine:
   ```bash
   git clone https://github.com/FreeRTOS/FreeRTOS-Kernel.git
    ```

   N.B: I suggest you clone it in /home/your_username/Documents/

# Coding
If you are able to resolve all the issues and you have the Raspberry Pi Pico board, you can start coding in the src/serial directory in the .c file.

In the main.c file, you can select the mode of operation (SEQ_MODE or CONC_MODE) by uncommenting the corresponding line. The SEQ_MODE is for sequential implementation of the execution, while the CONC_MODE is for concurrent execution, with shared memory, synchronization and generally Real-Time concepts.

# Compiling and running
1. Open a terminal and navigate to the project directory:
   ```bash
   cd /home/your_username/Documents/watchdog_rtes
   ```
2. Run the following commands:
   ```bash
   source setenv
   ./compile.sh
   ```
3. If no errors are reported, you've successfully compiled the project and flashed the firmware to the Raspberry Pi Pico board.
4. To see the output of Raspberry Pi Pico, execute the following command:
   ```bash
   minicom -D /dev/ttyACM0 -b 9600
   ```

   IMP! To make sure the Raspberry Pi Pico is accessible via the file /dev/ttyACM0, you need to add the command:
   `stdio_init_all();`
   in the main.c file of the src/serial directory.
5. To exit the minicom interface press `Ctrl + A` followed by `X` and then `Enter`.

## Important note
You'll need to install some packages and tool and i don't remember all of them, we'll se together.
Some of the are: gcc, cmake, ...

# TODO
- Remove lines of print to clean up the output.
- Implement a fetching of data from logs of output to plot shared resource increasement and reset over time.
- Prepare a theoretical report of the project.
- Prepare a presentation (.pptx or canvas) of the project. 