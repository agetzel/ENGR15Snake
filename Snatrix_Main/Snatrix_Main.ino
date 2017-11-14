#include <Adafruit_CircuitPlayground.h>

//Snake Class
class Snake 
{
  private: //Set all variables and functions as public
  //2D array of int row and column coordinates of the snake 
  int location[256][2]; 
  //Integer variable pointing to the tail of the snake (pointer to the location 2D array)
  int tail; 
  //Integer variable pointing to the head of the snake (pointer to the location 2D array) 
  int head;
  //Character variable specifying snake direction
  char directionSnake; 

  public:
  //Constructor for Snake
  Snake() {
    //Set initial location of snake to be: (7, 6), (7, 7), (7, 8), (7, 9)
    location[0][0] = 7;
    location[0][1] = 6;
    location[1][0] = 7;
    location[1][1] = 7;
    location[2][0] = 7;
    location[2][1] = 8;
    location[3][0] = 7;
    location[3][1] = 9;

    //Set head and tail pointers 
    tail = 0;
    head = 3;   

    //Set direction to right
    directionSnake = 'r';
  }
  int getTail() {
    return tail;
  }
  int getHead() {
    return head;
  }
  int getLocationRow(int x) {
    return location[x][0];
  }
  int getLocationColumn(int y) {
    return location[y][1];
  }
  char getDirection() {
    return directionSnake;
  }
  setDirection(char dir) {
    if(dir == 'u' || dir || 'd' && dir || 'l' && dir || 'r') {
      directionSnake = dir; 
    }
  }
  bool newLocation() { 
    //Create a outOfBounds checker to be returned
    bool outOfBounds = false;
    //save previous head and tail locations 
    int prevHead = head;
    int prevTail = tail;
    //set new head location
    head = head + 1;
    //set new tail location
    tail = tail + 1;
    //Wrap around the circular array if head > 255 or tail > 255
    if(head > 255) {
      head = 0;
    }
    if(tail > 255) {
      tail > 255;
    }
    //Get previous location
    int newRow = getLocationRow(prevHead);
    int newColumn = getLocationColumn(prevHead); 
    //Update location according to direction
    switch(directionSnake) {
      case 'u': 
        newRow = newRow - 1; 
        break;
      case 'd':
        newRow = newRow + 1;
        break;
      case 'l':
        newColumn = newColumn - 1;
        break;
      case 'r':
        newColumn = newColumn + 1;
        break;
    }
    //Check for out of bounds. Reset if out of bounds.
      if(newRow >= 16) {
        newRow = newRow - 1;
        outOfBounds = true;    
      }
      if(newRow < 0) {
        newRow = newRow + 1; 
        outOfBounds = true;   
      }
      if(newColumn >= 16) {
        newColumn = newColumn - 1;   
        outOfBounds = true; 
      }
      if(newColumn < 0) {
        newColumn = newColumn + 1;  
        outOfBounds = true;  
      }
    //Update new head location
    location[head][0] = newRow;
    location[head][1] = newColumn;
    return outOfBounds;
  }
};

//Declare a 16 by 16 2D array of bools corresponding to the 16 by 16 board
bool board[16][16];

//Define a variable to store current elapsed time
unsigned long currentTime; 
//Define a variable to store time of last board update 
unsigned long previousTime = 0; 
//Define a constant for board update interval
const long interval = 3000;

//Define a snake object
Snake snake;

//Define a boolean to check whether to continue the game
bool runGame;

//Define boolean values for checking if buttons are pressed
bool leftCur;
bool leftPrev;
bool rightCur;
bool rightPrev;

void setup() {
  CircuitPlayground.begin();
  Serial.begin(9600);
  //Populate board and boardStatus
  for(int row = 0; row < 16; row++) {
    for(int column = 0; column < 16; column++) {
      //Initialize entire board as off
      board[row][column] = 0;
    }
  }
  //Initialize a snake object
  snake = Snake();
  
  //Set runGame to be true
  runGame = true;
}

void loop() {
  while(runGame) {
  //Set currentTime equal to millis
  currentTime = millis(); 

  //Assign leftCur to left button input and rightCur to right button input 
  leftCur = CircuitPlayground.leftButton();
  rightCur = CircuitPlayground.rightButton();

  //Check if left button has been pressed and released 
  if(leftCur != leftPrev && leftPrev == 1) {
    //Toggle direction 
    char snakeCurDirection = snake.getDirection();
    char snakeFutureDirection;
    switch(snakeCurDirection) {
      case 'r': 
        snakeFutureDirection = 'u';
        break;
      case 'u':
        snakeFutureDirection = 'l';
        break;
      case 'l':
        snakeFutureDirection = 'd';
        break;
      case 'd':
        snakeFutureDirection = 'r';
        break;
    }
    snake.setDirection(snakeFutureDirection);
    Serial.println(snakeFutureDirection);
  }
  //Check if right button has been pressed and released 
  if(rightCur != rightPrev && rightPrev == 1) {
    //Toggle direction 
    char snakeCurDirection = snake.getDirection();
    char snakeFutureDirection;
    switch(snakeCurDirection) {
      case 'r': 
        snakeFutureDirection = 'd';
        break;
      case 'd':
        snakeFutureDirection = 'l';
        break;
      case 'l':
        snakeFutureDirection = 'u';
        break;
      case 'u':
        snakeFutureDirection = 'r';
        break;
    }
    snake.setDirection(snakeFutureDirection);
    Serial.println(snakeFutureDirection);
  }
  //Set current state as previous state for buttons
  leftPrev = leftCur;
  rightPrev = rightCur;
  
  //Update board for every defined interval of time
  if(currentTime - previousTime >= interval) {
    previousTime = currentTime; 


    //Clear board
    for(int row = 0; row < 16; row++) {
      for(int column = 0; column < 16; column++) {
        board[row][column] = 0;
      }
    }
    
    //Update Snake location on Board 
    for(int coordinate = snake.getTail(); coordinate <= snake.getHead(); coordinate++) {
       int snakeRow = snake.getLocationRow(coordinate);
       int snakeColumn = snake.getLocationColumn(coordinate);
       board[snakeRow][snakeColumn] = 1;
    }
    
    //Print board to Serial
    for(int row = 0; row < 16; row++) {
      for(int column = 0; column < 16; column++) {
        Serial.print(board[row][column]);
      }
      Serial.println("");
    }
    Serial.println("");

    //Update Snake location 
    bool outOfBounds = snake.newLocation();
    if(outOfBounds) {
      Serial.print("\n");
      Serial.print("GAME OVER!");
      runGame = false;
    }
  }
  
  }
}
