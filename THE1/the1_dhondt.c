
    
    #include <stdio.h>

int main() {
    int n, m,i,j;
    int votes[50];
    int seats[50];
    char temp_char;
    char parties[50];
    scanf("%d %d", &n, &m);



    for (i = 0; i < n; i++) {
        scanf("%d", &votes[i]);
        parties[i] = 'A' + i;
    }


    for ( i = 0; i < n; i++) {
        seats[i] = 0;
    }

    for ( i = 0; i < m; i++) {
        float max_quotient = 0;
        int max_index = -1;
        for ( j = 0; j < n; j++) {
            float quotient =  votes[j] / (seats[j] + 1.0);
            if (quotient > max_quotient) {
                max_quotient = quotient;
                max_index = j;
            }
            else if (quotient == max_quotient && votes[j] > votes[max_index]) {
                max_index = j;
            }
        }
        seats[max_index]++;
    }


    for ( i = 0; i < n; i++) {
        for ( j = i+1; j < n; j++) {
            if (seats[i] < seats[j]) {
                int temp = seats[i];
                seats[i] = seats[j];
                seats[j] = temp;

                 temp_char = parties[i];
                parties[i] = parties[j];
                parties[j] = temp_char;
            }
            else if (seats[i] == seats[j] && parties[i] > parties[j]) {
                 temp_char = parties[i];
                parties[i] = parties[j];
                parties[j] = temp_char;
            }
        }
    }


    for ( i = 0; i < n; i++) {
        if (seats[i] > 0) {
            printf("%c: %d\n", parties[i], seats[i]);
        }
    }

    return 0;
}

    
    
   
