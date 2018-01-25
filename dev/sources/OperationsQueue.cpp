#include "OperationsQueue.hpp"

#include <iostream>

using std::vector;
using std::string;
using std::tuple;
using std::cout;
using std::endl;


    //class OperationsQueue

OperationsQueue::OperationsQueue() : realLast(-1), currentLast(-1) {}

int OperationsQueue::getRealLast() const {return realLast;}
int OperationsQueue::getCurrentLast() const {return currentLast;}
bool OperationsQueue::canWeForward () const {return currentLast < realLast;}
bool OperationsQueue::canWePrevious () const {return currentLast > -1;}

std::tuple<int,int,InfoCase,InfoCase> OperationsQueue::getPrevious() {
    std::tuple<int,int,InfoCase,InfoCase> t = queue[currentLast];
    currentLast --;

    /*cout<< ":   i=" << std::get<0> (t) <<"   j=" << std::get<1> (t)
            << "    Previous:  type=" << std::get<2>(t).getType() << "   color=" << std::get<2>(t).getColor()
            << "    Current:  type=" << std::get<3>(t).getType() << "   color=" << std::get<3>(t).getColor()
            << endl;*/
    return t;
}

std::tuple<int,int,InfoCase,InfoCase> OperationsQueue::getForward() {
    if (currentLast < realLast)
        currentLast ++;
    std::tuple<int,int,InfoCase,InfoCase> t = queue[currentLast];

    return t;
}

void OperationsQueue::addOp (const std::tuple<int,int,InfoCase,InfoCase> & t) {
    
    currentLast++;
    realLast = currentLast;
    if (realLast + 1> (int)queue.size())
        queue.resize(realLast+1);
    queue[realLast]= t;
}

void OperationsQueue::display() const {
    tuple<int,int,InfoCase,InfoCase> t;
    cout<< "Queue size : " << queue.size() << "     realLast: " << realLast <<"     currentLast: " << currentLast << endl   ;
    for (unsigned i=0; i<queue.size(); i++) {
        t=queue[i];
        cout<< i << ":   i=" << std::get<0> (t) <<"   j=" << std::get<1> (t)
            << "    Previous:  type=" << std::get<2>(t).getType() << "   color=" << std::get<2>(t).getColor()
            << "    Current:  type=" << std::get<3>(t).getType() << "   color=" << std::get<3>(t).getColor()
            << endl;
    }

}