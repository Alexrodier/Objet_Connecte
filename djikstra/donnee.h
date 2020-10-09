#include <iostream>

class Donnee{
  public:

    Donnee(char point_A, int distances); //Constructeur
    char from;
    int distance;
};

Donnee::Donnee(char point_A, int distances){ //Constructeur
  from = point_A;
  distance = distances;
}