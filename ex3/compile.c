#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void printToAsmbl(FILE *input, FILE *output, int *arr, int arrSize);

void findMinMax(int *min, int *max, FILE *input);

void printCase(char *chunk, FILE *output, int index);

void printLine(char *line, FILE *output);

void printOperator(char *token, FILE *output);

void printSide(char *token, FILE *output);

void printJumpTable(int* indexArray, FILE* output, int arrSize);


int main() {
    int min = 0, max = 0;
    int i = 0;
    FILE *input = fopen("switch.c", "r");
    if (input == NULL) {
        printf("could not open input file");
        exit(1);
    }
    FILE *asbmlFile = fopen("switch.s", "w");
    if (asbmlFile == NULL) {
        printf("could not open output file");
        exit(1);
    }
    fprintf(asbmlFile, "\t.section\t.text\n\t.globl\tswitch2\nswitch2:");
    findMinMax(&min, &max, input);
    fprintf(asbmlFile, "\n\tmovq $0, %%rax\n\tsubq $%d, %%rdx\n\tcmpq $%d, %%rdx\n\tja .L%d\n\tjmp *.L%d(,%%rdx,8)\n",
            min, (max - min), (max - min + 1), max - min + 2);
    // need to open the input file again for the pointer to be located at the beginnig
    input = fopen("switch.c", "r");
    if (input == NULL) {
        printf("could not open input file");
        exit(1);
    }
    //int indexArray[max - min + 1];
    int *indexArray = (int *) malloc(sizeof(int) * (max - min + 1));
    // make array = {min, min+1,...,max} = {21, 22, 23,...,31}
    for (i = 0; i < max - min + 1; i++) {
        indexArray[i] = min + i;
    }
    printToAsmbl(input, asbmlFile, indexArray, max - min + 1);
    fprintf(asbmlFile,"\t ret\n");
    printJumpTable(indexArray, asbmlFile, max - min + 1);

    return 0;
}

void findMinMax(int *min, int *max, FILE *input) {
    char *line, *num;
    int numInt = 0, flag = 0;
    size_t length = 0;
    while (getline(&line, &length, input) != -1) {
        strtok_r(line, " ", &num);
        if (!strcmp(line, "case")) {
            strtok_r(num, ":", &line);
            numInt = atoi(num);
            // not the first num we get
            if (flag) {
                if (numInt < *max) {
                    *min = numInt;
                } else {
                    *max = numInt;
                }
            } else {
                // first num we get
                *min = numInt;
                *max = numInt;
                flag = 1;
            }
        }
    }
}

void printToAsmbl(FILE *input, FILE *output, int *arr, int arrSize) {
    // print to output file by order of inupt
    char *line;
    char *num = NULL;
    char *caseText = "";
    int numInt = 0, min = arr[0], flagCase = 0, defFlag = 0, i =0;
    size_t length = 0;
    while (getline(&line, &length, input) != -1) {
        if (strstr(line, "{")) {
            continue;
        }
        if (strstr(line, "}")) {
            if (defFlag == 1) {
                // send to print in asmbl defualt case - printCase
                printCase(caseText, output, arrSize);
                caseText = "";
                defFlag = 0;
            }
            continue;
        }
        if (strstr(line, "case")) {
            if (flagCase) {
                // send to print in asmbl - printCase
                printCase(caseText, output, numInt - min);
                caseText = "";
            }
            strtok_r(line, " ", &num);
            strtok_r(num, ":", &line);
            numInt = atoi(num);
            // put 1 in array
            arr[numInt - min] = 1;
            flagCase = 1;
            continue;
        }
        if (strstr(line, "break")) {
            caseText = (char *) realloc(caseText, sizeof(char) * (strlen(line) + strlen(caseText)));
            strcat(caseText, line);
            continue;
        }
        if (strstr(line, "return")) {
            continue;
        }
        if (strstr(line, "default")) {
            if (flagCase) {
                // send to print in asmbl - print last case before default
                printCase(caseText, output, numInt - min);
                caseText = "";
            }
            defFlag = 1;
            continue;
        }
        if ((flagCase) && (!strcmp(caseText, ""))) {
            caseText = (char *) malloc(sizeof(char) * strlen(line));
            strcpy(caseText, line);
        } else if (flagCase) {
            caseText = (char *) realloc(caseText, sizeof(char) * (strlen(line) + strlen(caseText)));
            strcat(caseText, line);
        }
    }
    // if num in input is in arr make it 1, otherwise 0. for the jumpTable
    for (i = 0; i < arrSize; i++) {
        if (arr[i] != 1) {
            arr[i] = 0;
        }
    }
}

