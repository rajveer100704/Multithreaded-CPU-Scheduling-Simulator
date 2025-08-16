#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <map>
#include <mutex>
#include <optional>
#include <queue>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

struct Process {
    std::string pid;
    int arrival = 0;
    int burst = 0;
    int priority = 0;
    int remaining = 0;
    int start_time = -1;
    int finish_time = -1;
};

struct Metrics {
    int turnaround = 0;
    int waiting = 0;
};

enum class Policy { RR, SJF, PRIORITY };

struct RRQueue {
    std::queue<int> q;
    void push(int i) { q.push(i); }
    bool empty() const { return q.empty(); }
    int pop() { int i = q.front(); q.pop(); return i; }
};

struct SJFQueue {
    struct Cmp {
        const std::vector<Process>* procs;
        Cmp(const std::vector<Process>* p=nullptr): procs(p) {}
        bool operator()(int a, int b) const {
            return (*procs)[a].remaining > (*procs)[b].remaining;
        }
    };
    std::priority_queue<int,std::vector<int>,Cmp> pq;
    SJFQueue(const std::vector<Process>* p=nullptr): pq(Cmp(p)) {}
    void push(int i){ pq.push(i); }
    bool empty() const { return pq.empty(); }
    int pop(){ int i=pq.top(); pq.pop(); return i; }
};

struct PriorityQueueQ {
    struct Cmp {
        const std::vector<Process>* procs;
        Cmp(const std::vector<Process>* p=nullptr): procs(p) {}
        bool operator()(int a, int b) const {
            const auto& A = (*procs)[a];
            const auto& B = (*procs)[b];
            if (A.priority == B.priority) return A.arrival > B.arrival;
            return A.priority > B.priority;
        }
    };
    std::priority_queue<int,std::vector<int>,Cmp> pq;
    PriorityQueueQ(const std::vector<Process>* p=nullptr): pq(Cmp(p)) {}
    void push(int i){ pq.push(i); }
    bool empty() const { return pq.empty(); }
    int pop(){ int i=pq.top(); pq.pop(); return i; }
};

struct Scheduler {
    Policy policy = Policy::RR;
    int cores = 2;
    int quantum = 4;
    std::vector<Process> procs;

    std::mutex m;
    std::condition_variable cv_ready;
    bool all_arrived = false;
    std::atomic<int> remaining_proc{0};

    RRQueue rrq;
    SJFQueue sjfq{&procs};
    PriorityQueueQ prioq{&procs};

    Scheduler(Policy p, int c, int q): policy(p), cores(c), quantum(q) {}

    int current_time = 0;

    void enqueue_if_arrived(int now){
        for (int i=0;i<(int)procs.size();++i){
            if (procs[i].arrival == now){
                procs[i].remaining = procs[i].burst;
                if (policy == Policy::RR) rrq.push(i);
                else if (policy == Policy::SJF) sjfq.push(i);
                else prioq.push(i);
            }
        }
    }

    std::optional<int> next_proc(){
        if (policy == Policy::RR) {
            if (rrq.empty()) return std::nullopt;
            return rrq.pop();
        } else if (policy == Policy::SJF) {
            if (sjfq.empty()) return std::nullopt;
            return sjfq.pop();
        } else {
            if (prioq.empty()) return std::nullopt;
            return prioq.pop();
        }
    }

    void requeue_if_needed(int idx){
        if (procs[idx].remaining > 0){
            if (policy == Policy::RR) rrq.push(idx);
            else if (policy == Policy::SJF) sjfq.push(idx);
            else prioq.push(idx);
        } else {
            procs[idx].finish_time = current_time;
            remaining_proc--;
        }
    }

    void dispatcher_thread(){
        int last_arrival = 0;
        for (auto& p: procs) last_arrival = std::max(last_arrival, p.arrival);

        while (true){
            {
                std::unique_lock<std::mutex> lk(m);
                enqueue_if_arrived(current_time);
                cv_ready.notify_all();
            }
            if (remaining_proc.load() == 0) break;
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
            current_time++;
            if (current_time > 100000) break;
        }
        {
            std::lock_guard<std::mutex> lk(m);
            all_arrived = true;
            cv_ready.notify_all();
        }
    }

    void worker_thread(int){
        while (true){
            int idx = -1;
            {
                std::unique_lock<std::mutex> lk(m);
                cv_ready.wait(lk, [&]{ return remaining_proc.load()==0 || !ready_empty(); });
                if (remaining_proc.load()==0) break;
                auto np = next_proc();
                if (!np.has_value()) continue;
                idx = *np;
                if (procs[idx].start_time == -1) procs[idx].start_time = current_time;
            }

            int slice = (policy == Policy::RR) ? std::min(quantum, procs[idx].remaining) : procs[idx].remaining;
            std::this_thread::sleep_for(std::chrono::milliseconds(slice));
            {
                std::lock_guard<std::mutex> lk(m);
                procs[idx].remaining -= slice;
                requeue_if_needed(idx);
                cv_ready.notify_all();
            }
        }
    }

    bool ready_empty(){
        if (policy == Policy::RR) return rrq.empty();
        if (policy == Policy::SJF) return sjfq.empty();
        return prioq.empty();
    }

