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
     vector<tuple<string,int,char>> colorsList; //ou une map ? //pas besoin du char ?
     
public:
     //constructors
     Colors();
     Colors(const Colors &);

     //getters
     int getNbColors() const;
     string getDefaultColor() const;
     //vector<tuple<string,int,char>> getColorsList() const;
     const vector<tuple<string,int,char>>& getColorsList() const;
     //vector<tuple<string,int,char>> const& getColorsList() const;
     int getColorValue(string);
     char getColorChar(int c);

     //setters
     void setNbColors (int);//appelé uniquement à la construction
     void setDefaultColor(string);
     void addColor(string,int,char);

     //others
     char getChar(int); //récupère le char associé à la valeur de la couleur stockée dans InfoCase

};




class Picross {
     string id; //mettre le nom du fichier (sans le ".xml")   //a faire
     string title;
     string author;
     string copyright;
     string description;
     int nbLignes, nbColonnes;
     Colors colors;
     vector< vector<InfoCase> > grille; //tableau principal=lignes    //infoCase contient le type de la case et sa couleur
     vector< vector<InfoCase> > indicationsLignes;                    //infoCase contient la longueur d'un bloc et sa couleur
     vector< vector<InfoCase> > indicationsColonnes;                  //infoCase contient la longueur d'un bloc et sa couleur
     
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
     vector< vector<InfoCase> > getGrille() const;                     //modif en const &
     vector< vector<InfoCase> > getIndicationsLignes() const;
     vector< vector<InfoCase> > getIndicationsColonnes() const;

     //setters
     void addIndicationsLignes (int, int, int);   //addIndicationsLignes(num_ligne,type,color) fait un push_back(InfoCase<int,int> i(type,color) ) à indicationsLignes[num_ligne]
     void addIndicationsColonnes (int, int, int); //idem pour indicationsColonnes

     void setGrilleIJ(int,int,int,int);

     //xml
     string getXMLText (tinyxml2::XMLElement* , string);//retourne val (ex: <title>val</title>
     string getXMLAttributeText(tinyxml2::XMLElement* , string, const char *);
     int getXMLInt(tinyxml2::XMLElement*);
     void getXMLColors(tinyxml2::XMLElement*);
     void getXMLGrid (tinyxml2::XMLElement*);
};





class OperationsQueue {
     vector<tuple<int,int,InfoCase,InfoCase> > queue;
     
public:

};



class Resolution { //classe abstraite
     
     Picross * picross;
     Picross res;

public:
     
};


class ClassicResolution : public Resolution {

public:
     
     
};


class ColorResolution : public Resolution {

public:
     
};



#endif // GRILLE_HPP
