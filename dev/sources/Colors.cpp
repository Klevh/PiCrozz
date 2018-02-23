#include "Colors.hpp"
#include <iostream>

using std::vector;
using std::string;
using std::tuple;

         //class Colors

//constructors
Colors::Colors() {}
Colors::Colors(const Colors & c)
    : nbColors(c.getNbColors()),
      defaultColor(c.getDefaultColor())
      
{
    int len = c.getColorsList().size();

    for(int i=0; i<len; i++)
        addColor(std::get<0> (c.getColorsList()[i]),  std::get<1> (c.getColorsList()[i]), std::get<2> (c.getColorsList()[i]));

}

//getters
int Colors::getNbColors() const {return nbColors;}
const string& Colors::getDefaultColor() const {return defaultColor;}
const vector<tuple<string,int,char>>& Colors::getColorsList() const {return colorsList;}


int Colors::getColorFromName(const string& s) const {
    bool fin = false;
    unsigned i = 0;
    int res = -1;
    while (!fin && i<colorsList.size()) {

        if(std::get<0> (colorsList[i]) == s) {
            res = std::get<1> (colorsList[i]);
            fin = true;
        }
        ++i;
    }

    return res;
}

char Colors::getCharFromColor(int c) const {
    bool fin = false;
    unsigned i = 0;
    char res = ' ';
    while (!fin && i<colorsList.size()) {

        if(std::get<1> (colorsList[i]) == c) {
            res = std::get<2> (colorsList[i]);
            fin = true;
        }
        ++i;
    }

    return res;
}

int Colors::getColorFromChar(char c) const {
    bool fin = false;
    unsigned i = 0;
    int res = -1;
    while (!fin && i<colorsList.size()) {

        if(std::get<2> (colorsList[i]) == c) {
            res = std::get<1> (colorsList[i]);
            fin = true;
        }
        ++i;
    }

    return res;
}

//setters
void Colors::setNbColors (int nb) {nbColors = nb;}
void Colors::setDefaultColor(const string& c) {defaultColor = c;}
void Colors::addColor(const string& s,int nb,char c) {
    tuple<string,int,char> t;
    std::get<0> (t) = s;
    std::get<1> (t) = nb;
    std::get<2> (t) = c;

    colorsList.push_back(t);
}



std::ostream& operator<< (std::ostream &flux, Colors & c)
{
    try
    {
        flux << "\nnbColors : " << c.getNbColors()
        << "\nDefaultColor : " << c.getDefaultColor();
        
        for (unsigned i = 0; i<c.getColorsList().size(); i++) {
            flux << "\n string : " << std::get<0> (c.getColorsList()[i])
            << "\n value : " << std::get<1> (c.getColorsList()[i])
            << "\n char : " << std::get<2> (c.getColorsList()[i]);
        }
        flux << std::endl;
    }
    catch (...)
    {
        std::cerr << "\n*** Une erreur s'est produite ! ***\n";
    }
    return flux;
}


std::ostream& operator<< (std::ostream &flux, const Colors & c)
{
    try
    {
        flux << "\nnbColors : " << c.getNbColors()
        << "\nDefaultColor : " << c.getDefaultColor();
        
        for (unsigned i = 0; i<c.getColorsList().size(); i++) {
            flux << "\n string : " << std::get<0> (c.getColorsList()[i])
            << "   value : " << std::get<1> (c.getColorsList()[i])
            << "   char : " << std::get<2> (c.getColorsList()[i]);
        }
        flux << std::endl;
    }
    catch (...)
    {
        std::cerr << "\n*** Une erreur s'est produite ! ***\n";
    }
    return flux;
}
