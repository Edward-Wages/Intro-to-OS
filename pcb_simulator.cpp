#include <iostream>
#include <queue>
#include <vector>
#include <set>
#include <string>
#include <algorithm>

// Define the PCB structure (YOU MUST IMPLEMENT THIS)
struct PCB 
{
    // TODO: Add fields: pid, state, pc, total_work - DONE
    int pid;
    std::string state;
    int pc;
    int total_work;
    // TODO: Add constructor PCB - DONE?
    PCB(int pid, int total_work) //Constructor. I am assuming the other values are not needed to be declared in the constructor
    {
        this->pid = pid;
        this->total_work = total_work;
    }
};

// Print states of all processes, sorted by PID (PROVIDED - DO NOT MODIFY)
void printProcessStates(const std::vector<PCB>& pcbs, int timeSlice) 
{
    std::cout << "Interrupt " << timeSlice << ":" << std::endl;
    std::vector<PCB> sorted_pcbs = pcbs;
    std::sort(sorted_pcbs.begin(), sorted_pcbs.end(), 
              [](const PCB& a, const PCB& b) 
              { return a.pid < b.pid; 
            });
    for (const PCB& pcb : sorted_pcbs) 
    {
        std::cout << "PID " << pcb.pid << ": " << pcb.state  << ", at pc " << pcb.pc << std::endl;
    }
    std::cout << std::flush;
}

// Kernel simulator (YOU MUST IMPLEMENT THIS)
void kernelSimulator(std::vector<PCB>& pcbs, int timeQuantum) 
{
    //Ed's Notes
    //Insert all pcbs in the vector into a queue
    std::queue<PCB*> readyQueue;
    for (PCB& pcb : pcbs) //For each PCB object in the vector
    {
        pcb.state = "READY"; //Assigns state to ready
        pcb.pc = 0; //Sets program counter to 0
        readyQueue.push(&pcb); //Adds the pcb to the queue
    }

    while (!readyQueue.empty()) //Main loop for the schedulinng
    {
        
    }


    //Implement scheduling as decribed in the project 1 description
        /*
        - Processes are placed in a ready queue. - Done?
        - The first process in the queue gets to run for the time quantum (e.g., 2 units).
        - If the process finishes within the time quantum, it’s done and removed from the queue.
        - If it’s not finished, it’s moved to the back of the queue, and the next process runs.
        - This repeats until all processes are complete.
        */
    //For each timed interrupt call printProcessStates
    //You may create helper functions as needed
    //Add comments to describe your implementation of this function and/or other helper functions
}


int main() 
{
    // Read from standard input via redirection (PROVIDED)
    int numProcesses;   //The number of PCB objects
    if (!(std::cin >> numProcesses)) 
    {
        std::cerr << "Error: Invalid input for number of processes" << std::endl;
        return 1;
    }
    if (numProcesses <= 0) 
    {
        std::cerr << "Error: Invalid number of processes" << std::endl;
        return 1;
    }
    
    std::vector<PCB> pcbs;  //Where we'll keep track of each PCB
    std::set<int> pids; // Track unique PIDs from the PCB objects?
    for (int i = 0; i < numProcesses; ++i) //For each PCB object
    {
        int pid, work;
        if (!(std::cin >> pid >> work)) 
        {
            std::cerr << "Error: Invalid input format for process data" << std::endl;
            return 1;
        }
        if (work <= 0) 
        {
            std::cerr << "Error: Invalid work units for PID " << pid << std::endl;
            return 1;
        }
        if (pids.count(pid) > 0) 
        {
            std::cerr << "Error: Duplicate PID " << pid << " detected" << std::endl;
            return 1;
        }
        // TODO: Add check for unique PIDs (e.g insert pid into the set pids) - Done?
        pids.insert(pid); //Insert pid into the set pids

        // TODO: Create PCB and add to pcbs (e.g., pcbs.emplace_back(pid, work)) - Done?
        pcbs.emplace_back(pid, work); //Create PCB and add to pcbs
    }
    
    int timeQuantum = 2; //Given time quantum
    kernelSimulator(pcbs, timeQuantum); //Method to Implement
    
    std::cout << "All processes completed." << std::endl;
    return 0;
}