#include "Picross.hpp"

#include <fstream>
#include <iostream>

using std::vector;
using std::string;
using std::tuple;
using std::cout;
using std::endl;

#ifdef XMLCheckResult
#undef XMLCheckResult
#endif

#if defined(DEBUG) && !defined(NDEBUG)
#include <iostream>
#define XMLCheckResult(a_eResult)					\
    if ((a_eResult) != tinyxml2::XML_SUCCESS) {				\
	std::cout << "Error: " << (a_eResult) << std::endl;		\
    }while(0)
#define SHOW_LOG(a) std::cout << (a) << std::endl
#else
#define XMLCheckResult(a)
#define SHOW_LOG(a)
#endif

//xml

static string getXMLText (tinyxml2::XMLElement * elmt, const string& name) {

     const char * attribute = nullptr;
     const char * value;
     string s_attr;
     string s_val;
     string res;

     attribute = (char*) elmt->Value();
     if (attribute == nullptr)
	  s_attr = "";
     else
	  s_attr = attribute;
     value = elmt->GetText();
     if (value == nullptr)
	  s_val = "";
     else
	  s_val = value;

     if (s_attr == name)
	  res = s_val;
     else
	  res="";
     return res; 
}

static string getXMLAttributeText(tinyxml2::XMLElement* elmt, const string& name, const char * type) {

     const char * attribute_val = nullptr;
     const char * value;
     string s_attr;
     string s_val;
     string res;

     attribute_val = (char*) elmt->Value();
     if (attribute_val == nullptr)
      s_attr = "";
     else
      s_attr = attribute_val;
     

     value = elmt->Attribute(type);
     if (value == nullptr)
      s_val = "";
     else
      s_val = value;

     if (s_attr == name)
      res = s_val;
     else
      res="";
     
     return res;    

}
static int getXMLInt(tinyxml2::XMLElement* elmt) {

     int res = -1;
     elmt->QueryIntText(&res);

     return res; 
}






        //class Picross


//constructeurs

Picross::Picross() {}


Picross::Picross(const string & path) {

     const char * path2 = path.c_str();
     tinyxml2::XMLDocument xmlDoc;
     tinyxml2::XMLError eResult = xmlDoc.LoadFile(path2);
     
     XMLCheckResult(eResult);
     
     tinyxml2::XMLNode * pRoot = xmlDoc.FirstChildElement("puzzleset"); //section puzzleset
     
     tinyxml2::XMLElement * pElement = pRoot->FirstChildElement("puzzle"); //tricherie pour que ca fonctionne (ne fonctionne pas si on fait cet appel après la ligne suivante)
     pRoot = pRoot->FirstChildElement("puzzle"); //section puzzle

     //selectionner la couleur par défaut
     colors.setDefaultColor(getXMLAttributeText(pElement, "puzzle", "defaultcolor"));

     //cout<<colors.getDefaultColor()<<"   bouh"<< endl;
     
     /*if (pRoot == nullptr)
       return tinyxml2::XML_ERROR_FILE_READ_ERROR;*/

     pElement = pRoot->FirstChildElement("title");

     
     title = getXMLText (pElement,"title");
     SHOW_LOG(title);

     pElement = pRoot->FirstChildElement("author");
     author = getXMLText (pElement,"author");
     SHOW_LOG(author);

     
     pElement = pRoot->FirstChildElement("copyright");
     copyright = getXMLText (pElement,"copyright");
     SHOW_LOG(copyright);


     pElement = pRoot->FirstChildElement("description");
     description = getXMLText (pElement,"description");
     SHOW_LOG(description);
     
     pElement = pRoot->FirstChildElement("color");
     getXMLColors(pElement);

     pElement = pRoot->FirstChildElement("clues");
     getXMLGrid(pElement);

}


Picross::Picross(const Picross & p) : title(p.getTitle()), author(p.getAuthor()),
      copyright(p.getCopyright()), description(p.getDescription()),
      nbLignes(p.getNbLignes()), nbColonnes(p.getNbColonnes()), 
      colors(p.getColors())
{
    *this = p;
}


