# User-space and Kernel-space Task Schedulers

## Introduction
**Platforms** - Linux

This project focused on synchronization using mutexes and semaphores in a multithreaded scheduling environment. It consists of a user space scheduler, which interfaces with a kernel space scheduler via syscalls, and a kernel space scheduler, which manages tasks and resources. The primary goals of this project include implementing a simulation for task scheduling and handling task creation and management.

### Features
* Adds tasks and resources to a scheduler environment
* Acquires and releases resources to execute tasks
* Uses multithreading to handle tasks and resources concurrently
* Aging policy that increases the priority of tasks that have been waiting for a while


## Installation and Setup
### Setup

Install up-to-date dependencies for building this program and going through the demo:

**Debian**
```bash
sudo apt update
sudo apt install gcc make valgrind gedit coreutils
```

### Build and Compile
Compile the shell executable using the Makefile:
```bash
cd project2/user_space
make
gcc -o ../tests/kernel_tests ../tests/kernel_tests.c
```
Run the simulation executable using the provided makefile in project2/user_space/:
```bash
make run
```
or run using Valgrind for memory leak detection:
```bash
make val
```
Run the kernel space simulation from the project2/tests/ directory and read dmesg output to track execution of tasks:
```bash
./kernel_tests
sudo dmesg
```

## Troubleshooting
### Known Issues
None

## References
### External Resources
* [Programiz](https://www.programiz.com/)
* [GeeksForGeeks](https://www.geeksforgeeks.org/)
* [cppreference](https://en.cppreference.com/) 
* [tutorialspoint](https://www.tutorialspoint.com/)
* [man7](https://www.man7.org/)
