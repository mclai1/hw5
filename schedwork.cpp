#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool solve(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    size_t day,
    std::vector<size_t>& shiftTracker,
    size_t numDays,
    size_t numWorkers
);


// Add your implementation of schedule() and other helper functions here
bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below
    size_t numDays = avail.size();
    size_t numWorkers = avail[0].size();

    std::vector<size_t> shiftTracker;
    for (size_t i = 0; i < numWorkers; ++i) {
        shiftTracker.push_back(0);
    }

    for (size_t i = 0; i <numDays; i++){
        vector<Worker_T> day;
        sched.push_back(day);
    }

    return solve(avail, dailyNeed, maxShifts, sched, 0, shiftTracker, numDays, numWorkers);
}

// Define any helper functions here
bool solve(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    size_t day,
    std::vector<size_t>& shiftTracker,
    size_t numDays,
    size_t numWorkers
){
    // We have reached the end of the schedule and found a solution
    if (day >= numDays){
        return true;
    }

    // Check if we have scheduled enough workers for the current day
    if (sched[day].size() >= dailyNeed){
        return solve(avail, dailyNeed, maxShifts, sched, day + 1, shiftTracker, numDays, numWorkers);
    }

    // Look at all possible workers, find which ones are valid, and add it to the current schedule
    for (size_t i = 0; i < numWorkers; i++){
        if (shiftTracker[i] < maxShifts && avail[day][i] == true && std::find(sched[day].begin(), sched[day].end(), i) == sched[day].end()){
            // Add the worker to the schedule
            sched[day].push_back(i);
            shiftTracker[i]++;

            // Call solve to check the next day
            if (solve(avail, dailyNeed, maxShifts, sched, day, shiftTracker, numDays, numWorkers)){
                return true;
            }

            // No solution, backtrack and remove the worker from the schedule
            sched[day].pop_back();
            shiftTracker[i]--;
        }
    }
    // No solution found for the day
    return false;
}

