#include <iostream>

class Chemin{
  public:

    Chemin(char point_A, char point_B, int poid); //Constructeur
    char point_A;
    char point_B;
    int poid;
};

Chemin::Chemin(char A, char B, int poids){ //Constructeur
  point_A = A;
  point_B = B;
  poid = poids;
}