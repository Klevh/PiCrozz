#ifndef PICROSS_HPP
#define PICROSS_HPP

#include <string>
#include <vector>
#include <tuple>
#include "tinyxml2.h"
#include "InfoCase.hpp"
#include "Colors.hpp"


class Picross {
    std::string id; //mettre le nom du fichier (sans le ".xml")   //A FAIRE
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
    
    //secondary getters
     int getMaxSizeIndicationsLignes() const;
     int getMaxSizeIndicationsColonnes() const;

    //setters
    void addIndicationsLignes (int, int, int);   //addIndicationsLignes(num_ligne,type,color) fait un push_back(InfoCase<int,int> i(type,color) ) à indicationsLignes[num_ligne]
    void addIndicationsColonnes (int, int, int); //idem pour indicationsColonnes

    void setGrilleIJ(int,int,int,int);
    void setGrilleIJ(int,int,char); 

    void setIndicationsLignesIJ(int,int,int,int);
    void setIndicationsColonnesIJ(int,int,int,int);

    // xml
    void getXMLColors(tinyxml2::XMLElement*);
    void getXMLGrid (tinyxml2::XMLElement*);


    //display
     void displayClassic() const;
};



class OperationsQueue {
    std::vector<std::tuple<int,int,InfoCase,InfoCase> > queue;
     
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



#endif // PICROSS_HPP
