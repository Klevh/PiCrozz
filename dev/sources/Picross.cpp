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

     pElement = pRoot->FirstChildElement("id");
     id = getXMLText (pElement,"id");
     id.erase(0,1);
     int i = 0;
     while (id[i]>47 && id[i]<58)
        ++i;
     id.erase(i);
     SHOW_LOG(id);

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

            initiateGrilleIJ(i,j,p.getGrille()[i][j].getType(),p.getGrille()[i][j].getColor());
        }
    }
     
     //cout<< "lignes" <<endl;
     indicationsLignes.resize(nbLignes);
     for(int i = 0; i<nbLignes; i++) {
        unsigned len = p.getIndicationsLignes()[i].size();
        indicationsLignes[i].resize(len);
        for(unsigned j = 0; j<len; j++) {

            setIndicationsLignesIJ(i,j,p.getIndicationsLignes()[i][j].getType(),p.getIndicationsLignes()[i][j].getColor());
        }
    }
     
     //cout<< "Colonnes" <<endl;
     indicationsColonnes.resize(nbColonnes);
     for(int i = 0; i<nbColonnes; i++) {
        unsigned len = p.getIndicationsColonnes()[i].size();
        indicationsColonnes[i].resize(len);
        for(unsigned j = 0; j<len; j++) {

            setIndicationsColonnesIJ(i,j,p.getIndicationsColonnes()[i][j].getType(),p.getIndicationsColonnes()[i][j].getColor());
        }
    } 
     
    return *this;
}

//getters

int Picross::getNbLignes() const {return nbLignes;}
int Picross::getNbColonnes() const {return nbColonnes;}
const Colors& Picross::getColors() const {return colors;}
const string& Picross::getId() const {return id;}
const string& Picross::getTitle() const {return title;}
const string& Picross::getAuthor() const {return author;}
const string& Picross::getCopyright() const {return copyright;}
const string& Picross::getDescription() const {return description;}
const vector< vector<InfoCase> >& Picross::getGrille() const {return grille;}
const vector< vector<InfoCase> >& Picross::getIndicationsLignes() const {return indicationsLignes;}
const vector< vector<InfoCase> >& Picross::getIndicationsColonnes() const {return indicationsColonnes;}
const OperationsQueue& Picross::getQueue() const {return queue;}

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


void Picross::initiateGrilleIJ(int i, int j) {
    int color = colors.getColorFromName("white");

    grille[i][j].setType(-1);
    grille[i][j].setColor(color);      //on met la couleur par défaut plutôt que -1 sauf que la couleur par défaut n'est pas celle rentrée dans Colors, c'est le blanc
}

void Picross::initiateGrilleIJ(int i, int j, int type, int color) {
    grille[i][j].setType(type);
    grille[i][j].setColor(color);
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
    
    std::tuple<int,int,InfoCase,InfoCase> t;
    int color = colors.getColorFromChar(c);
    
    std::get<0> (t) = i;
    std::get<1> (t) = j;
    std::get<2> (t) = InfoCase(grille[i][j].getType(),grille[i][j].getColor());
    std::get<3> (t) = InfoCase(0,color);

    grille[i][j].setType(0);         //à modifier je pense
    grille[i][j].setColor(color);

    queue.addOp(t);
}

void Picross::setGrilleIJ(int i, int j,int type, char c) {
    
    std::tuple<int,int,InfoCase,InfoCase> t;
    int color = colors.getColorFromChar(c);
    
    std::get<0> (t) = i;
    std::get<1> (t) = j;
    std::get<2> (t) = InfoCase(grille[i][j].getType(),grille[i][j].getColor());
    std::get<3> (t) = InfoCase(type,color);

    grille[i][j].setType(type);         //à modifier je pense
    grille[i][j].setColor(color);

    queue.addOp(t);
}


void Picross::setIndicationsLignesIJ (int i, int j, int type, int color) {
    
    indicationsLignes[i][j].setType(type);
    indicationsLignes[i][j].setColor(color);
}

