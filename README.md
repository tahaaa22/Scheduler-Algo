Project Name: Process Scheduler Simulation
This is a program that simulates the operation of the process scheduler in a multiprocessor system and calculates some related statistics in order to help improve the overall scheduling system.

Project Overview
In a multiprocessor system, multiple processes are running concurrently. The process scheduler is responsible for determining which process gets to run on a CPU core at any given time. The scheduling algorithm used can have a significant impact on system performance, and it is important to choose the right algorithm for the system.

This project implements four different scheduling algorithms, including First Come First Serve (FCFS), Shortest Job First (SJF), Round Robin (RR), and Earliest Deadline First (EDF). Each algorithm has its own unique ready queue and specific scheduling algorithm to determine which process should be scheduled next.

Project Details
The program is designed to simulate a multiprocessor system with six different states for each process. The states include:

New: When a process is in the new queue and waiting to be assigned to a CPU core.
Ready: When a process is in the ready queue and waiting to be scheduled.
Run: When a process is currently executing on a CPU core.
Block: When a process is waiting for I/O resources and is inside the block queue.
Terminated: When a process's CPU time is finished and it no longer has time remaining to execute.
Orphan: When a process is a child of a parent process that is formed inside the FCFS processor.
The program also supports various actions that can happen to a process, such as stealing, migration, or being killed by a killing signal. After the simulation is complete, an output file is generated that contains all the details of the system, including statistics related to the scheduling algorithm used.

How to Use
To use this program, simply download the source code from the GitHub repository and compile it using your preferred compiler. Run the executable file and provide the necessary input, such as the number of processes, their burst times, and any other parameters required by the scheduling algorithm.

The program will then simulate the scheduling algorithm and generate an output file containing the results. You can use these results to analyze the performance of the scheduling algorithm and make improvements to the overall system.

Conclusion
This project provides a useful tool for simulating the operation of the process scheduler in a multiprocessor system. With support for multiple scheduling algorithms and various actions that can happen to a process, it is an effective way to evaluate the performance of different scheduling algorithms and improve the overall system.
