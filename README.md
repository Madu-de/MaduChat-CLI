# MaduChat-CLI

I created this CLI to work more efficiently with [MaduChat](https://github.com/Madu-de/MaduChat).

## How to add MaduChat-CLI to my machine

1. Compile the main.c file
   ```bash
   gcc main.c -o maduchat
   ```
2. Add the outputfile to your PATH
    #### On Linux
    - Backup your PATH
      ```bash
      echo $PATH > ~/path.txt
      ```
   - Move the outputfile to <code>~/maduchat/FILE_HERE</code>
   - Add the outputfile to your PATH (To use it after reopen the terminal, you have to add this at the bottom of your ~/.profile file)
     ```bash
     export PATH="$PATH:~/maduchat"
     ```
    #### On Windows
    - Search for "Environment Variables"
    - Add the outputfile to your PATH
