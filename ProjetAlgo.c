#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//*******************  THE STRUCTURES  *****************//

typedef struct Player{
    int num, age;
    char name[20];
    int game_score , score , succesifs_points;
    int nb_wins, nb_losses, nb_succ_wins , nb_succ_losses;
    bool is_winner;
    struct Player *next;
} Player;

typedef struct Game{
    int num_game;
    char name_P1 , name_P2 [20];
    int score_P1 , score_P2;
} Game;

//Structure P to store the round
typedef struct G{
    Game round;
    struct G *Next ;
} G;

//Node is a queue and LG, LP element
typedef struct Node{
    Player P;
    struct Node *Pnext;
} Node;

typedef struct Queue{
    Node *Head;
    Node *Tail;
} Queue;

typedef struct List {
    Node *L;
}List;


//*******************  THE PRIMITIVES  *****************//

// ===== Primitive 1: Initialization of the queue ====
void Init_Queue (Queue *F)
{
    F->Head = NULL;
    F->Tail = NULL;
}

// ===== Primitive 2: Empty queue ====
bool EmptyQueue (Queue F)
{
    if(F.Head == NULL)
    {
        return true;
    }
    else
    {
        return false;
    }

}

// ===== Primitive 3: Adding an element to the queue ====
void EnQueue (Queue *F , Player P)
{
    Player *newP = malloc(sizeof (Player));
    
     if(newP == NULL)
    {
        printf("Memory allocation error\n");
        return;
    }

    newP->num = P.num;
    strcpy(newP->name , P.name);
    newP->age = P.age;
    newP->game_score = P.game_score;
    newP->score = P.score;
    newP->succesifs_points = P.succesifs_points;
    newP->nb_wins = P.nb_wins;
    newP->nb_losses = P.nb_losses;
    newP->nb_succ_wins = P.nb_succ_wins;
    newP->nb_succ_losses = P.nb_succ_losses;
    // not sure!!!!
    newP->is_winner = P.is_winner;

    newP->next = NULL;

    if(F->Tail == NULL) // Empty queue
    {
        F->Head = newP;
        F->Tail = newP;
    }
    else
    {
        F->Tail->Pnext = newP;
        F->Tail = newP;
    }
}

// ===== Primitive 4: Delete an element from the queue ====
void DeQueue (Queue *F , Player *P)
{
    if(F->Head == NULL) // Empty queue
    {
        printf("Error: queue is empty\n");
        return;
    }

    Node *temp = F->Head; // temp points to the first node in the queue

    // Copy the fields from the first node into temp
    P->num = temp->P.num;
    strcpy(P->name , temp->P.name);
    P->age = temp->P.age;
    P->game_score = temp->P.game_score;
    P->score = temp->P.score;
    P->succesifs_points = temp->P.succesifs_points;
    P->nb_wins = temp->P.nb_wins;
    P->nb_losses = temp->P.nb_losses;
    P->nb_succ_wins = temp->P.nb_succ_wins;
    P->nb_succ_losses = temp->P.nb_succ_losses;
    // not sure!!!!
    P->is_winner = temp->P.is_winner;

    F->Head = temp->Pnext; // Move the head

    // The queue contained only one element and became empty
    if (F->Head == NULL)
    {
        F->Tail = NULL;
    }

    free(temp);

}

// ===== Primitive 5: Display the queue ====


// main programme
int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    return 0;
}
