//
// Created by meni on 14/11/2019.
//

#include <stdlib.h>
#include <stdio.h>
#include "bytes.h"
static int count = 0;
void print(byte_t* arr, int count);
void print(byte_t* arr, int count) {
  int i = 0;
  while (i<count) {
    printf("%u", arr[i]);
    i++;
  }
}
byte_t* create_bytes(char* file_name)
{
  int i = 2;// represent the current size of array
  byte_t* byte_arr_ptr = (byte_t*)malloc( i * sizeof(byte_t));
  unsigned char j = 0; //represent the number of shift to the left
  char line[20];
  int temp_grade;
  unsigned char grade = 0;
  char* token = NULL;
  char* wordcounter= NULL;
  FILE* file = fopen(file_name, "r");
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
      token = line; //i didnt know if its ok to use strtok
      wordcounter = line;
      while(*wordcounter != '\n') {
        wordcounter++;
      }
      *wordcounter = '\0';
      while(*token != '\0') {
        temp_grade = atoi(token);
        temp_grade = temp_grade << j;
        grade = grade | temp_grade;
        j = j + 2; //shift 2 to the left
        token = token + 2;
      }
      byte_arr_ptr[count - 0] = temp_grade;
    }
    byte_arr_ptr = (byte_t*)realloc(byte_arr_ptr, count * sizeof(byte_t));
    if (byte_arr_ptr == NULL) {
      printf("error: memory allocation proccess failed, program terminating....");
      exit(1);
    }
    print(byte_arr_ptr, count);
    fclose(file);
  }
  byte_arr_ptr = (byte_t*)realloc(byte_arr_ptr,count * sizeof(byte_t));
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