Picross& Picross::operator=(const Picross& p)
{

    grille.resize(nbLignes);
    for (int i = 0; i<nbLignes; i++) {
        grille[i].resize(nbColonnes);
        for(int j = 0; j<nbColonnes; j++) {

            setGrilleIJ(i,j,p.getGrille()[i][j].getType(),p.getGrille()[i][j].getColor());
        }
    }
     
     //cout<< "lignes" <<endl;
     indicationsLignes.resize(nbLignes);
     for(int i = 0; i<nbLignes; i++) {
        int len = p.getIndicationsLignes()[i].size();
        indicationsLignes[i].resize(len);
        for(unsigned int j = 0; j<len; j++) {

            setIndicationsLignesIJ(i,j,p.getIndicationsLignes()[i][j].getType(),p.getIndicationsLignes()[i][j].getColor());
        }
    }
     
     //cout<< "Colonnes" <<endl;
     indicationsColonnes.resize(nbColonnes);
     for(int i = 0; i<nbColonnes; i++) {
        int len = p.getIndicationsColonnes()[i].size();
        indicationsColonnes[i].resize(len);
        for(unsigned int j = 0; j<len; j++) {

            setIndicationsColonnesIJ(i,j,p.getIndicationsColonnes()[i][j].getType(),p.getIndicationsColonnes()[i][j].getColor());
        }
    } 
     
    return *this;
}

//getters

int Picross::getNbLignes() const {return nbLignes;}
int Picross::getNbColonnes() const {return nbColonnes;}
const Colors& Picross::getColors() const {return colors;}
const string& Picross::getTitle() const {return title;}
const string& Picross::getAuthor() const {return author;}
const string& Picross::getCopyright() const {return copyright;}
const string& Picross::getDescription() const {return description;}
const vector< vector<InfoCase> >& Picross::getGrille() const {return grille;}
const vector< vector<InfoCase> >& Picross::getIndicationsLignes() const {return indicationsLignes;}
const vector< vector<InfoCase> >& Picross::getIndicationsColonnes() const {return indicationsColonnes;}

//secondary getters

int Picross::getMaxSizeIndicationsLignes() const {
    unsigned max = 0;
    for(int i = 0; i<nbLignes; i++)
        max = ( (max < indicationsLignes[i].size()) ? indicationsLignes[i].size() : max);
    return max;
}

int Picross::getMaxSizeIndicationsColonnes() const {
    unsigned max = 0;
    for(int i = 0; i<nbColonnes; i++)
        max = ( (max < indicationsColonnes[i].size()) ? indicationsColonnes[i].size() : max);
    return max;
}


//setters

void Picross::addIndicationsLignes (int num_ligne, int type, int color) {
    InfoCase i(type,color);
    indicationsLignes[num_ligne].push_back(i);
}

void Picross::addIndicationsColonnes (int num_colonne, int type, int color) {
    InfoCase i(type,color);
    indicationsColonnes[num_colonne].push_back(i);
}

void Picross::setGrilleIJ(int i, int j, int type, int color) {
    std::tuple<int,int,InfoCase,InfoCase> t;
    std::get<0> (t) = i;
    std::get<1> (t) = j;
    std::get<2> (t) = InfoCase(grille[i][j].getType(),grille[i][j].getColor());
    std::get<3> (t) = InfoCase(type,color);
    
    grille[i][j].setType(type);
    grille[i][j].setColor(color);


    queue.addOp(t);
}

void Picross::setGrilleIJ(int i, int j, char c) {
    
    int color = colors.getColorFromChar(c);
    grille[i][j].setType(0);
    grille[i][j].setColor(color);
}


void Picross::setIndicationsLignesIJ (int i, int j, int type, int color) {
    
    indicationsLignes[i][j].setType(type);
    indicationsLignes[i][j].setColor(color);
}

void Picross::setIndicationsColonnesIJ (int i, int j, int type, int color) {

    indicationsColonnes[i][j].setType(type);
    indicationsColonnes[i][j].setColor(color);
}


//xml
void Picross::getXMLColors(tinyxml2::XMLElement* elmt) {

    while (elmt != nullptr) {

        const char * tmp = getXMLAttributeText(elmt,"color","char").c_str();
        char c = tmp[0];

        //std::get<1>(t) = getXMLInt(elmt);
        //std::get<1>(t) = atoi (getXMLText(elmt,"color").c_str());
        //std::get<1>(t) = std::stoul(getXMLText(elmt,"color").c_str(), nullptr, 16);

        colors.addColor(getXMLAttributeText(elmt,"color","name"), std::stoul(getXMLText(elmt,"color").c_str(), nullptr, 16), c);

        elmt = elmt->NextSiblingElement("color");
    }

}


