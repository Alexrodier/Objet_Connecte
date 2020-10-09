#include <iostream>
using namespace std;

class Intersection{
  public:

    Intersection(char intersection, char l, char d, char r, char u); //Constructeur
    char intersection;
    vector<char> way;
};

Intersection::Intersection(char intersections, char l, char d, char r, char u){ //Constructeur
  intersection = intersections;
  way = {l, d, r, u};
}