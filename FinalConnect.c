#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Vertex               // definition of Vertex
{
  char *game;               //board state
  int value;                //"value" of the move
  int subValue;             //does not get recalculated on nonwinning move...reused if have to recalculate value
  int win;                  //identifying that it's a winning move
  struct Vertex **edges;    //nodes that are pointers to vertexes that go with each possible move
  struct Vertex *next;
};

void DisplayBoard(char* array, int horizSize, int vertSize) {   //displays game board each time after a move is made

    for(int i = 0; i < vertSize; i++) {
        for(int j = 0; j < horizSize; j++) {
                printf("%c ", *((array+i*horizSize)+j));
        }
        printf("\n");
    }

}

void ZeroArray(char* array, int horizSize, int vertSize) {    //"zeroes" out dimensions(every cell) of game board

    for(int i = 0; i < vertSize; i++) {
        for(int j = 0; j < horizSize; j++) {
            *((array+i*horizSize)+j) = '0';
        }
    }
    *((array+vertSize*horizSize)) = '\0';
}

char *CopyArray(char *array, int horizSize, int vertSize) {    //want a copy of existing board state so that we can look at/form possible moves
  char *copy = (char *) malloc(horizSize * vertSize * sizeof(char) + sizeof(char));   //allocating space for the copy
  for(int i = 0; i < vertSize; i++) {
      for(int j = 0; j < horizSize; j++) {
          *((copy+i*horizSize)+j) = *((array+i*horizSize)+j);
      }
  }
  *((array+vertSize*horizSize)) = '\0';   //looks like a string, so we can search it like it is a string
  return copy;
}

struct Vertex *newVertex(char *game, int horizSize) {      // creation of a Vertex
  struct Vertex *vertex = malloc(sizeof(struct Vertex));   //allocating space for new vertex
  vertex->game = game;
  vertex->next = NULL;
  vertex->value = 0;
  vertex->subValue = 0;
  vertex->win = 0;
  vertex->edges = malloc(sizeof(struct Vector *) * horizSize);
  for (int i = 0; i < horizSize; i++) {
    vertex->edges[i] = NULL;
  }
  return vertex;
}

void delVertex(struct Vertex *vertex) {   //deletes or frees individual vertex 

  free(vertex->edges);
  free(vertex->game);
  free(vertex);
}

void delGraph(struct Vertex *graph) {    //called at end of each search to delete entire graph

  struct Vertex *curr = graph;
  while(curr != NULL) {
    struct Vertex *temp = curr;
    curr = curr->next;
    delVertex(temp);
  }
}

struct Vertex *makeMove(struct Vertex *graph, int horizSize, int vertSize, int move, int player) { //makes copy and then uses while loop to compare copy that is modified to other game states

  char *copy = CopyArray(graph->game, horizSize, vertSize);
  int i = 0;
  while(i < vertSize && *((copy+i*horizSize)+move) == '0') {  
      i++;
  }
  i--;
  *((copy+i*horizSize)+move) = '0' + player;
  struct Vertex *curr = graph;
  while (curr != NULL) {                    //advances through graph for matching element
    if (strcmp(curr->game, copy) == 0) {    //if finds matching element(game state), frees copy b/c does not anymore
      free(copy);                         
      break;
    }
    struct Vertex *temp = curr;
    curr = curr->next;                      //advances curr to next entry b/c game states we no longer can reach
    delVertex(temp);
  }
  if (curr == NULL) {                       //for Double b/c it won't find "match" b/c only one entry everytime
    curr = newVertex(copy, horizSize);
  }
  return curr;                              //returns curr, which is new head of graph or current game state
}

struct Vertex *HumanPlayer(struct Vertex *graph, int horizSize, int vertSize, int playerNumber) {   //called for "human players" 
    
    DisplayBoard(graph->game, horizSize, vertSize);

    int column = -1, i = 0, j = 0;
    int status;
    int temp;
    while(column < 0 || column >= horizSize || *(graph->game+column) != '0') {
      printf("Player %d - Which column?\n", playerNumber);
      
      status = scanf("%d", &column);                                                //checks that user inputs a valid column
      while(column != 1) { 
        while((temp=getchar()) != EOF && temp != '\n');
        break;
        status = scanf("%d", &column);
      }
      if(column == 0) {
        break;
      }

    }

