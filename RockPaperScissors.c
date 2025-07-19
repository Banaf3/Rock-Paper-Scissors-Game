// MINI PROJECT BCI1023 PROGRAMMING TECHNIQUES SEM 2 2023/2024
// DOMAIN: ENTERTAINMENT       TITLE: ROCK PAPER SCISSORS GAME
// PREPARED BY: GROUP 25       SECTION 03A
// PREPARED FOR: DR.ZURIANI

#include <stdio.h>
#include <stdlib.h> // for rand(), srand() and exit()
#include <string.h> // for strcmp() and strcpy()
#include <time.h> // for srand(time(NULL))

#define MAX_USERS 100

int newFile(FILE**, int[MAX_USERS][3], char[MAX_USERS][20], int[MAX_USERS], int*);
void registerLogin(FILE**,int[MAX_USERS][3], char[MAX_USERS][20], int[MAX_USERS], int*, int*, int);
void calcTotalGames(int[MAX_USERS][3], int[MAX_USERS], int);
void showScore(int[MAX_USERS][3], char[MAX_USERS][20], int[MAX_USERS], int);
void printMenu();
int fMenuChoice();
void playersPlay(int*, int*);
int result(int, int, int[MAX_USERS][3], char[MAX_USERS][20], int);
int fLogOut();
int deleteUser(int[MAX_USERS][3], char[MAX_USERS][20], int[MAX_USERS], int*, int);
void updateFile(int[MAX_USERS][3], char[MAX_USERS][20], int[MAX_USERS], int);

int main(){
    char user[MAX_USERS][20];
    int stats[MAX_USERS][3], totalGames[MAX_USERS], userIndex = 0, usersNum = 0, needToLogin, menuChoice, userPlay, compPlay, playAgain, confirmDelete = 0, logOut = 0;
    FILE *statsFile;

    do {
        logOut = 0, confirmDelete = 0;
        needToLogin = newFile(&statsFile, stats, user, totalGames, &usersNum);
        registerLogin(&statsFile, stats, user, totalGames, &userIndex, &usersNum, needToLogin);
        system("cls"); // to clear the screen
        do {
            showScore(stats, user, totalGames, userIndex);
            printMenu();
            menuChoice = fMenuChoice();
            switch (menuChoice){
                case 1:
                    do {
                        playersPlay(&userPlay, &compPlay);
                        playAgain = result(userPlay, compPlay, stats, user, userIndex);
                    } while (playAgain == 1);
                    break;
                case 2:
                    confirmDelete = deleteUser(stats, user, totalGames, &usersNum, userIndex);
                    break;
                case 3:
                    logOut = fLogOut();
                    break;
                case 4:
                    return 0;
                default:
                    system("cls");
                    printf("\nInvalid choice! Try again...\n");
            }
            calcTotalGames(stats, totalGames, usersNum);
            updateFile(stats, user, totalGames, usersNum);
        } while (menuChoice == 1 || (logOut == 0 && confirmDelete == 0));
    } while (logOut == 1 || confirmDelete == 1);
    
    return 0;
}

// a new file and user will be created if there isn't a file
int newFile(FILE **statsFile, int stats[MAX_USERS][3], char user[MAX_USERS][20], int totalGames[MAX_USERS], int *usersNum) {
    int needToLogin = 1; // it means there's a file if it remains 1
    *statsFile = fopen("RPSstats.txt", "r+"); // will be closed in 'registerLogin()' if found
    if (*statsFile == NULL) { // a new file is created if the file doesn't exist
        printf("File not found! Creating new file...\n");
        *statsFile = fopen("RPSstats.txt", "w");
        if (*statsFile == NULL) {
            printf("Error creating new file!\n");
            exit(1);
        }
        printf("Enter the new username: ");
        scanf(" %s", user[0]);
        for (int i = 0; i < 3; i++) {
            stats[0][i] = 0;
        }
        totalGames[0] = 0;
        fprintf(*statsFile, "%s %d %d %d %d\n", user[0], 0, 0, 0, 0);
        fclose(*statsFile); 
        *usersNum = 1;
        needToLogin = 0; // no need to run 'registerLogin()' since a new user is loggedin
        system("cls");
    }
    return needToLogin; // to determine whether 'registerLogin()' need to be run (needToLogin == 1) or not (needToLogin ==0)
}

