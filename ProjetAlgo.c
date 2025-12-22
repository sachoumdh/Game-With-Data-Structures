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

// ===== Primitive 3: Adding an element to the queue (at the end) ====
void EnQueue (Queue *F , Player P1)
{
    // Create a new node
    Node *newP = malloc(sizeof (Node));
     if(newP == NULL)
    {
        printf("Memory allocation error\n");
        return;
    }

    // Put the player's information in the node
    newP->P.num = P1.num;
    strcpy(newP->P.name , P1.name);
    newP->P.age = P1.age;
    newP->P.game_score = P1.game_score;
    newP->P.score = P1.score;
    newP->P.succesifs_points = P1.succesifs_points;
    newP->P.nb_wins = P1.nb_wins;
    newP->P.nb_losses = P1.nb_losses;
    newP->P.nb_succ_wins = P1.nb_succ_wins;
    newP->P.nb_succ_losses = P1.nb_succ_losses;
    // not sure!!!!
    newP->P.is_winner = P1.is_winner;

    // The new node becomes the tail
    newP->Pnext = NULL;

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
bool DeQueue (Queue *F , Player *P)
{
    if(F->Head == NULL) // Empty queue
    {
        printf("Error: queue is empty\n");
        return false;
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

    return true;
}

// ===== Primitive 5: Display the queue ====
void Display_Queue (Queue F)
{
    Node *temp;

    if(EmptyQueue(F))
    {
        printf("The queue is empty \n");
        return;
    }

    // temp is used to browse the queue without modifying and losing the head
    temp = F.Head;

    while(temp != NULL)
    {
        printf("Player: %s\n", temp->P.name);
        temp = temp->Pnext; // move to the next element
    }
}

//*******************  MAIN  *****************//
int main(int argc, const char * argv[])
{
    Queue F;
    Player P;

    // Initialization of the queue F
    Init_Queue(&F);

    // Display queue test (empty queue)
    Display_Queue(F);

    // Create players
    Player P1 = {01, 12, "Sarah", 0, 0, 0, 0, 0, 0, 0, false};
    Player P2 = {02, 18, "Lyna", 0, 1, 9, 0, 8, 0, 0, true};
    Player P3 = {03, 20, "You", 1, 2, 3, 0, 0, 0, 0, false};
    Player P4 = {04, 16, "Me", 0, 1, 0, 0, 8, 7, 6, true};

    // Test EnQueue
    EnQueue(&F, P1);
    EnQueue(&F, P2);
    EnQueue(&F, P3);
    EnQueue(&F, P4);

    // Display queue (players)
    Display_Queue(F);

    // Test DeQueue
    printf("=== DeQueue ===\n");
    while (DeQueue(&F, &P))
    {
        printf("The player removed is:%s\n", P.name);
        printf("Queue state: \n");
        Display_Queue(F); // Display queue state
    }

    return 0;
}
