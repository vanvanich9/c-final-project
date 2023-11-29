# Battleship game


## Program structure

The program first displays a menu to the user to select an action and waits for one of three actions.
- If 1 is selected, the game starts and continues until either the user or the computer wins, and until the user chooses to end the game themselves. In addition, the user can either save the game or exit without saving. If the user chooses to save the game, the program prompts for the player's name and save name. Everything is saved in the format ```/saves/<Player name>/<Save name>```.
- If 2 was selected, then it displays another menu for loading games, there you can display all saved games with command 1, with command 2 you can display a specific save game, with command 3 you can display fields from a specific save, with command 4 you can load the game and it will be the process that was described when selecting item 1 in the main menu, and using command 5 to return to the main menu.
- If 3 is selected, the program exits.

## Key functions

### int generate_field(char *field[8][8])
- Takes in value an array into which the playing field will be saved. Generation occurs of five ships of lengths 5, 4, 3, 3 and 2 in all directions. Based on the result, the function returns the number of generated ships.


### void play_with_computer(int state, char *first_player_field[8][8], char *second_player_field[8][8], int *first_player_ships, int *second_player_ships)
- This function simulates the entire game process between the player and the computer. It monitors the situation on the fields, regulates the actions of the user and the computer, and records the result when the game ends. The parameters are the number of the game's turn, the players' fields and their remaining number of ships. Also, the function is able to start the game from any point, everything also depends on the transferred data.


### void load_game();
- Responsible for the menu for loading saved games. It can display the entire list of saved games, as well as for each user separately. Also, it can show the fields in the game selected by the user, as well as load a specific game.