void Picross::setIndicationsColonnesIJ (int i, int j, int type, int color) {

    indicationsColonnes[i][j].setType(type);
    indicationsColonnes[i][j].setColor(color);
}


//queue

InfoCase Picross::previousOp() {

    if (queue.canWePrevious()) {
        std::tuple<int,int,InfoCase,InfoCase> t = queue.getPrevious();
        InfoCase inf = std::get<2>(t);

        grille[std::get<0>(t)][std::get<1>(t)].setType(inf.getType());
        grille[std::get<0>(t)][std::get<1>(t)].setColor(inf.getColor());
    }

    return inf;
}

InfoCase Picross::forwardOp() {
    if (queue.canWeForward()) {
        std::tuple<int,int,InfoCase,InfoCase> t = queue.getForward();

        InfoCase inf = std::get<3>(t);
        grille[std::get<0>(t)][std::get<1>(t)].setType(inf.getType());
        grille[std::get<0>(t)][std::get<1>(t)].setColor(inf.getColor());
    }

    return inf;
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
            
            this->initiateGrilleIJ(i,j);
        }
    }    

}







//save /load

void Picross::save() const {
    
    string chemin = "./ressources/Grids/Saves/";
    chemin += id;
    chemin += ".dat";

    cout << "chemin:  " << chemin << endl;

    try
    {   
        if (chemin == "")
            throw "Chemin vide !";
        if (chemin.size() > 300)
            throw "Chemin trop long";
        std::ofstream file (chemin.c_str(), std::ios::binary);
        if (!file)
        {
            std::cerr << "\a\n\nImpossible de creer le fichier de sauvegarde\n\n";
            return;
        }
        file.write ((char*) id.c_str(), id.size());
        file.write ("\0", sizeof(char));
        file.write ((char*) title.c_str(), title.size()+1);
        file.write ((char*) author.c_str(), author.size()+1);
        file.write ((char*) copyright.c_str(), copyright.size()+1);
        file.write ((char*) description.c_str(), description.size()+1);
        file.write ((char*) &nbLignes, sizeof (nbLignes));
        file.write ((char*) &nbColonnes, sizeof (nbColonnes));

        int tempInt;

        //save of colors
        tempInt = colors.getNbColors();
        file.write ((char*) &tempInt, sizeof (tempInt));
        file.write ((char*) colors.getDefaultColor().c_str(), colors.getDefaultColor().size()+1);
        tempInt = colors.getColorsList().size();
        file.write ((char*) &tempInt, sizeof (tempInt));   //size of the vect (==nbColors ??)

        for (unsigned i = 0; i<colors.getColorsList().size();i++) {
            file.write ((char*) std::get<0> ( colors.getColorsList()[i]).c_str(), (std::get<0> ( colors.getColorsList()[i]) ).size() +1 );
            tempInt = std::get<1> (colors.getColorsList()[i]);
            file.write ((char*) &tempInt, sizeof (tempInt ) );
            file.write ((char*) &( std::get<2> ( colors.getColorsList()[i]) ), sizeof (std::get<2> ( colors.getColorsList()[i]) ) );
        }

        //save of grille
        for (int i = 0; i<nbLignes; i++) {
            for (int j = 0; j<nbColonnes; j++) {
                tempInt = grille[i][j].getType();
                file.write ((char*) &tempInt, sizeof (tempInt));
                tempInt = grille[i][j].getColor();
                file.write ((char*) &tempInt, sizeof (tempInt));
            }
        }

        //save of indicationsLignes
        for (int i = 0; i<nbLignes; i++) {
            tempInt = indicationsLignes[i].size();
            cout<<"SAVEtempINT : " <<tempInt;
            file.write ((char*) &tempInt, sizeof (tempInt));
            for (unsigned j = 0; j<indicationsLignes[i].size(); j++) {
                tempInt = indicationsLignes[i][j].getType();
                cout<<"  type : " <<tempInt;
                file.write ((char*) &tempInt, sizeof (tempInt));
                tempInt = indicationsLignes[i][j].getColor();
                cout<<"  color : " <<tempInt<<endl;
                file.write ((char*) &tempInt, sizeof (tempInt));
            }
        }

        //save of indicationsColonnes
        for (int i = 0; i<nbColonnes; i++) {
            tempInt = indicationsColonnes[i].size();
            file.write ((char*) &tempInt, sizeof (tempInt));
            for (unsigned j = 0; j<indicationsColonnes[i].size(); j++) {
                tempInt = indicationsColonnes[i][j].getType();
                file.write ((char*) &tempInt, sizeof (tempInt));
                tempInt = indicationsColonnes[i][j].getColor();
                file.write ((char*) &tempInt, sizeof (tempInt));
            }
        }

        //save of queue

        /*save la queue que si sa taille > 0*/

        tempInt = queue.getRealLast();
        file.write ((char*) &tempInt, sizeof (tempInt));
        tempInt = queue.getCurrentLast();
        file.write ((char*) &tempInt, sizeof (tempInt));
        tempInt = queue.getQueue().size();
        file.write ((char*) &tempInt, sizeof (tempInt));

        for (unsigned i = 0; i<queue.getQueue().size(); i++) {
            tempInt = std::get<0> (queue.getQueue()[i]);
            file.write ((char*) &tempInt, sizeof (tempInt));
            tempInt = std::get<1> (queue.getQueue()[i]);
            file.write ((char*) &tempInt, sizeof (tempInt));
            tempInt = std::get<2> (queue.getQueue()[i]).getType();
            file.write ((char*) &tempInt, sizeof (tempInt));
            tempInt = std::get<2> (queue.getQueue()[i]).getColor();
            file.write ((char*) &tempInt, sizeof (tempInt));
            tempInt = std::get<3> (queue.getQueue()[i]).getType();
            file.write ((char*) &tempInt, sizeof (tempInt));
            tempInt = std::get<3> (queue.getQueue()[i]).getColor();
            file.write ((char*) &tempInt, sizeof (tempInt));
        }



        file.close();
    }
    catch (const char *exception)
    {
        std::cerr << "\n*** " << exception << " ***\n";
    }
    catch (...)
    {
        std::cerr << "\n*** Une erreur s'est produite ! ***\n";
    }
}


