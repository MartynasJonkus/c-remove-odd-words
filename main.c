//LSP - 2213732, el. paštas - martynas.jonkus@mif.stud.vu.lt
//3 pratybų užduotis, 12 variantas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void openFile(FILE **fileName, char *mode);

void processText(FILE *inputFile, FILE *outputFile);
void addToOutput(char **output, int *outputSize, char add);
void writeOutput(char output[], int outputSize, FILE *outputFile);
void nextLine(char buffer[], FILE *inputFile);

int main() {
    //Get the input/output file names and open the files
    FILE *inputFile, *outputFile;
    openFile(&inputFile, "r");
    openFile(&outputFile, "w");

    //Read and process the input file text, output the rearranged text to results file
    processText(inputFile, outputFile);

    //Close the files and exit the program
    fclose(inputFile);
    fclose(outputFile);
    return 0;
}

void openFile(FILE **fileName, char *mode){
    char input[100];
    int c;

    while(1){
        if (!strcmp(mode, "r")){
            printf("Input the source file name: ");
        }else{
            printf("Input the results file name: ");
        }
        scanf("%99s", input);
        while ((c = getchar()) != '\n' && c != EOF);
        *fileName = fopen(input, mode);
        if (*fileName == NULL) {
            printf("File can't be opened.\n");
            continue;
        }
        break;
    }
}

void processText(FILE *inputFile, FILE *outputFile){
    int bufferSize = 255;
    char buffer[255];
    char *output = NULL;
    int outputSize, symbolCount, lastChar;

    //Repeats till it reaches the end of the file
    while(fgets(buffer, bufferSize + 1, inputFile)){
        outputSize = 0;
        symbolCount = 0;
        lastChar = 0;

        for(int i = 0; i < strlen(buffer); ++i){
            if ((i == (strlen(buffer) - 1)) && buffer[i] != ' ')
                lastChar = 1;

            if(buffer[i] == ' ' || buffer[i] == '\n' || lastChar){
                if (lastChar)
                    ++symbolCount;

                //Adds the word with an even amount of symbols to the output buffer
                if((symbolCount % 2) == 0){
                    for(int j = (symbolCount - lastChar); j > (0 - lastChar); --j){
                        addToOutput(&output, &outputSize, buffer[i - j]);
                    }
                }

                //Adds the space into the output buffer
                if (buffer[i] == ' ')
                    addToOutput(&output, &outputSize, buffer[i]);
                symbolCount = 0;
            }else{
                ++symbolCount;
            }
        }

        //Adds a newline symbol to the output if it doesn't find one already present
        if(!strchr(output, '\n') && !feof(inputFile)){
            addToOutput(&output, &outputSize, '\n');
        }

        writeOutput(output, outputSize, outputFile);

        nextLine(buffer, inputFile);
    }
    free(output);
}

void addToOutput(char **output, int *outputSize, char add){
    (*output) = realloc((*output),(*outputSize)+1);
    (*output)[(*outputSize)] = add;
    ++(*outputSize);
}

void writeOutput(char output[], int outputSize, FILE *outputFile){
    for(int i = 0; i < outputSize; ++i){
        fputc(output[i], outputFile);
    }
}

void nextLine(char buffer[], FILE *inputFile){
    int c;
    if(!strchr(buffer, '\n')){
        while (c = fgetc(inputFile), c != EOF && c != '\n');
    }
}