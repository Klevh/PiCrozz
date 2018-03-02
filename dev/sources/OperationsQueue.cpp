#include "OperationsQueue.hpp"

#include "Debug.hpp"
#include <algorithm>

using std::vector;
using std::string;
using std::tuple;

    //class OperationsQueue

//constructeur
OperationsQueue::OperationsQueue() : realLast(-1), currentLast(-1) {}

//getters
int OperationsQueue::getRealLast() const {return realLast;}
int OperationsQueue::getCurrentLast() const {return currentLast;}
const std::vector<std::tuple<int,int,InfoCase,InfoCase>>& OperationsQueue::getQueue() const {return queue;}

//setters
void OperationsQueue::setRealLast(int last) {realLast = last;}
void OperationsQueue::setCurrentLast(int last) {currentLast = last;}
void OperationsQueue::setQueueI (int i, const std::tuple<int,int,InfoCase,InfoCase> & t) {queue[i]=t;}

// operators
OperationsQueue& OperationsQueue::operator=(const OperationsQueue& oq){
    queue.resize(oq.queue.size());
    std::copy(oq.queue.begin(), oq.queue.end(), queue.begin());

    realLast = oq.realLast;
    currentLast = oq.currentLast;

    return *this;
}

//tests
bool OperationsQueue::canWeForward () const {return currentLast < realLast;}
bool OperationsQueue::canWePrevious () const {return currentLast > -1;}

//others

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


void OperationsQueue::resize(int i) {queue.resize(i);}

#if defined(DEBUG) && !defined(NDEBUG)
using std::cout;
using std::endl;
void OperationsQueue::display() const {
    std::tuple<int,int,InfoCase,InfoCase> t;
    
    LOG_DEBUG("Queue size : " << queue.size() << "     realLast: " << realLast <<"     currentLast: " << currentLast);
    
    for (unsigned i=0; i<queue.size(); i++) {
        t=queue[i];
	LOG_DEBUG(i << ":   i=" << std::get<0> (t) <<"   j=" << std::get<1> (t)
		  << "    Previous:  type=" << std::get<2>(t).getType() << "   color=" << std::get<2>(t).getColor()
		  << "    Current:  type=" << std::get<3>(t).getType() << "   color=" << std::get<3>(t).getColor());
    }

}
#endif