void Picross::load(const string& idGrid) {
    
    string chemin = "./ressources/Grids/Saves/";
    chemin += idGrid;
    chemin += ".dat";

    try
    {
        if (chemin == "")
            throw "Chemin vide !";
        if (chemin.size() > 300)
            throw "Chemin trop long";
        std::ifstream file(chemin.c_str(), std::ios::binary);
        if (!file)
        {
            std::cerr << "\a\n\nImpossible de lire le fichier de sauvegarde\n\n";
            return;
        }

        //load of attributes from picross
        std::getline(file, id, '\0');
        std::getline(file, title, '\0');
        std::getline(file, author, '\0');
        std::getline(file, copyright, '\0');
        std::getline(file, description, '\0');
        file.read ((char*) &nbLignes, sizeof (nbLignes));
        file.read ((char*) &nbColonnes, sizeof (nbColonnes));

        int tempInt, tempInt2, tempInt3;
        char tempChar;
        string tempString;

        //load of colors
        file.read ((char*) &tempInt, sizeof (tempInt));
        colors.setNbColors(tempInt);

        std::getline(file, tempString, '\0');
        colors.setDefaultColor(tempString);

        file.read ((char*) &tempInt, sizeof (tempInt));
        //colors.getColorsList().resize(tempInt);

        for (int i = 0; i<tempInt;i++) {
            std::getline(file, tempString, '\0');
            file.read ((char*) &tempInt2, sizeof (tempInt2) );
            file.read ((char*) &tempChar, sizeof (tempChar) );
            colors.addColor(tempString,tempInt2,tempChar);
        }

        //load of grille
        grille.resize(nbLignes);
        for (int i = 0; i<nbLignes; i++)
            grille[i].resize(nbColonnes);

        for (int i = 0; i<nbLignes; i++) {
            for (int j = 0; j<nbColonnes; j++) {
                file.read ((char*) &tempInt2, sizeof (tempInt2));
                file.read ((char*) &tempInt3, sizeof (tempInt3));
                initiateGrilleIJ(i,j,tempInt2,tempInt3);             //check
            }
        }

        //load of indicationsLignes
        indicationsLignes.resize(nbLignes);
        for (int i = 0; i<nbLignes; i++) {
            file.read ((char*) &tempInt, sizeof (tempInt));
            indicationsLignes[i].resize(tempInt);
            cout<<"tempINT : " <<tempInt << "\n";
            for (int j = 0; j<tempInt; j++) {
                file.read ((char*) &tempInt2, sizeof (tempInt2));
                file.read ((char*) &tempInt3, sizeof (tempInt3));
                setIndicationsLignesIJ(i,j,tempInt2,tempInt3);
            }
        }

        //load of indicationsColonnes
        indicationsColonnes.resize(nbColonnes);
        for (int i = 0; i<nbColonnes; i++) {
            file.read ((char*) &tempInt, sizeof (tempInt));
            indicationsColonnes[i].resize(tempInt);
            for (int j = 0; j<tempInt; j++) {
                file.read ((char*) &tempInt2, sizeof (tempInt2));
                file.read ((char*) &tempInt3, sizeof (tempInt3));
                setIndicationsColonnesIJ(i,j,tempInt2,tempInt3);
            }
        }

        //load of queue
        file.read ((char*) &tempInt, sizeof (tempInt));
        queue.setRealLast(tempInt);
        file.read ((char*) &tempInt, sizeof (tempInt));
        queue.setCurrentLast(tempInt);
        file.read ((char*) &tempInt, sizeof (tempInt));
        queue.resize(tempInt);

        for (int i = 0; i<tempInt; i++) {
            std::tuple<int,int,InfoCase,InfoCase> t;
            file.read ((char*) &std::get<0>(t), sizeof (tempInt2));
            //std::get<0> (t) = tempInt2;
            file.read ((char*) &std::get<1>(t), sizeof (tempInt2));
            //std::get<1> (t) = tempInt2;
            file.read ((char*) &tempInt2, sizeof (tempInt2));
            std::get<2>(t).setType(tempInt2);
            file.read ((char*) &tempInt2, sizeof (tempInt2));
            std::get<2>(t).setColor(tempInt2);
            file.read ((char*) &tempInt2, sizeof (tempInt2));
            std::get<3>(t).setType(tempInt2);
            file.read ((char*) &tempInt2, sizeof (tempInt2));
            std::get<3>(t).setColor(tempInt2);

            queue.setQueueI(i,t);
        }

        //voir si on peut save une InfoCase d'un coup

        file.close();
    }
    catch (const char *exception)
    {
        std::cerr << "\n*** " << exception << " ***\n";
    }
    catch (...)
    {
        std::cerr << "\n*** Une erreur s'est produite ! ***\n";
    }
}




