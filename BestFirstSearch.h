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
        
        
        // If we get to here, we never found a solution and returned it higher up
        return nullptr;
    }
};
/*
class Comp{
	public:
	short int hValue;
	std::shared_ptr<Searchable> succ;
	Comp(unique_ptr<Searchable> s):succ(std::move(s)){
		hValue = succ.get()->heuristicValue();
	}
	~Comp(){
	}
	bool operator<(const Comp& other)const{
		return hValue < other.hValue;
	}
	
	bool isSolution()const{
		return succ.get()->isSolution();
	}
	/*
	bool operator()(const short int&a,const short int&b){
		return a<b;
	}*
};

struct comparator{
	bool operator()(int&a,int&b){
		return a<b;
	}
};
class BestFirstSearch {
	
protected:

    /// Make sure you increment this every time you 'expand' a node, by getting it successors and putting them on the queue        
    int nodes = 0;
    std::priority_queue<int,unique_ptr<Searchable>,comparator> Q;
    
public:

    BestFirstSearch(std::unique_ptr<Searchable> && startFrom) {
        /** Constructor: move the pointer out of startFrom into the queue, Q *
        Q.push(startFrom->heuristicValue(),std::move(startFrom));
    }

    int getNodesExpanded() const {
        return nodes;
    }

    /** Solve the problem.  If a solution found, return a pointer to it.  Otherwise, return nullptr *
    Searchable * solve() {
        
        // If there are still incomplete solutions on the queue
        while (!Q.empty()) {
        	std::cout<<"A ";
            if (Q.top().isSolution()) {
                return Q.top().succ.get(); // return the pointer 
            }
                  
            ++nodes; // we've now looked at one more node, increment our counter
            
            // Steal the pointer to the board at the front of the queue, by moving it into a unique_ptr here
            // After this, the pointer on the front of the queue is `nullptr`...
            unique_ptr<Searchable> current(startFrom->heuristicValue(),std::move(Q.top()/*.succ.get()*));
            
            // Get the successors...
            vector<unique_ptr<Searchable>> successors = current->successors();
            
            Q.pop();
            
        	std::cout<<"B ";
            for (auto & successor : successors) {
                // and push each one onto the back of queue.
                Q.push(std::move(successor));
            }
        	std::cout<<Q.size()<<"C ";
        }
        std::cout<<"TUA MADRE Quella BOCCHINARA";
        
        // If we get to here, we never found a solution and returned it higher up
        return nullptr;
    }
};


// Do not edit below this line
*/
#endif
