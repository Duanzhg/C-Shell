#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

void del_stat(const char* name)
{
	DIR* fl = opendir(name);
	chdir(name);
	
	for(struct dirent* dir = readdir(fl); NULL!=dir;)
	{
		if(dir->d_type == DT_DIR)
		{
			if(strcmp(dir->d_name,"..")&&strcmp(dir->d_name,"."))
				del_stat(dir->d_name);
		}
		remove(dir->d_name);
		dir = readdir(fl);
	}
	chdir("..");
	remove(name);
}

int main(int argc,char *argv[])
{
	if(argc > 2){
	printf("Just use myrmdir is enough,including file and directory!");
	}
	
	struct stat sta;
	if(0 > stat(argv[1],&sta))
	{
		perror("open");
		return -1;
	}

	if(!S_ISDIR(sta.st_mode))
	{
		
		
		remove(argv[1]);
		return 0;
	}
	
	del_stat(argv[1]);
	
}
