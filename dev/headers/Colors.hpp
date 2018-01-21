#ifndef COLORS_HPP
#define COLORS_HPP

#include <string>
#include <vector>
#include <tuple>


class Colors {
    int nbColors;  // 1 par défaut si grille noir et blanc
    std::string defaultColor;
    std::vector<std::tuple<std::string,int,char>> colorsList; //ou une map ? //pas besoin du char
     
public:
    //constructors
    Colors();
    Colors(const Colors &);

    //getters
    int getNbColors() const;
    const std::string& getDefaultColor() const;
    const std::vector<std::tuple<std::string,int,char>>& getColorsList() const;
    int getColorFromName(const std::string&) const;
    char getCharFromColor(int) const; //récupère le char associé à la valeur de la couleur stockée dans InfoCase
    int getColorFromChar(char) const;

    //setters
    void setNbColors (int);
    void setDefaultColor(const std::string&);
    void addColor(const std::string&,int,char);
};


#endif // COLORS_HPP
