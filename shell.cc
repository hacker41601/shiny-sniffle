

#include <iostream> //cerror
#include <iomanip> //flush
#include <string> //getline, string
#include <string.h> //sterror
#include <fstream> //ifstream
#include <errno.h> //errno
#include <sys/types.h> //fork, open, wait
#include <sys/stat.h> //open
#include <sys/wait.h> //wait
#include <fcntl.h> //open
#include <unistd.h> //fork, execvp
#include <stdlib.h> //exit
#include <stdio.h> //perror
#include "shell.h"

using namespace std;

const int STDOUT_FD = 1;
const int STDIN_FD = 0;

void call_redirected(const command *cmd)
{
        pid_t pid = fork();

        if (pid<0)
        {
                perror("could not create a child process");
                return;
        }
        else if (pid==0)
        {
		if (cmd->out_redir)
		{
                	int fd = open(cmd->out_redir, O_WRONLY | O_CREAT | O_TRUNC, 0666);
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
		}//end of if statement for redirecting out
		
		if (cmd->in_redir)
		{
			int fd = open(cmd->in_redir, O_RDONLY | O_CREAT | O_TRUNC, 0666);
			if (fd < 0)
			{
				perror("could not open file for redirection");
				exit(1);
			}
			if (dup2(fd, STDIN_FD) < 0)
			{
				perror("could not duplicate file descriptor");
				exit(2);
			}

		}//end of if statement for redirecting in
   
		execvp(cmd->args[0], cmd->args);
                perror("could not execute process");
                exit(3);
        	}
        else
        {
                int status = 0;
                if (waitpid(-1,&status,0) < 0)
                {
                        cerr << "Could not wait for child: " << strerror(errno) << endl;
                        return;
                }
                if (WIFEXITED(status))
		{
			       	if (WEXITSTATUS(status) != 0)
				{
					cerr << "Command exited with status: " << WEXITSTATUS(status) << endl;
				} 
		} 	
		if (WIFSIGNALED(status))
		{
			cerr << "Killed by signal: " << WTERMSIG(status) << endl;
		}
	}

}//pulled from lab3 and modified

int main(int argc, char** argv)
{
	ifstream scriptf;
	bool from_script = false;

	if (argc > 2)
	{
		cerr << "Usage: " << argv[0] << " [<scriptfile>]" <<endl;
		exit(1);
	}

	else if (argc == 2)
	{
		from_script = true;
		scriptf.open(argv[1]);
		if (scriptf.bad())
		{
			cerr << "Could not open input file " << argv[1] << ": " << strerror(errno) << endl;
			exit(1);
		}
	}

	istream &inf = from_script ? scriptf : cin; //the ? evaluates whether or not to use scriptf or cin

	string line;

	if(!from_script)
		cerr << "shell> " <<flush;

	while (getline(inf, line))
	{
		command *cmd = parse_command(line.c_str());
		if (cmd->args[0]) 
		{
			if (string(cmd->args[0]) == "cd")
			{
				int cdRet;
				const char* dir = cmd->args[1];	
				const char *homedir;
				homedir = getenv("HOME");
				if (cmd->args[1] !=  nullptr)
				{	
					cdRet = chdir(dir);
					if (cdRet == -1)
					{
						cerr << "Error using cd: " << strerror(errno) << endl;
					}
				}
				else if (cmd->args[1] == nullptr)
				{
					cdRet = chdir(homedir);
					if (cdRet == -1)
					{
						cerr << "Error using cd: " << strerror(errno) << endl;
					}
					else if (homedir == NULL)
					{
						cerr << "No home directory found\n";
					}
				}
			}//end of cd command
			else if (string(cmd->args[0]) == "setenv")
			{
				int setEnvRet;

				if ((cmd->args[1] != nullptr)&&(cmd->args[2]!=nullptr))
				{
					setEnvRet = setenv(cmd->args[1], cmd->args[2], 1);

					if (setEnvRet == -1)
					{
						cerr << "Error using setenv(): " << strerror(errno) << endl;
					}

				}
				else if ((cmd->args[1] != nullptr)&&(cmd->args[2] == nullptr))	
				{
					setEnvRet = unsetenv(cmd->args[1]);
					
					if (setEnvRet == -1)
					{
						cerr << "Error using unsetenv(): " << strerror(errno) << endl;
					}
				}
				else
				{
					cerr << "Error: No arguments given\n";
				}

			}//end of setenv command
			else if (string(cmd->args[0]) == "exit")
			{
				exit(0);
			}
			else
			{
				//external command
				call_redirected(cmd);
			}//end of exit command

		} //else, empty command: do nothing
		
		free_command(cmd);
		if (!from_script)
			cerr << "shell> " << flush;

	} //end of while loop to read lines

	if (inf.bad())
	{
		cerr << "Error reading input: " <<strerror(errno) << endl;
		exit(1);
	}

	exit(0);

}//end of main
