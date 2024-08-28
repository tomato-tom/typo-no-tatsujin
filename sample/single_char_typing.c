#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <sys/time.h>

#define ROUNDS 30

char get_random_char() {
    int choice = rand() % 3;
    
    switch(choice) {
        case 0: return 'a' + (rand() % 26);
        case 1: return 'A' + (rand() % 26);
        case 2:
            if (rand() % 2) {
                return '0' + (rand() % 10);
            } else {
                char symbols[] = "~`!@#$%^&*()-_=+[]{}|\\;:\'\",.<>?/";
                return symbols[rand() % strlen(symbols)];
            }
    }
    return 'a';
}

char getch() {
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if(tcgetattr(0, &old) < 0) perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if(tcsetattr(0, TCSANOW, &old) < 0) perror("tcsetattr ICANON");
    if(read(0, &buf, 1) < 0) perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if(tcsetattr(0, TCSADRAIN, &old) < 0) perror("tcsetattr ~ICANON");
    return buf;
}

double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}


int main() {
    srand(time(NULL));
    int score = 0;
    double total_time = 0.0;

    printf("拡張アルファベット・数字・記号タイピングゲームを始めます！\n");
    printf("%d回のラウンドがあります。表示された文字をタイプしてください\n\n", ROUNDS);

    for (int i = 0; i < ROUNDS; i++) {
        char target = get_random_char();
        printf("ラウンド %d => %c ", i + 1, target);
        fflush(stdout);  // バッファを即座に出力

        double start_time = get_time();
        char input = getch();
        double end_time = get_time();
        printf("%c\n", input);  // 入力した文字を表示


        double round_time = end_time - start_time;
        total_time += round_time;

        if (input == target) {
            printf("正解！ 所要時間: %.3f秒\n", round_time);
            score++;
        } else {
            printf("不正解。正しい文字は '%c' でした。\n", target);
        }
    }

    double average_time = total_time / ROUNDS;
    printf("\nゲーム終了！あなたのスコアは %d / %d です。\n", score, ROUNDS);
    printf("平均反応時間: %.3f秒\n", average_time);

    return 0;
}
