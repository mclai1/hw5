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
    std::vector<size_t>& current
);

bool isValid(
    const AvailabilityMatrix& avail,
    const size_t maxShifts,
    DailySchedule& sched,
    size_t day,
    Worker_T worker
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
    std::vector<size_t> current;
    return solve(avail, dailyNeed, maxShifts, sched, 0, current);
}

// Define any helper functions here
bool solve(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    size_t day,
    std::vector<size_t>& current
){
    // We have reached the end of the schedule and found a solution
    if (day == avail.size()){
        return true;
    }

    // Check if we have scheduled enough workers for the current day
    if (current.size() == dailyNeed){
        sched.push_back(std::vector<Worker_T>());
        // Add the workers for the current day to the schedule
        for (size_t x = 0; x < current.size(); ++x) {
            sched[day].push_back(static_cast<Worker_T>(current[x]));
        }
        std::vector<size_t> next_day_workers;
        // Check the next day
        if (solve(avail, dailyNeed, maxShifts, sched, day + 1, next_day_workers)){
            return true;
        }
        // No solution, remove workers from current day and return false
        sched.pop_back();
        return false;
    }

    // Look at all possible workers, find which ones are valid, and add it to the current schedule
    for (Worker_T worker = 0; worker < avail[0].size(); ++worker){
        // Check if schedule is valid
        if (isValid(avail, maxShifts, sched, day, worker)) {
            bool already_used = false;
            for (size_t i = 0; i < current.size(); i++) {
                if (current[i] == worker) {
                    already_used = true;
                    break;
                }
            }
            if (!already_used) {
                current.push_back(worker);
                if (solve(avail, dailyNeed, maxShifts, sched, day, current)) {
                    return true;
                }
                current.pop_back();
            }
        }
    }
    // No solution found for the day
    return false;
}

bool isValid(
    const AvailabilityMatrix& avail,
    const size_t maxShifts,
    DailySchedule& sched,
    size_t day,
    Worker_T worker
){
    // Check if worker is available on the day
    if(!avail[day][worker]){
        return false;
    }
    // Check if worker has already worked the max shifts
    size_t shifts_worked = 0;
    for (size_t x = 0; x < sched.size(); ++x) {
        for (size_t y = 0; y < sched[x].size(); ++y) {
            if (sched[x][y] == worker) {
                shifts_worked++;
            }
        }
    }
    if (shifts_worked >= maxShifts) {
        return false;
    }
    return true;
}
