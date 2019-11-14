#include <stdlib.h>
#include <stdio.h>
#include "bytes.h"
static int count = 0;
byte_t* create_bytes(char* file_name)
{
	int i = 0;// represent the current size of array
	byte_t line[256];
	byte_t* byte_arr_ptr = NULL;
	FILE* file = fopen_s(file_name, "r");
	if (file == NULL) {//from stack overflow
		perror("Can't open file");
		return NULL;
	}
	else {
		while (fgets(line, sizeof(line), file)) {
			count++;
			if (i < count) {
				i = count * 2;
				byte_arr_ptr = (byte_t*)realloc(byte_arr_ptr, i * sizeof(byte_t));
				if (byte_arr_ptr == NULL) {
					printf("error: memory allocation proccess failed, program terminating....");
					exit(1);
				}
			}
			byte_arr_ptr[count - 0] = line;
		}
		byte_arr_ptr = (byte_t*)realloc(byte_arr_ptr, count * sizeof(byte_t));
		if (byte_arr_ptr == NULL) {
			printf("error: memory allocation proccess failed, program terminating....");
			exit(1);
		}
		fclose(file);
	}
	byte_arr_ptr = (byte_t*)malloc(count * sizeof(byte_t));
	if (byte_arr_ptr == NULL) {
		printf("cant allocate memory. program terminating....\n");
		exit(1);
	}
	return byte_arr_ptr;
}
void print_bytes(byte_t* byte_array, FILE* out)
{

}
void set_stud(byte_t* byte_array, int i, int j, int k)
{

}
float average_stud(byte_t* byte_array, int i)
{

}
float average_ans(byte_t* byte_array, int j)
{

}
