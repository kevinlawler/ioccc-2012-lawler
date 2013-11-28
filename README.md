ioccc-2012-lawler
=================

This was my entry for the 2012 International Obfuscated C Code Contest. It was
not selected as a winner. I believe the techniques it uses are novel, so I am
sharing the entry in case anyone finds the techniques interesting.

Tips for running the program:

1. Don't forget to comment out the `sleep` line before compiling.  
1. To query my server's IP instead of the IOCCC's, use the replacement line

```
  i[]={[5]=8276,[4]=17735,[0]=2,20480,-32338,5603};
```


## Author's Comments:

Magic Numbers / Future Winners
==============================

Introduction:
-------------

Anything the winners of the next competition can do, this program can do. It
takes a while to finish.

It starts by sleeping until the winners of the following competition are
announced. (Two years.) When it wakes up it connects to the IOCCC webserver,
issues a GET request for the complete set of winning entries "all.tar.bz2",
untars it, and then makes and runs all winning contest entries. It avoids using
the high-level calls read(), write(), and open().

To show that it works, it also runs the winners from competitions that have
already happened. Comment the sleep call to try this out, but note that the
compilation process can still take a while. If your system doesn't echo the
compilation to the terminal, you can use top to check that the program is still
working. Somewhat scandalously, every other winning entry in this year's
competition exhibits strictly less functionality. If the contest winners
have not yet been announced the program will try again via the program
loop. See below.

This program impolitely spawns many processes. You may need to issue a
broad-reaching kill command like: kill `ps -o pid`

The program will "loop" because it executes itself, and so technically it
exhibits all the future winners, not just those from the next contest. If this
program wins the IOCCC, then instead of merely looping, the number of program
instances will double every contest.


Technical Notes:
----------------

This program is deceptively portable. It is designed to run on popular modern
systems. The requirements for this program are less unusual than those for an
X11/X Windows program.

Known to run on: 

             *   32-bit FreeBSD 8
             *   32-bit Linux - CentOS 5
             *   32-bit Linux - Ubuntu 9
             *   32-bit Mac OSX/Darwin - Snow Leopard
             *   32-bit Mac OSX/Darwin - Lion
             *   32-bit Mac OSX/Darwin - Mountain Lion
             *   32-bit Sun Solaris 10
             *   64-bit Linux - CentOS 5
             *   64-bit Linux - Ubuntu 11
             *   64-bit Mac OSX/Darwin - Snow Leopard
             *   64-bit Mac OSX/Darwin - Lion
             *   64-bit Mac OSX/Darwin - Mountain Lion

("-bit" indicates kernel, not architecture)

I have tried to strike the right balance between elegant obscurity and
portability. For this reason the Makefile handles two cases that would be
awkward to handle in the code. The entry is compatible with FreeBSD if the line
*i=512 is added. It is compatible with Sun Solaris if the second argument in
the socket call is changed to a 2. On other systems the Makefile is
unnecessary.

The entry is widely compatible. It will run on almost everyone's computer.
Even so, a test suite is in place to guarantee compatibility. Systems that fail
the test suite instead run a program that prints a humorous string.

The "/tmp/all/all.tar.bz2\n" string is a red herring. The memmove() call copies
the string "GET " over the first four characters, producing the valid HTTP
request "GET /all/all.tar.bz2\n". This string is written to the socket
connection open to the IOCCC server. 
 
The IP address of ioccc.org is hardcoded using the standard inet_pton() value.
If the IP address changes the program needs a minor modification. So far the
IP has been the same for over a year. The IP makes the program somewhat
susceptible to bit rot. (Using a domain-name would be a slight improvement.)
There are two other likely causes of bit rot. The judges could choose a new
name for the tar file, or they could choose to stop updating the "all.tar.bz2"
package entirely.

This program abuses syscall(), but it does so in a portable way. Because
read/write/open are a standardized distance away from each other in modern
system call tables, once you've figured out the position of one, you've figured
out the position of all three. This program tries other system calls for
failure in order to determine the kind of table it's dealing with. It is
possible to determine dynamically which system table is present based on
whether a call using a certain integer succeeds or fails. Once the table has
been established the write call is anchored to the appropriate index. This
method produces the correct system calls on Linux, OSX/Darwin, BSD, Solaris,
etc., for both 32-bit and 64-bit kernels.

The open() flag 833 is a cross-platform compatible flag. It was created by
taking the bitwise OR over O_CREAT and O_WRONLY on Linux, OSX/Darwin, BSD, and
Solaris. There are no ill effects. This method works correctly on all systems.

This code is not compatible with big-endian systems; however, it is probably
time to relax that guideline since chips made in 2011 are primarily
little-endian or bi-endian.


