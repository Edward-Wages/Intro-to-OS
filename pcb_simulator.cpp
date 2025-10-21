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
    
    int pid; //Process ID
    string state; //State of the process (Ready, Running, Terminated)
    int pc; //Program Counter
    int total_work; //Total work units required for the process
    
    PCB(int pid, int total_work) //Constructor. I am assuming the other values are not needed to be declared in the constructor
    {
        this->pid = pid;
        this->total_work = total_work;
    }
};

// Print states of all processes, sorted by PID (PROVIDED - DO NOT MODIFY)
void printProcessStates(const std::vector<PCB>& pcbs, int timeSlice) 
{
    cout << "Interrupt " << timeSlice << ":" << endl;
    vector<PCB> sorted_pcbs = pcbs;
    sort(sorted_pcbs.begin(), sorted_pcbs.end(), 
              [](const PCB& a, const PCB& b) 
              { return a.pid < b.pid; 
            }
        );
    for (const PCB& pcb : sorted_pcbs) 
    {
        cout << "PID " << pcb.pid << ": " << pcb.state  << ", at pc " << pcb.pc << endl;
    }
    cout << flush;
}

// Kernel simulator (YOU MUST IMPLEMENT THIS)
void kernelSimulator(vector<PCB>& pcbs, int timeQuantum) 
{
    //Insert all pcbs in the vector into a queue
    queue<PCB*> readyQueue;
    for (PCB& pcb : pcbs) //For each PCB object in the vector
    {
        pcb.state = "Ready"; //Assigns state to ready
        pcb.pc = 0; //Sets program counter to 0
        readyQueue.push(&pcb); //Adds the pcb to the queue
    }

    int interruptCount = 1; //Will be needed to keep count of which interrupt we are outputting
    while (!readyQueue.empty()) //Main loop for the schedulinng
    {
        readyQueue.front()->state = "Running";
        readyQueue.front()->pc += timeQuantum;
        
        //If the process is complete (pc >= total_work), label it as terminated & remove it from the queue
        if (readyQueue.front()->pc >= readyQueue.front()->total_work)
        {
            readyQueue.front()->pc = readyQueue.front()->total_work; //Sets pc to the total work so it doesn't exceed it
            readyQueue.front()->state = "Terminated"; //Sets value to terminated since it is done
            
        }

        //When the time quantom is up, print the process states
        printProcessStates(pcbs, interruptCount);
        interruptCount++;

        //Move the process to the back of the queue if not complete & start from the top
        if (!readyQueue.empty() && readyQueue.front()->state == "Terminated")
        {
            readyQueue.pop(); //Removes the front of the queue if it is terminated
        }
        
        else if (!readyQueue.empty() && readyQueue.front()->state != "Terminated")
        {
            readyQueue.front()->state = "Ready"; //Sets state back to ready
            PCB* temp = readyQueue.front(); //Temporary variable to hold the front of the queue
            readyQueue.pop(); //Removes the front of the queue
            readyQueue.push(temp); //Adds the process to the back of the queue
        }
    }
}


int main() 
{
    // Read from standard input via redirection (PROVIDED)
    int numProcesses;   //The number of PCB objects
    if (!(cin >> numProcesses)) 
    {
        cerr << "Error: Invalid input for number of processes" << std::endl;
        return 1;
    }
    if (numProcesses <= 0) 
    {
        cerr << "Error: Invalid number of processes" << std::endl;
        return 1;
    }
    
    vector<PCB> pcbs;  //Where we'll keep track of each PCB
    set<int> pids; // Track unique PIDs from the PCB objects?
    for (int i = 0; i < numProcesses; ++i) //For each PCB object
    {
        int pid, work;
        if (!(cin >> pid >> work)) 
        {
            cerr << "Error: Invalid input format for process data" << std::endl;
            return 1;
        }
        if (work <= 0) 
        {
            cerr << "Error: Invalid work units for PID " << pid << std::endl;
            return 1;
        }
        if (pids.count(pid) > 0) 
        {
            cerr << "Error: Duplicate PID " << pid << " detected" << std::endl;
            return 1;
        }
        // TODO: Add check for unique PIDs (e.g insert pid into the set pids) - Done?
        pids.insert(pid); //Insert pid into the set pids

        // TODO: Create PCB and add to pcbs (e.g., pcbs.emplace_back(pid, work)) - Done?
        pcbs.emplace_back(pid, work); //Create PCB and add to pcbs
    }
    
    int timeQuantum = 2; //Given time quantum
    kernelSimulator(pcbs, timeQuantum); //Method to Implement
    
    cout << "All processes completed." << endl;
    return 0;
}