    return makeMove(graph, horizSize, vertSize, column, playerNumber);          //calls makeMove but returns curr b/c it goes through last if in makeMove
}



int ScoreHoriz(char* array, int horizSize, int vertSize, int score[]) {   //array: call by reference

  for (int row = 0; row < vertSize; row++) {                  //from any given starting position, see how many nonzero of same in a row
    for (int col = 0; col < horizSize; col++) {               
      for (int ofst = 1; col + ofst < horizSize; ofst++) {    //for each starting point, offset starts at 1 to check we are in bounds
        if (array[row * horizSize + col + ofst] == '0') {     //3 in a row more valuable than 2...but does not matter HOW much valuable
          break;
        }
        if (array[row * horizSize + col] != array[row * horizSize + col + ofst]){
          break;
        }
        if (ofst == 3) {                                      //win, so return 1
          return 1;
        }
        if (array[row * horizSize + col] == '1') {            //accumulation of what value of position is to player 1
          score[0] += ofst;
        }
        else {                                               //game state of player 2 
          score[1] += ofst;
        }
      }
    }
  }
  return 0;
}

int ScoreVert(char* array, int horizSize, int vertSize, int score[]) {

  for (int col = 0; col < horizSize; col++) {
    for (int row = 0; row < vertSize; row++) {
      for (int ofst = 1; row + ofst < vertSize; ofst++) {
        if (array[(row + ofst) * horizSize + col] == '0') {
          break;
        }
        if (array[row * horizSize + col] != array[(row + ofst) * horizSize + col]){
          break;
        }
        if (ofst == 3) {
          return 1;
        }
        if (array[row * horizSize + col] == '1') {
          score[0] += ofst;
        }
        else {
          score[1] += ofst;
        }
      }
    }
  }
  return 0;
}

int ScoreLeftRight(char* array, int horizSize, int vertSize, int score[]) {

  for (int row = 0; row < vertSize; row++) {
    for (int col = 0; col < horizSize; col++) {
      for (int ofst = 1; col + ofst < horizSize && row + ofst < vertSize; ofst++) {
        if (array[(row + ofst) * horizSize + col + ofst] == '0') {
          break;
        }
        if (array[row * horizSize + col] != array[(row + ofst) * horizSize + col + ofst]){
          break;
        }
        if (ofst == 3) {
          return 1;
        }
        if (array[row * horizSize + col] == '1') {
          score[0] += ofst;
        }
        else {
          score[1] += ofst;
        }
      }
    }
  }
  return 0;
}

int ScoreRightLeft(char* array, int horizSize, int vertSize, int score[]) {

  for (int row = 0; row < vertSize; row++) {
    for (int col = 0; col < horizSize; col++) {
      for (int ofst = 1; col - ofst >= 0 && row + ofst < vertSize; ofst++) {
        if (array[(row + ofst) * horizSize + col - ofst] == '0') {
          break;
        }
        if (array[row * horizSize + col] != array[(row + ofst) * horizSize + col - ofst]){
          break;
        }
        if (ofst == 3) {
          return 1;
        }
        if (array[row * horizSize + col] == '1') {
          score[0] += ofst;
        }
        else {
          score[1] += ofst;
        }
      }
    }
  }
  return 0;
}

int ScoreAll(char *array, int horizSize, int vertSize, int score[]) {     //scoring done by "short ciruit" evalutation (ie. detecting win takes precedence over anything else)
  return ScoreVert(array, horizSize, vertSize, score) || ScoreHoriz(array, horizSize, vertSize, score)
    || ScoreLeftRight(array, horizSize, vertSize, score) || ScoreRightLeft(array, horizSize, vertSize, score);
}

int NoMoves(char *array, int horizSize)     //stalemate function
{
  for(int i = 0; i < horizSize; i++) {      
    if (array[i] == '0') {
      return 0;
    }
  }
  return 1;
}

void DisplayScoreBoard(int scoreboard[]) {    //scoreboard displays after each game; kept as an array to pass better

    printf("\n*****SCOREBOARD*****\n");
    printf("Draw: %d\n", scoreboard[0]);
    printf("Player 1: %d\n", scoreboard[1]);
    printf("Player 2: %d\n", scoreboard[2]);
    printf("Computer Player: %d\n\n", scoreboard[3]);

  return;
}

