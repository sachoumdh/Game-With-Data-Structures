#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
 

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
    int totalPlayers; //total Players of the game
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

//***********Game Logic Functions*************//
void ADDtoLG(List *LG , Player player){
    //Allocate new node and fill it with player
    Node *new = malloc(sizeof(Node));
    new->P = player;
    
    //since LG is ordered
    //if LG's Head = NIL or score is superior than the first element's score : new becomes head of list
    if(LG->L == NULL || player.succesifs_points > LG->L->P.succesifs_points){
        new->Pnext = LG->L;
        LG->L = new;
    } else{
        //we insert player in its order
        Node *Q = LG->L;
        while(Q->Pnext != NULL && Q->Pnext->P.succesifs_points >= player.succesifs_points){
            //move pointer Q to apropriate order
            Q = Q->Pnext;
        }
        new->Pnext = Q->Pnext;
        Q->Pnext = new;
    }
}

void ADDtoLP(List *LP , Player player){
    //since LP is not ordered we add element at the head and it becomes the head
    Node *new = malloc(sizeof(Node));
    new->P = player;
    new->Pnext = LP->L;
    LP->L = new;
}

// Helping function that checks if a Queue has two players
bool HasTwoPlayers(Queue F) {
    if (EmptyQueue(F)) return false;
    return (F.Head->Pnext != NULL);
}

