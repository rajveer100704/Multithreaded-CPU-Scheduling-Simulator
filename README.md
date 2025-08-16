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
      
      ./scheduler --policy rr --cores 2 --quantum 4


    Build with CMake:

      mkdir -p build && cd build
      cmake .. && cmake --build . -j
      ./scheduler --policy sjf --cores 1
 

    Usage

      ./scheduler [--policy rr|sjf|priority] [--cores N] [--quantum Q] [--input file.csv]

        --policy  : Scheduling policy (default: rr)
        --cores   : Number of worker threads / CPU cores (default: 2)
        --quantum : Time quantum (ticks) for RR (default: 4, ignored for SJF/Priority)
        --input   : Optional CSV file with processes → pid,arrival,burst,priority

       If no input is provided, the program uses a sample workload.

 CSV Example:

     # pid,arrival,burst,priority
        P1,0,7,2
        P2,1,4,1
        P3,2,9,3
        P4,3,5,2


 Sample Output:

     | Process | Arrival | Burst | Priority | Completion | Turnaround | Waiting |
     | ------- | ------- | ----- | -------- | ---------- | ---------- | ------- |
     | P1      | 0       | 7     | 2        | 7          | 7          | 0       |
     | P2      | 1       | 4     | 1        | 11         | 10         | 6       |
     | P3      | 2       | 9     | 3        | 20         | 18         | 9       |
     | P4      | 3       | 5     | 2        | 25         | 22         | 17      |

     Average Waiting Time: 8.0
     Average Turnaround Time: 14.25
     Throughput: 0.16 processes/unit time

💡Design Notes:

    Logical time is used for metrics.

     Worker threads simulate execution & coordinate with the scheduler using **mutex + condition_variable.**

     Ready queues differ by policy:

      1.RR → FIFO queue with quantum slicing

      2.SJF → Min-heap ordered by burst time

      3.Priority → Min-heap ordered by priority → arrival

🎯 Learning Outcomes

    Deep understanding of CPU scheduling policies

    Hands-on experience with C++ multithreading & synchronization

    Familiarity with system-level build tools (Make, CMake)

    Strong foundation for driver / OS development roles

    Demonstrates safe shared state, scalable worker pools, and OS-level scheduling behavior.

🔮 Future Work / Improvements

    🧵 Preemptive Scheduling: Implement Preemptive SJF (Shortest Remaining Time First) and Preemptive Priority Scheduling

    💻 Multi-core / Load Balancing: Dynamically distribute processes across multiple CPU cores

    🎨 Visualization: Add a GUI or Gantt Chart to visualize process execution timelines

    📂 Configurable Input: Support larger workloads and JSON/YAML-based configuration files

    ⚡ Performance Profiling: Benchmark algorithms for throughput and CPU utilization

    ☁️ Distributed Simulation: Extend to cluster-level scheduling across multiple machines


📊 Graphical Gantt Chart / Timeline (Markdown)

    Legend:

     🟦 P1 | 🟩 P2 | 🟨 P3 | 🟪 P4

  
     Round Robin (Quantum = 4)
 
      P1 : 🟦🟦🟦🟦     🟦🟦🟦

      P2 :     🟩🟩🟩🟩

      P3 :             🟨🟨🟨🟨🟨🟨🟨🟨🟨
   
      P4 :                         🟪🟪🟪🟪🟪
  
 
     SJF (Non-preemptive)
   
      P1 :             🟦🟦🟦🟦🟦🟦🟦
    
      P2 : 🟩🟩🟩🟩
    
      P3 :                         🟨🟨🟨🟨🟨🟨🟨🟨🟨
   
      P4 :     🟪🟪🟪🟪🟪

  
     Priority Scheduling
   
      P1 :     🟦🟦🟦🟦🟦🟦🟦
    
      P2 : 🟩🟩🟩🟩
   
      P3 :                         🟨🟨🟨🟨🟨🟨🟨🟨🟨
    
      P4 :             🟪🟪🟪🟪🟪


    ## 📬 Contact
     Created by **Rajveer Singh Saggu**  
      📧 Email: [rajveer19255@gmail.com](mailto:rajveer19255@gmail.com)  
      🔗 LinkedIn: [Rajveer Singh Saggu](https://www.linkedin.com/in/rajveer-singh-13aa952)  

       ---
     Made with ❤️ in C++17 to simplify OS scheduling concepts.
