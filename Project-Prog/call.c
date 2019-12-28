#include "call.h"
const char *HD = "HD";

superblock* read_sb(int fd);
inode* read_inode(int fd, int i_number);
void print_dir_mappings(int fd, int i_number);

int open_t(char *pathname)
{
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
	//printf("npath: %d\n", npath);
	
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
		//printf("Reading inode: %d \n", ninode);
		//print_dir_mappings(fd, ninode);
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
		//printf("cinode->file_num:%d", cinode->file_num);
		//printf("Looking for: %s\n", path_comps[y]);
		while (fidx < cinode->file_num) {
			//printf("p_block[%d].dir: %s\n", fidx, p_block[fidx].dir);
			if (strcmp(p_block[fidx].dir, path_comps[y]) == 0) {
				ninode = p_block[fidx].inode_number;
				fidx = cinode->file_num;
				//printf("Done at %d\n", ninode);
				done = 1;
			}
			fidx++;
		}
		if (done != 1) {
			printf("Error: directory not found\n");
			return -1;
		}
	}

	//printf("Returning inode: %d\n", ninode);	
	return ninode;
}

int read_t(int inode_number, int offset, void *buf, int count)
{
	//printf("MAX_FILE_SIZE: %d\n", MAX_FILE_SIZE);
	int fd = open(HD, O_RDWR);
	inode* ip = read_inode(fd, inode_number);
	int read_bytes;
	int block_start;
	int a = offset/BLOCK_SIZE;
	int b = offset % BLOCK_SIZE;
	printf("a: %d, b: %d\n", a, b);	
	if (offset >= 1048576) {
		return 0;
	}

	if (a < 2) {
		block_start = ip->direct_blk[a];
		printf("a < 2, block_start: %d\n", block_start);
	} else {
		block_start = ip->indirect_blk+(a-2);
		printf("a >= 2, block_start: %d\n", block_start);
	}
	
	int aa = ((offset + count-1) / 4096);
	int bb = ((offset + count-1) % 4096);
	printf("aa: %d bb:%d\n", aa, bb);
	int block_end;
	if( aa < 2) {
		block_end = ip->direct_blk[aa];
		printf("aa < 2, block_end: %d\n", block_end);
	} else {
		block_end = ip->indirect_blk + (aa-2);
		printf("aa > 2, block_end: %d\n", block_end);
	}
	int currpos = lseek(fd, DATA_OFFSET + block_start * BLOCK_SIZE + b, SEEK_SET);
	if(currpos == -1) {
		printf("Error: lseek()\n");
		return -1;
	}
	printf("block_end: %d block_start: %d b: %d bb: %d\n", block_end, block_start, b, bb);
	if((offset + count) >= 1048576) {
		read_bytes = (1048576 - offset);	
	} if(block_end == block_start) {
		read_bytes = (bb - b) + 1;
	} else {
		read_bytes = BLOCK_SIZE*(block_end-block_start) + ((bb - b) + 1);
	}
	//buf = (void *)malloc(read_bytes);
	read(fd, buf, read_bytes);
	return read_bytes; 
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
	if(currpos == -1) {
		printf("lseek: error\n");
	}
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
