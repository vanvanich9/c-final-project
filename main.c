#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

int choose_game();
int generate_field(char *field[8][8]);
void play_with_computer(int state, char *first_player_field[8][8], char *second_player_field[8][8], int *first_player_ships, int *second_player_ships);
void load_game();

int state_in_game;

int main() {
    time_t t;
    srand((unsigned) time(&t));

    char *first_player_field[8][8];
    char *second_player_field[8][8];
    int first_player_ships;
    int second_player_ships;
    int active_menu = 1;
    while(active_menu) {
        int option = choose_game();
        switch (option) {
            case 1:
                first_player_ships = generate_field(first_player_field);
                second_player_ships = generate_field(second_player_field);
                play_with_computer(0, first_player_field, second_player_field, &first_player_ships, &second_player_ships);
                break;
            case 2:
                load_game();
                break;
            case 3:
                active_menu = 0;
                printf("See you later!\n");
                break;
        }
    }
    return 0;
}


int choose_game() {
    int option;
    printf("------------------------\n");
    printf("1. Play a new game\n");
    printf("2. Load already saved game\n");
    printf("3. Exit the game\n");
    printf("Choose one of this number: ");
    scanf("%d", &option);
    while(option < 1 || option > 3) {
        printf("Don't understand your option. Choose between 1 and 3: ");
        scanf("%d", &option);
    }
    return option;
}


char* concat(char *s1, char *s2) {
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}


int add_ship(char *field[8][8], int ship_lenght, char letter) {
    // Select one of four directions
    int direction = rand() % 4;
    int x, y;
    int state = 1;
    switch (direction) {
        // direction to 0 hours
        case 0:
            x = rand() % 8;
            y = rand() % (8 - ship_lenght + 1);
            for(int step = 0; step < ship_lenght; step++) {
                if(field[y + step][x] != '.') {
                    state = 0;
                }
            }
            if(state) {
                for(int step = 0; step < ship_lenght; step++) {
                    field[y + step][x] = letter;
                }
            }
            break;
        // direction to 3 hours
        case 1:
            x = rand() % (8 - ship_lenght + 1);
            y = rand() % 8;
            for(int step = 0; step < ship_lenght; step++) {
                if(field[y][x + step] != '.') {
                    state = 0;
                }
            }
            if(state) {
                for(int step = 0; step < ship_lenght; step++) {
                    field[y][x + step] = letter;
                }
            }
            break;
        // direction to 6 hours
        case 2:
            x = rand() % 8;
            y = (ship_lenght - 1) + rand() % (8 - ship_lenght + 1);
            for(int step = 0; step < ship_lenght; step++) {
                if(field[y - step][x] != '.') {
                    state = 0;
                }
            }
            if(state) {
                for(int step = 0; step < ship_lenght; step++) {
                    field[y - step][x] = letter;
                }
            }
            break;
        // direction to 9 hours
        case 3:
            x = (ship_lenght - 1) + rand() % (8 - ship_lenght + 1);
            y = rand() % 8;
            for(int step = 0; step < ship_lenght; step++) {
                if(field[y][x - step] != '.') {
                    state = 0;
                }
            }
            if(state) {
                for(int step = 0; step < ship_lenght; step++) {
                    field[y][x - step] = letter;
                }
            }
            break;
        }
    return state;
}


int generate_field(char *field[8][8]) {
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            field[i][j] = '.';
        }
    }
    int cnt = 0;
    // Aircraft Carrier
    while(cnt < 1) {
        int state = add_ship(field, 5, 'A');
        if(state) cnt++;
    }
    cnt = 0;
    // Battleship
    while(cnt < 1) {
        int state = add_ship(field, 4, 'B');
        if(state) cnt++;
    }
    cnt = 0;
    // Cruiser
    while(cnt < 1) {
        int state = add_ship(field, 3, 'C');
        if(state) cnt++;
    }
    cnt = 0;
    // Submarine
    while(cnt < 1) {
        int state = add_ship(field, 3, 'S');
        if(state) cnt++;
    }
    cnt = 0;
    // Destroyer
    while(cnt < 1) {
        int state = add_ship(field, 2, 'D');
        if(state) cnt++;
    }

    return 5;
}


void print_full_field(char *field[8][8]) {
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            if (!i && !j) printf("  ");
            // For numbers on field
            else if (!i) printf("%d ", j);
            // For letters on field
            else if (!j) printf("%c ", i + 'A' - 1);
            else printf("%c ", field[i - 1][j - 1]);
        }
        printf("\n");
    }
}