void Picross::getXMLGrid (tinyxml2::XMLElement* elmt) {
    tinyxml2::XMLElement * elmt_line = elmt->FirstChildElement("line");
    tinyxml2::XMLElement * elmt_count = elmt_line->FirstChildElement("count");
    int i=0,j=0; //compteurs de nb lignes et colonnes;


    //ajout des indications sur les colonnes;
    while (elmt_line != nullptr) {
        //cout<<"size: " << indicationsColonnes.size() << "   ";
        indicationsColonnes.resize(indicationsColonnes.size()+1);
        //cout<<"size: " << indicationsColonnes.size() <<  "      j: " << j <<endl;
        
        while(elmt_count != nullptr) {

            string s = getXMLAttributeText(elmt_count,"count","color");
            if(s == "")
                s=colors.getDefaultColor();

            int taille = getXMLInt(elmt_count);

            int color = colors.getColorFromName(s);
            //cout << "s: " << s << "      taille: " << taille <<"        color: " << color <<endl;

            addIndicationsColonnes(j,taille,color);

            elmt_count = elmt_count->NextSiblingElement("count");
        }
        elmt_line = elmt_line->NextSiblingElement("line");
        if (elmt_line != nullptr)
            elmt_count = elmt_line->FirstChildElement("count");
        j++;
        //cout << endl<<endl;
    }


    elmt_line = elmt->NextSiblingElement("clues")->FirstChildElement("line"); // on se place sur les lignes
    elmt_count = elmt_line->FirstChildElement("count");
    while (elmt_line != nullptr) {
        //cout<<"size: " << indicationsLignes.size() << "   ";
        indicationsLignes.resize(indicationsLignes.size()+1);
        //cout<<"size: " << indicationsLignes.size() <<  "      i: " << i <<endl;
        
        while(elmt_count != nullptr) {

            string s = getXMLAttributeText(elmt_count,"count","color");
            if(s == "")
                s=colors.getDefaultColor();

            int taille = getXMLInt(elmt_count);

            int color = colors.getColorFromName(s);
            //cout << "s: " << s << "      taille: " << taille <<"        color: " << color <<endl;

            addIndicationsLignes(i,taille,color);

            elmt_count = elmt_count->NextSiblingElement("count");
        }
        elmt_line = elmt_line->NextSiblingElement("line");
        if (elmt_line != nullptr)
            elmt_count = elmt_line->FirstChildElement("count");
        i++;
        //cout << endl<<endl;
    }

    nbLignes = i;
    nbColonnes = j;

    //on "alloue" la grille
    grille.resize(i);
    for (unsigned k = 0; k<grille.size(); k++)
        grille[k].resize(j);

    //on init la grille

    for (int i=0; i < nbLignes; i++) {
        
        for (int j=0; j < nbColonnes; j++) {

            this->setGrilleIJ(i,j,-1,-1);
        }
    }    

}



void Picross::displayClassic() const {

    int margin = 10;
    int temp = 0;
    //affichage grille + indications lignes après

    int max = getMaxSizeIndicationsColonnes();
    int size = 0;

    /*
    for (int j = 0; j<max; j++) {
        
        cout<<"     |";
        for (int i = 0; i<nbColonnes; i++) {

            size = indicationsColonnes[i].size();


            if (size < (max-j)) {
                cout<<"  |";                
            }
            else
                cout<< indicationsColonnes[i][size-j].getType()<<"|";

            cout<<endl;
        }
    }*/


    for (int i = 0; i<nbColonnes; i++) {
        cout<<"|";
        for (int j = 0; j<indicationsColonnes[i].size(); j++) {

            cout<< indicationsColonnes[i][j].getType()<<"|";
        }
        cout<<endl;
    }


    for(int i = 0; i<nbLignes; i++) {

        cout << i;
        if(i<10)
            cout <<":   |";
        else
            cout <<":  |";

        for(int j = 0; j<nbColonnes; j++) {
            int color = grille[i][j].getColor();
            //cout << "color: " << color <<endl;
            cout << colors.getCharFromColor(color) << "|";
        }
        cout<<"     |";
        
        for (unsigned k = 0; k<indicationsLignes[i].size(); k++) {
            temp = indicationsLignes[i][k].getType();
            if (temp!=0) {
                if (temp<10)
                    cout << " " << temp << "|";
                else
                    cout<< temp << "|";
            }
        }
        cout<<endl;
        /*
        cout << "     .";
        for(int j = 0; j<nbColonnes; j++) {
            cout << "-.";
        }
        cout<<endl;*/
    }

}

