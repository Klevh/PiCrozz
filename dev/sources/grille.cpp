#include "grille.hpp"

#include <fstream>

using std::vector;
using std::string;
using std::tuple;

#ifdef XMLCheckResult
#undef XMLCheckResult
#endif

#if defined(DEBUG) && !defined(NDEBUG)
#include <iostream>
#define XMLCheckResult(a_eResult)					\
    if ((a_eResult) != tinyxml2::XML_SUCCESS) {				\
	std::cout << "Error: " << (a_eResult) << std::endl;		\
    }while(0)
#define SHOW_LOG(a) std::cout << (a) << std::endl
#else
#define XMLCheckResult(a)
#define SHOW_LOG(a)
#endif

//xml

static string getXMLText (tinyxml2::XMLElement * elmt, const string& name) {

     const char * attribute = nullptr;
     const char * value;
     string s_attr;
     string s_val;
     string res;

     attribute = (char*) elmt->Value();
     if (attribute == nullptr)
	  s_attr = "";
     else
	  s_attr = attribute;
     value = elmt->GetText();
     if (value == nullptr)
	  s_val = "";
     else
	  s_val = value;

     if (s_attr == name)
	  res = s_val;
     else
	  res="";
     return res; 
}

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




         //class Colors

//constructors
Colors::Colors() {}
Colors::Colors(const Colors & c)
    : nbColors(c.getNbColors()),
      defaultColor(c.getDefaultColor()),
      colorsList(c.getColorsList())
{}

//getters
int Colors::getNbColors() const {return nbColors;}
const string& Colors::getDefaultColor() const {return defaultColor;}
const vector<tuple<string,int>>& Colors::getColorsList() const {return colorsList;}

//setters
void Colors::setNbColors (int nb) {nbColors = nb;}
void Colors::setDefaultColor(const string& c) {defaultColor = c;}






        //class Picross


//constructeurs

Picross::Picross() {}


Picross::Picross(const string & path) {

     const char * path2 = path.c_str();
     tinyxml2::XMLDocument xmlDoc;
     tinyxml2::XMLError eResult = xmlDoc.LoadFile(path2);
     
     XMLCheckResult(eResult);
     
     tinyxml2::XMLNode * pRoot = xmlDoc.FirstChildElement("puzzleset"); //section puzzleset
     pRoot = pRoot->FirstChildElement("puzzle"); //section puzzle

     //selectionner la couleur par défaut

     
     /*if (pRoot == nullptr)
       return tinyxml2::XML_ERROR_FILE_READ_ERROR;*/

     tinyxml2::XMLElement * pElement = pRoot->FirstChildElement("title");

     title = getXMLText (pElement,"title");
     SHOW_LOG(title);

     pElement = pRoot->FirstChildElement("author");
     author = getXMLText (pElement,"author");
     SHOW_LOG(author);

     
     pElement = pRoot->FirstChildElement("copyright");
     copyright = getXMLText (pElement,"copyright");
     SHOW_LOG(copyright);


     pElement = pRoot->FirstChildElement("description");
     description = getXMLText (pElement,"description");
     SHOW_LOG(description);
     
     //vector de tuples<string, char, int> pour stocker toutes les couleurs
     //dans une classe ?
     //stocker la couleur par défaut pour après
     
     
}


Picross::Picross(const Picross & p)
    : title(p.getTitle()), author(p.getAuthor()),
      copyright(p.getCopyright()), nbLignes(p.getNbLignes()),
      nbColonnes(p.getNbColonnes()), colors(p.getColors())
{
    *this = p;
}

Picross& Picross::operator=(const Picross& p){grille.resize(nbLignes);
     for (int i = 0; i<nbLignes; i++) {
	  grille[i].resize(nbColonnes);
	  for(int j = 0; j<nbColonnes; j++) {
	       grille[i][j].setType(p.getGrille()[i][j].getType());
	       grille[i][j].setColor(p.getGrille()[i][j].getColor());
	  }
     }
     
     indicationsLignes.resize(nbLignes);
     for(int i = 0; i<nbLignes; i++) {
	  for(unsigned int j = 0; j<p.getIndicationsLignes().size(); j++) {
	       indicationsLignes[i][j].setType(p.getIndicationsLignes()[i][j].getType());
	       indicationsLignes[i][j].setType(p.getIndicationsLignes()[i][j].getColor());
	  }
     }
     
     indicationsColonnes.resize(nbColonnes);
     for(int i = 0; i<nbColonnes; i++) {
        for(unsigned int j = 0; j<p.getIndicationsColonnes().size(); j++) {
	     indicationsColonnes[i][j].setType(p.getIndicationsColonnes()[i][j].getType());
	     indicationsColonnes[i][j].setType(p.getIndicationsColonnes()[i][j].getColor());
        }
     }
}

//getters

int Picross::getNbLignes() const {return nbLignes;}
int Picross::getNbColonnes() const {return nbColonnes;}
const Colors& Picross::getColors() const {return colors;}
const string& Picross::getTitle() const {return title;}
const string& Picross::getAuthor() const {return author;}
const string& Picross::getCopyright() const {return copyright;}
const string& Picross::getDescription() const {return description;}
const vector< vector<InfoCase> >& Picross::getGrille() const {return grille;}
const vector< vector<InfoCase> >& Picross::getIndicationsLignes() const {return indicationsLignes;}
const vector< vector<InfoCase> >& Picross::getIndicationsColonnes() const {return indicationsColonnes;}
