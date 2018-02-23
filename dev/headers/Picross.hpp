#ifndef PICROSS_HPP
#define PICROSS_HPP

#include <string>
#include <vector>
#include <tuple>
#include <iostream>

#include "tinyxml2.h"
#include "InfoCase.hpp"
#include "Colors.hpp"
#include "OperationsQueue.hpp"


class Picross {
    //mettre le nom du fichier (sans le ".xml")   //A FAIRE
    std::string id;              /**< id of the Picross*/
    std::string title;           /**< title of the Picross*/
    std::string author;          /**< author of the Picross*/
    std::string copyright;       /**< copyrights of the Picross*/
    std::string description;     /**< description of the Picross*/
    int nbLignes, nbColonnes;    /**< number of rows and columns of the Picross*/
    Colors colors;               /**< contains informations on all the colors, see documentation of class Colors*/
    //tableau principal=lignes
    std::vector< std::vector<InfoCase> > grille;                 /**< grid of Picross, grille[i][j] is the box of the i th row and j th column*/
    std::vector< std::vector<InfoCase> > indicationsLignes;      /**< information about the rows */
    std::vector< std::vector<InfoCase> > indicationsColonnes;    /**< information about the columns */
    OperationsQueue queue;                                       /**< stack of game operations, see Documentation of class OperationsQueue */
     
public:
    //constructeurs
    /**
     * @brief default constructor
     */
    Picross();
    /**
     * @brief constructor from an xml file
     * @param path : path of the xml file
     */
    Picross(const std::string & path);
    /**
     * @brief copy constructor
     */
    Picross(const Picross &);
    Picross& operator=(const Picross&);

    //getters
    int getNbLignes() const;
    int getNbColonnes() const;
    const Colors& getColors() const;
    const std::string& getId() const;
    const std::string& getTitle() const;
    const std::string& getAuthor() const;
    const std::string& getCopyright() const;
    const std::string& getDescription() const;
    const std::vector< std::vector<InfoCase> >& getGrille() const;
    const std::vector< std::vector<InfoCase> >& getIndicationsLignes() const;
    const std::vector< std::vector<InfoCase> >& getIndicationsColonnes() const;
    const OperationsQueue& getQueue() const;
    
    //secondary getters
    /**
     * @brief return max(indicationsLignes[i].size(), i€[0,nbLignes-1]
     */
     int getMaxSizeIndicationsLignes() const;
     /**
     * @brief return max(indicationsColonnnes[i].size(), i€[0,nbColonnes-1]
     */
     int getMaxSizeIndicationsColonnes() const;

    //setters
     /**
     * @brief adds an element to indicationsLignes[num_ligne]
     * @param num_ligne : id of the row
     * @param type : type of the box block
     * @param color : color of the box block
     */
    void addIndicationsLignes (int num_ligne, int type, int color);   //addIndicationsLignes(num_ligne,type,color) fait un push_back(InfoCase<int,int> i(type,color) ) à indicationsLignes[num_ligne]
    /**
     * @brief adds an element to indicationsColonnes[num_colonne]
     * @param num_colonne : id of the column
     * @param type : type of the box block
     * @param color : color of the box block
     */
    void addIndicationsColonnes (int num_colonne, int type, int color); //idem pour indicationsColonnes
    /**
     * @brief sets grille[i][j] with type and color of the box
     * @param i : row id
     * @param j : column id
     * @param type : type of the box block
     * @param color : color of the box block
     */

    /**
     * @brief sets grille[i][j] without setting the queue (type and color set to -1)
     * @param i : row id
     * @param j : column id
     */
    void initiateGrilleIJ(int i, int j);
    /**
     * @brief sets grille[i][j] without setting the queue
     * @param i : row id
     * @param j : column id
     * @param type : type of the box block
     * @param color : color of the box block
     */
    void initiateGrilleIJ(int i, int j, int type, int color);
 
    /**
     * @brief sets grille[i][j]
     * also sets the queue
     * @param i : row id
     * @param j : column id
     * @param type : type of the box block
     * @param color : color of the box block
     */
    
    void setGrilleIJ(int i, int j, int type, int color);
    /**
     * @brief sets grille[i][j] knowing only the char of the color (will set type and color)
     * also sets the queue
     * @param i : row id
     * @param j : column id
     * @param char : char of the color of the box
     */

    //A NE JAMAIS UTILISER
    void setGrilleIJ(int i, int j, char c); 

    /*
     * @brief sets indicationsLignes[i][j] with type and color of the box
     * @param i : row id
     * @param j : column id
     * @param type : type of the box block
     * @param char : char of the color of the box block
     */
    void setGrilleIJ(int i, int j, int type, char c); 

    /*
     * @brief sets indicationsLignes[i][j] with type and color of the box
     * @param i : row id
     * @param j : column id
     * @param type : type of the box block
     * @param color : color of the box block
     */
    void setIndicationsLignesIJ(int i, int j, int type, int color);
    /**
     * @brief sets indicationsColonnes[i][j] with type and color of the box
     * @param i : row id
     * @param j : column id
     * @param type : type of the box block
     * @param color : color of the box block
     */
    void setIndicationsColonnesIJ(int i, int j, int type, int color);


    //queue
    /**
     * @brief sets grille to its previous operation (if it exists, does nothing otherwise)
     * for instance: if grille[i][j] was last set from InfoCase(-1,-1) to InfoCase(0,0)
     * it will set back grille[i][j] to InfoCase(-1,-1)
     */
    InfoCase previousOp();
    /**
     * @brief sets grille to its forward operation (if it exists, does nothing otherwise)
     * for instance: if grille[i][j] was set back from InfoCase(0,0) to InfoCase(-1,-1) thx to previousOp()
     * it will set back grille[i][j] to InfoCase(0,0)
     */
    InfoCase forwardOp();

    // xml
    /**
     * @brief initiates the class Colors
     * @param elmt : pointer to an xml elmt, should be called with the first XMLElement containing the colors
     */
    void getXMLColors(tinyxml2::XMLElement* elmt);
    /**
     * @brief initiates "indicationsLignes", "indicationsColonnes" and allocates and initiates "grille"
     * @param elmt : pointer to an xml elmt, should be called with the first XMLElement containing the clues
     */
    void getXMLGrid (tinyxml2::XMLElement* elmt);


    // save / load

    void save() const;
    void load(const std::string & idGrid);

    //check
    int checkFinishedClassicLigne(int id);
    int checkFinishedClassicColonne(int id);
    int checkFinishedClassicGrid();

    //display
     void displayClassic() const;

};






std::ostream& operator<< (std::ostream &flux, Picross &p);













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