int ScoreMoves(struct Vertex *graph, int horizSize, int vertSize, int move, int player, int turns) {    //looks through vertexes to find moves already calculated
  if (turns <= 0 || NoMoves(graph->game, horizSize))                                                    
  {                                                        
    return 0;
  }

  char *copy = CopyArray(graph->game, horizSize, vertSize);   //calls copy to start process of graph
  int newNode = 0;
  int i = 0;
  while(*((copy+i*horizSize)+move) == '0' && i < vertSize) {
      i++;
  }
  struct Vertex *curr = graph;
  struct Vertex *prev = NULL;
  if (i != 0) {
    i--;
    *((copy+i*horizSize)+move) = '0' + player;
    //printf("%s + %d @ %d #%d\n", copy, player, move, turns);      //may use printf to see how program scored
    while(curr != NULL) {
      prev = curr;
      if (strcmp(curr->game, copy) == 0) {
        free(copy);
        if (curr->win) {
          return curr->value;
        }
        break;
      }
      curr = curr->next;                                          //looking through vertexes
    }
    if (curr == NULL) {
      curr = newVertex(copy, horizSize);
      prev->next = curr;
      turns--;
      newNode = 1;
    }
    turns--;
    graph->edges[move] = curr;
    
    int subScore [] = {0, 0};

    if (newNode && ScoreAll(curr->game, horizSize, vertSize, subScore)) {
      curr->value = horizSize * vertSize * 32768;                           //fixed value to help score
      if (player == 1) {
        curr->value *= -1;
      }
      
      curr->win = 1;
      return curr->value;
    }
    else {
      if (newNode) {
        
        curr->subValue = subScore[1] - subScore[0];
      }
      curr->value = curr->subValue;
      for (int nextMove = 0; nextMove < horizSize; ++ nextMove)
      {
        curr->value += ScoreMoves(curr, horizSize, vertSize, nextMove, (player) % 2 + 1, turns) / horizSize;
      }
    }
  }
  return curr->value;
}

struct Vertex *ComputerPlayer(struct Vertex *graph, int horizSize, int vertSize) {    //tracking for Computer Player
  int move;
  int bestMove;
  int bestScore = -999999999;               //worst value possible because we are going to get negative values anyways
  int score;
  for (int move = 0; move < horizSize; move++) {
    if (*(graph->game+move) == '0') {
      score = ScoreMoves(graph, horizSize, vertSize, move,  2,  5);        //depth; it looks and evaluates
      
      if (score > bestScore)  {
        bestScore = score;
        bestMove = move;
      }
    }
  }
  
  graph = makeMove(graph, horizSize, vertSize, bestMove, 2);          //calls to decide move and make it
  printf("\nWatch out! Computer player making its move...\n\n");
  return graph;
}

struct Vertex *PlayComputer(struct Vertex *graph, int horizSize, int vertSize, int scoreboard[]) {  //function called when user decides what game mode

    int win = 0;
    int subScore[] = {0, 0};
    while(!win) {
        graph = HumanPlayer(graph, horizSize, vertSize, 1);               //Player 1(Human Player) goes first
        
        win = ScoreAll(graph->game, horizSize, vertSize, subScore);

        if(win) {
          printf("Player 1 wins!\n");
          scoreboard[1]++;
          DisplayScoreBoard(scoreboard);
          break;
        }
        if(NoMoves(graph->game, horizSize)) {
          printf("No more moves!\n");
          scoreboard[0]++;
          DisplayScoreBoard(scoreboard);
          break;
        }

        graph = ComputerPlayer(graph, horizSize, vertSize);              //Computer Player goes second
        
        win = ScoreAll(graph->game, horizSize, vertSize, subScore);

        if(win) {
          printf("Copmuter player wins!\n");
          scoreboard[3]++;
          DisplayScoreBoard(scoreboard);
          break;
        }
        if(NoMoves(graph->game, horizSize)) {
          printf("No more moves!\n");
          scoreboard[0]++;
          DisplayScoreBoard(scoreboard);
        }

    }
    return graph;
}

