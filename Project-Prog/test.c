#include<stdlib.h>
#include<stdio.h>
#include<string.h>

int open_t(char *pathname);

int main(void) {
	open_t("myest/path/test.c");

}
int open_t(char *pathname)
{
	//int inode_number;
	// write your code here.
	// count the number of path components
	int npath = 0;
	int cmax = 0;
	int max = 0;
	int i = 0;
	while(pathname[i] != '\0') {
		if(pathname[i] == '/') {
			npath++;
			if (cmax > max) {
				max = cmax;
			}
			cmax = 0;
		} else {
			cmax++;
		}
		i++;
	}
	npath++;
	if (cmax > max) {
		max = cmax;
	}
	printf("max: %d\n", max);
	printf("npath: %d\n", npath);

	char **path_comps = (char **) malloc(npath * sizeof(char*));
	for(int y = 0; y < npath; y++) {
		path_comps[y] = (char*) malloc(max * sizeof(char));
	}
	
	char *path_comp = malloc(max * sizeof(char));
	
	i = 0;
	int y = 0;
	int z = 0;
	while(pathname[i] != '\0') {
		if (pathname[i] == '/') {
			strcpy(path_comps[y], path_comp);
			y++;
			z = 0;
		} else {
			path_comp[z] = pathname[i];
			z++;
		}
		i++;
	}
	path_comps[y] = path_comp;
	y++;

	for(i = 0; i < y; i++) {
		printf("comp %d : %s\n", i, path_comps[i]);
	}

	return 1;
}
