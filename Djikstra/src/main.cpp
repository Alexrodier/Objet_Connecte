#include <Arduino.h>
#include <iostream>
#include "chemin.h"
#include "donnee.h"
#include "trajet.h"
#include "direction.h"

using namespace std;

#define INF 100
#define ALREADY_USE -1

bool uniques = true;

vector<char> PARCOURS = {'A', 'C', 'D'};

Chemin plan[12] = {Chemin('S', '6', 5), Chemin('6', '7', 2),
                   Chemin('7', 'A', 3), Chemin('7', '8', 3),
                   Chemin('8', 'B', 3), Chemin('8', '9', 4),
                   Chemin('8', '0', 4), Chemin('6', '0', 1),
                   Chemin('7', '0', 2), Chemin('9', 'C', 2),
                   Chemin('0', '9', 3), Chemin('0', 'D', 3)};

Direction liste_direction[12] = {Direction('S', '6', '7', 'G'), Direction('S', '6', '0', 'T'),
                                 Direction('6', '7', 'A', 'T'), Direction('6', '7', '8', 'D'),
                                 Direction('6', '10', '8', 'G'), Direction('6', '10', '9', 'T'),
                                 Direction('6', '10', 'D', 'D'), Direction('S', '6', '5', 'G'),
                                 Direction('S', '6', 'D', 'G'), Direction('S', '6', '5', 'G')}

vector<vector<Donnee>> liste = {{Donnee('S', INF), Donnee('A', INF),
                        Donnee('B', INF), Donnee('C', INF),
                        Donnee('D', INF), Donnee('6', INF),
                        Donnee('7', INF), Donnee('8', INF),
                        Donnee('9', INF), Donnee('0', INF)}};

void init_liste(){
  liste = {{Donnee('S', INF), Donnee('A', INF),
            Donnee('B', INF), Donnee('C', INF),
            Donnee('D', INF), Donnee('6', INF),
            Donnee('7', INF), Donnee('8', INF),
            Donnee('9', INF), Donnee('0', INF)}};
}  

void afficher_liste(){
  for(int k=0;k<liste.size();k++){
    for(Donnee donnee : liste[k]){
      cout << donnee.from << " " << donnee.distance << " ";
      if(donnee.distance < 100){
        cout << " ";
        if(donnee.distance > 0){
          cout << " ";
        }
      }
    }
    cout << endl;
  }
  cout << endl;
}

void afficher_trajet(Trajet trajet){
  for(char point : trajet.parcours){
    cout << "-> " << point;
  }
  cout << endl;
  cout << "Distance : " << trajet.distance << endl;
}

int chercher_distance(char point_A, char point_B){
  int distance = INF;

  for(Chemin chemin : plan){
    if((chemin.point_A == point_A && chemin.point_B == point_B) || (chemin.point_A == point_B && chemin.point_B == point_A)){
      distance = chemin.poid;
    }
  }
  return distance;
}

Donnee new_position(){
  Donnee minimum = Donnee(' ', INF);
  int derniere_ligne = liste.size()-1;

  for(int k=0;k<liste[derniere_ligne].size();k++){
    if(liste[derniere_ligne][k].distance < minimum.distance && liste[derniere_ligne][k].distance > 0){
      minimum.distance = liste[derniere_ligne][k].distance;
      minimum.from = liste[0][k].from;
    }
  }
  return minimum;
}

bool it_is_smaller_distance(int new_distance, int k){
  for(int n=0;n<liste.size();n++){
    if(liste[n][k].distance < new_distance && liste[n][k].distance > 0){
      return false;
    }
  }
  return true;
}

bool has_a_distance(int k){
  for(int n=0;n<liste.size();n++){
    if(liste[n][k].distance != INF && liste[n][k].distance > 0){
      return true;
    }
  }
  return false;
}

