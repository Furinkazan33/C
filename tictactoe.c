/* Programm name : Tictactoe.c */

#include <stdio.h>
#include <stdlib.h>

#define CHECK_LINES(max, tab, i, n) for (i = 0; i < max; i++) { res = check_line(tab[i], n); if (res != 0) { return res; } }
#define COLS 4
#define LINES 4
#define N_PLAYERS 2
#define N_WIN 3

const int tokens[] = { 100, 200, 300 };
const char *players[] = { "Player 1", "Player 2", "Player 3" };


int get_player_from_token(int token) {
    int i;

    for (i = 0; i < N_PLAYERS; i++) {
        if (token == tokens[i]) {
            return i + 1;
        }
    }
    return 0;
}

/* Returns winning player's number, or 0 */
int check_line(int line[], int n) {
    int i, cpt;
    int token = *line;

    for(cpt = 0, i = 0; cpt < N_WIN && i < n; i++) {
        if (line[i] == token) {
            cpt++;
            if (cpt >= N_WIN) {
                return get_player_from_token(token);
            }
        } else {
            token = line[i];
            cpt = 1;
        }
    }

    return 0;
}

/* Returns winning player's number, or 0 */
int check_lines(int board[LINES][COLS]) {
    int i;
    int res;

    CHECK_LINES(LINES, board, i, COLS);

    return 0;
}

/* Returns winning player's number, or 0 */
int check_cols(int board[LINES][COLS]) {
    int reverse[COLS][LINES];
    int l, c, res;

    /* reversing */
    for(l = 0; l < LINES; l++) {
        for(c = 0; c < COLS; c++) {
            reverse[c][l] = board[l][c];
        }
    }

    CHECK_LINES(COLS, reverse, c, LINES);

    return 0;
}

/* Returns winning player's number, or 0 */
int check_diags() {
    return 0;
}


void print_board(int board[LINES][COLS]) {
    int i, j;

    for (i = 0; i < LINES; i++) {
        for (j = 0; j < COLS; j++) {
            printf("| %-4d ", board[i][j]);
        }
        printf("|\n");
    }
}

int get_line_from_choice(int choice) {
    if (choice % COLS == 0) {
        return choice / COLS - 1;
    }
    return choice / COLS;
}

int get_col_from_choice(int choice) {
    if (choice % COLS == 0) {
        return COLS - 1;
    }
    return choice % COLS - 1;
}

int get_player_from_turn(int turn) {
    int r = turn % N_PLAYERS;

    return r == 0 ? N_PLAYERS : r;
}

int main(void) {
    int board[LINES][COLS];
    int l, c, cpt;

    for (cpt = 1, l = 0; l < LINES; l++) {
        for (c = 0; c < COLS; c++) {
            board[l][c] = cpt++;
        }
    }

    int i, winner, player, turn, choice;
    int token;

    for (turn = 1, winner = 0; turn <= LINES * COLS && winner == 0; turn++) {
        player = get_player_from_turn(turn);
        token = tokens[player - 1];

        print_board(board);

        printf("Votre choix : ");
        scanf("%d", &choice);

        int choice_c = get_col_from_choice(choice);
        int choice_l = get_line_from_choice(choice);

        board[choice_l][choice_c] = token;

        winner = check_lines(board) || check_cols(board) || check_diags(board);
    }

    print_board(board);

    if (winner != 0) {
        printf("Player %d is winner !\n", winner);
    }

    return 0;
}

