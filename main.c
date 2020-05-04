/*
# Exercise 1 Operating systems SS2020 Group 02
# "hawsh" is a simple Shell
# Lennart Draeger und Sven Reymann
# 05.05.2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>


#define MAX_INPUT 30
#define VERSION 1.0
#define AUTOREN "Lennart Draeger und Sven Reymann"
#define NUMBER_OF_BUILT_IN_COMMANDS 3

int PIDStatus;
int status;
bool running = true;

bool ampersand_set(char *command);
void exec_command(int buildInCom, char *command, char *params);
void start_shell();
void show_help();
int type_prompt();
int starts_with_slash(char *command);
void read_command(char command[MAX_INPUT]);
void quit();
void version();
void change_directory(char *command);
void execBuildIn(int i, char *command);
int in(char *command);


int main() {
    start_shell();
    return 0;
}

void start_shell(){

    char command[MAX_INPUT];
    char params[MAX_INPUT];
    
    while(running) {
        type_prompt();
        read_command(command);
        bool ampersandIsSet = ampersand_set(command);
        int builtInCom = in(command);
        if (starts_with_slash(command) == 4) {
            builtInCom = 4;
        }
        if (builtInCom == 0) {
            quit();
            continue;
        } else if (builtInCom == 4) {
            change_directory(command);
            continue;
        }
        PIDStatus = fork();
        if (PIDStatus < 0) {
            printf("Unable to fork");
            continue;
        }

        if (!ampersandIsSet) {
            if (PIDStatus > 0) {
                waitpid(PIDStatus, &status, 0);
            }
            else {
                exec_command(builtInCom,command, params);
            }
        } else {
            if (PIDStatus == 0) {
                exec_command(builtInCom, command, params);
            }
        }
    }
}


// This function shows the help text for this shell
void show_help(){
    printf("%17s     %s\n", "Name des Befehls:", "Beschreibung:");
    printf("---------------------------------------------------\n");
    printf("%17s     %s\n", "Quit:", "Beenden der HAW-Shell");
    printf("%17s     %s\n", "version:", "Anzeige des Autors und der Versionsnummer der HAW-Shell");
    printf("%17s     %s\n", "/[Pfadname]:", "Wechsel des aktuellen Arbeitsverzeichnisses (analog zu cd). Es muss immer ein kompletter Pfadname eingegeben werden.");
    printf("%17s     %s\n\n", "Help:", "Anzeige der möglichen Built-In-Befehle mit Kurzbeschreibung");
}

bool ampersand_set(char *command) {
    u_long length;
    char* pos;
    length = strlen(command);
    pos = command + length - 1;
    if (*pos == '&'){
        *pos = '\0';
        return true;
    }
    return false;
}


int type_prompt() {
    char cwd[1024];
    printf("%s -> Was möchten Sie tun, %s? ", getcwd(cwd, sizeof(cwd)), getenv("USER"));
    return 0;
}

void exec_command(int buildInCom, char *command, char *params) {
    //printf("in exec\nbuiltInCom: %d\ncommand: %s\n", buildInCom, command);

    if (buildInCom != -1) {
        execBuildIn(buildInCom, command);
        exit(0);
    } else {
        int retVal = execlp(command, params, 0);
        if (retVal < 0) {
            printf("command: %s not found.\n", command);
            exit(-1);
        }
        exit(0);
    }
}

int starts_with_slash(char *command) {
    //printf("in start_with_slash\n");

    char* pos;
    pos = command;
    if (*pos == '/') {
        //printf("true\n");
        return 4;
    }
    return -1;
}


void read_command(char command[MAX_INPUT]) {
    fgets(command, MAX_INPUT, stdin);
    strtok(command, "\n");
}



int in(char *command) {
    char *builtInCommands[MAX_INPUT] = {"quit", "version", "help"};
    for (int i = 0; i < NUMBER_OF_BUILT_IN_COMMANDS; i++) {
        if (strcmp(builtInCommands[i], command) == 0) {
            return i;
        }
    }
    return -1;
}

void execBuildIn(int i, char command[]) {
    if (i == 0) {
        quit();
    } else if (i == 1) {
        version();
    } else if (i == 2) {
        show_help();
    } else {
        change_directory(command);
    }
}

void quit(){
    running = false;
}

void version(){
    printf(" HAW-Shell Version %f, Autoren: %s\n", VERSION, AUTOREN);
}

void change_directory(char *command){
    //printf("in change_dir\n");
    int retVal = chdir(command);

    if (retVal < 0) {
        printf("dir: %s not found\n", command);
    }
}
