#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long file_split_size = 1024*1024*50; // 50MB for split file size


long getFileSize(char *file_name) 
{ 
    long size = -1;
    
    // opening the file in read mode and checking if the file exist or not 
    FILE* fp = fopen(file_name, "rb");
    if (fp == NULL) {
        printf("File: '%s' Not Found!\n", file_name); 
        return -1; 
    }

    if(!fseek(fp, 0L, SEEK_END)){
        size = ftell(fp);
    } else { printf("Error seek file!\n"); }

    fclose(fp); 

    return size; 
} 


int getSplitPieces(long fileSize)
{
    int pieces = 0;
    
    pieces = (int)(fileSize / file_split_size);
    if (fileSize % file_split_size !=0){ pieces +=1; }
    
    return pieces;
}


int splitFile(char *fileIn, long maxSize)
{
    int result = 0;
    FILE *fIn;
    FILE *fOut;
    char buffer[1024 * 16];
    unsigned long size, read, write;

    if ((fileIn != NULL) && (maxSize > 0)) {
        fIn = fopen(fileIn, "rb");
        if (fIn != NULL) {
            fOut = NULL;
            result = 1;   /* we need at least one part */

            while (!feof(fIn)) {
                /* initialize (next) output file if no output file opened */
                if (fOut == NULL) {
                    sprintf(buffer, "%s.%03d", fileIn, result);
                    fOut = fopen(buffer, "wb");
                    if (fOut == NULL) {
                        result *= -1;
                        break;
                    }
                    size = 0;
                }

                /* calculate size of data to be read from input file in order to not exceed maxSize */
                read = sizeof(buffer);
                if ((size + read) > maxSize) {
                    read = maxSize - size;
                }

                /* read data from input file */
                read = fread(buffer, 1, read, fIn);
                if (read == 0) {
                    result *= -1;
                    break;
                }

                /* write data to output file */
                write = fwrite(buffer, 1, read, fOut);
                if (write != read) {
                    result *= -1;
                    break;
                }

                /* update size counter of current output file */
                size += write;
                if (size >= maxSize)   /* next split */
                {
                    fclose(fOut);
                    fOut = NULL;
                    result++;
                }
            }

            /* clean up */
            if (fOut != NULL) { fclose(fOut); }
            fclose(fIn);
        }
    }

    return (result);
}


int ShowUsage() {
    printf("**************************************************************************\n");
    printf("This application used to spilt the target file to small pieces (50MB)\n");
    printf("\n");
    printf("Usage: ./oofilesplit filename2split\n");
    printf("\n");
    printf("Enjoy it!\n");
    printf("**************************************************************************\n");

    return 0;
}


/* Main */
int main(int argc, char** argv) {
    long fileSize = 0L;

    if (argc != 2){
        printf("Error: Please indicate the target file you wish to split!\n");
        return -1;
    }

    if (!(strcmp(argv[1], "-h"))){
        ShowUsage();
        return 0;
    }
    
    if (!(strcmp(argv[1], "--help"))) {
        ShowUsage();
        return 0;
    }
    
    fileSize = getFileSize(argv[1]);
    if(fileSize <0) {
        printf("Error: Get file size error!\n");
        return -1;
    }
    
    printf("Info:\n\tSize of: %s: %ld\n", argv[1], fileSize);
    printf("\t%s will be split to %d piece(s)!\n", argv[1], getSplitPieces(fileSize));

    splitFile(argv[1], file_split_size);
    
    return (EXIT_SUCCESS);
}
