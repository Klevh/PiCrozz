#ifndef GRILLE_HPP
#define GRILLE_HPP

#include <string>
#include <vector>
#include <tuple>
#include "tinyxml2.h"

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
    std::string defaultColor;
    std::vector<std::tuple<std::string,int>> colorsList; //ou une map ? //pas besoin du char
     
public:
    //constructors
    Colors();
    Colors(const Colors &);

    //getters
    int getNbColors() const;
    const std::string& getDefaultColor() const;
    const std::vector<std::tuple<std::string,int>>& getColorsList() const;

    //setters
    void setNbColors (int);
    void setDefaultColor(const std::string&);
};




class Picross {
    std::string id; //mettre le nom du fichier (sans le ".xml")   //a faire
    std::string title;
    std::string author;
    std::string copyright;
    std::string description;
    int nbLignes, nbColonnes;
    Colors colors;
    std::vector< std::vector<InfoCase> > grille; //tableau principal=lignes
    std::vector< std::vector<InfoCase> > indicationsLignes;
    std::vector< std::vector<InfoCase> > indicationsColonnes;
     
public:
    //constructeurs
    Picross();
    Picross(const std::string &); // à partir d'un fichier XML
    Picross(const Picross &); // contructeur de copie
    Picross& operator=(const Picross&);

    //getters
    int getNbLignes() const;
    int getNbColonnes() const;
    const Colors& getColors() const;
    const std::string& getTitle() const;
    const std::string& getAuthor() const;
    const std::string& getCopyright() const;
    const std::string& getDescription() const;
    const std::vector< std::vector<InfoCase> >& getGrille() const;
    const std::vector< std::vector<InfoCase> >& getIndicationsLignes() const;
    const std::vector< std::vector<InfoCase> >& getIndicationsColonnes() const;
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
