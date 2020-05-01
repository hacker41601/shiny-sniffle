#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

const int STDOUT_FD = 1;
extern char **environ;

int call_redirected(const char *program, const char *args[], const char *outfile)
{
	pid_t pid = fork();

	if (pid<0)
	{
		perror("could not create a child process");
		return 0;
	}
	else if (pid==0) 
	{
		int fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (fd < 0)
		{
			perror("could not open file for redirection");
			exit(1);
		}
		if (dup2(fd, STDOUT_FD) <0)
		{
			perror("could not duplicate file descriptor");
			exit(2);
		}

		execve(program, (char*const*) args, environ);
		perror("could not execute process");
		exit(3);
	}
	else
	{
		int status = 0;
		if (wait(&status) < 0)
		{
			perror("could not wait for child");
			return 0;
		}
		return WIFEXITED(status) && WEXITSTATUS(status) == 0;
	}

}

int main()
{
	const char *args[] = { "ls", "-l", "/nothing", nullptr};
	
	if (call_redirected("/bin/ls", args, "ls-out.txt"))
	{
		cout << "Success!\n";
		return 0;
	}
	else 
	{
		cout << "Failure :(\n";
		return 1;
	}
}