// to login or register (also to store the file contents in arrays)
void registerLogin(FILE **statsFile, int stats[MAX_USERS][3], char user[MAX_USERS][20], int totalGames[MAX_USERS], int *index, int *usersNum, int needToLogin) {
    char enteredUser[20]; // the username entered by the user
    int userFound = 0, choice;

    while(needToLogin == 1){ // if the file exists
        needToLogin = 0;
        printf("\nEnter user (it will be registered if it's new): ");
        scanf(" %s", enteredUser);

        rewind(*statsFile); // to read from the beginning

        // looking for the entered username and saving the file's content in the arrays on the way :)
        for(*usersNum = 0; fscanf(*statsFile, "%s %d %d %d %d", user[*usersNum], &stats[*usersNum][0], &stats[*usersNum][1], &stats[*usersNum][2], &totalGames[*usersNum]) != EOF; (*usersNum)++){
            if(strcmp(enteredUser, user[*usersNum]) == 0){
                *index = *usersNum;
                userFound = 1;
            }
        }

        if(userFound == 0){ // if the user is not found, ask whether to add the username as a new user
            printf("User not found! Do you want to add %s as a new user?\n1. Yes\n2. No\nEnter a number: ", enteredUser);
            do {
                needToLogin = 0;
                scanf(" %d", &choice);

                if(choice == 1){
                    strcpy(user[*usersNum], enteredUser);
                    for(int i = 0; i < 3; i++) {
                        stats[*usersNum][i] = 0;
                    }
                    totalGames[*usersNum] = 0;
                    fprintf(*statsFile, "%s %d %d %d %d\n", enteredUser, 0, 0, 0, 0);
                    *index = *usersNum;
                    (*usersNum)++; // now there is 1 more user
                } else if(choice == 2) {
                    needToLogin = 1; // going back to enter another username
                } else {
                    printf("\nInvalid input! Please try again.\n\nDo you want to add %s as a new user?\n1. Yes\n2. No\nEnter a number: ", enteredUser);
                    needToLogin = 2;
                }
            } while(needToLogin == 2);
        }
    }
    fclose(*statsFile);
}

// to calculate the total games have been played by each user
void calcTotalGames(int stats[MAX_USERS][3], int totalGames[MAX_USERS], int usersNum) {
    for (int i = 0; i < usersNum; i++) {
        totalGames[i] = stats[i][0] + stats[i][1] + stats[i][2];
    }  
}

// to show the score of the logged in user at the top of the menu page
void showScore(int stats[MAX_USERS][3], char user[MAX_USERS][20], int totalGames[MAX_USERS], int index) {
    printf("\nUser: %s    Wins: %d    Losses: %d    Draws: %d    Total games: %d", user[index], stats[index][0], stats[index][1], stats[index][2], totalGames[index]);
}

// to print the menu
void printMenu() {
    printf(
       "\n _____         _      _____                    _____     _                     \n"
       "| __  |___ ___| |_   |  _  |___ ___ ___ ___   |   __|___|_|___ ___ ___ ___ ___ \n"
       "|    -| . |  _| '_|  |   __| .'| . | -_|  _|  |__   |  _| |_ -|_ -| . |  _|_ -|\n"
       "|__|__|___|___|_,_|  |__|  |__,|  _|___|_|    |_____|___|_|___|___|___|_| |___|\n"
       " _____                         |_|                                             \n"
       "|   __|___ _____ ___                                                            \n"
       "|  |  | .'|     | -_|                                                           \n"
       "|_____|__,|_|_|_|___|                                                           \n");
    printf("\n1) Play\n2) Delete your account\n3) Log out\n4) Exit\nSelect your choice (a number): ");
}

// to get the menu choice
int fMenuChoice() {
    int mChoice;
    scanf(" %d", &mChoice);
    return mChoice;
}

// to get the user's choice and computer's choice to play
void playersPlay(int *userPlay, int *compPlay) {
    system("cls");
    // to get the user's choice
    printf(
        "              ,---------------------------,\n"
        "              |  /---------------------\\  |\n"
        "              | |                       | |\n"
        "              | |        I dare         | |\n"
        "              | |         you...        | |\n"
        "              | |                       | |\n"
        "              |  \\_____________________/  |\n"
        "              |___________________________|\n"
        "            ,---\\_____            _______/------,\n"
        "          /         /______________\\           /|\n"
        "        /___________________________________ /  | \n"
        "        |                                   |   |  \n"
        "        |  _ _ _                 [-------]  |   |  \n"
        "        |  o o o                 [-------]  |  /  \n"
        "        |__________________________________ |/    \n"
    );
    do { 
        printf("\n1) Rock\n2) Paper\n3) Scissors\nSelect your choice (a number): ");
        scanf(" %d", userPlay);
        if(*userPlay < 1 || *userPlay > 3){
            printf("\nInvalid input! Please try again.\n");
        }
    } while (*userPlay < 1 || *userPlay > 3);

    // to get the computer's choice
    srand(time(NULL)); // to seed with the current system time
    *compPlay = rand() % 3 + 1; // to generate a random number between 1 and 3
}

