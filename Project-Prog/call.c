#include "call.h"
const char *HD = "HD";

superblock* read_sb(int fd);
inode* read_inode(int fd, int i_number);
void print_dir_mappings(int fd, int i_number);

int open_t(char *pathname)
{
	int inode_number;
	// write your code here.
	

//----------parse the path components into an array of strings------------------
	
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
	if(pathname[0] != '/' && pathname[i-1] != '/') {
		npath++;
	}
	if (cmax > max) {
		max = cmax;
	}
	//printf("max: %d\n", max);
	printf("npath: %d\n", npath);
	
	//malloc space for an array of path components
	char **path_comps = (char **) malloc(npath * sizeof(char*));
	for(int y = 0; y < npath; y++) {
		path_comps[y] = (char*) malloc(max * sizeof(char));
	}
	
	char *path_comp = malloc(max * sizeof(char));
	
	int l = 0;
	int y = 0;
	int z = 0;

	//if the first component is '/'
	if(pathname[0] == '/') {
		l = 1;
	}
	printf("i: %d", i);

	//if only '/'
	if(pathname[0] == '/' && i == 1 && pathname[i-1] == '/') {
		path_comp = ".";
	}

	//parse
	while(pathname[l] != '\0') {
		if (pathname[l] == '/') {
			strcpy(path_comps[y], path_comp);
			y++;
			z = 0;
		} else {
			path_comp[z] = pathname[l];
			z++;
		}
		l++;
	}
	path_comps[y] = path_comp;
	y++;

//------------------------end of parser--------------------------------------
//------------------------find the inode_number of path=---------------------
	//open HD
	int fd = open(HD, O_RDWR);
	if(fd<0){
		printf("Error: failed to open hard disk\n");
		return -1;
	}

	int ninode = 0;
	int fidx;
	int done;
	for(y = 0; y < npath; y++) {
		printf("Reading inode: %d \n", ninode);
		print_dir_mappings(fd, ninode);
		inode* cinode = read_inode(fd, ninode);
		if(cinode == NULL){
			printf("Error: failed to read root inode\n");
			return -1;
		}
		DIR_NODE* p_block = (DIR_NODE*)malloc(BLOCK_SIZE);
		int block_number = cinode->direct_blk[0];
		int currpos = lseek(fd, DATA_OFFSET + block_number * BLOCK_SIZE, SEEK_SET);
		if(currpos == -1) {
			printf("Error: lseek failed\n");
			return -1;
		}
		read(fd, p_block, BLOCK_SIZE);
		fidx = 0;
		done = 0;
		printf("cinode->file_num:%d", cinode->file_num);
		printf("Looking for: %s\n", path_comps[y]);
<<<<<<< HEAD
		while (fidx < cinode->file_num) {
			printf("p_block[%d].dir: %s\n", fidx, p_block[fidx].dir);
			if (strcmp(p_block[fidx].dir, path_comps[y]) == 0) {
				ninode = p_block[fidx].inode_number;
				fidx = cinode->file_num;
=======
		while (fidx < cinode->file_num) {
			printf("p_block[%d].dir: %s\n", fidx, p_block[fidx].dir);
			if (strcmp(p_block[fidx].dir, path_comps[y]) == 0) {
				ninode = p_block[fidx].inode_number;
				fidx = cinode->file_num;
>>>>>>> 1c259ccb0fc72618629bbdf0db93ecf681be3177
				printf("Done at %d\n", ninode);
				done = 1;
			}
			fidx++;
		}
		if (done != 1) {
			printf("Error: directory not found\n");
			return -1;
		}
	}

	printf("Returning inode: %d\n", ninode);	
	return ninode;
}

int read_t(int inode_number, int offest, void *buf, int count)
{
	int read_bytes;
	// write your code here.
//	return read_bytes; 
	return 1;
}

// you are allowed to create any auxiliary functions that can help your implementation. But only “open_t()” and "read_t()" are allowed to call these auxiliary functions.

superblock* read_sb(int fd) {
	superblock* sb = malloc(sizeof(superblock));
	int currpos=lseek(fd, SB_OFFSET, SEEK_SET);
	if(currpos<0){
		printf("Error: lseek()\n");
		return NULL;
	}
	
	//read superblock from disk
	int ret = read(fd, sb, sizeof(superblock));
	if(ret != sizeof (superblock) ){
		printf("Error: read()\n");
		return NULL;
	}
	return sb;
}

inode* read_inode(int fd, int i_number){
	inode* ip = malloc(sizeof(inode));
	int currpos=lseek(fd, INODE_OFFSET + i_number * sizeof(inode), SEEK_SET);
	if(currpos<0){
		printf("Error: lseek()\n");
		return NULL;
	}
	
	//read inode from disk
	int ret = read(fd, ip, sizeof(inode));
	if(ret != sizeof (inode) ){
		printf("Error: read()\n");
		return NULL;
	}
	return ip;
}

void print_dir_mappings(int fd, int i_number)
{
	inode* ip;
	ip = read_inode(fd, i_number);
	//if(ip->i_type != DIR)
	//{
	//	printf("Wrong path!\n");
	//	return;
	//}

	DIR_NODE* p_block = (DIR_NODE* )malloc(BLOCK_SIZE);
	// Consider that SFS only supports at most 100 inodes so that only direct_blk[0] will be used,
	// the implementation is much easier
	int block_number = ip->direct_blk[0];
	int currpos=lseek(fd, DATA_OFFSET + block_number * BLOCK_SIZE, SEEK_SET);
	read(fd, p_block, BLOCK_SIZE);

	int file_idx = 0;
	printf("dir \t inode_number\n");
	//printf("ip->file_num:%d\n", ip->file_num);
	for(file_idx = 0; file_idx < 6; file_idx++)
	{
		printf("%s \t %d\n", p_block[file_idx].dir, p_block[file_idx].inode_number);
	}
	//free(p_block);
}
