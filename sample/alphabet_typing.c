// 小文字のアルファベットのみ、タイピングゲーム

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define ROUNDS 10

int main() {
    srand(time(NULL));
    int score = 0;

    printf("アルファベットタイピングゲームを始めます！\n");
    printf("%d回のラウンドがあります。表示された文字をタイプしてEnterを押してください。\n\n", ROUNDS);

    for (int i = 0; i < ROUNDS; i++) {
        char target = 'a' + (rand() % 26);
        char input;

        printf("ラウンド %d: タイプする文字は '%c' です: ", i + 1, target);
        scanf(" %c", &input);

        if (tolower(input) == target) {
            printf("正解！\n");
            score++;
        } else {
            printf("不正解。正しい文字は '%c' でした。\n", target);
        }
    }

    printf("\nゲーム終了！あなたのスコアは %d / %d です。\n", score, ROUNDS);

    return 0;
}