// to check and show the result
int result(int userPlay, int compPlay, int stats[MAX_USERS][3], char user[MAX_USERS][20], int index) {
    int playAgain;
    system("cls");
    if(userPlay == 1){ // for rock
        if(compPlay == 1){
            printf(
            "\n   YOU				 COMPUTER\n"
            "    _______           _______\n"
            "---'   ____)         (____   '---\n"
            "      (_____)       (_____)      \n"
            "      (_____)       (_____)      \n"
            "      (____)         (____)      \n"
            "---.__(___)           (___)__.---\n"
            "    ____  ____  ___ _       ____  \n"
            "   / __ \\/ __ \\/   | |     / / /  \n"
            "  / / / / /_/ / /| | | /| / / /   \n"
            " / /_/ / _, _/ ___ | |/ |/ /_/    \n"
            "/_____/_/ |_/_/  |_|__/|__(_)     \n\n");
            stats[index][2] = stats[index][2] + 1; // to increase the number of draws
        } else if(compPlay == 2){
            printf(
            "\n   YOU				 COMPUTER\n"
            "    _______               _______\n"
            "---'   ____)         ____(____   '---\n"
            "      (_____)       (______          \n"
            "      (_____)      (_______          \n"
            "      (____)        (_______         \n"
            "---.__(___)           (__________.---\n"
            "__  __               __                  \n"
            "\\ \\/ /___  __  __   / /   ____  ________ \n"
            " \\  / __ \\/ / / /  / /   / __ \\/ ___/ _ \\\n"
            " / / /_/ / /_/ /  / /___/ /_/ (__  )  __/\n"
            "/_/\\____/\\__,_/  /_____/\\____/____/\\___/ \n\n");
            stats[index][1] = stats[index][1] + 1; // to increase the number of losses
        } else {
            printf(
            "\n   YOU				 COMPUTER\n"
            "    _______               _______\n"
            "---'   ____)         ____(____   '---\n"
            "      (_____)       (______          \n"
            "      (_____)      (__________       \n"
            "      (____)             (____)      \n"
            "---.__(___)               (___)__.---\n"
            "__  ______  __  __   _       _______   ____ \n"
            "\\ \\/ / __ \\/ / / /  | |     / /  _/ | / / / \n"
            " \\  / / / / / / /   | | /| / // //  |/ / /  \n"
            " / / /_/ / /_/ /    | |/ |/ // // /|  /_/   \n"
            "/_/\\____/\\____/     |__/|__/___/_/ |_(_)    \n\n");
            stats[index][0] = stats[index][0] + 1; // to increase the number of wins
        }
    } else if(userPlay == 2){ // for paper
        if(compPlay == 1){
            printf(
            "\n   YOU				 COMPUTER\n"
            "    _______              _______\n"
            "---'   ____)____        (____   '---\n"
            "          ______)      (_____)      \n"
            "          _______)     (_____)      \n"
            "         _______)       (____)      \n"
            "---.__________)          (___)__.---\n"
            "__  ______  __  __   _       _______   ____ \n"
            "\\ \\/ / __ \\/ / / /  | |     / /  _/ | / / / \n"
            " \\  / / / / / / /   | | /| / // //  |/ / /  \n"
            " / / /_/ / /_/ /    | |/ |/ // // /|  /_/   \n"
            "/_/\\____/\\____/     |__/|__/___/_/ |_(_)    \n\n");
            stats[index][0] = stats[index][0] + 1; // to increase the number of wins
        } else if(compPlay == 2){
            printf(
            "\n   YOU				 COMPUTER\n"
            "    _______                _______\n"
            "---'   ____)____      ____(____   '---\n"
            "          ______)    (______          \n"
            "          _______)  (_______          \n"
            "         _______)    (_______         \n"
            "---.__________)        (__________.---\n"
            "    ____  ____  ___ _       ____  \n"
            "   / __ \\/ __ \\/   | |     / / /  \n"
            "  / / / / /_/ / /| | | /| / / /   \n"
            " / /_/ / _, _/ ___ | |/ |/ /_/    \n"
            "/_____/_/ |_/_/  |_|__/|__(_)     \n\n");
            stats[index][2] = stats[index][2] + 1; // to increase the number of draws
        } else {
            printf(
            "\n   YOU				 COMPUTER\n"
            "    _______                _______\n"
            "---'   ____)____      ____(____   '---\n"
            "          ______)    (______          \n"
            "          _______)  (__________       \n"
            "         _______)         (____)      \n"
            "---.__________)            (___)__.---\n"
            "__  __               __                  \n"
            "\\ \\/ /___  __  __   / /   ____  ________ \n"
            " \\  / __ \\/ / / /  / /   / __ \\/ ___/ _ \\\n"
            " / / /_/ / /_/ /  / /___/ /_/ (__  )  __/\n"
            "/_/\\____/\\__,_/  /_____/\\____/____/\\___/ \n\n");
            stats[index][1] = stats[index][1] + 1; // to increase the number of losses
        }
    } else { // for scissors
        if(compPlay == 1){
            printf(
            "\n   YOU				 COMPUTER\n"
            "    _______             _______\n"
            "---'   ____)____       (____   '---\n"
            "          ______)     (_____)      \n"
            "       __________)    (_____)      \n"
            "      (____)           (____)      \n"
            "---.__(___)             (___)__.---\n"
            "__  __               __                  \n"
            "\\ \\/ /___  __  __   / /   ____  ________ \n"
            " \\  / __ \\/ / / /  / /   / __ \\/ ___/ _ \\\n"
            " / / /_/ / /_/ /  / /___/ /_/ (__  )  __/\n"
            "/_/\\____/\\__,_/  /_____/\\____/____/\\___/ \n\n");
            stats[index][1] = stats[index][1] + 1; // to increase the number of losses
        } else if(compPlay == 2){
            printf(
            "\n   YOU				 COMPUTER\n"
            "    _______                 _______\n"
            "---'   ____)____       ____(____   '---\n"
            "          ______)     (______          \n"
            "       __________)    (_______          \n"
            "      (____)           (_______         \n"
            "---.__(___)              (__________.---\n"
            "__  ______  __  __   _       _______   ____ \n"
            "\\ \\/ / __ \\/ / / /  | |     / /  _/ | / / / \n"
            " \\  / / / / / / /   | | /| / // //  |/ / /  \n"
            " / / /_/ / /_/ /    | |/ |/ // // /|  /_/   \n"
            "/_/\\____/\\____/     |__/|__/___/_/ |_(_)    \n\n");
            stats[index][0] = stats[index][0] + 1; // to increase the number of wins
        } else {
            printf(
            "\n   YOU				 COMPUTER\n"
            "    _______                 _______\n"
            "---'   ____)____       ____(____   '---\n"
            "          ______)     (______          \n"
            "       __________)   (__________       \n"
            "      (____)               (____)      \n"
            "---.__(___)                 (___)__.---\n"
            "    ____  ____  ___ _       ____  \n"
            "   / __ \\/ __ \\/   | |     / / /  \n"
            "  / / / / /_/ / /| | | /| / / /   \n"
            " / /_/ / _, _/ ___ | |/ |/ /_/    \n"
            "/_____/_/ |_/_/  |_|__/|__(_)     \n\n");
            stats[index][2] = stats[index][2] + 1; // to increase the number of draws
        }
    }

    // to ask the user whether to play again or return to the menu
    do {
        printf("\n1) Play Again\n2) Return to the menu and save\nSelect a number: ");
        scanf(" %d", &playAgain);
        if(playAgain == 1){
            return 1;
        } else if(playAgain == 2){
            system("cls");
            return 0;
        } else {
            printf("\nInvalid input! Try again.\n");
        }
    } while (playAgain != 1 && playAgain != 2);
}

