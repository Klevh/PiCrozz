#include "grille.hpp"


#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != tinyxml2::XML_SUCCESS) { printf("Error: %i\n", a_eResult); /*return a_eResult;*/ }
#endif


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
Colors::Colors(const Colors & c) : nbColors(c.getNbColors()),  defaultColor(c.getDefaultColor()),
				   colorsList(c.getColorsList()) {
}

//getters
int Colors::getNbColors() const {return nbColors;}
string Colors::getDefaultColor() const {return defaultColor;}
vector<tuple<string,int>> Colors::getColorsList() const {return colorsList;}

//setters
void Colors::setNbColors (int nb) {nbColors = nb;}
void Colors::setDefaultColor(string c) {defaultColor = c;}






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
     cout<<title<<"   bouh"<< endl;

     pElement = pRoot->FirstChildElement("author");
     author = getXMLText (pElement,"author");
     cout<<author<<"   bouh"<< endl;

     
     pElement = pRoot->FirstChildElement("copyright");
     copyright = getXMLText (pElement,"copyright");
     cout<<copyright<<"   bouh"<< endl;


     pElement = pRoot->FirstChildElement("description");
     description = getXMLText (pElement,"description");
     cout<<description<<"   bouh"<< endl;
     
     //vector de tuples<string, char, int> pour stocker toutes les couleurs
     //dans une classe ?
     //stocker la couleur par défaut pour après
     
     
}


Picross::Picross(const Picross & p) : title(p.getTitle()), author(p.getAuthor()),
				      copyright(p.getCopyright()), nbLignes(p.getNbLignes()),
				      nbColonnes(p.getNbColonnes()), colors(p.getColors())  {
     
     grille.resize(nbLignes);
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
Colors Picross::getColors() const {return colors;}
string Picross::getTitle() const {return title;}
string Picross::getAuthor() const {return author;}
string Picross::getCopyright() const {return copyright;}
string Picross::getDescription() const {return description;}
vector< vector<InfoCase> > Picross::getGrille() const {return grille;}
vector< vector<InfoCase> > Picross::getIndicationsLignes() const {return indicationsLignes;}
vector< vector<InfoCase> > Picross::getIndicationsColonnes() const {return indicationsColonnes;}


//xml

string Picross::getXMLText (tinyxml2::XMLElement * elmt, string name) {

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
