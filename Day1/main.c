#include <malloc.h>
#include <string.h>

typedef struct Link Link;
struct Link {
    int value;
    struct Link* next;
};
void delete(Link* link){
    if(! link->next) return;
    delete(link->next);
    free(link->next);
}

int main() {
    FILE *stream;
    stream= fopen("../input.txt", "r");
    if(!stream) return -1;
    printf("File open\n");

    char* numbers[] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    int numbersSize[] = {4, 3, 3, 5, 4, 4, 3, 5, 5, 4};
    int maxSize=5;

    char* line;
    size_t lineLen;
    Link first={0,0};
    Link* current=&first;
    while (getline(&line, &lineLen, stream) != -1) {
        char letter='a';
        for(size_t i = 0; letter!='\0';++i){
            letter=line[i];
            if ('0' <= letter && letter <= '9') {
                if(current->value==0) current->value = (letter - 48) * 10;
                else
                    current->value += -current->value % 10 + letter - '0';
                }else if ('a' <= letter && letter <= 'z'){
                size_t len=lineLen-i;
                if(len<2) continue;
                if(len>maxSize) len=maxSize;
                char number[len];
                strncpy(number, &line[i], len);
                for(int j=0;j<10; ++j)
                    if (!memcmp(number,numbers[j],len<numbersSize[j]?len:numbersSize[j]))
                        if(current->value==0) current->value = j * 10;
                        else current->value += -current->value%10 + j;
            }
        }
        printf("\n %s %d\n",line,current->value);
        if( current->value%10==0 ) current->value+=current->value/10;
        Link* _tmp= calloc(1,sizeof(Link));
        current->next=_tmp;
        current=_tmp;
    }
    free(line);
    fclose(stream);
    printf("Read done\n");

    current=&first;
    int sum=0;
    while (current->next) {
        sum += current->value;
        current = current->next;
    }
    sum += current->value; //last one

    printf("Total sum : %d\n",sum);

    delete(&first);

    return 0;
}