void printCase(char *chunk, FILE *output, int index) {
    fprintf(output, ".L%d:\n", index);
    char *line;
    line = strtok_r(chunk, "\n", &chunk);
    while (line) {
        if (strstr(line, "break")) {
            fprintf(output, "\t ret\n");
            break;
        }
        printLine(line, output);
        line = strtok_r(chunk, "\n", &chunk);
    }
}

void printLine(char *line, FILE *output) {
    char *src, *dst, *token;
    dst = strtok_r(line, " ", &line);
    token = strtok_r(line, " ", &line);
    src = strtok_r(line, ";", &line);
    if (((!strcmp(token, "<<=")) || (!strcmp(token, ">>="))) || (strstr(src, "*") &&
    strstr(dst, "*"))) {
        fprintf(output, "\tmovq\t");
        printSide(src, output);
        fprintf(output, ",\t%%rcx\n");
        printOperator(token, output);
        if (strstr(src, "*") && strstr(dst, "*")) {
            fprintf(output, "%%rcx,");
            printSide(dst, output);
            fprintf(output, "\n");
        } else if ((!strcmp(token, "<<=")) || (!strcmp(token, ">>="))) {
            fprintf(output, "%%cl,\t");
            printSide(dst, output);
            fprintf(output, "\n");
        }
    } else {
        printOperator(token, output);
        printSide(src, output);
        if (strcmp(token, "*=")) {
            fprintf(output, ",\t");
            printSide(dst, output);
        }
        fprintf(output, "\n");
    }
}

void printSide(char *token, FILE *output) {
    int i;
    char *temp = token;
    if (!strcmp(token, "*p1")) {
        fprintf(output, "(%%rdi)");
    } else if (!strcmp(token, "*p2")) {
        fprintf(output, "(%%rsi)");
    } else if (!strcmp(token, "result")) {
        fprintf(output, "%%rax");
    } else {
        i = atoi(token);
        if (i) {
            fprintf(output, "$%d", i);
        } else {
            if (!strcmp(temp, "0")) {
                fprintf(output, "$0");
            }
        }
    }
}

void printOperator(char *token, FILE *output) {
    if (!strcmp(token, "=")) {
        fprintf(output, "\tmovq\t");
    } else if (!strcmp(token, "<<=")) {
        fprintf(output, "\tsalq\t");
    } else if (!strcmp(token, ">>=")) {
        fprintf(output, "\tsarq\t");
    } else if (!strcmp(token, "*=")) {
        fprintf(output, "\tmulq\t");
    } else if (!strcmp(token, "+=")) {
        fprintf(output, "\taddq\t");
    } else if (!strcmp(token, "-=")) {
        fprintf(output, "\tsubq\t");
    }
}

void printJumpTable(int *indexArray, FILE* output, int arrSize) {
    int i = 0;
    fprintf(output, "\t.section\t.rodata\n\t.align 8\n.L%d:\n", arrSize + 1);
    for (i = 0; i < arrSize; i++) {
        if (indexArray[i] == 1) {
            fprintf(output, "\t.quad\t.L%d\n", i);
        } else {
            fprintf(output, "\t.quad\t.L%d\n", arrSize);
        }
    }
}

