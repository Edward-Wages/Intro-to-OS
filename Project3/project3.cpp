#include <iostream>
#include <vector>
#include <string>
#include <iomanip> 
using namespace std;

/*
In this project you need to implement the deadlock avoidance algorithm using C++. The Banker’s algorithm
consists of:
- the safety algorithm
- the resource-request algorithm
*/

// Safety algorithm: returns true if the system is in a safe state
bool safetyAlgorithm(const vector<int>& available, const vector<vector<int>>& max, const vector<vector<int>>& allocation)
{
    int processCount = (int)max.size();
    int resourceCount = (int)available.size();

    //Calculate Need matrix
    vector<vector<int>> need(processCount, vector<int>(resourceCount));
    for (int i = 0; i < processCount; ++i)
    {
        for (int j = 0; j < resourceCount; ++j)
        {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }

    //Initialize work and finish
    vector<int> work = available;
    vector<bool> finish(processCount, false);

    //Find a process i such that:
    //    a) Finish[i] == false
    //    b) Need[i] <= Work
    bool progress = true;
    while (progress)
    {
        progress = false;
        for (int i = 0; i < processCount; ++i)
        {
            if (finish[i]) continue; // Already finished

            // Check if Need[i] <= Work
            bool canRun = true;
            for (int j = 0; j < resourceCount; ++j)
            {
                if (need[i][j] > work[j])
                {
                    canRun = false;
                    break;
                }
            }

            if (canRun)
            {
                //Update work and finish
                //This process can finish, release its resources
                for (int j = 0; j < resourceCount; ++j)
                {
                    work[j] += allocation[i][j];
                }
                finish[i] = true;
                progress = true;
            }
        }
    }

    //Check if all processes are finished
    for (int i = 0; i < processCount; ++i)
    {
        if (!finish[i])
        {
            return false; // System is in an unsafe state
        }
    }
    
    return true; // System is in a safe state
}

int main()
{
    //Read in the input
    int resourceCount = 0;
    int processCount = 0;
    string dummy;
    cin >> dummy >> resourceCount;
    cin >> dummy >> processCount;

    cin >> dummy; //Available
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

    //Check if the initial state is safe
    if (!safetyAlgorithm(available, max, allocation))
    {
         cout << "The current system is in unsafe state." << endl;
         return 0; // Cannot proceed
    }
    else
    {
        cout << "Before granting the request of " << processID << ", the system is in safe state." << endl;
    }


    // Calculate need = max - allocation
    vector<vector<int>> need(processCount, vector<int>(resourceCount));
    for (int i = 0; i < processCount; ++i)
    {
        for (int j = 0; j < resourceCount; ++j)
        {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }

    //Check if is Request[i] <= Need[i]
    bool requestExceedsNeed = false;
    for (int j = 0; j < resourceCount; ++j)
    {
        if (request[j] > need[pid][j])
        {
            requestExceedsNeed = true;
            break;
        }
    }
    
    if (requestExceedsNeed)
    {
        cout << processID << "'s request cannot be granted. Error: Process has exceeded its maximum claim." << endl;
        return 0; // Stop
    }

    //Check if Request[i] <= Available
    bool requestExceedsAvailable = false;
    for (int j = 0; j < resourceCount; ++j)
    {
        if (request[j] > available[j])
        {
            requestExceedsAvailable = true;
            break;
        }
    }

    if (requestExceedsAvailable)
    {
        cout << processID << "'s request cannot be granted. Process must wait, resources are not available." << endl;
        return 0; // Stop
    }

    //Tentatively allocate resources
    cout << "Simulating granting " << processID << "'s request." << endl;

    vector<int> new_available = available;
    vector<vector<int>> new_allocation = allocation;
    vector<vector<int>> new_need = need;

    for (int j = 0; j < resourceCount; ++j)
    {
        new_available[j] -= request[j];
        new_allocation[pid][j] += request[j];
        new_need[pid][j] -= request[j]; 
    }

    // Print the new need matrix
    cout << "New Need" << endl;
    for (int i = 0; i < processCount; ++i)
    {
        for (int j = 0; j < resourceCount; ++j)
        {
            cout << new_need[i][j] << " ";
        }
        cout << endl;
    }


    //Run Safety Algorithm on the *new* state
    bool isSafe = safetyAlgorithm(new_available, max, new_allocation);

    if (isSafe)
    {
        cout << processID << "'s request can be granted. The system will be in safe state." << endl;
    }
    else
    {
        cout << processID << "'s request cannot be granted. The system will be in unsafe state." << endl;
    }

    return 0;
}