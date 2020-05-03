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


int PIDStatus;
int status;
bool running = true;


void start_shell();
void show_help();
int type_prompt();
void read_command(char command[MAX_INPUT]);
void quit();
void version();
void change_directory(char *command);
void execBuildIn(int i, char *command);
int in(char **builtInCommands, int length, char *command);


int main() {
    start_shell();
    return 0;
}

void start_shell(){

    char command[MAX_INPUT];
    char params[MAX_INPUT];


    char *builtInCommands[MAX_INPUT] = {"quit", "version", "help","/\\.*"};

    while(running) {
        type_prompt();
        read_command(command);
        int builtInCom = in(builtInCommands, 4, command);
        //printf("%d", builtInCom);
        if (builtInCom != -1) {
            execBuildIn(builtInCom, command);
        } else {
            PIDStatus = fork();
            if (PIDStatus < 0) {
                printf("Unable to fork");
                continue;
            }
            u_long length;
            char* pos;
            length = strlen(command - 1);
            pos = command + length - 1;
            printf("command: %s\n",command);
            printf("last char: %c\n", *pos);
            if (command[strlen(command - 1)] != '&') {
                //printf("%c\n", command[(strlen(command - 1))]);
                if (PIDStatus > 0) {
                    waitpid(PIDStatus, &status, 0);
                }
                else {
                    execlp(command, params, 0);
                    exit(0);
                }
            } else {
                //printf("&\n");
                if (PIDStatus == 0) {
                    command[strlen(command - 1)] = 0;
                    //printf("%s",command);
                    execlp(command, params, 0);
                    exit(0);
                }
            }

        }

        //&& command[strlen(command) - 1] != '&'

    }
}

// This function shows the help text for this shell
void show_help(){
    printf("%15s %s\n", "Name des Befehls:", "Beschreibung:");
    printf("%15s %s\n", "Quit", "Beenden der HAW-Shell");
    printf("%15s %s\n", "version", "Anzeige des Autors und der Versionsnummer der HAW-Shell");
    printf("%15s %s\n", "/[Pfadname]", "Wechsel des aktuellen Arbeitsverzeichnisses (analog zu cd)."
                                     " Es muss immer ein kompletter Pfadname eingegeben werden");
    printf("%15s %s\n", "Help", "Anzeige der möglichen Built-In-Befehle mit Kurzbeschreibung");
}

int type_prompt() {
    char cwd[1024];
    printf("%s -> Was möchten Sie tun, %s?\n", getcwd(cwd, sizeof(cwd)), getenv("USER"));
    return 0;
}


void read_command(char command[MAX_INPUT]) {

    fgets(command, MAX_INPUT, stdin);
    strtok(command, "\n");



}



int in(char **builtInCommands, int length, char *command) {

    for (int i = 0; i < length; i++) {
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
    printf("in change_dir\n");
    chdir(command);
}
