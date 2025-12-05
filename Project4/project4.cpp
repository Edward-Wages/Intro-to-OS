#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // Required for std::max_element
using namespace std;

// Project 4 - Bonus
//Edward Wages

/* Sample input:
Frames 3
7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1

Frames 3 means each process is allocated maximum 3 frames
the long list is a regerence string representing the page being referenced at each time step
*/

/* Sample output:
7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1
Page replacement using LRU
time step 0: 7
time step 1: 7 0
time step 2: 7 0 1
time step 3: 2 0 1
time step 4: 2 0 1
time step 5: 2 0 3
time step 6: 2 0 3
time step 7: 4 0 3
time step 8: 4 0 2
time step 9: 4 3 2
time step 10: 0 3 2
time step 11: 0 3 2
time step 12: 0 3 2
time step 13: 1 3 2
time step 14: 1 3 2
time step 15: 1 0 2
time step 16: 1 0 2
time step 17: 1 0 7
time step 18: 1 0 7
time step 19: 1 0 7
total number of page faults = 12
*/

// Project is to simulate the least recently used page replacement algorithm

int main()
{
    int frames = 0;
    string dummy;
    cin >> dummy >> frames;

    vector<int> references;
    int page;
    while (cin >> page)
    {
        references.push_back(page);
    }

    //At this point we can start simulating the LRU
    vector<int> memory; //Current pages in memory
    vector<int> age = vector<int>(frames); //How long each page has been in memory

    int pageFaults = 0;

    for(int i = 0; i < references.size(); i++)
    {
        cout << references[i] << " ";
    }
    cout << endl;
    cout << "Page replacement using LRU" << endl;

    int timeStep = 0;
    for(int i = 0; i < references.size(); i++)
    {
        //Do the algorithm here
        bool pageFound = false;
        
        // Check if page is already in memory
        for(int j = 0; j < memory.size(); j++)
        {
            if(memory[j] == references[i])
            {
                pageFound = true;
                age[j] = 0; //Reset age since it was just used
                break;
            }
        }
        
        // If page not found, need to add it
        if (!pageFound)
        {
            if (memory.size() < frames) //While the memory isn't full (really only helpful in the beginning)
            {
                memory.push_back(references[i]);
                age.push_back(0); // New page starts with age 0
                pageFaults++;
            }

            else //Replace a page in memory - also need to reset it's age
            {
                //Need to find the oldest page by checking the age vector
                int oldestIndex = 0;
                int oldestAge = 0;
                for(int k = 0; k < age.size(); k++)
                {
                    if(age[k] > oldestAge)
                    {
                        oldestIndex = k;
                        oldestAge = age[k];
                    }
                }

                //Replace the oldest page with the new one
                memory[oldestIndex] = references[i];
                age[oldestIndex] = 0;
                pageFaults++;
            }
        }

        //Print out the step
        cout << "time step " << timeStep << ": ";
        for (int j = 0; j < memory.size(); j++)
        {
            cout << memory[j] << " ";
        }
        cout << endl;
        timeStep++;

        for(int p = 0; p < memory.size(); p++)
        {
            age[p]++;
        }

    }
    cout << "total number of page faults = " << pageFaults << endl;
    return 0;
}
