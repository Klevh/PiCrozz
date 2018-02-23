#ifndef COLORS_HPP
#define COLORS_HPP

#include <string>
#include <vector>
#include <tuple>


class Colors {
    // 1 par défaut si grille noir et blanc
    int nbColors;  /**< number of colors */ 
    std::string defaultColor; /**< Default Color */ 
    std::vector<std::tuple<std::string,int,char>> colorsList; /**< name,value and char of all colors */
     
public:
    //constructors
    /**
     * @brief InfoCase default constructor
     */
    Colors();
    /**
     * @brief InfoCase copy constructor
     */
    Colors(const Colors &);

    //getters
    /**
     * @brief returns the number of colors attribute
     */
    int getNbColors() const;
    /**
     * @brief returns the default color
     */
    const std::string& getDefaultColor() const;
    /**
     * @brief returns a const ref to the list of all colors
     */
    const std::vector<std::tuple<std::string,int,char>>& getColorsList() const;
    
    /**
     * @brief returns the value of the color which has the name s
     * @param s : name of a color
     */
    int getColorFromName(const std::string& s) const;
    /**
     * @brief returns the char of the color which has the value c
     * @param c : int value of a color
     */
    char getCharFromColor(int c) const;
     /**
     * @brief returns the value of the color which has the char c
     * @param c : char of a color
     */
    int getColorFromChar(char c) const;

    //setters
    void setNbColors (int);
    void setDefaultColor(const std::string&);
    /**
     * @brief adds a color to the color list
     * @param s : name of the color
     * @param nb : value of the color
     * @param c : char of the color
     */
    void addColor(const std::string& s, int nb, char c);
};


std::ostream& operator<< (std::ostream &flux, Colors & c);
std::ostream& operator<< (std::ostream &flux, const Colors & c);

#endif // COLORS_HPP
