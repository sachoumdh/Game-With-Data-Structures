#include <stdio.h>
#include <stdbool.h>

//record Joueur
typedef struct {
    int num, age;
    char nom[20];
    int score_partie , points_succesifs;
    int nb_victoires, nb_defaites, nb_victoires_succ , nb_defaites_succ;
    bool is_winner;
} Joueur;

//Record Partie
typedef struct {
    int num_partie;
    char nom_J1 , nom_J2 [20];
    int score_J1 , score_J2;
} partie;

//Structure P pour stocker les parties 
typedef struct {
    partie round;
    struct P *Next ;
} P;

//Node est un element de file et LG , LP
typedef struct {
    Joueur J;
    struct Node *Jnext;
} Node;
//structure File
typedef struct {
    Node *Head;
    Node *Tail;
} File;

//List
typedef struct {
    Node *L;
}Liste;

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    return 0;
}
