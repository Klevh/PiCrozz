#ifndef INFOCASE_HPP
#define INFOCASE_HPP

/** @file InfoCase.hpp*/ 

class InfoCase {
    // type de la case: vide = -1, croix = 0, case pleine = 1, plein_test = 2, croix_test = 3;
    // ces valeurs sont ulilisés dans l'attribut grille de la classe Grille
    // pour les attributs indicationsLignes et indicationsColonnes de la classe Grille, type sera simplement la taille d'un bloc
    
    //couleur de la case; ex: 0xFFFFFF

    int type; /**< type attribute of the Case: 
            its value is: 
                either the type of the case if used in the attribute "grille" of the class Picross, for instance: empty = -1, cross = 0, full = 1, full_try = 2, cross_try = 3
                or the size of a block if used in the attributes "indicationsLignes" or "indicationsColonnes" of the class Picross
            */          
    int color; /**< Color attribute of the Case */ 

public:
    //contructeurs
    /**
     * @brief InfoCase default constructor
     */
    InfoCase();
    /**
     * @brief InfoCase constructor with attribute initialisation
     */
    InfoCase(int,int);
    
    //getters
    int getType() const;
    int getColor() const;
     
    //setters
    void setType(int);
    void setColor(int);
};


#endif // INFOCASE_HPP
