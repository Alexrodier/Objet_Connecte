#include <iostream>
using namespace std;

class Intersection{
  public:

    Intersection(char intersection, char l, char d, char r, char u, unsigned long uid); //Constructeur
    char intersection;
    vector<char> way;
    unsigned long uid;
};

Intersection::Intersection(char intersections, char l, char d, char r, char u, unsigned long uids){ //Constructeur
  intersection = intersections;
  way = {l, d, r, u};
  uid = uids;
}

char rechercher_Intersection(unsigned long current_uid, vector<Intersection> liste_intersection){
  for(Intersection inter : liste_intersection){
    if (current_uid == inter.uid){
      return inter.intersection;
    }
  }
}
