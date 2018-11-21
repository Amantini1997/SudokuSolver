#ifndef BEST_FIRST_SEARCH_H
#define BEST_FIRST_SEARCH_H

#include "Searchable.h"

#include <deque>
using std::deque;

#include <memory>
using std::unique_ptr;
class BestFirstSearch{
protected:
    /** A queue of incomplete solutions: initially, the board we start with */
    std::deque<unique_ptr<Searchable>>Q;
    
    /** How many nodes (boards) we made successors for, in order to solve the problem */    
    int nodes = 0;
public:

    /** Constructor: move the pointer out of startFrom into the queue, Q */
    BestFirstSearch(std::unique_ptr<Searchable> && startFrom) {
        Q.push_back(std::move(startFrom));
    }

    int getNodesExpanded() const {
        return nodes;
    }

    /** Solve the problem.  If a solution found, return a pointer to it.  Otherwise, return nullptr */
    Searchable * solve() {
        // If there are still incomplete solutions on the queue
        while (!Q.empty()) {
            if (Q.front()->isSolution()) {
                return Q.front().get(); // return the pointer 
            }
                  
            ++nodes; // we've now looked at one more node, increment our counter
            
            // Steal the pointer to the board at the front of the queue, by moving it into a unique_ptr here
            // After this, the pointer on the front of the queue is `nullptr`...
            unique_ptr<Searchable> current(std::move(Q.front()));
            
            // ...which can then be popped off the front of the queue
            Q.pop_front();
            
            // Get the successors...
            vector<unique_ptr<Searchable>> successors = current->successors();
            
            for (auto & successor : successors) {
                // and push each one onto the back of queue.
                Q.push_back(/*startFrom.get()->heuristicValue(),*/std::move(successor));
            }
        }
        
    }
};
#endif
