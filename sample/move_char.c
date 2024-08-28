#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>

#define ROUNDS 10
#define WIDTH 30  // 画面幅
#define HIDE_CURSOR printf("\033[?25l")
#define SHOW_CURSOR printf("\033[?25h")

typedef struct {
    int score;
    char target;
    int position;
    int speed;
    int round;
} Game;

typedef struct {
    double start_time;
    double current_time;
    double round_time;
    double elapsed_time;
    double average_time;
    double total_time;
    int flag;
} Timer;

struct termios orig_termios;

void move_cursor(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1);
}

void clear_screen() {
    printf("\033[2J\033[H");  // 画面をクリアし、カーソルをホーム位置に移動
}

void clear_line(int line) {
    move_cursor(1, line);
    printf("\033[2K");
}

void disableEcho() {
    struct termios new_termios;

    tcgetattr(STDIN_FILENO, &orig_termios);
    new_termios = orig_termios;

    new_termios.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}

void restoreTerminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

int kbhit() {
    int ch;
    int oldf;

    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

void sleep_ms(int milliseconds) {
    usleep(milliseconds * 1000);
}

void beep(int count) {
    for (int i = 0; i < count; i++) {
        printf("\a");
        fflush(stdout);
        sleep_ms(200);
    }
}

char get_random_char() {
    char symbols[] = "~`!@#$%^&*()-_=+[]{}|\\;:\'\",.<>?/";
    int choice = rand() % 4;
    
    switch(choice) {
        case 0: return 'a' + (rand() % 26);
        case 1: return 'A' + (rand() % 26);
        case 2:
            return '0' + (rand() % 10);
        case 3:
            return symbols[rand() % strlen(symbols)];
    }
    return 'a';
}

void start_round(Game *game) {
    printf("Typing Game\n");
    printf("Round %d", game->round);
    game->target = get_random_char();
    game->position = 0;
    game->speed = 90;
}

double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

void start_timer(Timer *timer) {
    timer->start_time = get_time();
    timer->current_time = 0;
    timer->round_time = 0;
    timer->elapsed_time = 0;
    timer->flag = 0;
}

void update_timer(Timer *timer, int round) {
    timer->current_time = get_time();
    if (!timer->flag) {
        timer->elapsed_time = timer->current_time - timer->start_time;
    }

    double total = timer->total_time + timer->elapsed_time;
    timer->average_time = total / round;
}

int main() {
    HIDE_CURSOR;
    disableEcho();
    srand(time(NULL));

    Game game = {0};  // Game構造体のインスタンスを初期化
    Timer timer = {0};  // Timer構造体のインスタンスを初期化

    clear_screen();
    printf("タイピングゲームを始めます！\n");
    printf("%d回のラウンドがあります。動く文字をタイプしてください\n\n", ROUNDS);
    sleep_ms(1000);  // 1秒間表示したままにする

    for (int i = 0; i < ROUNDS; i++) {
        clear_screen();
        char *emoji = "";
        game.round = i + 1;

        start_round(&game);
        start_timer(&timer);

        move_cursor(1, 10);
        
        while (game.position < WIDTH) {
            move_cursor(game.position, 4);

            if (game.position < (WIDTH - 1)) {
                if (emoji[0] == '\0') {
                    printf("%c", game.target);
                } else {
                    printf("%s", emoji);
                    game.speed *= 0.9;
                }
            }

            update_timer(&timer, game.round);

            move_cursor(1, 7);
            printf("time: %.3f  avg: %.3f\n", timer.elapsed_time, timer.average_time);

            fflush(stdout);

            if (kbhit()) {
                char key = getchar();
                update_timer(&timer, game.round);
                timer.round_time = timer.elapsed_time;
                timer.flag = 1;

                beep(1);

                if (key == game.target) {
                    move_cursor(1, 9);
                    printf("正解！ 所要時間: %.2f秒\n", timer.round_time);

                    emoji = "😀";
                    game.score++;
                } else {
                    move_cursor(1, 9);
                    printf("不正解。正しい文字は %c でした。\n", game.target);
                    emoji = "😭";
                }
            }

            sleep_ms(game.speed);  // 文字の移動速度調整
            move_cursor(game.position, 4);
            printf(" ");
            game.position++;
        } 

        if (!timer.flag) {
            update_timer(&timer, game.round);
            timer.round_time = timer.elapsed_time;

            beep(3);
            move_cursor(1, 9);
            printf("時間切れ...\n");
        }

        clear_line(7);
        timer.total_time += timer.round_time;
        timer.average_time = timer.total_time / game.round;
        sleep_ms(1000);  // 1秒間表示したままにする
        tcflush(STDIN_FILENO, TCIFLUSH); // 入力バッファをクリア

    } 

    clear_line(2);
    move_cursor(1, 9);
    printf("ゲーム終了！あなたのスコアは %d / %d です。\n", game.score, ROUNDS);
    printf("平均反応時間: %.3f秒\n", timer.average_time);

    SHOW_CURSOR;
    restoreTerminal();

    return 0;
}