int Picross::checkFinishedClassicLigne(int i) {
   int res = 1;
    int j = 0;
    int color;
    unsigned idBloc = 0;
    int currentBlocSize = 0;
    int nbCasesTotalActuel = 0, nbCasesTotalReel = 0;

    while (j<nbColonnes && res == 1 /*&& idBloc < (indicationsLignes[i].size() - 1)*/ ) {
        color = indicationsLignes[i][idBloc].getColor();

        if (color == grille[i][j].getColor()) {

            while(j<nbColonnes && (color == grille[i][j].getColor()  )  && (grille[i][j].getType() == 1) ) {
                ++currentBlocSize;
                ++nbCasesTotalActuel;
                ++j;
            }
            if (indicationsLignes[i][idBloc].getType() != currentBlocSize)
                res=0;

            currentBlocSize=0;
            ++idBloc;
        }

        else
            ++j;
    }

    for(unsigned k = 0; k<indicationsLignes[i].size(); k++) {
        nbCasesTotalReel += indicationsLignes[i][k].getType();
    }
    if(nbCasesTotalActuel != nbCasesTotalReel)
        res = 0;

    return res;
}


int Picross::checkFinishedClassicColonne(int j) {
    int res = 1;
    int i = 0;
    int color;
    unsigned idBloc = 0;
    int currentBlocSize = 0;
    int nbCasesTotalActuel = 0, nbCasesTotalReel = 0;

    while (i<nbLignes && res == 1 /*&& idBloc < (indicationsColonnes[j].size() - 1)*/ ) {
        color = indicationsColonnes[j][idBloc].getColor();


        if (color == grille[i][j].getColor()) {

            while(i<nbLignes && (color == grille[i][j].getColor() )  && (grille[i][j].getType() == 1) ) {
                ++currentBlocSize;
                ++nbCasesTotalActuel;
                ++i;
            }
            if (indicationsColonnes[j][idBloc].getType() != currentBlocSize)
                res=0;

            currentBlocSize=0;
            ++idBloc;
        }

        else
            ++i;
    }

    for(unsigned k = 0; k<indicationsColonnes[j].size(); k++) {
        nbCasesTotalReel += indicationsColonnes[j][k].getType();
    }
    if(nbCasesTotalActuel != nbCasesTotalReel)
        res = 0;

    return res;
}


