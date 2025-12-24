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
    char name_P1 [20];  
    char name_P2 [20];
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

typedef struct GameState{
    Queue F , F1 , F3;
    List LG , LP;
    G *GameHistory;  //List of all the game rounds
    Player *currWinner; // the player that won but hasn't moved to F1
    int totalGames;  // Number of game rounds
    int Strategy;    //Strategy 1 or 2
}GameState;

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
        // The display is as follows: [ num_Player | name_Player ]
        printf("[ %d | %s ] -> ",temp->P.num, temp->P.name);
        temp = temp->Pnext; // move to the next element
    }

    // Print NULL at the end of the queue
    printf("NULL\n");
}

//*********** Game Logic Functions *************//

// Helping function that checks if a Queue has two players
bool HasTwoPlayers(Queue F) {
    if (EmptyQueue(F)) return false;
    return (F.Head->Pnext != NULL);
}

//function that returns whether there are two players available and chooses them according to Queue priorities
bool Priority(GameState *g , Player *P1 , Player *P2){
    
    //First Check if we have a current winner
    if (g->currWinner != NULL) {
            // 1. curr Winner + F1 player
            if (!EmptyQueue(g->F1)) {
                *P1 = *(g->currWinner);
                DeQueue(&g->F1, P2);
                free(g->currWinner); // free the  currWinner memory
                g->currWinner = NULL; // set currWinner pointer to NULL
                return true;
            }
            
            // 2.  curr Winner + F Player
            if (!EmptyQueue(g->F)) {
                *P1 = *(g->currWinner);
                DeQueue(&g->F, P2);
                free(g->currWinner);
                g->currWinner = NULL;
                return true;
            }
        
            // 3. curr Winner + F3 Player
            if (!EmptyQueue(g->F3)) {
                *P1 = *(g->currWinner);
                DeQueue(&g->F3, P2);
                free(g->currWinner);
                g->currWinner = NULL;
                return true;
            }
        }
        
        // No lastWinner check other combinations
        
    // 4. two F1 players
    if (HasTwoPlayers(g->F1)) {
        DeQueue(&g->F1, P1);
        DeQueue(&g->F1, P2);
        return true;
    }

    //Priority to F1
    if (!EmptyQueue(g->F1))  {
        
        // 5. F1 player + F player
        if (!EmptyQueue(g->F)) {
            DeQueue(&g->F1, P1);
            DeQueue(&g->F, P2);
            return true;
        }
        // 6. F1 player + F3 player
        if (EmptyQueue(g->F3)) {
            DeQueue(&g->F1, P1);
            DeQueue(&g->F3, P2);
            return true;
        }
    }
    
    // 7. two F players
    if (HasTwoPlayers(g->F)) {
        DeQueue(&g->F, P1);
        DeQueue(&g->F, P2);
        return true;
    }
        
    // 8. F player + F3 player
    if (!EmptyQueue(g->F) && !EmptyQueue(g->F3)) {
        DeQueue(&g->F, P1);
        DeQueue(&g->F3, P2);
        return true;
    }
    
    // 9. two F3 players
    if (HasTwoPlayers(g->F3)) {
        DeQueue(&g->F3, P1);
        DeQueue(&g->F3, P2);
        return true;
    }
    
    //No players available
    printf("No players Available .\n");
    return false;
}

int PlayTurn(int PlayerID , char *PlayerName){
    int r = rand() % 1000000; // generate random number from 0 to 999999
    printf("You have generated the value : %d ." , r);
    // compute the sum of digits
    int n = r ;
    int sum = 0;
    while (n>0){
        sum += n % 10;
        n = n/10;
    }
    //check if sum of digits is multiple of 5
    int point;
    if (sum % 5 == 0){
         point = 1;
        printf("The player %d , %s scored a point ! \n" , PlayerID , PlayerName);
    } else{
        point = 0;
        printf("The player %d , %s scored no point :(  \n" , PlayerID , PlayerName);
    }
    return point;
}

//Function that runs a game round and stores the returned game
Game PlayGame( GameState *g , Player P1 , Player P2){
    //Start a game round
    Game game;
    game.num_game = ++g->totalGames;
    //store the Players's names
    strcpy(game.name_P1, P1.name);
    strcpy(game.name_P2, P2.name);
    //initialise the players's scores in the game to 0
    game.score_P1 = 0;
    game.score_P2 = 0;
    
    printf("\nStart of Game %d : Player %d : %s VS Player %d :%s \n", game.num_game,P1.num, P1.name , P2.num , P2.name);
    
    int turn = 1;
    while (turn<= 12 && abs(game.score_P1 - game.score_P2) < 3) {
        printf ("Beggining of round %d : \n " , turn);
        // Player 1’s turn
        printf("%s’s turn: " , P1.name);
        game.score_P1 += PlayTurn(P1.num, P1.name);
                
        printf("Current: %s %d - %d %s\n ", P1.name, game.score_P1, game.score_P2, P2.name);
                
        // Check if game should end
        if (abs(game.score_P1 - game.score_P2) >= 3) {
            printf("Yay ! Gap of 3 points reached! Game ends.\n ");
            break;
        }
        // Player 2’s turn (if not last turn)
        if (turn < 12) {
            printf(" \n%s’s turn: " , P2.name);
            game.score_P2 += PlayTurn(P2.num, P2.name);
            printf(" Current: %s %d - %d %s\n ",P1.name, game.score_P1, game.score_P2, P2.name);
            
            // Check if game should end
            if (abs(game.score_P1 - game.score_P2) >= 3) {
                printf(" 🎯 Gap of 3 points reached! Game ends.\n ");
                break;
            }
        }
        turn++;
            }
        printf("Game Results !! \n ");
            if (game.score_P1 > game.score_P2) {
                printf(" THE WINNER IS : %s (P%d)\n ", P1.name, P1.num);
                printf(" Score: %s %d - %d %s\n ",P1.name, game.score_P1, game.score_P2, P2.name);
            } else if (game.score_P2 > game.score_P1) {
                printf(" THE WINNER IS : %s (P%d)\n ", P2.name, P2.num);
                printf(" Score: %s %d - %d %s\n ",P2.name, game.score_P2, game.score_P1, P1.name);
            } else {
                printf (" OH IT'S A TIE !! \n");
                printf(" Score: %s %d - %d %s\n ",P2.name, game.score_P2, game.score_P1, P1.name);
                printf("Congratulations to both !");
            }
    return game;
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
