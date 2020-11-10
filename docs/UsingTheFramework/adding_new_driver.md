# Developing a New Device Driver

## Embedded VM Driver Ethos



## Template Files

## Process

Users also need to write their own drivers in their source without messing with the framework. Make templates + examples for this.


Drivers use multiple inheritence to compose functionality:
Unfortunately, misguided usage has made this once popular feature almost completely disappear. On the other hand, sometimes you want to inherit from parents that are completely separate from each other. This is where multiple inheritance can become productive. Classes that have orthogonal behaviors are the best case scenario for multiple inheritance.


* The kernel is a slave to the application. Code in the kernel (such as in a driver) is passive in that it only reacts to requests from processes in user space. Drivers should not initiate any I/O activity on their own.
* User processes cannot take direct interrupts. As a corollary to the previous point, kernel interrupt threads cannot jump to user space. Instead, if your application must be made aware of interrupts, it should provide a thread on which to deliver a notification of them.

start() should begin operation + initiate HW