int Picross::checkFinishedClassicGrid() {
    int res = 1;

    int i =0;
    while (i<nbLignes && res == 1) {
        res = checkFinishedClassicLigne(i);
        ++i;
    }
    i=0;
    while (i<nbColonnes && res == 1) {
       res = checkFinishedClassicColonne(i);
       ++i;
    }
   
    return res;
}


void Picross::displayClassic() const {

    //int margin = 10;
    int temp = 0;
    //affichage grille + indications lignes après

    //int max = getMaxSizeIndicationsColonnes();
    //int size = 0;

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
        for (unsigned j = 0; j<indicationsColonnes[i].size(); j++) {

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





std::ostream& operator<< (std::ostream &flux, Picross & p)
{
    try
    {
        flux << "\nid : " << p.getId()
        << "\ntitle : " << p.getTitle()
        << "\nauthor : " << p.getAuthor()
        << "\ncopyright : " << p.getCopyright()
        << "\ndescription : " << p.getDescription()
        << "\nnbLignes : " << p.getNbLignes()
        << "\nnbColonnes : " << p.getNbColonnes()
        <<"\n\nCOLORS" << p.getColors()
        << "\n\nINDICATIONS LIGNES\n";

        for (int i = 0; i<p.getNbLignes(); i++) {
            flux<<"|";
            for (unsigned j = 0; j<p.getIndicationsLignes()[i].size(); j++) {
                flux<< p.getIndicationsLignes()[i][j].getType()<<"|";
            }
            flux<<"\n";
        }
        
        flux<< "\n\nINDICATIONS COLONES\n";

        for (int i = 0; i<p.getNbColonnes(); i++) {
            flux<<"|";
            for (unsigned j = 0; j<p.getIndicationsColonnes()[i].size(); j++) {
                flux<< p.getIndicationsColonnes()[i][j].getType()<<"|";
            }
            cout<<"\n";
        }

        flux<< "\n\nGrille\n";

        for (int i = 0; i<p.getNbLignes(); i++) {
            flux<<"|";
            for(int j = 0; j<p.getNbColonnes(); j++) {
                int color = p.getGrille()[i][j].getColor();
                //cout << "color: " << color <<endl;
                flux << p.getColors().getCharFromColor(color) << "|";
            }
            flux<<"\n";
        }

    }
    catch (...)
    {
        std::cerr << "\n*** Une erreur s'est produite ! ***\n";
    }
    return flux;
}
