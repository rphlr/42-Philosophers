# Philosopher's Problem

## Overview
This program aims to solve the classic computer science problem known as the "Dining Philosophers" problem. The code is written in C and uses POSIX threads (pthreads) and mutex locks to simulate philosophers eating at a round table.

## Code Explanation
The code follows the following steps:

1. It creates the philosophers and assigns them a fork (which are modeled as mutex locks).
2. It then creates a thread for each philosopher to perform a life cycle (eating, sleeping, and thinking).
3. An overseer thread is used to monitor the philosophers' states and print out their current actions.

Each philosopher is modeled as a struct (`t_philo`) with properties like ID, current state, times to sleep/eat, etc. Similarly, a utility structure (`t_utils`) is used to hold the program start time and global stop conditions.

The `main` function verifies the input parameters (number of philosophers, time to sleep/eat, etc.) and then initializes the philosophers and their life cycles.

The life cycle (`life_cycle` function) of a philosopher involves picking up forks (mutex locks), eating, sleeping, and thinking in a loop until a stopping condition is reached. The status of each action is printed out.

A few important functions are:

- `can_pick_fork`: Checks if a philosopher can pick up a fork.
- `eat`, `sleep`, `think`: Simulate the actions of a philosopher.
- `init`: Initialize the philosophers and their threads.
- `overseer`: Monitors the philosophers' states and handles stop conditions.

## Usage
The program expects the following command-line arguments:

1. Number of philosophers
2. Time until a philosopher dies without eating
3. Time a philosopher needs to eat
4. Time a philosopher needs to sleep
5. (Optional) Number of times each philosopher must eat

Example: `./program_name 5 800 200 200 7`

This would simulate 5 philosophers, where each philosopher would die if they don't start eating within 800 milliseconds, takes 200 milliseconds to eat, and sleeps for 200 milliseconds after eating. Each philosopher should eat 7 times.

## Tips

- Make sure your command-line arguments are valid numbers. The program doesn't check if the input parameters are valid numbers.
- The program doesn't limit the number of philosophers, but having too many philosophers could lead to system performance issues.
- This code uses POSIX threads and mutex locks, which are non-portable and may not work on non-POSIX systems.
- This code doesn't include any error checking for the creation of threads or initialization of mutex locks. In a real-world application, it would be good to check the return values of these functions for any errors.
- Make sure to have a basic understanding of threads, mutex locks, and the dining philosophers problem to understand this code.

## Disclaimer
This code is provided as-is and may have potential bugs or undefined behaviors. Please use at your own discretion.