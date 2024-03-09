#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PATH "~/dev/maduchat" // Change this to your own MaduChat root directory


//////////////////////////////////////////////////////////////
// Helper functions
//////////////////////////////////////////////////////////////

void input_is_wrong_message()  {
  printf("Usage: maduchat [start|code] [code_path]\n");
}

void execute_on_root(char *command) {
  char cmd[80];
  strcpy(cmd, "cd ");
  strcat(cmd, PATH);
  strcat(cmd, " && ");
  strcat(cmd, command);
  system(cmd);
}

char* get_code_path(int argc, char *path_argument) {
  char *path;
  if (3 != argc) {
    path = ".";
  } else {
    path = path_argument;
  }
  return path;
}


//////////////////////////////////////////////////////////////
// Argument functions
//////////////////////////////////////////////////////////////

void start() {
  printf("Starting MaduChat...\n");
  execute_on_root("docker compose up");
}

void code(char *path) {
  printf("Opening MaduChat with vscode...\n");
  char cmd[80];
  strcpy(cmd, "code ");
  strcat(cmd, path);
  execute_on_root(cmd);
}


//////////////////////////////////////////////////////////////
// main
//////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
  if (argc == 2 && 0 == strcmp(argv[1], "start")) {
    start();
    return 0;
  }

  if (argc >= 2 && 0 == strcmp(argv[1], "code")) {
    char *path = get_code_path(argc, argv[2]);
    code(path);
    return 0;
  }

  input_is_wrong_message();
  return -1;
}