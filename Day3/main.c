#include <stdio.h>
#include <malloc.h>
#include <math.h>

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
    _tmp->maxSize=150;
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
int isANumber(char letter){
    return '0'<=letter && letter<='9';
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
    int sum=0;
    while( fread(&newLetter,1,1,stream) ) {
        add(next, newLetter);
        if(newLetter==10){
            for(int i=0;i<actual->size;++i){
                if(get(actual,i)=='*'){
                    int value[2];
                    value[0]=0;value[1]=0;
                    int valueFound=0;
                    int j=i-1;
                    //previous
                    while (isANumber(get(previous,j))){
                        value[valueFound]+=(get(previous,j)-'0')*pow(10,i-j-1);
                        --j;
                    }
                    j=i;
                    while (isANumber(get(previous,j))){
                        value[valueFound]=value[valueFound]*10+get(previous,j)-'0';
                        j++;
                    }
                    if(j==i)j=i+1;
                    if(value[valueFound]!=0) valueFound++;
                    while (isANumber(get(previous,j))){
                        value[valueFound]=value[valueFound]*10+get(previous,j)-'0';
                        j++;
                    }
                    //actual
                    j=i-1;
                    if(value[valueFound]!=0) valueFound++;
                    while (isANumber(get(actual,j))){
                        value[valueFound]+=(get(actual,j)-'0')*pow(10,i-j-1);
                        --j;
                    }
                    j=i+1;
                    if(value[valueFound]!=0) valueFound++;
                    while (isANumber(get(actual,j))){
                        value[valueFound]=value[valueFound]*10+get(actual,j)-'0';
                        j++;
                    }
                    //next
                    j=i-1;
                    if(value[valueFound]!=0) valueFound++;
                    while (isANumber(get(next,j))){
                        value[valueFound]+=(get(next,j)-'0')*pow(10,i-j-1);
                        --j;
                    }
                    j=i;
                    while (isANumber(get(next,j))){
                        value[valueFound]=value[valueFound]*10+get(next,j)-'0';
                        j++;
                    }
                    if(j==i)j=i+1;
                    if(value[valueFound]!=0) valueFound++;
                    while (isANumber(get(next,j))){
                        value[valueFound]=value[valueFound]*10+get(next,j)-'0';
                        j++;
                    }

                    if(value[0]!=0 && value[1]!=0){
                        printf("%d * %d = %d\n",value[0],value[1],value[0]*value[1]);
                        sum+=value[0]*value[1];
                    }
                }

            }

            delete(&previous);
            previous=actual;
            actual=next;
            next=create();
        }
    }
    for(int i=0;i<actual->size;++i){
        if(get(actual,i)=='*'){
            int value[2];
            value[0]=0;value[1]=0;
            int valueFound=0;
            int j=i-1;
            //previous
            while (isANumber(get(previous,j))){
                value[valueFound]+=(get(previous,j)-'0')*pow(10,i-j-1);
                --j;
            }
            j=i;
            while (isANumber(get(previous,j))){
                value[valueFound]=value[valueFound]*10+get(previous,j)-'0';
                j++;
            }
            if(j==i)j=i+1;
            if(value[valueFound]!=0) valueFound++;
            while (isANumber(get(previous,j))){
                value[valueFound]=value[valueFound]*10+get(previous,j)-'0';
                j++;
            }
            //actual
            j=i-1;
            if(value[valueFound]!=0) valueFound++;
            while (isANumber(get(actual,j))){
                value[valueFound]+=(get(actual,j)-'0')*pow(10,i-j-1);
                --j;
            }
            j=i+1;
            if(value[valueFound]!=0) valueFound++;
            while (isANumber(get(actual,j))){
                value[valueFound]=value[valueFound]*10+get(actual,j)-'0';
                j++;
            }
            //next
            j=i-1;
            if(value[valueFound]!=0) valueFound++;
            while (isANumber(get(next,j))){
                value[valueFound]+=(get(next,j)-'0')*pow(10,i-j-1);
                --j;
            }
            j=i;
            while (isANumber(get(next,j))){
                value[valueFound]=value[valueFound]*10+get(next,j)-'0';
                j++;
            }
            if(j==i)j=i+1;
            if(value[valueFound]!=0) valueFound++;
            while (isANumber(get(next,j))){
                value[valueFound]=value[valueFound]*10+get(next,j)-'0';
                j++;
            }

            if(value[0]!=0 && value[1]!=0){
                printf("%d * %d = %d\n",value[0],value[1],value[0]*value[1]);
                sum+=value[0]*value[1];
            }
        }

    }
    delete(&previous);
    delete(&actual);

    printf("sum : %d",sum);

    return 0;
}
