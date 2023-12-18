#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>

#define HEIGHT 25
#define WIDTH 25
#define MAX_TAIL_LENGTH 100

struct Player {
    char name[50];
    int score;
};

int x, y, fruitX, fruitY, flag, gameOver, score;
int* tailX;
int* tailY;
int tailLength = 0;

// Function prototypes for file handling
void saveHighScore(struct Player player);
void displayHighScores();
void loadHighScores();

// Other function prototypes
void getInput();
void updateGame();
void drawGame();
void initializeGame();
void allocateTailMemory();

int main() {
    // Load high scores from the file
    loadHighScores();

    allocateTailMemory();
    initializeGame();

    while (!gameOver) {
        getInput();
        updateGame();
        drawGame();

        usleep(100000); // Adjust the delay as needed
    }

    printf("Game Over! Your score: %d\n", score);

    struct Player currentPlayer;
    printf("Enter your name: ");
    scanf("%s", currentPlayer.name);
    currentPlayer.score = score;

    // Save the player's information to the file
    saveHighScore(currentPlayer);

    // Display the high scores
    printf("\nHigh Scores:\n");
    displayHighScores();

    // Free the dynamically allocated memory for the snake's tail
    free(tailX);
    free(tailY);

    return 0;
}

// Function to save the player's information to a file
void saveHighScore(struct Player player) {
    FILE* file = fopen("highscores.txt", "a");
    if (file != NULL) {
        fprintf(file, "%s %d\n", player.name, player.score);
        fclose(file);
    } else {
        printf("Error: Could not save high score.\n");
    }
}

// Function to display high scores from the file
void displayHighScores() {
    FILE* file = fopen("highscores.txt", "r");
    if (file != NULL) {
        char name[50];
        int score;

        while (fscanf(file, "%s %d", name, &score) != EOF) {
            printf("%s: %d\n", name, score);
        }

        fclose(file);
    } else {
        printf("Error: Could not display high scores.\n");
    }
}

// Function to load high scores from the file
void loadHighScores() {
    printf("High Scores:\n");
    displayHighScores();
}

// Function to dynamically allocate memory for the snake's tail
void allocateTailMemory() {
    tailX = (int*)malloc(MAX_TAIL_LENGTH * sizeof(int));
    tailY = (int*)malloc(MAX_TAIL_LENGTH * sizeof(int));
}

// Function to get user input
void getInput() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'w':
                flag = 1;
                break;
            case 's':
                flag = 2;
                break;
            case 'a':
                flag = 3;
                break;
            case 'd':
                flag = 4;
                break;
        }
    }
}

// Function to update the game state
void updateGame() {
    int i, prevX, prevY, prev2X, prev2Y;

    prevX = tailX[0];
    prevY = tailY[0];
    tailX[0] = x;
    tailY[0] = y;

    for (i = 1; i <= tailLength; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (flag) {
        case 1:
            x--;
            break;
        case 2:
            x++;
            break;
        case 3:
            y--;
            break;
        case 4:
            y++;
            break;
    }

    if (x == 0 || x == HEIGHT || y == 0 || y == WIDTH) {
        gameOver = 1;
    }

    if (x == fruitX && y == fruitY) {
        fruitX = rand() % (HEIGHT - 2) + 1;
        fruitY = rand() % (WIDTH - 2) + 1;
        tailLength++;
        score++;
    }
}

// Function to draw the game
void drawGame() {
    system("cls");

    for (int i = 0; i <= HEIGHT; i++) {
        for (int j = 0; j <= WIDTH; j++) {
            if (i == 0 || i == HEIGHT || j == 0 || j == WIDTH) {
                printf("*");
            } else if (i == x && j == y) {
                printf("0");
            } else if (i == fruitX && j == fruitY) {
                printf("@");
            } else {
                int isTail = 0;
                for (int k = 0; k <= tailLength; k++) {
                    if (i == tailX[k] && j == tailY[k]) {
                        printf("o");
                        isTail = 1;
                    }
                }
                if (!isTail) {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
    printf("Score %d\n", score);
}

// Function to initialize the game
void initializeGame() {
    x = HEIGHT / 2;
    y = WIDTH / 2;

    fruitX = rand() % (HEIGHT - 2) + 1;
    fruitY = rand() % (WIDTH - 2) + 1;

    gameOver = 0;
    score = 0;
    tailLength = 0;
}