    void run(){
        remaining_proc = (int)procs.size();
        std::thread disp(&Scheduler::dispatcher_thread, this);
        std::vector<std::thread> workers;
        for (int i=0;i<cores;i++) workers.emplace_back(&Scheduler::worker_thread, this, i);
        disp.join();
        {
            std::lock_guard<std::mutex> lk(m);
            cv_ready.notify_all();
        }
        for (auto& t: workers) t.join();
    }
};

std::vector<Process> load_csv(const std::string& path){
    std::vector<Process> v;
    std::ifstream f(path);
    if (!f.good()) return v;
    std::string line;
    while (std::getline(f,line)){
        if (line.size()==0 || line[0]=='#') continue;
        std::stringstream ss(line);
        std::string pid, sarr, sburst, sprio;
        if (std::getline(ss,pid,',') && std::getline(ss,sarr,',') &&
            std::getline(ss,sburst,',') && std::getline(ss,sprio,',')){
            Process p;
            p.pid = pid;
            p.arrival = std::stoi(sarr);
            p.burst = std::stoi(sburst);
            p.priority = std::stoi(sprio);
            p.remaining = p.burst;
            v.push_back(p);
        }
    }
    return v;
}

std::vector<Process> sample_workload(){
    return {
        {"P1",0,7,2,7},
        {"P2",1,4,1,4},
        {"P3",2,9,3,9},
        {"P4",3,5,2,5},
        {"P5",6,3,1,3},
    };
}

#include <limits>

struct CLI {
    Policy policy = Policy::RR;
    int cores = 2;
    int quantum = 4;
    std::string input;
};

Policy parse_policy(const std::string& s){
    if (s=="rr") return Policy::RR;
    if (s=="sjf") return Policy::SJF;
    if (s=="priority") return Policy::PRIORITY;
    return Policy::RR;
}

CLI parse_args(int argc, char** argv){
    CLI c;
    for (int i=1;i<argc;i++){
        std::string a = argv[i];
        auto next = [&](int& i)->std::string{ if (i+1<argc) return std::string(argv[++i]); return ""; };
        if (a=="--policy") c.policy = parse_policy(next(i));
        else if (a=="--cores") c.cores = std::stoi(next(i));
        else if (a=="--quantum") c.quantum = std::stoi(next(i));
        else if (a=="--input") c.input = next(i);
        else if (a=="-h" || a=="--help") {
            std::cout << "Usage: ./scheduler [--policy rr|sjf|priority] [--cores N] [--quantum Q] [--input file.csv]\n";
            std::exit(0);
        }
    }
    if (c.cores < 1) c.cores = 1;
    if (c.quantum < 1) c.quantum = 1;
    return c;
}

std::map<std::string, Metrics> compute_metrics(const std::vector<Process>& procs){
    std::map<std::string, Metrics> m;
    for (auto& p: procs){
        Metrics x;
        x.turnaround = p.finish_time - p.arrival;
        x.waiting = x.turnaround - p.burst;
        m[p.pid] = x;
    }
    return m;
}

void print_report(const std::vector<Process>& procs){
    auto M = compute_metrics(procs);
    double sumT=0, sumW=0;
    std::cout << "\n=== Report ===\n";
    std::cout << std::left << std::setw(6) << "PID"
              << std::setw(10) << "Arrival"
              << std::setw(8) << "Burst"
              << std::setw(9) << "Finish"
              << std::setw(11) << "Turnaround"
              << std::setw(9) << "Waiting" << "\n";
    for (auto& p: procs){
        auto x = M.at(p.pid);
        sumT += x.turnaround;
        sumW += x.waiting;
        std::cout << std::left << std::setw(6) << p.pid
                  << std::setw(10) << p.arrival
                  << std::setw(8) << p.burst
                  << std::setw(9) << p.finish_time
                  << std::setw(11) << x.turnaround
                  << std::setw(9) << x.waiting << "\n";
    }
    std::cout << std::fixed << std::setprecision(2);
    int max_finish = 0;
    for (auto& p : procs) max_finish = std::max(max_finish, p.finish_time);
    std::cout << "Avg Turnaround: " << (sumT/procs.size())
              << " | Avg Waiting: " << (sumW/procs.size())
              << " | Throughput: " << (double)procs.size() / (double)(max_finish + 1) << " proc/tick\n";
}

int main(int argc, char** argv){
    CLI cli = parse_args(argc, argv);

    Scheduler sched(cli.policy, cli.cores, cli.quantum);

    if (!cli.input.empty()){
        sched.procs = load_csv(cli.input);
        if (sched.procs.empty()){
            std::cerr << "Failed to load input file, using sample workload.\n";
            sched.procs = sample_workload();
        }
    } else {
        sched.procs = sample_workload();
    }

    sched.sjfq = SJFQueue(&sched.procs);
    sched.prioq = PriorityQueueQ(&sched.procs);

    std::cout << "Policy: " 
              << (cli.policy==Policy::RR? "RR": (cli.policy==Policy::SJF? "SJF":"PRIORITY"))
              << " | Cores: " << cli.cores
              << " | Quantum: " << cli.quantum << "\n";

    sched.run();
    print_report(sched.procs);
    return 0;
}
