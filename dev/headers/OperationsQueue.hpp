#ifndef OPERATIONSQUEUE_HPP
#define OPERATIONSQUEUE_HPP

#include <vector>
#include <tuple>

#include "InfoCase.hpp"



class OperationsQueue {
    int realLast;
    int currentLast;
    std::vector<std::tuple<int,int,InfoCase,InfoCase> > queue;
    //bool canWeForward;

public:

    //constructeur
    OperationsQueue();

    //getter
    int getRealLast() const;
    int getCurrentLast() const;

    //tests
    bool canWeForward() const;
    bool canWePrevious() const;

    //others
    std::tuple<int,int,InfoCase,InfoCase> getPrevious();
    std::tuple<int,int,InfoCase,InfoCase> getForward();
    void addOp (const std::tuple<int,int,InfoCase,InfoCase> &);

    void display() const;
};



#endif // OPERATIONSQUEUE_HPP
