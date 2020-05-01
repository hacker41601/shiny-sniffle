//Author: Monynich Kiem
//Reference: Code with Me video by Dr. Moore
//Date: 04/07/2020
//CS270 Lab 2

#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(void)
{
	DIR *directory = opendir(".");
	struct dirent *entry;
	//opendir() returns a DIR pointer
	
	if (directory == NULL)
	{
		//opendir failed
		perror("opendir failed");
		exit(1);
	}

	while ((entry = readdir(directory)) != NULL)
	{
		printf("%s\n", entry->d_name); 
	}

	if (errno != 0)
	{
		perror("readdir failed");
		exit(1);
	}

	if (closedir(directory) != 0)
	{
		perror("closedir failed");
		exit(1);
	}
	
	
}
