#include <iostream>
#include <vector>
#include <string>
using namespace std;



/*
In this project you need to implement the deadlock avoidance algorithm using C++. The Banker’s algorithm
consists of:
- the safety algorithm
- the resource-request algorithm

Sample Input:
R 3
P 5
Available
3 3 2
Max
7 5 3
3 2 2
9 0 2
2 2 2
4 3 3
Allocation
0 1 0
2 0 0
3 0 2
2 1 1
0 0 2
P1 1 0 2

R 3 means there are 3 resource types: R0, R1, and R2.
- P 5 means there are 5 processes: P0, P1, P2, P3, and P4.
- Available is a vector with 3, 3, and 2 representing the number of available instances for R0, R1, and R2
respectively.
- Max is a matrix, where each row represents the maximum number of instances of each resource type (R0,
R1, R2) needed by the corresponding process.
- Allocation is a matrix, where each row represents the number of instances of each resource type (R0, R1,
R2) currently allocated to the corresponding process.
- P1 1 0 2 means P1 is requesting 1 instance of R0, 0 instances of R1, and 2 instances of R2 respectively


TODO
- Write the Banker’s algorithm in a file called project3.cpp.
- Assume that the input is read via redirection
- Compile and run project3.cpp. Make sure you keep the input file (e.g input.txt) in the same directory where
your program is located.
- Submit project3.cpp to GradeScope.
*/


// Safety algorithm: returns true if the system is in a safe state
bool safetyAlgorithm(const vector<int>& available, const vector<vector<int>>& max, const vector<vector<int>>& allocation)
{
    int processCount = (int)max.size();
    int resourceCount = (int)available.size();

    // compute need = max - allocation
    vector<vector<int>> need(processCount, vector<int>(resourceCount));
    for (int i = 0; i < processCount; ++i)
    {
        for (int j = 0; j < resourceCount; ++j)
        {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }

    vector<int> work = available;
    vector<bool> finish(processCount, false);

    bool progress = true;
    while (progress)
    {
        progress = false;
        for (int i = 0; i < processCount; ++i)
        {
            if (finish[i]) continue;
            bool ok = true;
            for (int j = 0; j < resourceCount; ++j)
            {
                if (need[i][j] > work[j]) { ok = false; break; }
            }
            if (ok)
            {
                // this process can finish
                for (int j = 0; j < resourceCount; ++j) work[j] += allocation[i][j];
                finish[i] = true;
                progress = true;
            }
        }
    }

    for (int i = 0; i < processCount; ++i)
        if (!finish[i]) return false;
    return true;
}

//Resource-request algorithm
bool resourceRequestAlgorithm()
{


    return NULL; //Stub
}

int main()
{
    //Read in the input
    int resourceCount = 0;
    int processCount = 0;
    string dummy;
    cin >> dummy >> resourceCount;
    cin >> dummy >> processCount;

    cin >> dummy; //Avaialable
    vector<int> available(resourceCount); //Vector for numbr of available instances for each resource
    for (int i = 0; i < resourceCount; i++)
    {
        cin >> available[i]; //Read available instances
    }
    cin >> dummy; //Max
    // use vectors for matrices
    vector<vector<int>> max(processCount, vector<int>(resourceCount));
    for (int i = 0; i < processCount; i++)
    {
        for (int j = 0; j < resourceCount; j++)
        {
            cin >> max[i][j]; //Read max matrix
        }
    }
    cin >> dummy; //Allocation
    vector<vector<int>> allocation(processCount, vector<int>(resourceCount));
    for (int i = 0; i < processCount; i++)
    {
        for (int j = 0; j < resourceCount; j++)
        {
            cin >> allocation[i][j]; //Read allocation matrix
        }
    }

    //Read in the request
    string processID;
    vector<int> request(resourceCount);
    cin >> processID;
    for (int i = 0; i < resourceCount; i++)
    {
        cin >> request[i];
    }
    int pid = stoi(processID.substr(1));

    /*READING INPUT STOPS HERE*/

    // Resource-request algorithm
    // Compute need
    vector<int> avail = available; // copy
    vector<vector<int>> need(processCount, vector<int>(resourceCount));
    for (int i = 0; i < processCount; ++i)
        for (int j = 0; j < resourceCount; ++j)
            need[i][j] = max[i][j] - allocation[i][j];

    // Check request <= need
    bool leNeed = true;
    for (int j = 0; j < resourceCount; ++j)
        if (request[j] > need[pid][j]) leNeed = false;

    if (!leNeed)
    {
        cout << "The current system is in unsafe state." << endl;
        return 0;
    }

    // Check request <= available
    bool leAvail = true;
    for (int j = 0; j < resourceCount; ++j)
        if (request[j] > avail[j]) leAvail = false;

    if (!leAvail)
    {
        cout << "The current system is in unsafe state." << endl;
        return 0;
    }

    // Tentatively allocate
    for (int j = 0; j < resourceCount; ++j)
    {
        avail[j] -= request[j];
        allocation[pid][j] += request[j];
        need[pid][j] -= request[j];
    }

    bool isSafe = safetyAlgorithm(avail, max, allocation);
    if (isSafe)
    {
        cout << "Before granting the request of " << processID << ","  "the system is in safe state." << endl;
        cout << "Simulating granting " << processID << "'s request." << endl;
            //Execure the process request if safe

    }
    else
    {
        cout << "The current system is in unsafe state." << endl;
    }

    return 0;
}