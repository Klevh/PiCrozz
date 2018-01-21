#include "InfoCase.hpp"

         //class InfoCase


//contructeurs
InfoCase::InfoCase() {}
InfoCase::InfoCase(int t,int c) : type(t), color(c) {}

//getters
int InfoCase::getType() const {return type;}
int InfoCase::getColor() const {return color;}

//setters
void InfoCase::setType(int t) {type=t;}
void InfoCase::setColor(int c) {color=c;}
