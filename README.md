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

# Coding
If you are able to resolve all the issues and you have the Raspberry Pi Pico board, you can start coding in the src/serial directory in the .c file.

# Compiling and running
1. Open a terminal and navigate to the project directory:
   ```bash
   cd /home/your_username/Documents/watchdog_rtes
   ```
2. Run the following commands:
   ```bash
   ./setenv
   ./compile.sh
   ```
3. If no errors are reported, you've successfully compiled the project and flashed the firmware to the Raspberry Pi Pico board.
4. To see the output of Raspberry Pi Pico, execute the following command:
   ```bash
   minicom -D /dev/ttyACM0 -b 9600
   ```
5. To exit the minicom interface press `Ctrl + A` followed by `X` and then `Enter`.

## Important note
You'll need to install some packages and tool and i don't remember all of them, we'll se together.
Some of the are: gcc, cmake, ...