struct Vertex *PlayDouble(struct Vertex *graph, int horizSize, int vertSize, int scoreboard[]) {    //function called when user decides what game mode

    int win = 0;
    int subScore[] = {0, 0};
    while(!win){
        graph = HumanPlayer(graph, horizSize, vertSize, 1);                     //Player 1 always goes first
        
        win = ScoreAll(graph->game, horizSize, vertSize, subScore);

        if(win) {
          printf("Player 1 wins!\n");
          scoreboard[1]++;
          DisplayScoreBoard(scoreboard);
          break;
        }
        if (NoMoves(graph->game, horizSize)) {
          printf("No more moves!\n");
          scoreboard[0]++;
          DisplayScoreBoard(scoreboard);
          break;
        }

        graph = HumanPlayer(graph, horizSize, vertSize, 2);                     //Player 2 always goes second
        
        win = ScoreAll(graph->game, horizSize, vertSize, subScore);

        if(win) {
          printf("Player 2 wins!\n");
          scoreboard[2]++;
          DisplayScoreBoard(scoreboard);
          break;
        }
        if (NoMoves(graph->game, horizSize)) {
          printf("No more moves!\n");
          scoreboard[0]++;
          DisplayScoreBoard(scoreboard);
          break;
        }
    }
    return graph;
}



int main(void) {

    int horizSize = 0;
    int vertSize = 0;
    int horizStatus;
    int horizTemp;
    int vertStatus;
    int vertTemp;
    
    int scoreboard[] = {0,0,0,0};
    
    while(1) {                                                    //endless loop until terminates (break)
    while(1) {                                                    //Whiles so it may loop back around to menu until exit(0)
      printf("Welcome to Connect Four!\nReady to play?\n");
      printf("Enter 0 below (for the number of columns) to exit.\n");  
      printf("Enter number of columns: ");
      
      horizStatus = scanf("%d", &horizSize);                      //checks if user input is valid to proposal
      while(horizStatus != 1) {
        while((horizTemp=getchar()) != EOF && horizTemp != '\n');   
        printf("Error. Did not enter valid dimension. Goodbye.\n");
        exit(0);
        horizStatus = scanf("%d", &horizSize);
      }

      if(horizSize == 0) { 
          printf("Goodbye.\n");
          exit(0);
        }

      if(horizSize < 0) {
        printf("Error. Did not enter valid dimension. Goodbye.\n");
        exit(0);
      }


      printf("Enter number of rows: ");
      
      vertStatus = scanf("%d", &vertSize);                             //checks if user input is valid to proposal
      while(vertStatus != 1) {
        while((vertTemp=getchar()) != EOF && vertTemp != '\n');
        printf("Error. Did not enter valid dimension. Goodbye.\n");
        exit(0);
        vertStatus = scanf("%d", &vertSize);
      }

      if(vertSize <= 0) {
        printf("Error. Did not enter valid dimension. Goodbye.\n");
        exit(0);
      }
     

      break;
    }
    
    struct Vertex *graph = newVertex((char *) malloc(horizSize * vertSize * sizeof(char) + sizeof(char)), horizSize);

    char choice = '\0';
    while (choice != '1' && choice != '2') {
      printf("Choose options: Single (1) or Double (2)\n");
      while ((getchar()) != '\n');
      choice = getchar();
    }
    printf("Choice: %c \n", choice);
    ZeroArray(graph->game, horizSize, vertSize);

    switch (choice) {

        case '1':
          printf("You will be Player 1. Your 'tile' will be the number: 1. You will go first!\n");
          printf("Note: Board starts with column 0.\n");
          graph = PlayComputer(graph, horizSize, vertSize, scoreboard);
          break;

        case '2':
          printf("Player 1's 'tile' will be represented by the number: 1. Player 2's 'tile' will be represented by the number: 2.\n");
          printf("Player 1 will go first!\n");
          printf("Note: Board starts with column 0.\n");
          graph = PlayDouble(graph, horizSize, vertSize, scoreboard);
          break;

    }




    DisplayBoard(graph->game, horizSize, vertSize);

    delGraph(graph);


    }
    return 0;
    
}
