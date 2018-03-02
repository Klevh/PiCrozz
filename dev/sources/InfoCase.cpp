#include "InfoCase.hpp"

         //class InfoCase


//contructeurs
InfoCase::InfoCase() {}
InfoCase::InfoCase(int t,int c) : type(t), color(c) {}

InfoCase::InfoCase(const InfoCase & i) : type(i.getType()), color(i.getColor())
{
    *this = i;
}

InfoCase& InfoCase::operator=(const InfoCase& i) {
    type = i.type;
    color = i.color;
    return *this;
}

//getters
int InfoCase::getType() const {return type;}
int InfoCase::getColor() const {return color;}

//setters
void InfoCase::setType(int t) {type=t;}
void InfoCase::setColor(int c) {color=c;}
