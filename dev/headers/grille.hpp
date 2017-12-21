#ifndef GRILLE_HPP
#define GRILLE_HPP

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <tuple>
#include "tinyxml2.h"

using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::string;
using std::tuple;

class InfoCase {
    // type de la case: vide = -1, croix = 0, case pleine = 1, plein_test = 2, croix_test = 3;
    // ces valeurs sont ulilisés dans l'attribut grille de la classe Grille
    // pour les attributs indicationsLignes et indicationsColonnes de la classe Grille, type sera simplement la taille d'un bloc
    int type;
    int color; //couleur de la case; ex: 0xFFFFFF

public:
    //contructeurs
     InfoCase();
     InfoCase(int,int);
     
     //getters
     int getType() const;
    int getColor() const;
     
     //setters
     void setType(int);
     void setColor(int);
};


class Colors {
     int nbColors;  // 1 par défaut si grille noir et blanc
     string defaultColor;
     vector<tuple<string,int>> colorsList; //ou une map ? //pas besoin du char
     
public:
     //constructors
     Colors();
     Colors(const Colors &);

     //getters
     int getNbColors() const;
     string getDefaultColor() const;
     vector<tuple<string,int>> getColorsList() const;

     //setters
     void setNbColors (int);
     void setDefaultColor(string);
};




class Picross {
     string id; //mettre le nom du fichier (sans le ".xml")   //a faire
     string title;
     string author;
     string copyright;
     string description;
     int nbLignes, nbColonnes;
     Colors colors;
     vector< vector<InfoCase> > grille; //tableau principal=lignes
     vector< vector<InfoCase> > indicationsLignes;
     vector< vector<InfoCase> > indicationsColonnes;
     
public:
     //constructeurs
     Picross();
     Picross(const string &); // à partir d'un fichier XML
     Picross(const Picross &); // contructeur de copie

     //getters
     int getNbLignes() const;
     int getNbColonnes() const;
     Colors getColors() const;
     string getTitle() const;
     string getAuthor() const;
     string getCopyright() const;
     string getDescription() const;
     vector< vector<InfoCase> > getGrille() const;
     vector< vector<InfoCase> > getIndicationsLignes() const;
     vector< vector<InfoCase> > getIndicationsColonnes() const;

     //xml
     string getXMLText (tinyxml2::XMLElement* , string);//retourne val (ex: <title>val</title>
     
};




/*
class OperationsQueue {

     
public:

};*/



class Resolution {
     
     Picross * picross;
     Picross res;

public:
     
};

#endif // GRILLE_HPP
