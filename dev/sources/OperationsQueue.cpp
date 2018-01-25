#include "OperationsQueue.hpp"

    //class OperationsQueue

OperationsQueue::OperationsQueue() {
    last = -1; //-1 ?
    canWeForward = false;
}

int OperationsQueue::getLast() const {return last;}
bool OperationsQueue::getCanWeForward () const {return canWeForward;}

std::tuple<int,int,InfoCase,InfoCase> OperationsQueue::getPrevious() const {
    last --;
    canWeForward = true;
}

std::tuple<int,int,InfoCase,InfoCase> OperationsQueue::getForward() const {
    last ++;
}

void OperationsQueue::addOp (const std::tuple<int,int,InfoCase,InfoCase> & t) {
    last++;
    if (last > queue.size())
        queue.resize(last);
    queue[last]=t*;
}
