#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <vector>
#include <string>

#include "Picross.hpp"

TEST_CASE("Picross Tests : reading the grid"){
    SECTION("Normal behavior"){
	std::vector<std::string> v = {"5435.xml","30511.xml"};
	struct{
	    std::string author;
	    std::string title;
	    int size[2];
	} grid[] = {
	    {"mishele","you can take notes...",{20,20}},
	    {"sanane samanye","I am further away", {5,5}}
	};

	for(unsigned i = 0; i < v.size(); ++i){
	    Picross p("ressources/" + v[i]);

	    REQUIRE(p.getGrille().size() == grid[i].size[0]);
	    for(unsigned j = 0; j < p.getGrille().size(); ++j){
		REQUIRE(p.getGrille()[j].size() == grid[i].size[1]);
	    }
	    REQUIRE(p.getAuthor() == grid[i].author);
	    REQUIRE(p.getTitle() == grid[i].title);
	}
    }

    SECTION("Hazardous behavior"){
	//Picross p("ibeiufzeiu");
    }
}
