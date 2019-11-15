//
// Created by meni on 14/11/2019.
//
#include <stdlib.h>
#include <stdio.h>
#include "bytes.h"
static int count = 0;
void print(byte_t* arr, int count);
void print(byte_t* arr, int count) {
	byte_t* ptr = arr;
	int i = 0;
	while (i < count) {
		printf("%u", *ptr);
		ptr++;
		i++;
	}
}
byte_t* create_bytes(char* file_name)
{
	byte_t* byte_arr_ptr = NULL;
	unsigned char j = 0; //represent the number of shift to the left
	int arr_index = 0;
	char line[20];
	int temp_grade;
	unsigned char grade = 0;
	char* token = NULL;
	char* wordcounter = NULL;
	FILE* file = fopen(file_name, "r");
	if (file == NULL) {//from stack overflow
		perror("Can't open file");
		return NULL;
	}
	else {
		while (fgets(line, sizeof(line), file)) {
			count++;
			byte_arr_ptr = (byte_t*)realloc(byte_arr_ptr, count * sizeof(byte_t));
			if (byte_arr_ptr == NULL) {
				printf("error: memory allocation proccess failed, program terminating....");
				exit(1);
			}
			token = line; //i didnt know if its ok to use strtok so i implemented it myself
			wordcounter = line;
			while (*wordcounter != '\n') {
				wordcounter++;
			}
			*wordcounter = '\0';
			j = 0;
			grade = 0;
			while (*token != '\0') {
				temp_grade = atoi(token);
				temp_grade = temp_grade << j;
				grade = grade | temp_grade;
				j = j + 2; //shift 2 to the left
				token = token + 2;
			}
			byte_arr_ptr[arr_index] = (unsigned char)grade;
			arr_index++;
		}
		fclose(file);
	}
	byte_arr_ptr = (byte_t*)realloc(byte_arr_ptr, count * sizeof(byte_t));
	return byte_arr_ptr;
}
void print_bytes(byte_t* byte_array, FILE* out)
{
	unsigned char byte_ans = 0;
	int i = 0, k = 0, t = 0;
	while (i < count) {//traverse the student's array
		byte_ans = byte_array[i];
		fprintf(out, "%02x", byte_ans); //from stack overflow
		i++;
	}
	fprintf(out, "\n");
}
void set_stud(byte_t* byte_array, int i, int j, int k)
{
	unsigned char first_bits=3, secondbits=12, thridbits=48, fourthbits=192;
	unsigned char new_ans = (unsigned char)k;
	int t = 0, s = j;
	unsigned char current_stu_ans = byte_array[i-1];
	first_bits = first_bits & current_stu_ans;
	secondbits = secondbits & current_stu_ans;
	thridbits = thridbits & current_stu_ans;
	fourthbits = fourthbits & current_stu_ans;
	while (s > 1) {
		t = t + 2;
		s--;
	}
	k = k << t;
	switch (j)
	{
	case 1 :
		byte_array[i-1] = k | secondbits | thridbits | fourthbits;
		break;
	case 2:
		byte_array[i-1] = first_bits | k | thridbits | fourthbits;
		break;
	case 3:
		byte_array[i-1] = first_bits | secondbits | k | fourthbits;
		break;
	case 4:
		byte_array[i-1] = first_bits | secondbits | thridbits | k;
		break;
	default:
		break;
	}
}
float average_stud(byte_t* byte_array, int i)
{
	float avg = 0;
	int m = 2;
	unsigned char numattwobits = 3;
	unsigned char current_num = byte_array[i-1];
	unsigned char current_res = 0;
	while (current_num > 0) {
		current_res += (numattwobits & current_num);
		current_num = current_num >> m;
	}
	avg = current_res;
	return avg / 4.0; //divide ans sum by number of answers
}
float average_ans(byte_t* byte_array, int j)
{
	int i = 0;
	float avg = 0;
	int k = 0;
	unsigned int temp_ans = 0;
	unsigned char ans_location = 3;
	unsigned char current_stu_ans = 0;
	while (j > 1) {
		k = k + 2;
		j--;
	}
	ans_location = ans_location << k;
	while (i < count) {
		current_stu_ans = byte_array[i] & ans_location;
		current_stu_ans = current_stu_ans >> k;
		avg += current_stu_ans;
		i++;
	}
	avg = avg / count;
	return avg;
}
