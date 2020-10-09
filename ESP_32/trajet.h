#include <iostream>
using namespace std;

class Trajet{
  public:

    Trajet(vector<char> parcour, int distances); //Constructeur
    vector<char> parcours;
    int distance;
};

Trajet::Trajet(vector<char> parcour, int distances){ //Constructeur
  parcours = parcour;
  distance = distances;
}