#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Vertex
{
  char *game;
  int value;
  int subScore;
  int win;
  struct Vertex **edges;
  struct Vertex *next;
};

struct Vertex *newVertex(char *game, int horizSize)
{
  struct Vertex *vertex = malloc(sizeof(struct Vertex));
  vertex->game = game;
  vertex->next = NULL;
  vertex->value = 0;
  vertex->subScore = 0;
  vertex->win = 0;
  vertex->edges = malloc(sizeof(struct Vector *) * horizSize);
  for (int i = 0; i < horizSize; i++) {
    vertex->edges[i] = NULL;
  }
  return vertex;
}

void delVertex(struct Vertex *vertex)
{
  free(vertex->edges);
  free(vertex->game);
  free(vertex);
}

void delGraph(struct Vertex *graph)
{
  struct Vertex *curr = graph;
  while(curr != NULL) {
    struct Vertex *temp = curr;
    curr = curr->next;
    delVertex(temp);
  }
}

void DisplayBoard(char* array, int horizSize, int vertSize) {

    for(int i = 0; i < vertSize; i++) {
        for(int j = 0; j < horizSize; j++) {
                printf("%c ", *((array+i*horizSize)+j));
        }
        printf("\n");
    }

}

void ZeroArray(char* array, int horizSize, int vertSize) {

    for(int i = 0; i < vertSize; i++) {
        for(int j = 0; j < horizSize; j++) {
            *((array+i*horizSize)+j) = '0';
        }
    }
    *((array+vertSize*horizSize)) = '\0';
}

char *CopyArray(char *array, int horizSize, int vertSize) {
  char *copy = (char *) malloc(horizSize * vertSize * sizeof(char) + sizeof(char));
  for(int i = 0; i < vertSize; i++) {
      for(int j = 0; j < horizSize; j++) {
          *((copy+i*horizSize)+j) = *((array+i*horizSize)+j);
      }
  }
  *((array+vertSize*horizSize)) = '\0';
  return copy;
}

int HumanPlayer(struct Vertex *graph, int horizSize, int vertSize, int playerNumber) {
    DisplayBoard(graph->game, horizSize, vertSize);

    int column = -1, i = 0, j = 0;
    while(column < 0 || column >= horizSize || *(graph->game+column) != '0') {
      printf("Player %d - Which column?\n", playerNumber);
      scanf("%d", &column);
    }


    while(*((graph->game+i*horizSize)+column) == '0' && i < vertSize) {
        i++;
    }

    i--;
    if(i < 0) {
        printf("Illegal move.");
        exit(1);
    }

    else {
       *((graph->game+i*horizSize)+column) = '0' + playerNumber;

    }




}


int DetectHorizWin(char* array, int horizSize, int vertSize) {

    int j = 0;
    int i = 0;

    while((j + 3) < horizSize) {

            i = 0;
            while(i < vertSize) {

                if(((j+3 < horizSize) && (*((array+i*horizSize)+j) != '0') &&
                *((array+i*horizSize)+j) == *((array+i*horizSize)+j+1) &&
                *((array+i*horizSize)+j) == *((array+i*horizSize)+j+2) &&
                *((array+i*horizSize)+j) == *((array+i*horizSize)+j+3)))

                return 1;

                i++;
            }
        j++;
    }

    return 0;


}