//function that returns whether there are two players available and chooses them according to Queue priorities
bool Priority(GameState *g , Player *P1 , Player *P2){
    
    //First Check if we have a current winner
    if (g->currWinner != NULL) {
        *P1 = (*g->currWinner); // we copy currwinner Data
            // 1. curr Winner + F1 player
            if (!EmptyQueue(g->F1)) {
                DeQueue(&g->F1, P2);
                printf("\nPlayers chosen : Current Winner + F1 Player");
                return true;
            }
            
            // 2.  curr Winner + F Player
            if (!EmptyQueue(g->F)) {
                DeQueue(&g->F, P2);
                printf("\nPlayers chosen : Current Winner + F Player");
                return true;
            }
        
            // 3. curr Winner + F3 Player
            if (!EmptyQueue(g->F3)) {
                DeQueue(&g->F, P2);
                printf("\nPlayers chosen : Current Winner + F3 Player");
                return true;
            }
        // if no player with current winner , game cannot proceed
        printf("\nPairing with current winner impossible .");
        return false;
        }
        
    // No current winner , we check other combinations
        
    // 4. two F1 players
    if (HasTwoPlayers(g->F1)) {
        DeQueue(&g->F1, P1);
        DeQueue(&g->F1, P2);
        printf("\nPlayers chosen : F1 player + F1 Player");
        return true;
    }

    //Priority to F1
    if (!EmptyQueue(g->F1))  {
        
        // 5. F1 player + F player
        if (!EmptyQueue(g->F)) {
            DeQueue(&g->F1, P1);
            DeQueue(&g->F, P2);
            printf("\nPlayers chosen : F1 player + F Player");
            return true;
        }
        // 6. F1 player + F3 player
        if (EmptyQueue(g->F3)) {
            DeQueue(&g->F1, P1);
            DeQueue(&g->F3, P2);
            printf("\nPlayers chosen : F1 player + F3 Player");
            return true;
        }
    }
    
    // 7. two F players
    if (HasTwoPlayers(g->F)) {
        DeQueue(&g->F, P1);
        DeQueue(&g->F, P2);
        printf("\nPlayers chosen : F player + F Player");
        return true;
    }
        
    // 8. F player + F3 player
    if (!EmptyQueue(g->F) && !EmptyQueue(g->F3)) {
        DeQueue(&g->F, P1);
        DeQueue(&g->F3, P2);
        printf("\nPlayers chosen : F player + F3 Player");
        return true;
    }
    
    // 9. two F3 players
    if (HasTwoPlayers(g->F3)) {
        DeQueue(&g->F3, P1);
        DeQueue(&g->F3, P2);
        printf("\nPlayers chosen : F3 player + F3 Player");
        return true;
    }
    
    //No players available
    printf("\nNo players Available .\n");
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

//*******************  LOGIC GAME 2ND PART *****************//

// Recursive function that calculates the GCD of two numbers
int GCD (int a , int b)
{
    if (b == 0)
    {
       return a;
    }
    else
    {
        return GCD(b, a%b);
    }
}


// Function that checks if the number contains a digit
bool number_contains_digit (int number, int digit)
{
    while (number > 0)
    {
        if ( (number % 10) == digit )
        {
            return true;
        }

        number = number / 10;
    }

    return false;
}


// Function to start the second strategy
int PlayTurn2(int PlayerID , char *PlayerName)
{
    int n;
    int digit;
    int point;
    bool find = false;

    int a = rand() % 1000000; // generate 1st random number from 0 to 999999
    int b = rand() % 1000000; // generate 2nd random number from 0 to 999999

    printf("You have generated the values: %d and %d .\n", a, b);

    // compute the GCD of a and b
    n = GCD(a, b);

    // check if the GCD contains at least one digit that belongs to one of the numbers a or b
    while ( (n>0) && (find == false) )
    {
        digit = n % 10;

        if ( (number_contains_digit(a, digit)) || (number_contains_digit(b, digit)) )
        {
            find = true;
        }

        n = n / 10;
    }

    if (find == true)
    {
        point = 1;
        printf("The player %d , %s scored a point ! \n" , PlayerID , PlayerName);
    }
    else
    {
        point = 0;
        printf("The player %d , %s scored no point :(  \n" , PlayerID , PlayerName);
    }

    return point;
}

// Function to Display the start date and end date of the game
void Display_Time ()
{
    time_t t = time(NULL);

    // this function return different type of struct called tm
    // this tm stores data about the calendar date

    struct tm date = *localtime(&t);
    printf("on: %d-%02d-%02d at: %02dh:%02dmin:%02dsec\n", date.tm_year + 1900, date.tm_mon + 1, date.tm_mday, date.tm_hour, date.tm_min, date.tm_sec);
}

Game GameRound( GameState *g , Player P1 , Player P2){
    //Start a game round
    Game game;
    game.num_game = ++g->totalGames;
    //store the Players's names
    strcpy(game.name_P1, P1.name);
    strcpy(game.name_P2, P2.name);
    //initialise the players's scores in the game to 0
    game.score_P1 = 0;
    game.score_P2 = 0;
    P1.game_score = 0;
    P2.game_score = 0;
    
    
    printf("\nStart of Game %d : Player %d : %s VS Player %d :%s \n", game.num_game,P1.num, P1.name , P2.num , P2.name);
    
    int turn = 1;
    int maxturns ;
    //define maxturns depending on game strategy chosen
    if (g->Strategy == 1){
        maxturns = 12;
    } else{
        maxturns = 16;
        printf("\nStarting time is :");
        Display_Time();
    }
    
    while (turn<= maxturns && abs(game.score_P1 - game.score_P2) < 3) {
        printf ("\nBeggining of round %d : \n " , turn);
        // Player 1’s turn
        printf("%s’s turn: " , P1.name);
        if ( g->Strategy == 1){
            game.score_P1 += PlayTurn(P1.num, P1.name);
        } else {
            game.score_P1 += PlayTurn2(P1.num, P1.name);
        }
                
        printf("Current: %s %d - %d %s\n ", P1.name, game.score_P1, game.score_P2, P2.name);
                
        // Check if game should end
        if (abs(game.score_P1 - game.score_P2) >= 3) {
            printf("Yay ! Gap of 3 points reached! Game ends.\n ");
            break;
        }
        // Player 2’s turn (if not last turn)
        if (turn < maxturns) {
            printf("%s’s turn: " , P2.name);
            if (g->Strategy == 1){
                game.score_P2 += PlayTurn(P2.num, P2.name);
            } else{
                game.score_P2 += PlayTurn2(P2.num, P2.name);
            }
            printf("Current: %s %d - %d %s\n ",P1.name, game.score_P1, game.score_P2, P2.name);
            
            // Check if game should end
            if (abs(game.score_P1 - game.score_P2) >= 3) {
                printf("Gap of 3 points reached! Game ends.\n ");
                break;
            }
        }
        turn++;
    }
    P1.game_score = game.score_P1;
    P2.game_score = game.score_P2;
        printf("Game Results !! \n ");
            if (game.score_P1 > game.score_P2) {
                printf("\n THE WINNER IS : %s (P%d)\n ", P1.name, P1.num);
                printf("\n Score: %s %d - %d %s\n ",P1.name, game.score_P1, game.score_P2, P2.name);
            } else if (game.score_P2 > game.score_P1) {
                printf("\n THE WINNER IS : %s (P%d)\n ", P2.name, P2.num);
                printf("\n Score: %s %d - %d %s\n ",P2.name, game.score_P2, game.score_P1, P1.name);
            } else {
                printf ("\n OH IT'S A TIE !! \n");
                printf("\n Score: %s %d - %d %s\n ",P2.name, game.score_P2, game.score_P1, P1.name);
                printf("\nCongratulations to both !");
            }
    if (g->Strategy == 2){
        printf("\nEnding time is : ");
        Display_Time();
    }
    return game;
}

void Update_P_Stats(Player *winner , Player *loser){
    //Updates the stats of the players at end of game round
    winner->nb_wins++;
    winner->nb_succ_wins++;
    winner->nb_succ_losses = 0;
    winner->succesifs_points= winner->succesifs_points + winner->game_score;
    
    loser->nb_losses++;
    loser->nb_succ_losses++;
    loser->nb_succ_wins = 0;
    loser->succesifs_points = 0;
}

void MovePlayer(GameState *g , Player *P){
    
    //First check if the player is actuallythe current winner
    bool isCurrWinner = (g->currWinner != NULL && g->currWinner->num == P->num);
    
    if (P->is_winner){
        if(g->Strategy == 1){ //Strategy 1 rules
            //we check if Player has 5 or more total wins
            if (P->nb_wins >= 5){
                //Player goes to LG
                printf("Yahoo ! Winner %d , %s is a definitive winner ! -> Goes to Winners List and exempt from playing .\n", P->num,P->name);
                if (isCurrWinner){
                    ADDtoLG(&g->LG, *(g->currWinner));
                    free(g->currWinner);
                    g->currWinner = NULL;
                }else{ //we create a copy in LG
                    ADDtoLG(&g->LG,*P);
                }
                return;
            }
            // check if 3 consecutive wins
            else if (P->nb_succ_wins >= 3){
                printf("Winner %d , %s won 3 times in a row ! -> Goes to F1 . \n", P->num , P->name);
                if (isCurrWinner){
                    //Move currWinner to F1
                    EnQueue(&g->F1, *(g->currWinner));
                    free(g->currWinner);
                    g->currWinner = NULL;
                } else { //Move the Queue Player to F1
                    EnQueue(&g->F1 , *P);
                }
                return;
            } else{
                //The player becomes the currWinner
                printf("Winner %d , %s becomes the curr winner and will play next round ." , P->num , P ->name);
                if(!isCurrWinner){
                    //if exists , we free previous currWinner
                    if (g->currWinner != NULL){
                        free(g->currWinner);
                    }
                    //we allocate new currWinner and copy player data
                    g->currWinner = malloc(sizeof(Player));
                    if (g->currWinner != NULL){
                        *(g->currWinner) = *P;
                    }
                }
            }
        } else  { // Strategy 2 winning rules
            if (P->nb_succ_wins >= 2){
                //Player goes to LG
                printf("Yahoo ! Winner %d , %s is a definitive winner ! -> Goes to Winners List and exempt from playing .\n", P->num,P->name);
                if (isCurrWinner){
                    ADDtoLG(&g->LG, *(g->currWinner));
                    free(g->currWinner);
                    g->currWinner = NULL;
                }else{ //we create a copy in LG
                    ADDtoLG(&g->LG,*P);
                }
                return;
            }
            else { //The player becomes current winner
                if(!isCurrWinner){
                    //if exists , we free previous currWinner
                    if (g->currWinner != NULL){
                        free(g->currWinner);
                    }
                    //we allocate new currWinner and copy player data
                    g->currWinner = malloc(sizeof(Player));
                    if (g->currWinner != NULL){
                        *(g->currWinner) = *P;
                    }
                }
            }
        }
    }else {    //The player lost
        if (g->Strategy == 1){
                // we check for 5 total losses first
                if (P->nb_losses >= 5){
                    printf("Ouch player %d , %s is a definitive loser :( -> Goes to Losers List and exempt from playing .\n", P->num,P->name);
                    if(isCurrWinner){ //the last current winner lost
                        ADDtoLP(&g->LP, *(g->currWinner)); // we move him to LP
                        free(g->currWinner);
                        g->currWinner = NULL ;
                    } else { //not current winner
                        ADDtoLP(&g->LP, *P);
                    }
                    return;
                }
            // Check for 3 or more total losses (consecutive losses included
            if (P->nb_losses >= 3){
                printf("Player %d , %s lost 3 or more times  -> Goes to F3 . \n", P->num , P->name);
                if (isCurrWinner){
                    EnQueue(&g->F3, *(g->currWinner));
                    free(g->currWinner);
                    g->currWinner = NULL;
                } else{
                    EnQueue(&g->F3 , *P);
                }
                return;
                
            } else {  // The player returns to F Queue
                printf("The player %d , %s  -> goes to F Queue . \n" , P->num , P->name);
                if(isCurrWinner){
                    EnQueue(&g->F, *(g->currWinner));
                    free(g->currWinner);
                    g->currWinner = NULL;
                } else{
                    EnQueue(&g->F, *P);
                }
            }
        } else { //Strategy 2 loser handling
            // we check for 2 total losses first
            if (P->nb_losses >= 2){
                printf("Ouch player %d , %s is a definitive loser :( -> Goes to Losers List and exempt from playing .\n", P->num,P->name);
                if(isCurrWinner){ //the last current winner lost
                    ADDtoLP(&g->LP, *(g->currWinner)); // we move him to LP
                    free(g->currWinner);
                    g->currWinner = NULL ;
                } else{
                    ADDtoLP(&g->LP, *P);
                }
                return;
            }
            else {  // The player returns to F3 Queue
                printf("The player %d , %s  -> goes to F3 Queue . \n" , P->num , P->name);
                if(isCurrWinner){
                    EnQueue(&g->F3, *(g->currWinner));
                    free(g->currWinner);
                    g->currWinner = NULL;
                } else{
                    EnQueue(&g->F, *P);
                }
            }
        }
    }
}

//function to process the game round results
void GameResults( GameState *g , Game game , Player *P1 , Player *P2){
    P1->game_score = game.score_P1;
    P2->game_score = game.score_P2;
    P1->score += game.score_P1;
    P2->score += game.score_P2;
    
    printf("\n  Game Results ! \n");
    
    if (game.score_P1 > game.score_P2){
        // P1 wins and P2 loses
        P1->is_winner = true ;
        P2->is_winner = false;
        
        printf("Player %d , %s Wins : %d-%d \n" , P1->num , P1->name , game.score_P1 , game.score_P2);
        
        //update player stats
        Update_P_Stats(P1, P2);
        
        //moving players to appropriate Lists and Queues
        printf("Let's place the players !\n");
        MovePlayer(g, P1);
        MovePlayer(g, P2);
    } else if (game.score_P2 > game.score_P1){
        // P2 wins and P1 loses
        P2->is_winner = true ;
        P1->is_winner = false;
        
        printf("Player %d , %s Wins : %d-%d \n" , P2->num , P2->name , game.score_P2 , game.score_P1);
        
        //update player stats
        Update_P_Stats(P2, P1);
        
        //moving players to appropriate Lists and Queues
        printf("\nLet's place the players !\n");
        MovePlayer(g, P2);
        MovePlayer(g, P1);
    } else {
        // it's a tie , both don't win nor lose
        printf("It's a tie ! : %d-%d \n" ,game.score_P2 , game.score_P1);
        P1->is_winner = false;
        P2->is_winner = false;
        //All consecutive points are lost for both
        P1->nb_succ_wins = 0;
        P1->nb_succ_losses = 0;
        P1->succesifs_points = 0;
        P2->nb_succ_wins = 0;
        P2->nb_succ_losses = 0;
        P2->succesifs_points = 0;
        
        //Moving both players to F
        printf("Both players return to F . \n");
        EnQueue(&g->F, *P1);
        EnQueue(&g->F, *P2);
        
        //clear the currWinner since it's a tie
        if(g->currWinner != NULL && (g->currWinner->num == P1->num || g->currWinner->num == P2->num )){
            free(g->currWinner);
            g->currWinner = NULL;
            printf("No current winner :/ \n");
        }
    }
}

//function to display state of game after round
void DisplayGameState ( GameState *g){
    printf("\n Current Game State : \n");
    
    printf("Total games played : %d | Strategy : %d \n", g->totalGames , g->Strategy);
    
    //We display the different queues
    printf("F Queue :\n");
    Display_Queue(g->F);
    printf("F1 Queue :\n");
    Display_Queue(g->F1);
    printf("F3 Queue :\n");
    Display_Queue(g->F3);
    
    //Display the current Winner
    printf("The current Winner is : \n");
    if(g->currWinner == NULL){
        printf("No current winner .\n");
    } else {
        printf("Player %d , %s , Wins : %d ,Consecutive wins : %d, Losses : %d , Consecutive losses : %d ,game score : %d , score : %d" , g->currWinner->num,g->currWinner->name , g->currWinner->nb_wins,g->currWinner->nb_succ_wins,g->currWinner->nb_losses,g->currWinner->nb_succ_losses,g->currWinner->game_score, g->currWinner->score);
    }
    
    //Display the lists
    printf("\nThe winners list : \n");
    if(g->LG.L == NULL ){
        printf("Empty List.\n");
    } else{
        Node *Q = g->LG.L;
        int i = 1;
        while (Q != NULL) {
            Player p = Q->P;
            printf("%d. Player %d, %s | Age: %d | Score: %d | Wins: %d | Losses: %d\n", i++, p.num, p.name, p.age, p.score, p.nb_wins, p.nb_losses);
            Q = Q->Pnext;
        }
    }
    
    printf("The losers list : \n");
    if(g->LP.L == NULL ){
        printf("Empty List.\n");
    } else{
        Node *Q = g->LP.L;
        int i = 1;
        while (Q != NULL) {
            Player p = Q->P;
            printf("%d. Player %d, %s | Age: %d | Score: %d | Wins: %d | Losses: %d\n", i++, p.num, p.name, p.age, p.score, p.nb_wins, p.nb_losses);
            i++;
            Q = Q->Pnext;
        }
    }
}

//function to create the initial queue F of players (to avoid keyboard input)
void CreateF(Queue *F , int n ){
    //field of name strings
    char *names[15] = {
        "Sarah" , "Lyna" , "Amine" , "Imene" , "Karim" , "Mehdi" , "Elias" , "Anais" ,"Maissa" , "Adam" , "Nazim" , "Racim " ,"Ramy" , "Farida" , "Meriem"
    };
    Player p;
    
    for(int i=0; i < n ; i++){
        strcpy(p.name, names[i%15]);//copy from the field of names above
        p.num = i + 1;
        p.age = 18 + rand() % 83 ;// assigns the players a random age between 18 and 100
        p.game_score = 0;
        p.score = 0;
        p.nb_losses = 0;
        p.nb_wins = 0;
        p.nb_succ_losses = 0;
        p.nb_succ_wins = 0;
        p.is_winner = false;

        EnQueue(F , p);
    }
}

//Function to determine whether the game ends (ends once all players in LG or LP)
bool FinishedGame(GameState *g){
    int ListedPlayers = 0; // counter
    // Compute players in LG
    Node *Q = g->LG.L;
    while (Q != NULL) {
        ListedPlayers++;
        Q = Q->Pnext;
    }
    //Compute players in LP
    Q = g->LP.L;
    while (Q != NULL) {
        ListedPlayers++;
        Q = Q->Pnext;
    }
    return (ListedPlayers == g->totalPlayers);
}

//function to display top 3 winners at end of game
void DisplayTop3Winners(List LG ){
    printf ("\nThe top 3 winners of the game are :");
    printf("\n-----------------------------------");
    printf("\n*drumroll Please*\n\n");
    
    if(LG.L == NULL ){ // list of winners is empty
        printf("No winners sadly ! \n");
        return;
    }
    Node *Q = LG.L;
    int i = 0;
    
    while (Q != NULL && i <= 3) {
        Player p = Q->P;
        printf("%d.Player %d, %s | Age: %d | Score: %d | Wins: %d | Losses: %d\n\n", i+1,p.num,p.name,p.age,p.score,p.nb_wins,p.nb_losses);
        Q = Q->Pnext;
        i++;
    }
}

//function to display n winners from LG and LP (End of Game)
void Display_N_Wins (List * LG, List * LP , int n)
{
    int found = 0;
    Node * current;

    // There is no element in the list LG and LP
    if ( (LG->L == NULL) && (LP->L == NULL) )
    {
        printf("\nThere is no player with %d wins in the lists LG and LP\n", n);
        return;
    }

    // Create pointer to browse the list without losing the head
    current = LG->L;

    printf("\nPlayers with %d win(s) in the list LG:\n", n);
    while (current != NULL)
    {
        if (current->P.nb_wins > n){
            break;
        }
        else if(current->P.nb_wins == n)
        {
            printf("In LG : Player: %s, Wins: %d\n", current->P.name, current->P.nb_wins);
            found++;
        }
        current = current->Pnext;
    }
    if (found == 0){
        printf("No player found in LG with %d number of wins !\n", n);
    }
    
    current = LP->L;
    found = 0;
    
    printf("\nPlayers with %d win(s) in the list LP:\n", n);
    while (current != NULL)
    {
        if(current->P.nb_wins == n)
        {
            printf("In LP : Player: %s, Wins: %d\n", current->P.name, current->P.nb_wins);
            found++;
        }
        current = current->Pnext;
    }
    if (found == 0){
        printf("No player found in LP with %d number of wins !\n", n);
    }
}

// Function to display n losses from LG and LP (End of Game)
void Display_N_Losses (List * LG, List * LP , int n)
{
    Node * current;
    int found = 0;

    // There is no element in the list LG and LP
    if ( (LG->L == NULL) && (LP->L == NULL) )
    {
        printf("\nThere is no player with %d losses in the lists LG and LP\n", n);
        return;
    }

    // Create pointer to browse the list without losing the head
    current = LP->L;

    printf("\nPlayers with %d loss(es) in the list LP:\n", n);
   
    while (current != NULL)
    {
       
        if(current->P.nb_losses == n)
        {
            printf("In LP : Player: %s, losses: %d\n", current->P.name, current->P.nb_losses);
            found++;
        }
        
        current = current->Pnext;
    }
    if (found == 0){
        printf("No player found in LP with %d number of wins !\n", n);
    }

    current = LG->L;
    found = 0;
    printf("\nPlayers with %d loss(es) in the list LG:\n", n);
    
    while (current != NULL)
    {
        if(current->P.nb_losses == n)
        {
            printf("In LG : Player: %s, Losses: %d\n", current->P.name, current->P.nb_losses);
        }

        current = current->Pnext;
    }
    if (found == 0){
        printf("No player found in LG with %d number of wins !\n", n);
    }
}

//game loop function
void PlayGame(GameState *g , int nbrPlayers){
    
    
    printf("\n  Start Game  !!  \n");
    printf("Number of players : %d \n" , nbrPlayers);
   
    CreateF(&g->F , nbrPlayers);
    g->totalPlayers = nbrPlayers;
    g->currWinner = NULL;
    g->Strategy = 1;
    g->totalGames = 0;
    bool change_strat = false;
    
    printf("\nInitial State of the game : \n");
    DisplayGameState(g);
    
    //Game loops until all players are in LG and LP hence all Queues Empty And a currWinner is ongoing
    while(!FinishedGame(g) && (!EmptyQueue(g->F) || !EmptyQueue(g->F1) || !EmptyQueue(g->F3) || g->currWinner != NULL)){
        
        if(g->totalGames>= (3*nbrPlayers)){ // Part 1 stops after 3n rounds
            printf("\n\n\n   End of Part I    \n");
            printf("Part I games played : %d", g->totalGames);
            printf("\n!!!!!!!\n");
            printf("\nEnded with participation of %d players !\n", nbrPlayers);
            printf("\nLet's display Part I results !\n");
            //Displaying players who won or lost 1 ,2 and 3 parts of the game
            Display_N_Wins(&g->LG, &g->LP, 1);  //Display players with 1 win
            Display_N_Wins(&g->LG, &g->LP, 2);  //Display players with 2 win
            Display_N_Wins(&g->LG, &g->LP, 3);  //Display players with 3 win
            printf("\n\n Losses Stats : \n");
            Display_N_Losses(&g->LG, &g->LP, 1);
            Display_N_Losses(&g->LG, &g->LP, 2);
            Display_N_Losses(&g->LG, &g->LP, 3);
            
            //Checking whether to start Part 2 aka non empty queues
            if(!EmptyQueue(g->F) || !EmptyQueue(g->F1) || !EmptyQueue(g->F3)){
                printf("\n\nSTART OF PART 2 STRATEGY\n");
                printf("------------------------\n");
                change_strat = true;
                printf("\nPress 'ENTER' to start Strategy 2 game :\n");
                getchar();
                break;
            }
        }
        Player P1 , P2 ; // each game round has 2 players
        if(Priority(g, &P1, &P2)){
            Game game = GameRound(g, P1, P2);
            GameResults(g, game, &P1, &P2);
            //store the game round in the game List
            G *gameH = malloc(sizeof(G));
            gameH->round = game;
            gameH->Next = g->GameHistory;
            g->GameHistory = gameH;
            //Display State of game after each round
            DisplayGameState(g);
            //Allow interaction with user
            printf("\n\nPress 'ENTER' for next game :\n");
            getchar();
        } else{  //NO players available (Priority returns False)
            
            if (g->currWinner != NULL){   //last player moves to F1
                printf("No opponent for current Winner %d , %s -> Player moved to F1\n" , g->currWinner->num, g->currWinner->name);
                EnQueue(&g->F1, *(g->currWinner));
                free(g->currWinner); //free the currWinner memory
                g->currWinner = NULL;
            }
            //last player in F3 moves to LP
            if (!EmptyQueue(g->F3) && EmptyQueue(g->F) && EmptyQueue(g->F1) && g->currWinner == NULL){
                Player lastPlayer;
                DeQueue(&g->F3, &lastPlayer);
                printf("Last Player in F3 %d , %s -> moves to LP\n" , lastPlayer.num,lastPlayer.name);
                ADDtoLP(&g->LP, lastPlayer);
            }
            break;
        }
        printf("\n  End of Round    \n");
        printf("Part 1 games played : %d\n\n", g->totalGames);
    }
    if (change_strat){
        g->Strategy = 2;
        
        printf("\nDisplaying PART II initial state : \n");
        DisplayGameState(g);
        int Part2Games = 0 ;
        
        while ((Part2Games < 2 * nbrPlayers) && (!FinishedGame(g)) && (!EmptyQueue(g->F) || !EmptyQueue(g->F1) || !EmptyQueue(g->F3) || g->currWinner != NULL)){
            //Same proceeding but with Strat 2
            Player P1 , P2 ; // each game round has 2 players
            if(Priority(g, &P1, &P2)){
                Game game = GameRound(g, P1, P2);
                GameResults(g, game, &P1, &P2);
                //store the game round in the game List
                G *gameH = malloc(sizeof(G));
                gameH->round = game;
                gameH->Next = g->GameHistory;
                g->GameHistory = gameH;
                //Display State of game after each round
                DisplayGameState(g);
                //Allow interaction with user
                printf("\n\nPress 'ENTER' for next game :\n");
                getchar();
                Part2Games++;
            } else{  //NO players available (Priority returns False)
                
                if (g->currWinner != NULL){   //last player moves to F1
                    printf("\n No opponent for current Winner : %d . %s -> Player moved to F1\n" , g->currWinner->num, g->currWinner->name);
                    EnQueue(&g->F1, *(g->currWinner));
                    free(g->currWinner); //free the currWinner memory
                    g->currWinner = NULL;
                }
                //last player in F3 moves to LP
                if (!EmptyQueue(g->F3) && EmptyQueue(g->F) && EmptyQueue(g->F1) && g->currWinner == NULL){
                    Player lastPlayer;
                    DeQueue(&g->F3, &lastPlayer);
                    printf("Last Player in F3 : %d. %s -> moves to LP\n" , lastPlayer.num,lastPlayer.name);
                    ADDtoLP(&g->LP, lastPlayer);
                }
                break;
            }
            printf("\n  End of Round    \n");
            printf("Part 2 games played : %d\n\n", Part2Games);
            
        }
        printf("\n\n\n   End of Part II    \n");
        printf("Part 2 games played : %d", Part2Games);
    }
    printf("\n\n    End of Game     \n");
    printf("Total game rounds played : %d \n", g->totalGames);
    
    //Move remaining players to appropriate lists
    if (g->currWinner != NULL){
        if (g->Strategy == 1){
            if(g->currWinner->nb_wins >= 5) { //Move currWinner to LG if total wins >= 5
                ADDtoLG(&g->LG, *(g->currWinner));
            } else { // Player belongs in F1
                EnQueue(&g->F1, *(g->currWinner));
            }
        } else { // Strategy 2
            if(g->currWinner->nb_wins >= 2){
                ADDtoLG(&g->LG, *(g->currWinner));
            }else{ //Player belongs to F
                EnQueue(&g->F, *(g->currWinner));
            }
        }//free currWinner memory
        free (g->currWinner);
        g->currWinner = NULL;
    }
    //Move last F1 players to LG
    while(!EmptyQueue(g->F1)){
        Player p;
        DeQueue(&g->F1, &p);
        
        ADDtoLG(&g->LG, p);
        
    }
    //Move last F players to LP
    while(!EmptyQueue(g->F)){
        Player p;
        DeQueue(&g->F, &p);
        ADDtoLP(&g->LP, p);
    }
    //Move last F3 players to LP
    while(!EmptyQueue(g->F3)){
        Player p;
        DeQueue(&g->F3, &p);
        ADDtoLP(&g->LP, p);
    }
    
    printf("\n\n     END OF GAME  RESULTS :     \n");
    DisplayTop3Winners(g->LG); //Display the top 3 winners at end of game
    
    printf("\n\n Win Stats : \n");
    Display_N_Wins(&g->LG, &g->LP, 0);  //Display players with no wins
    Display_N_Wins(&g->LG, &g->LP, 1);  //Display players with 1 win
    Display_N_Wins(&g->LG, &g->LP, 2);  //Display players with 2 win
    printf("\n\n Losses Stats : \n");
    Display_N_Losses(&g->LG, &g->LP, 1);
    Display_N_Losses(&g->LG, &g->LP, 2);
    
    printf("\n\n\n------------------------\n");
    printf("Thank you for playing our game ! :) \n");
    }



void Init_List(List *L){
    L->L = NULL;
}




//*******************  MAIN  *****************//
int main(int argc, const char * argv[])
{
    //initialisation of all game structures
    GameState g = {0};
    Init_Queue(&g.F);
    Init_Queue(&g.F1);
    Init_Queue(&g.F3);
    g.GameHistory = NULL;
    g.totalGames = 0;
    g.currWinner = NULL;
    g.totalPlayers = 0;
    g.Strategy = 1;
    Init_List(&g.LG);
    Init_List(&g.LP);
    
    printf("Hi Dear Players !\nWelcome to our game , conceived by Hamdi Sarah and Djenkal Lyna\n");
    printf("To start playing may you please enter the total number of players!!\n");
    int Number_Players;
    scanf("%d", &Number_Players);
    printf("Ok ! Now I invite you to enjoy :D\n\n");
    PlayGame(&g, Number_Players);

    return 0;
}
