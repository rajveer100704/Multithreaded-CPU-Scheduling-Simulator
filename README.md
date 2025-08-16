🖥️ Multithreaded CPU Scheduling Simulator (C++17)

A lightweight C++17 simulator demonstrating classic Operating System CPU scheduling algorithms with multithreaded worker threads:

✅ Round Robin (RR)

✅ Shortest Job First (SJF) (non-preemptive)

✅ Priority Scheduling (non-preemptive, smaller number = higher priority)

This project showcases Operating System fundamentals :

1.Multiprocessing & scheduling

2.Memory management concepts

3.Synchronization & parallelism

4.System-level programming with C++17

⚡ Features


Implements 3 CPU scheduling policies: RR, SJF, Priority

Multithreaded execution using std::thread, mutex, and condition_variable

Tracks per-process and aggregate metrics:

  1. Completion Time

  2. Waiting Time

  3. Turnaround Time

  4. Average Waiting / Turnaround Time

  5. Throughput

Modular design with Makefile and CMake support

🚀 Quick Start

Build with Make:

<img width="639" height="80" alt="image" src="https://github.com/user-attachments/assets/a60521dc-0dfe-4303-bd6b-7eebb45fd2dd" />


Build with CMake:


<img width="492" height="121" alt="image" src="https://github.com/user-attachments/assets/afc9ac4a-809f-4c28-8a73-0f90d3abd91f" />

Usage

<img width="975" height="264" alt="image" src="https://github.com/user-attachments/assets/53a4590d-078a-47c1-b19a-fe6e8861f58c" />

If no input is provided, the program uses a sample workload.

CSV Example:

<img width="394" height="212" alt="image" src="https://github.com/user-attachments/assets/dbb20b20-7929-4f42-84dd-797a6d7c3abb" />

Sample Output:

| Process | Arrival | Burst | Priority | Completion | Turnaround | Waiting |
| ------- | ------- | ----- | -------- | ---------- | ---------- | ------- |
| P1      | 0       | 7     | 2        | 7          | 7          | 0       |
| P2      | 1       | 4     | 1        | 11         | 10         | 6       |
| P3      | 2       | 9     | 3        | 20         | 18         | 9       |
| P4      | 3       | 5     | 2        | 25         | 22         | 17      |