Donnee calculer_chemin(char depart, char arrivee){
  init_liste();
  Donnee position = Donnee(depart, 0);
  vector<Donnee> new_ligne = {};
  int derniere_ligne = liste.size()-1;
  int new_distance;

  while(position.from != arrivee){
    for(int k=0;k<liste[derniere_ligne].size();k++){
      if(liste[0][k].from == position.from || liste[derniere_ligne][k].from == '~'){
        new_ligne.insert(new_ligne.end(), Donnee('~', ALREADY_USE));
      }else{
        new_distance = chercher_distance(position.from, liste[0][k].from);
        if(new_distance != INF){
          new_distance += position.distance;
          if(it_is_smaller_distance(new_distance, k)){
            new_ligne.insert(new_ligne.end(), Donnee(position.from, new_distance));
          }else{
            new_ligne.insert(new_ligne.end(), liste[derniere_ligne][k]);
          }
        }else{
          if(has_a_distance(k)){
            new_ligne.insert(new_ligne.end(), liste[derniere_ligne][k]);
          }else{
            new_ligne.insert(new_ligne.end(), Donnee('I', INF));
          }
        }
      }
    }
    liste.insert(liste.end(), new_ligne);
    derniere_ligne = liste.size()-1;
    new_ligne = {};
    position = new_position();
  }

  return position;
}

vector<char> retrouver_chemin(char position, char depart){
  vector<char> parcours = {};
  int n;
  int k = 0;
  parcours.insert(parcours.begin(), position);

  while(position != depart){
    for(int m=0;m<liste[0].size();m++){
      if(liste[0][m].from == position){
        k = m;
      }
    }
    n = liste.size()-1;
    while(liste[n][k].from == '~'){
      n--;
    }
    position = liste[n][k].from;
    parcours.insert(parcours.begin(), position);
  }

  return parcours;
}

Trajet djikstra(char depart, char arrivee){  
  Donnee position = Donnee(' ', INF);
  vector<char> parcours = {};

  position = calculer_chemin(depart, arrivee);
  //afficher_liste();
  parcours = retrouver_chemin(position.from, depart);

  Trajet trajet = Trajet(parcours, position.distance);
  return trajet;
}

Trajet compacter_trajet_complet(vector<Trajet> trajet_complet){
  Trajet trajet_final = Trajet({}, 0);

  for(Trajet sous_trajet : trajet_complet){
    for(int n=0;n<sous_trajet.parcours.size()-1;n++){
      trajet_final.parcours.insert(trajet_final.parcours.end(), sous_trajet.parcours[n]);
      trajet_final.distance += sous_trajet.distance;
    }
  }
  trajet_final.parcours.insert(trajet_final.parcours.end(), 'S');

  return trajet_final;
}

void trouver_trajet(vector<char> parcours){
  char position = 'S';
  vector<Trajet> liste_trajet = {};
  vector<Trajet> trajet_complet = {};
  Trajet trajet_mini = Trajet({}, INF);
  Trajet trajet = Trajet({}, INF);
  Trajet trajet_final = Trajet({}, INF);

  while(parcours.size()){
    for(char passage : parcours){ //On cherche un chemin pour tout les points
      trajet = djikstra(position, passage);
      liste_trajet.insert(liste_trajet.end(), trajet);
    }
    trajet_mini = Trajet({}, INF);
    for(Trajet trajet : liste_trajet){ //On cherche le point le plus proche
      if(trajet.distance < trajet_mini.distance){
        trajet_mini = trajet;
      }
    }
    trajet_complet.insert(trajet_complet.end(), trajet_mini);
    liste_trajet = {};
    parcours.erase(find(parcours.begin(), parcours.end(), trajet_mini.parcours[trajet_mini.parcours.size()-1]));

    position = trajet_mini.parcours[trajet_mini.parcours.size()-1];
  }

  trajet = djikstra(position, 'S'); //On retourne au Start
  trajet_complet.insert(trajet_complet.end(), trajet);

  trajet_final = compacter_trajet_complet(trajet_complet);
  cout << "Trajet final : ";
  afficher_trajet(trajet_final);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(uniques == true){
    trouver_trajet(PARCOURS);
  }
  uniques = false;
}