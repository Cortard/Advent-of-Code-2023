#include <stdio.h>
#include <malloc.h>

typedef struct Line Line;
struct Line{
    int size;
    int maxSize;
    char* text;
};
char get(Line* line, int i){
    if(line==NULL) return 0;
    if(i<0) return 0;
    if(i >= line->size) return 0;
    return line->text[i];
}
void add(Line* line, char letter){
    if(line->size==line->maxSize-1){
        line->maxSize+=10;
        line->text=realloc(line->text,line->maxSize);
        if(line->text==NULL) exit(EXIT_FAILURE);
    }
    line->text[line->size++]=letter;
    line->text[line->size]='\0';
}
Line* create(){
    Line* _tmp= malloc(sizeof(Line));
    _tmp->size=0;
    _tmp->maxSize=10;
    _tmp->text= malloc(_tmp->maxSize);
    _tmp->text[0]='\0';
    return _tmp;
}
void delete(Line** line){
    if(*line==NULL) return;
    free((*line)->text);
    free(*line);
    *line=NULL;
}

int isASymbol(char letter){
    return letter!=0 && letter!='.' && !( '0'<=letter && letter<='9' );
}

int main() {
    FILE *stream;
    stream= fopen("../input.txt", "r");
    if(!stream) return -1;
    printf("File open\n");

    Line* previous= create();
    Line* actual= create();
    Line* next= create();
    char newLetter;
    int i=0;
    int sum=0;
    int value=0,valide=0;
    while( fread(&newLetter,1,1,stream) ) {
        if(newLetter!=10) {
            add(next, newLetter);
        }
        //printf("\n~~~~\n%s\n%s\n%s\n",previous->text,actual->text,next->text);
        char letter=get(actual,i);
        if('0'<=letter && letter<='9'){
            if(value==0 && valide==0){
                if(isASymbol(get(previous,i-1)))
                    valide=1;
                if(isASymbol(get(actual,i-1)))
                    valide=1;
                if(isASymbol(get(next,i-1)))
                    valide=1;
            }
            if(valide==0) {
                if(isASymbol(get(previous,i)))
                    valide=1;
                if(isASymbol(get(next,i)))
                    valide=1;
            }
            value=value*10+letter-'0';
        }else{
            if(value==0) valide==0;
            if(isASymbol(get(previous,i)))
                valide=1;
            if(isASymbol(get(actual,i)))
                valide=1;
            if(isASymbol(get(next,i)))
                valide=1;
            if (valide==1 && value!=0){
                sum+=value;
                value=0;
            }else if(value!=0){
                value=0;
            }
            valide=0;

        }
        if(newLetter==10){
            delete(&previous);
            previous=actual;
            actual=next;
            next=create();
            i=0;
        }else ++i;
    }

    delete(&previous);
    previous=actual;
    actual=next;
    i=0;
    while (i<actual->size){
        char letter=get(actual,i);
        if('0'<=letter && letter<='9'){
            if(value==0 && valide==0){
                if(isASymbol(get(previous,i-1)))
                    valide=1;
                if(isASymbol(get(actual,i-1)))
                    valide=1;
            }
            if(valide==0) {
                if(isASymbol(get(previous,i)))
                    valide=1;
            }
            value=value*10+letter-'0';
        }else{
            if(value==0) valide==0;
            if(isASymbol(get(previous,i)))
                valide=1;
            if(isASymbol(get(actual,i)))
                valide=1;
            if (valide==1 && value!=0){
                sum+=value;
                value=0;
            }else if(value!=0){
                value=0;
            }
            valide=0;
        }
        ++i;
    }

    delete(&previous);
    delete(&actual);

    printf("sum : %d",sum);

    return 0;
}