// to update the file
void updateFile(int stats[MAX_USERS][3], char user[MAX_USERS][20], int totalGames[MAX_USERS], int usersNum){
    FILE *uStatsFile;
    uStatsFile = fopen("RPSstats.txt", "w");
    for(int i = 0; i < usersNum; i++){ // printing the new stats over the old ones
        fprintf(uStatsFile, "%s %d %d %d %d\n", user[i], stats[i][0], stats[i][1], stats[i][2], totalGames[i]);
    }
    fclose(uStatsFile);
}

// to log out to enter another user
int fLogOut(){
    int logout;
    do {
        printf("\nAre you sure you want to log out?\n1) Yes\n2) No\nSelect a number: ");
        scanf(" %d", &logout);
        if(logout == 1){
            system("cls");
            return 1;
        } else if (logout == 2){
            system("cls");
            return 0;
        } else {
            printf("\nInvalid input! Try again.\n");
        }
    } while(logout != 1 && logout != 2);
}

// to delete the logged in user
int deleteUser(int stats[MAX_USERS][3], char user[MAX_USERS][20], int totalGames[MAX_USERS], int *usersNum, int index){
    int confirm;
    do {
    printf("\nAre you sure you want to delete %s?\n1) Yes\n2) No\nSelect a number: ", user[index]);
    scanf(" %d", &confirm);
        if(confirm == 1){
            for(int i = index; i < *usersNum - 1; i++){
                strcpy(user[i], user[i+1]);
                for(int j = 0; j < 3; j++){
                    stats[i][j] = stats[i+1][j];
                }
                totalGames[i] = totalGames[i+1];
            }
            (*usersNum)--;
            system("cls");
            printf("\nThe user has been deleted successfully.\n");
            return 1;
        } else if (confirm == 2){
            system("cls");
            return 0;
        } else {
            printf("\nInvalid input! Try again.\n");
        }
    } while(confirm != 1 && confirm != 2);
}