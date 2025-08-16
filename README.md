# Multithreaded CPU Scheduling Simulator (C++)

A lightweight C++17 simulator demonstrating **OS scheduling algorithms** with **multithreaded workers**:
- **Round Robin (RR)**
- **Shortest Job First (SJF)** (non-preemptive)
- **Priority** (non-preemptive, smaller number = higher priority)

It showcases OS fundamentals required by roles like NVIDIA SWE Intern: **multiprocessing & scheduling, memory management concepts, synchronization, and parallelism**.

## Quick Start

### Build (Makefile)
```bash
make
./scheduler --policy rr --cores 2 --quantum 4
```

### Build (CMake)
```bash
mkdir -p build && cd build
cmake .. && cmake --build . -j
./scheduler --policy sjf --cores 1
```

## Usage
```bash
./scheduler [--policy rr|sjf|priority] [--cores N] [--quantum Q] [--input file.csv]
```

- `--policy`  : Scheduling policy (default: `rr`).
- `--cores`   : Number of worker threads / CPU cores (default: `2`).
- `--quantum` : Time quantum (ticks) for RR (default: `4`). Ignored for SJF/Priority.
- `--input`   : Optional CSV file with processes: `pid,arrival,burst,priority`

If no input is provided, the program uses a small **sample workload**.

## CSV Example
```
# pid,arrival,burst,priority
P1,0,7,2
P2,1,4,1
P3,2,9,3
P4,3,5,2
```

## Output
- Per-process metrics: **completion time, turnaround, waiting**
- Aggregates: **average turnaround**, **average waiting**, **throughput**

## Design Notes
- **Logical time** is used for metrics; worker threads simulate execution and use **mutex/condition_variable** to coordinate with the scheduler.
- Ready queues differ by policy:
  - **RR**: FIFO queue with quantum slicing (re-enqueue if remaining > 0).
  - **SJF**: Min-heap ordered by **remaining/burst time** (non-preemptive).
  - **Priority**: Min-heap ordered by **priority** then arrival (non-preemptive).
- Demonstrates **synchronization** (mutexes), **safe shared state**, and **scalable worker pools**.

## License
MIT
