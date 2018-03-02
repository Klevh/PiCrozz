#ifndef OPERATIONSQUEUE_HPP
#define OPERATIONSQUEUE_HPP

#include <vector>
#include <tuple>

#include "InfoCase.hpp"

#include "Debug.hpp"

class OperationsQueue {
    int realLast;
    int currentLast;
    std::vector<std::tuple<int,int,InfoCase,InfoCase>> queue;
    //bool canWeForward;

public:

    //constructeur
    OperationsQueue();

    //getter
    int getRealLast() const;
    int getCurrentLast() const;
    const std::vector<std::tuple<int,int,InfoCase,InfoCase>>& getQueue() const;

    //setters
    void setRealLast(int);
    void setCurrentLast(int);
    void setQueueI (int, const std::tuple<int,int,InfoCase,InfoCase> &);

    // operators
    OperationsQueue& operator=(const OperationsQueue& oq);

    //tests
    bool canWeForward() const;
    bool canWePrevious() const;

    //others
    std::tuple<int,int,InfoCase,InfoCase> getPrevious();
    std::tuple<int,int,InfoCase,InfoCase> getForward();
    void addOp (const std::tuple<int,int,InfoCase,InfoCase> &);
    void resize(int);

    #if defined(DEBUG) && !defined(NDEBUG)
    void display() const;
    #endif
};



#endif // OPERATIONSQUEUE_HPP
