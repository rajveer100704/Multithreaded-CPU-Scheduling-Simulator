🖥️ Multithreaded CPU Scheduling Simulator (C++17)

A lightweight C++17 simulator demonstrating classic Operating System CPU scheduling algorithms with multithreaded worker threads:

✅ Round Robin (RR)

✅ Shortest Job First (SJF) (non-preemptive)

✅ Priority Scheduling (non-preemptive, smaller number = higher priority)

This project showcases Operating System fundamentals relevant for roles like NVIDIA SWE Intern:

Multiprocessing & scheduling

Memory management concepts

Synchronization & parallelism

System-level programming with C++17

⚡ Features


Implements 3 CPU scheduling policies: RR, SJF, Priority

Multithreaded execution using std::thread, mutex, and condition_variable

Tracks per-process and aggregate metrics:

Completion Time

Waiting Time

Turnaround Time

Average Waiting / Turnaround Time

Throughput

Modular design with Makefile and CMake support

🚀 Quick Start
🔹 Build with Make
make
./scheduler --policy rr --cores 2 --quantum 4


🔹 Build with CMake
mkdir -p build && cd build
cmake .. && cmake --build . -j
./scheduler --policy sjf --cores 1

🔹 Usage
./scheduler [--policy rr|sjf|priority] [--cores N] [--quantum Q] [--input file.csv]

--policy  : Scheduling policy (default: rr)
--cores   : Number of worker threads / CPU cores (default: 2)
--quantum : Time quantum (ticks) for RR (default: 4, ignored for SJF/Priority)
--input   : Optional CSV file with processes → pid,arrival,burst,priority

If no input is provided, the program uses a sample workload.

