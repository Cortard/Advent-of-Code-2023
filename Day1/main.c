#include <stdio.h>
#include <malloc.h>

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
    
    char letter;
    Link first={0,0};
    Link* current=&first;
    while(fread(&letter,1,1,stream)){
        if(48<=letter && letter<=57) {
            if(current->value<10)
                current->value = (letter-48)*10;
            else
                current->value+=-current->value%10+letter-48;
        }else if(letter==10){
            if( current->value%10==0 ) current->value+=current->value/10;
            Link* _tmp= calloc(1,sizeof(FILE));
            current->next=_tmp;
            current=_tmp;
        }
    }
    if( current->value%10==0 ) current->value+=current->value/10;//Last one

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
