#include <stdio.h>

double calculate_pressure_on_block(int row, int column, double block_weight);
void printer(int N, double block_weight, int i, int j);

int main() {
    int N;
    double W;
   
    scanf("%d %lf", &N, &W);
    printer(N, W, 0, 0);

    return 0;
}

double calculate_pressure_on_block(int row, int column, double block_weight) {
    long double upper_left_pressure = 0.0;
    long double upper_right_pressure = 0.0;

    if (row == 0 && column == 0) {
        return 0.0;
    }

    
    if (column > 0) {
        upper_left_pressure = calculate_pressure_on_block(row - 1, column - 1, block_weight) * 0.15;
    }

    if (column < row) {
        upper_right_pressure = calculate_pressure_on_block(row - 1, column, block_weight) * 0.15;
    }

    if (column == 0 || column == row) {
        return block_weight / 2 + upper_left_pressure + upper_right_pressure;
    } else {
        return block_weight + upper_left_pressure + upper_right_pressure;
    }
}

void printer(int N, double block_weight, int i, int j) {
    if (i < N) {
        if (j <= i) {
            if (j <= i / 2) {
               
                printf("(%d,%d) %.8f\n", i, j, calculate_pressure_on_block(i, j, block_weight));
            } else {
                printf("(%d,%d) %.8f\n", i, j, calculate_pressure_on_block(i, i-j, block_weight));
            }
            printer(N, block_weight, i, j + 1);
        } else {
            printer(N, block_weight, i + 1, 0);
        }
    }
}
