#include <iostream>
#include <queue>
#include <deque>
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include <map>
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
    int remaining_quantum; //Set to the time quanton & decremented as process runs
    int turnaround_time;
};

//Example Input:
//q = 10
//P1 40 20 0
//P2 30 25 25
//P3 30 25 30
//P4 35 15 60
//P5 5 10 100
//P6 10 10 105

//Process, Priority, Burst Time, Arrival Time
void CPU_Scheduler_Simulator(vector<PCB>& processes, int time_quantum)
{
    // map from priority -> queue of PCBs; higher priority (larger int) comes first
    map<int, deque<PCB>, greater<int>> readyQueues;
    int currentTime = 0;
    vector<PCB> finished_processes; //Will move processes here once they are done so their stats are preserved
    bool hasCurrent = false;
    PCB currentProc; // stored by value when a process is running
    int idleTime = 0;

    //Sort processes based on arrival time
    sort(processes.begin(), processes.end(), [](const PCB& a, const PCB& b)
    {
        return a.arrival_time < b.arrival_time;
    });

    //This is how we can successfuly print a single timepoint for a process who runs for multiple quantum cycles
    // Logging: track the currently running id ("Idle" when idle) and the start time of the segment
    string lastRunningId = "Idle";
    int segmentStart = 0;
    // Collect printed segments so we can merge adjacent segments with the same process
    struct Segment { string id; int priority; int start; int end; };
    vector<Segment> segments;

    // Append a segment (does not print immediately) - We will merge adjacent identical segments later
    auto flushSegment = [&](int endTime, const string& id, int priority)
    {
        if (endTime <= segmentStart) return;
        segments.push_back({id, priority, segmentStart, endTime});
        segmentStart = endTime;
    };

    // Run until there are no more processes to arrive, no ready processes, and no current process
    while (!processes.empty() || !readyQueues.empty() || hasCurrent)
    {
        // Check for new arrivals at currentTime
        for (int i = 0; i < (int)processes.size(); /* increment inside */)
        {
            if (processes[i].arrival_time <= currentTime)
            {
                readyQueues[processes[i].priority].push_back(processes[i]);
                processes.erase(processes.begin() + i);
            }
            else
            {
                ++i;
            }
        }

        // Handle preemption: if there's a running process and a higher-priority process is waiting,
        // preempt the running one and put it back into its ready queue
        if (hasCurrent && !readyQueues.empty())
        {
            int topPriority = readyQueues.begin()->first;
            if (topPriority > currentProc.priority)
            {
                // flush current running segment up to now
                flushSegment(currentTime, currentProc.id, currentProc.priority);
                // push preempted process back to its queue (at front so it resumes sooner among same-priority)
                readyQueues[currentProc.priority].push_back(currentProc);
                hasCurrent = false;
                lastRunningId = "Idle"; // will be set when we pick next
            }
        }

        // If CPU is idle, pick next process
        if (!hasCurrent)
        {
            if (!readyQueues.empty())
            {
                auto it = readyQueues.begin(); // highest priority due to greater<int>
                deque<PCB>& q = it->second;
                currentProc = q.front();
                q.pop_front();
                if (q.empty()) readyQueues.erase(it);
                // reset quantum for this turn
                currentProc.remaining_quantum = time_quantum;
                hasCurrent = true;

                // logging: if new process differs from last segment, flush last and start new
                if (lastRunningId != currentProc.id)
                {
                    flushSegment(currentTime, lastRunningId, 0);
                    lastRunningId = currentProc.id;
                    segmentStart = currentTime;
                }
            }
            else
            {
                // CPU stays idle; if lastRunningId isn't IDLE start a new idle segment
                if (lastRunningId != "Idle")
                {
                    flushSegment(currentTime, lastRunningId, 0);
                    lastRunningId = "Idle";
                    segmentStart = currentTime;
                }
            }
        }

        // Run one time unit
        if (hasCurrent)
        {
            currentProc.remaining_time -= 1;
            currentProc.remaining_quantum -= 1;
        }
        else
        {
            idleTime += 1;
        }

        // After running this time unit, check for finished or quantum expiration
        if (hasCurrent)
        {
            if (currentProc.remaining_time == 0)
            {
                // process finished at time currentTime + 1 (end of this time unit)
                int finishTime = currentTime + 1;
                currentProc.last_run_time = finishTime;
                currentProc.turnaround_time = finishTime - currentProc.arrival_time;
                finished_processes.push_back(currentProc);
                // flush the running segment up to finishTime
                flushSegment(finishTime, currentProc.id, currentProc.priority);
                hasCurrent = false;
                lastRunningId = "Idle";
            }
            else if (currentProc.remaining_quantum == 0)
            {
                // quantum expired: push back to end of its ready queue
                readyQueues[currentProc.priority].push_back(currentProc);
                // flush the running segment up to currentTime + 1
                flushSegment(currentTime + 1, currentProc.id, currentProc.priority);
                hasCurrent = false;
                lastRunningId = "Idle";
            }
        }

        // advance time
        currentTime += 1;
    }

    // If any trailing segment is open, flush it (normally segmentStart == currentTime now)
    if (segmentStart < currentTime)
    {
        flushSegment(currentTime, lastRunningId, 0);
    }
    // Merge adjacent segments that belong to the same process with same priority
    vector<Segment> merged;
    for (const auto &s : segments)
    {
        if (!merged.empty())
        {
            auto &last = merged.back();
            if (last.id == s.id && last.priority == s.priority && last.end == s.start)
            {
                // extend last segment
                last.end = s.end;
                continue;
            }
        }
        merged.push_back(s);
    }

    // Print merged timeline
    for (const auto &s : merged)
    {
        if (s.id == "Idle")
            cout << "Time " << s.start << "-" << s.end << ": Idle" << '\n';
        else
            cout << "Time " << s.start << "-" << s.end << ": " << s.id << " (Priority " << s.priority << ")" << '\n';
    }

    // Print summary statistics
    //This whole mess is just to sort finished_processes by their ID
    sort(finished_processes.begin(), finished_processes.end(), 
    [](const PCB& a, const PCB& b) {
        
        // Get the substring after the 'P' (e.g., "1", "2", "10")
        string num_str_a = a.id.substr(1);
        string num_str_b = b.id.substr(1);
        
        // Convert those number strings to integers for a correct
        // numerical comparison
        int id_num_a = stoi(num_str_a);
        int id_num_b = stoi(num_str_b);
        
        // Sort based on the integer values
        return id_num_a < id_num_b;
    });


    int total_turnaround = 0;
    for (const auto& p : finished_processes) total_turnaround += p.turnaround_time;
    if (!finished_processes.empty())
    {
        double avg_turnaround = (double)total_turnaround / finished_processes.size();
        // not printed in sample output, but keep as debug info if needed
        // cout << "Average turnaround time: " << avg_turnaround << "\n";
    }

    cout << endl; //Buffer line before stats output
    cout << "Turnaround Time" << endl;
    for (const auto& p : finished_processes)
    {
        cout << p.id << " = " << p.turnaround_time << endl;
    }

    cout << "\nWaiting Time" << endl;
    for (const auto& p : finished_processes)
    {
        cout << p.id << " = " << (p.turnaround_time - p.burst_time) << endl;
    }

    cout << "\nCPU Utilization Time" << endl;
    cout << (currentTime - idleTime) << "/" << currentTime << endl;
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
    // Run the simulator with the read processes
    CPU_Scheduler_Simulator(processes, time_quantum);
    // You can create any data structures, classes, functions helpers as you wish
    // Do not forget to include comments describing how your simulator works.
    return 0;
}