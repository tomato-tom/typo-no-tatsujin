// 時間計測

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <sys/time.h>

#define ROUNDS 8
#define MAX_LENGTH 5
#define CLEAR printf("\033[2J\033[H");  // 画面をクリア

char get_random_char() {
    int choice = rand() % 4;
    char symbols[] = "~`!@#$%^&*()-_=+[]{}|\\;:\'\",.<>?/";
    int length = strlen(symbols);

    switch(choice) {
        case 0: return 'a' + (rand() % 26);
        case 1: return 'A' + (rand() % 26);
        case 2: return '0' + (rand() % 10); // 数字を文字として
        case 3: return symbols[rand() % length];
    }
    return 'a';
}

void get_random_string(char *str, int length) {
    for (int i = 0; i < length; i++) {
        str[i] = get_random_char();
    }
    str[length] = '\0';
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

    CLEAR;
    printf("アルファベット・数字・記号タイピングゲームを始めます！\n");
    printf("%dラウンド、1から8文字のランダム\n", ROUNDS);
    printf("Enterキーを押すとゲームが開始します。\n");

    while (1) {
        char ch = getch();
        if (ch == '\n' || ch == '\r') {
            break;
        }
    }

    int char_count = 0;
    for (int i = 0; i < ROUNDS; i++) {
        CLEAR;

        printf("ラウンド: %d / %d\n", i + 1, ROUNDS);
        printf("現在のスコア: %d\n\n", score);

        int length = 1 + (rand() % MAX_LENGTH);
        char_count += length;
        char target[MAX_LENGTH + 1];
        get_random_string(target, length);

        printf("タイプする文字列:\n%s\n", target);
        fflush(stdout);

        double start_time = get_time();
        char input[MAX_LENGTH + 1];
        for (int j = 0; j < length; j++) {
            input[j] = getch();
            printf("%c", input[j]);
        }
        input[length] = '\0';
        double end_time = get_time();

        double round_time = end_time - start_time;
        total_time += round_time;

        printf("\n\n");

        if (strcmp(input, target) == 0) {
            printf("正解！ 所要時間: %.3f秒\n", round_time);
            score++;
        } else {
            printf("不正解。正しい文字は '%s' でした。\n", target);
        }

        usleep(500000);
    }

    CLEAR;
    double average_time = total_time / char_count;
    printf("\nゲーム終了！あなたのスコアは %d / %d です。\n", score, ROUNDS);
    printf("平均反応時間: %.3f秒\n", average_time);

    return 0;
}