int ScoreHoriz(char* array, int horizSize, int vertSize, int score[])
{
  for (int row = 0; row < vertSize; row++) {
    for (int col = 0; col < horizSize; col++) {
      for (int ofst = 1; col + ofst < horizSize; ofst++) {
        if (array[row * horizSize + col + ofst] == '0') {
          break;
        }
        if (array[row * horizSize + col] != array[row * horizSize + col + ofst]){
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

int ScoreVert(char* array, int horizSize, int vertSize, int score[])
{
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

int ScoreLeftRight(char* array, int horizSize, int vertSize, int score[])
{
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

int ScoreRightLeft(char* array, int horizSize, int vertSize, int score[])
{
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

int ScoreAll(char *array, int horizSize, int vertSize, int score[]) {
  return ScoreVert(array, horizSize, vertSize, score) || ScoreHoriz(array, horizSize, vertSize, score)
    || ScoreLeftRight(array, horizSize, vertSize, score) || ScoreRightLeft(array, horizSize, vertSize, score);
}

 int DetectVertWin(char* array, int horizSize, int vertSize) {

    int j = 0;
    int i = 0;


    while(j < horizSize) {


            i = 0;
            while((i + 3) < vertSize) {

                if(((i+3 < horizSize) && (*((array+i*horizSize)+j) != '0') &&
                *((array+i*horizSize)+j) == *((array+(i+1)*horizSize)+j) &&
                *((array+i*horizSize)+j) == *((array+(i+2)*horizSize)+j) &&
                *((array+i*horizSize)+j) == *((array+(i+3)*horizSize)+j)))

                return 1;
                i++;
            }
        j++;
    }

    return 0;


}

int DetectDiagDownWin(char* array, int horizSize, int vertSize) {


    int j = 0;
    int i = 0;


    while(j+1 < horizSize) {


            i = 0;
            while((i + 1) < vertSize) {

                if((((i+3) < vertSize) && (j+3 < horizSize) && (*((array+i*horizSize)+j) != '0') &&
                *((array+i*horizSize)+j) == *((array+(i+1)*horizSize)+j+1) &&
                *((array+i*horizSize)+j) == *((array+(i+2)*horizSize)+j+2) &&
                *((array+i*horizSize)+j) == *((array+(i+3)*horizSize)+j+3) ))

                return 1;
                i++;
            }
        j++;
    }

    return 0;

}

int DetectDiagUpWin(char* array, int horizSize, int vertSize) {

    int j = 0;
    int i = 0;

    for(i = 3; i < vertSize; i++) {
        for(j = 0; j < 4; j++) {
            if (*((array+i*horizSize)+j) != '0' &&
                *((array+i*horizSize)+j) == *((array+(i-1)*horizSize)+j+1) &&
                *((array+i*horizSize)+j) == *((array+(i-2)*horizSize)+j+2) &&
                *((array+i*horizSize)+j) == *((array+(i-3)*horizSize)+j+3) ) {

                    return 1;
                }
        }
    }


    return 0;

}


int DetectWin(char* array, int horizSize, int vertSize) {

    int winner = 0;


    winner = DetectHorizWin(array, horizSize, vertSize);
    if(winner == 1) {
        //printf("horizontal win\n");
        return winner;

    }

    winner = DetectVertWin(array, horizSize, vertSize);
    if(winner == 1) {
        //printf("vertical win\n");
        return winner;

    }

    winner = DetectDiagDownWin(array, horizSize, vertSize);
    if(winner == 1) {
        //printf("diagnal down win\n");
        return winner;

    }

    winner = DetectDiagUpWin(array, horizSize, vertSize);
    if(winner == 1) {
        //printf("diagnal up win\n");
        return winner;

    }


    return 0;

}

int CountAdjacent(char *array, int horizSize, int vertSize, int col, int row)
{
  int score = 0;
  int player = *((array+row*horizSize) +col);
  for(int i = row + 1; i < vertSize && player == *((array+i*horizSize) + col); i++)
    score++;
  for(int i = col + 1; i < horizSize && player == *((array+row*horizSize) + i); i++)
    score++;
  for(int i = row + 1, j = col + 1;
    i < vertSize && j < horizSize && player == *((array+i*horizSize) + j); i++, j++)
    score++;
  for(int i = col - 1; i >= 0 && player == *((array+row*horizSize) + i); i--)
    score++;
  for(int i = row - 1, j = col + 1;
    i >= 0 && j < horizSize && player == *((array+i*horizSize) + j); i--, j++)
    score++;
  for(int i = row + 1, j = col - 1;
    i < vertSize && j >= 0 && player == *((array+i*horizSize) + j); i++, j--)
    score++;
  for(int i = row - 1, j = col - 1;
    i >= 0 && j >= 0 && player == *((array+i*horizSize) + j); i--, j--)
    score++;
  // up not needed
  return score;
}

int ScoreMoves(struct Vertex *graph, int horizSize, int vertSize, int move, int player, int turns) {
  if (turns == 0)
  {
    return 0;
  }
  char *copy = CopyArray(graph->game, horizSize, vertSize);
  int newNode = 0;
  int i = 0;
  int subScore [] = {0, 0}; //moved this here don't know if it affects anything
  struct Vertex *curr = graph;//" "
  struct Vertex *prev = NULL;//" " because had an error orginally for the else statement that curr and subScore weren't declared
  while(*((copy+i*horizSize)+move) == '0' && i < vertSize) {
      i++;
  }
  if (i != 0) {
    i--;
    *((copy+i*horizSize)+move) = '0' + player;
    struct Vertex *curr = graph;
    struct Vertex *prev = NULL;
    while(curr != NULL) {
      prev = curr;
      if (strcmp(curr->game, copy) == 0) {
        free(copy);
        if (curr->win) {
          return curr->value;
        }
        break;
      }
      curr = curr->next;
    }
    if (curr == NULL) {
      curr = newVertex(copy, horizSize);
      prev->next = curr;
      newNode = 1;
    }
    graph->edges[move] = curr;
    //printf("Trying %d for player %d \n", move, player);
    //DisplayBoard(copy, horizSize, vertSize);
    int subScore [] = {0, 0};
//    if (newNode && DetectWin(curr->game, horizSize, vertSize)) {
    if (newNode && ScoreAll(curr->game, horizSize, vertSize, subScore)) 
      curr->value = horizSize * vertSize * 32768;
      if (player == 1) {
        curr->value *= -1;
      }
      //free(copy);
      curr->win = 1;
      return curr->value;
    
  }

    else {
      if (newNode) {
        //curr->subScore = CountAdjacent(copy, horizSize, vertSize, move, i) * (horizSize + vertSize);
        curr->subScore = subScore[1] - subScore[0];
      }
      curr->value = curr->subScore;
      for (int nextMove = 0; nextMove < horizSize; ++ nextMove)
      {
        curr->value += ScoreMoves(curr, horizSize, vertSize, nextMove, (player) % 2 + 1, turns - 1) / horizSize;
      }
    }
  
  
  
  return curr->value;
}

struct Vertex *makeMove(struct Vertex *graph, int horizSize, int vertSize, int bestMove, int player)
{
  char *copy = CopyArray(graph->game, horizSize, vertSize);
  int i = 0;
  while(i < vertSize && *((copy+i*horizSize)+bestMove) == '0') {
      i++;
  }
  i--;
  *((copy+i*horizSize)+bestMove) = '0' + player;
  struct Vertex *curr = graph;
  while (curr != NULL) {
    if (strcmp(curr->game, copy) == 0) {
      free(copy);
      break;
    }
    struct Vertex *temp = curr;
    curr = curr->next;
    delVertex(temp);
  }
  if (curr == NULL) {
    curr = newVertex(copy, horizSize);
  }
  return curr;
}

struct Vertex *ComputerPlayer(struct Vertex *graph, int horizSize, int vertSize) {
  int move;
  int bestMove;
  int bestScore = -999999999;
  int score;
  for (int move = 0; move < horizSize; move++) {
    if (*(graph->game+move) == '0') {
      score = ScoreMoves(graph, horizSize, vertSize, move,  2, 42 / horizSize + 2);
      // printf("Score(%d):%d\n", move, score);
      if (score > bestScore)  {
        bestScore = score;
        bestMove = move;
      }
    }
  }
  //printf("Best Move %d\n", bestMove);
  graph = makeMove(graph, horizSize, vertSize, bestMove, 2);
  return graph;
}

struct Vertex *PlayComputer(struct Vertex *graph, int horizSize, int vertSize) {

    int win = 0;
    int subScore[] = {0, 0};
    while(!win) {
        graph = HumanPlayer(graph, horizSize, vertSize, 1);
        //win = DetectWin(graph->game, horizSize, vertSize);
        win = ScoreAll(graph, horizSize, vertSize, subScore);

        if(win) {
          printf("Player 1 wins!\n");
          break;
        }

        graph = ComputerPlayer(graph, horizSize, vertSize);
        //win = DetectWin(graph->game, horizSize, vertSize);
        win = ScoreAll(graph, horizSize, vertSize, subScore);

        if(win) {
          printf("Copmuter player wins!\n");
          break;
        }

    }
    return graph;
}

struct Vertex *PlayDouble(struct Vertex *graph, int horizSize, int vertSize) {

    int win = 0;
    int subScore[] = {0, 0};
    while(!win){
        graph = HumanPlayer(graph, horizSize, vertSize, 1);
        //win = DetectWin(graph->game, horizSize, vertSize);
        win = ScoreAll(graph, horizSize, vertSize, subScore);

        if(win) {
          printf("Player 1 wins!\n");
          break;
        }


        graph = HumanPlayer(graph, horizSize, vertSize, 2);
        //win = DetectWin(graph->game, horizSize, vertSize);
        win = ScoreAll(graph, horizSize, vertSize, subScore);

        if(win) {
          printf("Player 2 wins!\n");
          break;
        }
    }
    return graph;
}


int main(void) {

    int horizSize;
    int vertSize;
    int sizes = 0;
    while(sizes != 2 || horizSize < 4 || vertSize < 4) {
      printf("Enter board size (columns,rows): ");
      sizes = scanf("%d %d", &horizSize, &vertSize);
    }
    struct Vertex *graph = newVertex((char *) malloc(horizSize * vertSize * sizeof(char) + sizeof(char)), horizSize);

    char choice = '\0';
    while (choice != '1' && choice != '2') {
      printf("Choose options: Single (1) or Double (2)\n");
      //fseek(stdin, 0, SEEK_END);

      while ((getchar()) != '\n');
      choice = getchar();
    }
    printf("Choice: %c \n", choice);
    ZeroArray(graph->game, horizSize, vertSize);

    switch (choice) {

        case '1':
          graph = PlayComputer(graph, horizSize, vertSize);
          break;

        case '2':
          graph = PlayDouble(graph, horizSize, vertSize);
          break;

    }




    DisplayBoard(graph->game, horizSize, vertSize);

    delGraph(graph);



    return 0;
}
