#include <iostream>
#include <queue>
#include <vector>
#include <set>
#include <string>
#include <algorithm>
using namespace std;

//PCB structure
struct PCB 
{
    string id;
    int priority;
    int burst_time;
    int arrival_time;
    int remaining_time;
    // feel free to add more variables here
    int last_run_time;
    int remaining_quantum;
};

// TODO: Create CPU scheduler simulator
void CPU_Scheduler_Simulator(vector<PCB>& processes, int time_quantum)
{
    //Each process is assigned a numerical priority, with a higher number indicating a higher relative priority.
    //Each process may arrive at different time.
    //Processes with higher priority will always be scheduled ahead of the lower priority processes. 
    //If there are multiple processes have the same priority level, then they will be scheduled using round robin. 
    //If a process is currently using the CPU yet another process with higher priority arrives, the running process will be preempted by the new arrival process. 
    //In addition to the processes, the system also has an idle task (which consumes no CPU resources. 
    //This task has priority 0 and is scheduled whenever the system has no other available processes to run.
}

int main() 
{
    string line;
    char dummy;
    int time_quantum;
    vector<PCB> processes;
    // Read time quantum from standard input
    cin >> dummy >> time_quantum;
    // Read processes from standard input
    while (cin >> line) 
    {
        PCB p;
        p.id = line;
        cin >> p.priority >> p.burst_time >> p.arrival_time;
        p.remaining_time = p.burst_time;
        p.last_run_time = -1;
        p.remaining_quantum = time_quantum;
        processes.push_back(p);
    }
    // You can create any data structures, classes, functions helpers as you wish
    // Do not forget to include comments describing how your simulator works.
    return 0;
}