#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
int countEmpty(const std::string& in);
void words(std::string& current, const std::string& floating, const std::set<std::string>& dict, std::set<std::string>& results, int index, int empty);


// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    std::set<std::string> results;
    std::string current = in;
    int empty = countEmpty(current);
    words(current, floating, dict, results, 0, empty);
    return results;
}

// Define any helper functions here
int countEmpty(const std::string& in){
    int count = 0;
    for (size_t i = 0; i < in.length(); i++){
        if (in[i] == '-'){
            count++;
        }
    }
    return count;
}

void words(std::string& current, const std::string& floating, const std::set<std::string>& dict, std::set<std::string>& results, int index, int empty){
    // Word is complete
    if (index == int(current.size())){
        // Check that all floating characters are used
        for (size_t i = 0; i < floating.length(); i++){
            if (current.find(floating[i]) == std::string::npos){
                return;
            }
        }
        // Check if the word is in the dictionary
        if (dict.find(current) != dict.end()){
            results.insert(current);
        }
        return;
    }
    // Index is a fixed character
    if (current[index] != '-'){
        words(current, floating, dict, results, index + 1, empty);
        return;
    }
    // Try all characters
    for (char c = 'a'; c <= 'z'; c++){
        current[index] = c;
        // Check if c is a floating character
        size_t x = floating.find(c);
        if (x != std::string::npos){
            // Remove c from floating
            std::string newFloating = floating.substr(0, x) + floating.substr(x + 1);
            words(current, newFloating, dict, results, index + 1, empty - 1);
        }
        else{
            // Use c only if we can fit all the remaining floating characters
            if (empty > int(floating.size())){
                words(current, floating, dict, results, index + 1, empty - 1);
            }
        }
    }
    current[index] = '-';
}