//This test file ensures compatibility with the system
//Warning: contains spoilers

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/syscall.h>

#define A(x) {if(!(x)){fprintf(stderr,"System incompatibility: switching to reduced functionality version.\n");exit(1);}}
#define E errno
#define C "command -v "
#define D " >/dev/null"

void runTestCases();

int main()
{
  runTestCases();
  return 0;
}

void runTestCases()
{
  short s[] = {97};
  int i[] = {6579042};
  long long l[] = {30234823463757413};

  //Test Case: Must appear little-endian for int, long long, and short
  //Will not pass on big-endian systems or on certain mixed-endian systems like the PDP-11
  char *c;

  c=(char *)s;
  A(c[0] == 0x61); //'a'
  A(c[1] == 0x00);

  c=(char *)i;
  A(c[0] == 0x62); //'b'
  A(c[1] == 0x63); //'c'
  A(c[2] == 0x64); //'d'
  A(c[3] == 0x00);

  c=(char *)l;
  A(c[0] == 0x65); //'e'
  A(c[1] == 0x66); //'f'
  A(c[2] == 0x67); //'g'
  A(c[3] == 0x68); //'h'
  A(c[4] == 0x69); //'i'
  A(c[5] == 0x6a); //'j'
  A(c[6] == 0x6b); //'k'
  A(c[7] == 0x00);

  //Test Case: Must not have over-large sizeof
  A(sizeof(s) == 2);
  A(sizeof(i) == 4);
  A(sizeof(l) == 8);

  //Test Case: This is a redundant test of the POSIX standard.
  //             Ensure STDERR file descriptor is correctly set to 2.
  A(STDERR_FILENO == 2);
  
  //Test Case: Common settings for flags
  A(833 & O_CREAT);
  A(833 & O_WRONLY);
  A(438 & (S_IRUSR | S_IWUSR));

  //Test Case: System calls work
  char *filename = "testfile";
  int r, k, file;
  
  //Tese Case: Will branch on, typically, close or lstat
  E = 0;
  r = syscall(6,2);
  A(E != ENOSYS);

  if(r) k=1;
  else  k=4;

  //Test Case: This ensure we actually found read, write, open, and that they work
  {
    E = 0;

    //Open
    file = syscall(k+1, filename, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    A(E != ENOSYS);
    A(!E);

    char *s = "hello";
    char buf[255] = {0};

    //Write
    E = 0;
    syscall(k, file, s, strlen(s));
    A(E != ENOSYS);
    A(!E);

    //Open again for read()
    close(file);
    A(!E);
    file = open(filename, O_RDONLY);
    A(!E);

    //Read
    E = 0;
    syscall(k-1, file, buf, strlen(s));  
    A(E != ENOSYS);
    A(!E);
    A(!strncmp(buf, s, strlen(s)));

    //Cleanup
    close(file);
    unlink(filename);
  }

  //Test Case: Test for the presence of common shell tools
  A(0!=system(NULL)); //Shell is available
  //`command -v` is specified by POSIX
  //See http://pubs.opengroup.org/onlinepubs/009695399/utilities/command.html
  A(!system(C"make"D));
  A(!system(C"tar "D));
  A(!system(C"find"D));

  //Test Case: Usual location is reachable
  struct hostent *h = gethostbyname("206.197.161.153");
  A(h);
}

