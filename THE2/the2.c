#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 100000

char **labyrinth;
int rows, cols;
int start_x, start_y;
int found_way_out = 0;
int dot=0;
int outout=0;
int way_count=0;

int explore(int x, int y) {
    if (x < 0 || x >= rows  || y < 0 || y >= cols || labyrinth[x][y] == 'X' || labyrinth[x][y] == '*'||way_count >= 1) {
        return 0;
    }

    if ((x == 0 || x == rows - 1 || y == 0 || y == cols - 1) && labyrinth[x][y] == ' ') {
        labyrinth[x][y] = '*';
         found_way_out = 1;
         way_count++;
        return 1;
    }

    labyrinth[x][y] = '*';
    

   found_way_out = explore(x - 1, y) || explore(x + 1, y) || explore(x, y - 1) || explore(x, y + 1);


    if (!found_way_out    ) {
        labyrinth[x][y] = '.';
    }

    return found_way_out;
}

int asteriksfinder() {
    int i, j;
    int dot = 0; 
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (labyrinth[i][j] == '*') {
                dot = 1;
                break;
            }
        }
        if (dot == 1) {  
            break;
        }
    }

    return dot;
}


void doteraser() {
    int i, j;
    int dot = asteriksfinder(); 
    
    if (dot == 1) {
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols; j++) {
                if (labyrinth[i][j] == '.') {
                    labyrinth[i][j] = ' ';
                }
            }
        }
    }
}
int dotfinder() {
    int i;
    int outout = 0; 
    for (i = 0; i < rows; i++) {
        
            if (labyrinth[i][cols-1] == '.') {
                outout = 1;
                break;
            }
        
        if (outout == 1) {  
            break;
        }
    }

    return outout;
}
void dotchanger() {
    int i, j;
    int outout= dotfinder(); 
    
    if (outout == 1) {
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols; j++) {
                if (labyrinth[i][j] == '.') {
                    labyrinth[i][j] = '*';
                }
            }
        }
    }
}


int main() {
    int i = 0;
    char c;
    scanf("%d %d", &start_y, &start_x);

    scanf("%c", &c);

    labyrinth = (char **)malloc(MAX_SIZE * sizeof(char *));
    labyrinth[0] = (char *)malloc(MAX_SIZE * sizeof(char));

    while (fgets(labyrinth[i], MAX_SIZE, stdin) != NULL) {
        labyrinth[i][strcspn(labyrinth[i], "\n")] = '\0';
        i++;
        labyrinth[i] = (char *)malloc(MAX_SIZE * sizeof(char));
    }

    rows = i;
    cols = strlen(labyrinth[0]);

    explore(start_x, start_y);
    doteraser();
    dotchanger();

    for (i = 0; i < rows; i++) {
        printf("%s\n", labyrinth[i]);
        free(labyrinth[i]);
    }
    free(labyrinth);

    return 0;
}