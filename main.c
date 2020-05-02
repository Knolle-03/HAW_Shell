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
void read_command();
void quit();
void version();
void change_directory();



int main() {
    start_shell();
    return 0;
}

void start_shell(){

    char *builtInCommands[MAX_INPUT] = {"quit", "version", "/[Pfadname]", "help", ""};
    char params[MAX_INPUT];

    while(running) {
        type_prompt();
        read_command(&command, &params);
        printf("command: %s \n", command);
        PIDStatus = fork();
        if (PIDStatus < 0) {
            printf("Unable to fork");
            continue;
        }
        if (PIDStatus > 0) {
            waitpid(PIDStatus, &status, 0);
        }
        else {
            execve(command, params, 0);
        }
    }
}

// This function shows the help text for this bash
void show_help(){
    printf("%15s %s", "Name des Befehls:", "Beschreibung:");
    printf("%15s %s", "Quit", "Beenden der HAW-Shell");
    printf("%15s %s", "version", "Anzeige des Autors und der Versionsnummer der HAW-Shell");
    printf("%15s %s", "/[Pfadname]", "Wechsel des aktuellen Arbeitsverzeichnisses (analog zu cd)."
                                     " Es muss immer ein kompletter Pfadname eingegeben werden");
    printf("%15s %s", "Help", "Anzeige der möglichen Built-In-Befehle mit Kurzbeschreibung");
}

int type_prompt() {
    char cwd[100];
    printf("%s -> Was möchten Sie tun, %s?\n", getcwd(cwd, 100), getenv("USER"));
    return 0;
}


void read_command() {
    fgets(command, MAX_INPUT, stdin);
    strtok(command,"\n");


}

void quit(){
    running = false;
}

void version(){
    printf(" HAW-Shell Version %f, Autoren: %s\n", VERSION, AUTOREN);
    type_prompt();
}

void change_directory(){
    chdir("..");
    type_prompt();
}
