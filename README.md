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


make

<img width="639" height="80" alt="image" src="https://github.com/user-attachments/assets/a60521dc-0dfe-4303-bd6b-7eebb45fd2dd" />


mkdir -p build

cd build

cmake ..

cmake --build .

./scheduler --policy sjf --cores 1


Usage

./scheduler [--policy rr|sjf|priority] [--cores N] [--quantum Q] [--input file.csv]

Options

--policy : Scheduling policy (rr, sjf, priority) (default: rr)

--cores : Number of worker threads / CPU cores (default: 2)

--quantum : Time quantum (ticks) for RR (default: 4, ignored for SJF/Priority)

--input : Optional CSV file with processes → pid,arrival,burst,priority

If no input is provided, the program uses a sample workload.