void print_not_full_field(char *field[8][8]) {
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            if (!i && !j) printf("  ");
            // For numbers on field
            else if (!i) printf("%d ", j);
            // For letters on field
            else if (!j) printf("%c ", i + 'A' - 1);
            // For hidding ships
            else if (field[i - 1][j - 1] == 'A' || field[i - 1][j - 1] == 'B' || field[i - 1][j - 1] == 'C' || field[i - 1][j - 1] == 'D' || field[i - 1][j - 1] == 'S') printf(". ");
            else printf("%c ", field[i - 1][j - 1]);
        }
        printf("\n");
    }
}


void check_situation(char *field[8][8], int *ships, int y, int x) {
    // Check status of ship in four directions
    // If not will be find same letter of ship's letter - return that ship destroyed
    char letter = field[y][x];
    int state = 1;
    int tmp_y, tmp_x;

    tmp_y = y - 1;
    while(tmp_y >= 0 && field[tmp_y][x] == 'X') tmp_y--;
    if(tmp_y >= 0 && field[tmp_y][x] == letter) state = 0;

    tmp_y = y + 1;
    while(tmp_y < 8 && field[tmp_y][x] == 'X') tmp_y++;
    if(tmp_y < 8 && field[tmp_y][x] == letter) state = 0;

    tmp_x = x - 1;
    while(tmp_x >= 0 && field[y][tmp_x] == 'X') tmp_x--;
    if(tmp_x >= 0 && field[y][tmp_x] == letter) state = 0;

    tmp_x = x + 1;
    while(tmp_x < 8 && field[y][tmp_x] == 'X') tmp_x++;
    if(tmp_x < 8 && field[y][tmp_x] == letter) state = 0;

    if(state) {
        --(*ships);
        printf("DESTROYED!\n");
    }
}


int shot(char *field[8][8], int *ships, int y, int x) {
    if(field[y][x] == '.') {
        field[y][x] = 'O';
        printf("MISS!\n");
        return 0;
    }
    printf("HIT!\n");
    check_situation(field, ships, y, x);
    field[y][x] = 'X';
    return 1;
}


int convert_letter(char letter) {
    return letter - 'A';
}


int convert_number(char number) {
    return number - '1';
}


int check_available(char *field[8][8], int y, int x) {
    // Check that position not shotted yet
    if(field[y][x] == 'O' || field[y][x] == 'X')
        return 0;
    else
        return 1;
}


void save_game(int state, char *first_player_field[8][8], char *second_player_field[8][8], int *first_player_ships, int *second_player_ships) {
    mkdir("saves", 0777);
    FILE *fptr;
    char dirname[64], savename[64];
    printf("Write player name: ");
    scanf("%s", dirname);
    mkdir(concat("saves/", dirname), 0777);
    printf("Write save name: ");
    scanf("%s", savename);

    fptr = fopen(concat("saves/", concat(concat(dirname, "/"), savename)), "w");
    // Write all data in file
    fprintf(fptr, "%d\n", state - 1);
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            fprintf(fptr, "%c", first_player_field[i][j]);
        }
    }
    fprintf(fptr, "\n%d\n", *first_player_ships);
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            fprintf(fptr, "%c", second_player_field[i][j]);
        }
    }
    fprintf(fptr, "\n%d\n", *second_player_ships);
    fclose(fptr);
}


void player_turn(char *field[8][8], int *ships) {
    int is_player_turn = 1;
    printf("Current situation:\n");
    print_not_full_field(field);
    printf("Remaining ships: %d\n", *ships);
    while (is_player_turn && *ships) {
        printf("------------------------------------\n");
        printf("1. Exit from game without save\n");
        printf("2. Exit from game with save\n");
        printf("Either, write position like A1\n");
        printf("Choose one of this option: ");

        // Read option
        char buf[2];
        scanf("%s", &buf);
        if(!strcmp(buf, "1")) {
            state_in_game = 1;
            printf("Exit from game without save\n");
            break;
        }
        if(!strcmp(buf, "2")) {
            state_in_game = 2;
            printf("Exit from game with save\n");
            break;
        }

        // Convert letter and number to coordinates
        int y = convert_letter(buf[0]);
        int x = convert_number(buf[1]);

        if(0 <= x && x < 8 && 0 <= y && y < 8) {
            if(!check_available(field, y, x)) {
                printf("This position was shotted early. Please, choose another\n");
                is_player_turn = 1;
                continue;
            }
        } else {
            printf("Incorrect input. Please, write correct\n");
            is_player_turn = 1;
            continue;
        }

        // If shot was success, wiil be return 1 and loop continue work for player.
        // Else turn will be change to computer
        is_player_turn = shot(field, ships, y, x);
        print_not_full_field(field);
        printf("Remaining ships: %d\n", *ships);
    }
}


void computer_turn(char *field[8][8], int *ships) {
    int is_computer_turn = 1;
    while (is_computer_turn && *ships) {
        // Randomize coorinated
        int y = rand() % 8;
        int x = rand() % 8;

        if (!check_available(field, y, x)) {
            is_computer_turn = 1;
            continue;
        }

        // If shot was success, wiil be return 1 and loop continue work for computer.
        // Else turn will be change to player
        is_computer_turn = shot(field, ships, y, x);
        print_full_field(field);
        printf("Remaining ships: %d\n", *ships);
    }
}


