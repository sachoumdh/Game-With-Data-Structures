#include <stdio.h>

typedef struct {
    int num, age;
    char nom[20];
    int nb_victoires, nb_defaites;
} Joueur;

typedef struct {
    int num_partie;
    char nom_J1 , nom_J2 [20];
    int score_J1 , score_J2;
}partie;
typedef struct {
    partie round;
    struct P *Next ;
} P;

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    return 0;
}
