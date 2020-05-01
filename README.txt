Author: Monynich Kiem (mki273@uky.edu)
Date: 04/23/2020
Contents: Makefile, README.txt, shell.cc, shell.h, parser.c, and the test files that were provided. (I created a zip archive)

Running: In order to run th program I typed make into the command line and ran the program with ./shell

Implementation Notes: Most notes are in the program shell.cc itself and references are down below. 

References: I used Professor Moore's video on implementation to help me get started with my code and to understand what I needed to do to finish it. I also used my own work from lab3 which I noted in shell.cc

I recieved help from Logan T. Bannister on the setenv() function; more specifically, he reminded me to have if statements when I returned the value of setenv() for testing the failure. However, the rest is my own code.

I also used the cpp reference websites for chdir(), setenv(), unsetenv(), waitpid();
https://www.geeksforgeeks.org/chdir-in-c-language-with-examples/
https://pubs.opengroup.org/onlinepubs/009695399/functions/setenv.html
https://pubs.opengroup.org/onlinepubs/009695399/functions/unsetenv.html
https://www.tutorialspoint.com/unix_system_calls/waitpid.htm

This website helped me understand test #6: https://cpp.hotexamples.com/examples/-/-/WIFSIGNALED/cpp-wifsignaled-function-examples.html

Limitations: I had some difficulties with tests 10, 11, and 14 as I wasn't sure where I should implement the code for redirection of both in and out.

I later solved my problem for Test 14 by adding an if statement for the cmd->in_redir as well. However, when I tried similar tricks to completing tests 10 and 11 I was unable to figure out where to implement my code.