void play_with_computer(int state, char *first_player_field[8][8], char *second_player_field[8][8], int *first_player_ships, int *second_player_ships) {
    // State that need stop game, save game or continue to play
    state_in_game = 0;
    while (*first_player_ships && *second_player_ships && !state_in_game) {
        if(state++ % 2 == 0) {
            printf("YOUR TURN!\n");
            player_turn(second_player_field, second_player_ships);
        } else {
            printf("COMPUTER TURN!\n");
            computer_turn(first_player_field, first_player_ships);
        }
    }
    if(state_in_game == 1) {
        return;
    } else if(state_in_game == 2) {
        save_game(state, first_player_field, second_player_field, first_player_ships, second_player_ships);
    } else if(*first_player_ships == 0) {
        printf("COMPUTER WIN!\n");
    } else {
        printf("YOU WIN!\n");
    }
}


void read_game(char* filename, int option) {
    FILE *fptr;
    fptr = fopen(concat("saves/", filename), "r");

    printf("\n");
    if(!fptr) {
        printf("This save not found!\n");
        return;
    }

    int state;
    int first_player_ships;
    int second_player_ships;
    char *first_player_field[8][8];
    char *second_player_field[8][8];

    // Read data from file
    fscanf (fptr, "%d\n", &state);
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            first_player_field[i][j] = getc(fptr);
        }
    }
    fscanf (fptr, "\n%d\n", &first_player_ships);
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            second_player_field[i][j] = getc(fptr);
        }
    }
    fscanf (fptr, "\n%d", &second_player_ships);

    if(option == 3) {
        printf("Your field:\n");
        print_full_field(first_player_field);
        printf("Remaining ships: %d\n", first_player_ships);
        printf("\n");
        printf("Computer field:\n");
        print_not_full_field(second_player_field);
        printf("Remaining ships: %d\n", second_player_ships);
    }
    if(option == 4) {
        play_with_computer(state, first_player_field, second_player_field, &first_player_ships, &second_player_ships);\
    }
}


int load_game_menu() {
    int option;
    printf("------------------------\n");
    printf("1. List all saved games\n");
    printf("2. List all saved games for a particular player\n");
    printf("3. Show the board of one of the saved games\n");
    printf("4. Load a game\n");
    printf("5. Return to main menu\n");
    printf("Choose one of this number: ");
    scanf("%d", &option);
    while(option < 1 || option > 5) {
        printf("Don't understand your option. Choose between 1 and 5: ");
        scanf("%d", &option);
    }
    return option;
}


void load_games_from_dir(char* dirname) {
    int count_saves = 0;
    DIR *d;
    struct dirent *dir;
    d = opendir(concat("saves/", dirname));
    if (d) {
        dir = readdir(d);
        // Read and write save names in player folder
        while (dir != NULL) {
            if(strcmp(dir->d_name, ".") && strcmp(dir->d_name, "..")) {
                printf("    Save name: %s\n", dir->d_name);
                count_saves++;
            }
            dir = readdir(d);
        }
    }
    if(count_saves == 0) {
        printf("    No one saving for this player not found!\n");
    }
}


void load_dirs() {
    int count_dirs = 0;
    DIR *d;
    struct dirent *dir;
    d = opendir("saves");
    if (d) {
        dir = readdir(d);
        // Read and write player names
        while (dir != NULL) {
            if(strcmp(dir->d_name, ".") && strcmp(dir->d_name, "..")) {
                printf("Player: %s\n", dir->d_name);
                load_games_from_dir(dir->d_name);
                count_dirs++;
            }
            dir = readdir(d);
        }
    }
    if(count_dirs == 0) {
        printf("No one saving not found!\n");
    }
}


void load_game() {
    int state_menu = 1;
    while(state_menu) {
        int option = load_game_menu();
        char dirname[64];
        char savename[64];
        switch (option) {
            case 1:
                printf("\n");
                load_dirs();
                break;
            case 2:
                printf("Write player name: ");
                scanf("%s", dirname);
                printf("\n");
                printf("Player: %s\n", dirname);
                load_games_from_dir(dirname);
                break;
            case 3:
                printf("Write player name: ");
                scanf("%s", dirname);
                printf("Write save name: ");
                scanf("%s", savename);
                read_game(concat(concat(dirname, "/"), savename), option);
                break;
            case 4:
                printf("Write player name: ");
                scanf("%s", dirname);
                printf("Write save name: ");
                scanf("%s", savename);
                read_game(concat(concat(dirname, "/"), savename), option);
                break;
            case 5:
                state_menu = 0;
                break;
        }
    }
}
