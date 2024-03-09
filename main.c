#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#endif


#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"


//////////////////////////////////////////////////////////////
// Helper functions
//////////////////////////////////////////////////////////////

int does_file_exist(char *fname) {
  return access(fname, F_OK) == 0;
}

char* get_config_path() {
  char* file;
  char* directory = "/.maduchat/";
  char* fileName = "root_path.config";
  file = malloc(strlen(getenv("HOME")) + strlen(directory) + strlen(fileName) + 1);
  strcpy(file, getenv("HOME"));
  strcat(file, directory);

  struct stat st = {0};
  if (stat(file, &st) == -1) {
    #if defined(_WIN32)
    mkdir(file);
    #else
    mkdir(file, 0700);
    #endif
  }
  strcat(file, fileName);
  return file;
}

char* get_config_root_path() {
  FILE *fptr;
  char *file = get_config_path();
  fptr = fopen(file, "r");
  char* root_path = malloc(100);
  fgets(root_path, 100, fptr);
  fclose(fptr);
  return root_path;
}

void set_config_root_path(char path[100]) {
  FILE *fptr;
  char* file = get_config_path();
  fptr = fopen(file, "w");
  fprintf(fptr, "%s", path);
  fclose(fptr);
  free(file);
}

void input_is_wrong_message()  {
  printf("Usage:\n");
  printf("    maduchat start " ANSI_COLOR_GREEN "//Starts the application with docker (Make sure, that you started docker engine)" ANSI_COLOR_RESET "\n");
  printf("    maduchat watch " ANSI_COLOR_GREEN "//Watches all changes in the maduchat root path" ANSI_COLOR_RESET "\n");
  printf("    maduchat code [path] " ANSI_COLOR_GREEN "//Opens MaduChat in Visual Studio Code. With path, you can open specific files or directorys (Based on the maduchat root directory)" ANSI_COLOR_RESET "\n");
  printf("    maduchat config root-path [value] " ANSI_COLOR_GREEN "//Sets config settings" ANSI_COLOR_RESET "\n");
}

void execute_on_root(char *command) {
  printf("Checking for configuration...\n");
  if (!does_file_exist(get_config_path())) {
    printf("Unable to execute! Please set root-path -> " ANSI_COLOR_GREEN "maduchat config root-path [value]" ANSI_COLOR_RESET "\n");
    return;
  }
  printf("Configuration is fine!\n");
  printf("Executing command: %s\n", command);
  char* file = get_config_root_path();
  char cmd[80];
  strcpy(cmd, "cd ");
  strcat(cmd, file);
  strcat(cmd, " && ");
  strcat(cmd, command);
  system(cmd);
  free(file);
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

void watch() {
  printf("Watching MaduChat...\n");
  execute_on_root("docker compose watch");
}

int config(char *key, char *value) {
  if (strcmp(key, "root-path") == 0) {
    set_config_root_path(value);
    printf("root-path set to %s!\n", value);
    return 1;
  }
  return 0;
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

  if (argc == 2 && 0 == strcmp(argv[1], "watch")) {
    watch();
    return 0;
  }

  if (argc == 4 && 0 == strcmp(argv[1], "config")) {
    int successfull = config(argv[2], argv[3]);
    if (successfull) {
      return 0;
    }
  }

  if (argc >= 2 && 0 == strcmp(argv[1], "code")) {
    char *path = get_code_path(argc, argv[2]);
    code(path);
    return 0;
  }

  input_is_wrong_message();
  return -1;
}