#ifndef INFOCASE_HPP
#define INFOCASE_HPP

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


#endif // INFOCASE_HPP
