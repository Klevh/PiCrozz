#ifndef OPERATIONSQUEUE_HPP
#define OPERATIONSQUEUE_HPP

#include <vector>
#include <tuple>

#include "InfoCase.hpp"

class OperationsQueue {
    std::vector<std::tuple<int,int,InfoCase,InfoCase> > queue;
    int last;
    bool canWeForward;

public:

    OperationsQueue();

    void setLast(int); //inutile

    int getLast() const;
    bool getCanWeForward () const;
    std::tuple<int,int,InfoCase,InfoCase> getPrevious() const;
    std::tuple<int,int,InfoCase,InfoCase> getForward() const;
    void addOp (const std::tuple<int,int,InfoCase,InfoCase> &);

};



#endif // OPERATIONSQUEUE_HPP
