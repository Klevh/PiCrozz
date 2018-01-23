#ifndef PICROSS_HPP
#define PICROSS_HPP

#include <string>
#include <vector>
#include <tuple>
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
    const std::string& getTitle() const;
    const std::string& getAuthor() const;
    const std::string& getCopyright() const;
    const std::string& getDescription() const;
    const std::vector< std::vector<InfoCase> >& getGrille() const;
    const std::vector< std::vector<InfoCase> >& getIndicationsLignes() const;
    const std::vector< std::vector<InfoCase> >& getIndicationsColonnes() const;
    
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
    void setGrilleIJ(int i, int j, int type, int color);
    /**
     * @brief sets grille[i][j] knowing only the char of the color (will set type and color)
     * @param i : row id
     * @param j : column id
     * @param char : char of the color of the box
     */
    void setGrilleIJ(int i, int j, char c); 

    /**
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
    std::tuple<int,int,InfoCase> getPreviousOp();
    std::tuple<int,int,InfoCase> getForwardOp();
    void setPreviousOp();
    void setForwardOp();

    void previousOp();
    void forwardOp();

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


    //display
     void displayClassic() const;
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
