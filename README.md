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

## Important note
You'll need to install some packages and tool and i don't remember all of them, we'll se together.
Some of the are: gcc, cmake, ...