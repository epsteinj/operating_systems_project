#include<stdio.h>
#include<stdlib.h>
#include "call.h"

int main(void) { 
	//open_t("/dir4/dir1/");

//--------------open testing-------------------------------------
//	char filename[5][MAX_COMMAND_LENGTH] = {"/", "/dir5", "/dir5/dir1", "/dir5/dir1/file1", "/dir5/dir8/dir12/file2"};
	
//	int expected[5] = {0, 1, 5, 13, 14};

	//Start testing
//	for(int i = 0; i < 5; i++)
//	{
//		int inode_number = open_t(filename[i]);
//		printf("======case %d: open \'%s\' =======\n", i, filename[i]);
//		printf("returned inode number: %d\t expected result: %d\n\n", inode_number, expected[i]);
//	}

//-------------read testing--------------------------------------
	char filename[MAX_COMMAND_LENGTH]="/dir5/dir1/file1";

	/*
	Allocate a buf with MAX_FILE_SIZE.
	*/
	char buf[MAX_FILE_SIZE];

	int read_size;
	int test_inode=open_t(filename);
	printf("test_inode:%d\n", test_inode);
	//Start testi
	int offset_list[10] = {0,   4100, 8500,  40965, 15,   100,   9000,  1048576 - 50, 1048576 + 50, 10};
	int count_list[10] =  {100, 1000, 300,   800,   5000, 50000, 60000, 10000,        10,           MAX_FILE_SIZE - 100};
	int expected[10] =    {100, 1000, 300,   800,   5000, 50000, 60000, 50,           0,            1048566};
	//read_t test
	for(int i = 0; i < 10; i++)
	{
		int cnt = count_list[i];
		int off = offset_list[i];
		printf("====case %d: read %d bytes from %d offest=======\n", i, cnt, off);
		read_size = read_t(test_inode, off, buf, cnt);
		buf[read_size] = '\0';
		printf("read size: %d\t expected: %d\n\n",read_size, expected[i]);
	}
//	return 0;
//-----------read testing 2 (switch HD)--------------------------------

//	char filename[MAX_COMMAND_LENGTH]="/file1";

	/*
	Allocate a buf with MAX_FILE_SIZE.
	*/
//	char buf[MAX_FILE_SIZE];

//	int read_size;
//	int test_inode=open_t(filename);
	//Start testi
//	int offset_list[10] = {0};
//	int count_list[10] =  {50};
//	int expected[10] = {23};
//	char* expected_bytes[10] = {"Hello World, csci-3150."};
	//read_t test
//	for(int i = 0; i < 1; i++)
//	{
//		int cnt = count_list[i];
//		int off = offset_list[i];
//		printf("====case %d: read %d bytes from %d offest=======\n", i, cnt, off);
//		read_size = read_t(test_inode, off, buf, cnt);
//		buf[read_size] = '\0';
//		printf("read size: %d\t expected: %d\n",read_size, expected[i]);
//		printf("read bytes: %s\t expected: %s\n\n",buf, expected_bytes[i]);
//	}
	return 0;
}
	


