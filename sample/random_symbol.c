#include <stdio.h>
#include <string.h>  // strlen 関数のために必要
#include <stdlib.h>
#include <time.h>

char symbols[] = "~`!@#$%^&*()-_=+[]{}|\\;:\'\",.<>?/";

int main() {
    srand(time(NULL));
    int length = strlen(symbols);  // ヌル終端文字を除いた長さを取得
    int index;

    printf("Press Enter to display a random symbol, any other key to exit.\n");

    printf("length: %d\n", length);
    while (1) {
        int ch = getchar();
        if (ch == '\n') {
            index = rand() % length;
            char symbol = symbols[index];
            printf("symbol: %c\n", symbol);
        } else {
            break;
        }
    }

    return 0;
}

