#include <iostream>
#include "chemin.h"
#include "donnee.h"
#include "trajet.h"
#include "intersection.h"

using namespace std;

#define INF 100
#define ALREADY_USE -1

bool uniques = true;

Chemin plan[14] = {Chemin('S', '1', 2), Chemin('1', '4', 5),
                   Chemin('1', '2', 4), Chemin('1', '5', 6),
                   Chemin('2', 'C', 2), Chemin('2', '4', 4),
                   Chemin('2', '3', 4), Chemin('3', 'B', 2),
                   Chemin('4', '3', 5), Chemin('3', '6', 5), 
                   Chemin('4', '6', 4), Chemin('6', 'A', 2),
                   Chemin('6', '5', 4), Chemin('5', 'D', 2)};

Intersection liste_intersection[11] = {Intersection('S', 'R', 'R', 'R', '1'), Intersection('3', '4', '2', 'B', '6'),
                                      Intersection('1', '5', 'S', '2', '4'), Intersection('B', '3', 'R', 'R', 'R'),
                                      Intersection('2', '4', '1', 'C', '3'), Intersection('6', '5', '4', '3', 'A'),
                                      Intersection('C', '2', 'R', 'R', 'R'), Intersection('A', 'R', '6', 'R', 'R'),
                                      Intersection('4', '1', '2', '3', '6'), Intersection('5', 'D', '1', '6', 'R'), 
                                      Intersection('D', 'R', 'R', '5', 'R')};

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

void afficher_ordre(vector<char> ordres){
  for(char ordre : ordres){
    cout << "-> " << ordre;
  }
  cout << endl;
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

Trajet trouver_trajet(vector<char> parcours){
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

  return trajet_final;
}

char trouver_direction(char from, char inter, char to){
  Intersection intersection = Intersection(' ', ' ', ' ', ' ', ' ');
  vector<char> direction = {'B', 'R', 'F', 'L'};
  int pos = 0;

  for(Intersection intersect : liste_intersection){  //On cherche la bonne intersection
    if(intersect.intersection == inter){
      intersection = intersect;
    }
  }
  for(int k=0;k<4;k++){
    if(intersection.way[k] == from){
      pos = k;
    }
  }
  for(int k=0;k<4;k++){
    if(intersection.way[(pos+k)%4] == to){
      return direction[k];
    }
  }

  return 'E'; //Useless
}

vector<char> trouver_ordre(Trajet trajet){
  vector<char> ordre = {'G'};

  for(int k=0;k<trajet.parcours.size()-2;k++){
    ordre.insert(ordre.end(), trouver_direction(trajet.parcours[k], trajet.parcours[k+1], trajet.parcours[k+2]));
    ordre.insert(ordre.end(), 'G');
  }
  ordre.insert(ordre.end(), 'B');
  ordre.insert(ordre.end(), 'S');

  return ordre;
}

Trajet trajet = Trajet({}, 0);
vector<char> ordres;

vector<char> get_order(vector<char> parcours) {
  trajet = trouver_trajet(parcours);
  //cout << "Trajet final : ";
  afficher_trajet(trajet);

  ordres = trouver_ordre(trajet);
  //cout << "Ordres : ";
  afficher_ordre(ordres);
  return ordres;
}
