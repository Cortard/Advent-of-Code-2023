#include <stdio.h>

int main() {
    FILE *stream;
    stream= fopen("../input.txt", "r");

    char buffer[100];
    FILE* end;
    fseek(filePtr,0,SEEK_END);

    int size=filePtr->_bufsiz;
    printf("%d\n",size);
    char file[size];
    fgets(file, size, filePtr);

    printf("%s",file);
    return 0;
}
