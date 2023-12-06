#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

int isDigit(char letter){
    return '0'<=letter && letter<='9';
}

void goNextDataSet(FILE* stream){
    char letter=0;
    while (fread(&letter,1,1,stream)) if(letter==':') break;
}

typedef struct Seed{
    unsigned long long start;
    unsigned long long len;
    struct Seed* next;
}Seed;

void getSeeds(FILE* stream,Seed** seeds, unsigned long long* minLength){
    goNextDataSet(stream);

    char* number=malloc(1);
    number[0]='\0';
    size_t numberSize=0;
    size_t numberSizeMax=1;
    char letter=1,pastLetter=0;
    *seeds=NULL;
    Seed* current=*seeds;
    int fillState=0;
    while (fread(&letter,1,1,stream) && (letter!='\n' || letter!=pastLetter)){
        pastLetter=letter;
        if(isDigit(letter)){
            number[numberSize++]=letter;
            if(numberSize==numberSizeMax){
                char* tmp= realloc(number,++numberSizeMax);
                if(tmp==NULL){
                    printf("Error reallocate number");
                    free(number);
                    exit(EXIT_FAILURE);
                }
                number=tmp;
                number[numberSizeMax-1]='\0';
            }
        }else if(numberSize){
            switch (fillState) {
                case 0: {
                    Seed *new = calloc(1, sizeof(Seed));
                    new->start = strtoll(number, NULL, 10);

                    if (*seeds == NULL) {
                        *seeds = new;
                        current=new;
                    }else{
                        Seed* father=*seeds;
                        while (father->next && father->next->start < new->start) father=father->next;

                        if(father==*seeds){
                            if(father->start > new->start){
                                new->next=father;
                                *seeds=new;
                                current=new;
                                break;
                            }
                        }

                        new->next=father->next;
                        father->next=new;
                        current=new;
                    }

                    break;
                }
                case 1:
                    current->len= strtoll(number,NULL,10);
                    if(current->len>*minLength)
                        *minLength=current->len;
                    break;
                default:
                    exit(EXIT_FAILURE);
            }
            fillState=(1+fillState)%2;
            for(int i=0;i<numberSize;++i)
                number[i]='\0';
            numberSize=0;
        }
    }
    free(number);
}

typedef struct Range{
    unsigned long long dest;
    unsigned long long src;
    unsigned long long len;
    struct Range* next;
}Range;


void getMap(FILE* stream,Range** maps){
    char* number=malloc(1);
    number[0]='\0';
    size_t numberSize=0;
    size_t numberSizeMax=1;
    char letter=1,pastLetter=0;
    *maps = NULL;
    Range* current=*maps;
    int fillState=0;
    while (fread(&letter,1,1,stream) && (letter!='\n' || letter!=pastLetter)){
        pastLetter=letter;
        if(isDigit(letter)){
            number[numberSize++]=letter;
            if(numberSize==numberSizeMax){
                char* tmp= realloc(number,++numberSizeMax);
                if(tmp==NULL){
                    printf("Error reallocate number");
                    free(number);
                    exit(EXIT_FAILURE);
                }
                number=tmp;
                number[numberSizeMax-1]='\0';
            }
        }else if(numberSize){
            switch (fillState) {
                case 0: {
                    Range *new = calloc(1, sizeof(Range));
                    new->dest = strtoll(number, NULL, 10);

                    if (*maps == NULL) {
                        *maps = new;
                        current=new;
                    }else{
                        Range* father=*maps;
                        while (father->next && father->next->dest < new->dest) father=father->next;

                        if(father==*maps){
                            if(father->dest>new->dest){
                                new->next=father;
                                *maps=new;
                                current=new;
                                break;
                            }
                        }

                        new->next=father->next;
                        father->next=new;
                        current=new;
                    }

                    break;
                }
                case 1:
                    current->src= strtoll(number,NULL,10);
                    break;
                case 2:
                    current->len= strtoll(number,NULL,10);
                    break;
                default:
                    exit(EXIT_FAILURE);
            }
            fillState=(1+fillState)%3;
            for(int i=0;i<numberSize;++i)
                number[i]='\0';
            numberSize=0;
        }
    }
    if(!fread(&letter,1,1,stream)){
        current->len= strtoll(number,NULL,10);
    }
    free(number);
}

void getMaps(FILE* stream,Range** maps,int nbMap){
    for(int i=0;i<nbMap;++i){
        goNextDataSet(stream);//Go start next map
        getMap(stream,&(maps[i]));
    }
}

unsigned long long  reverseMap(Range* map,unsigned long long seedValue){
    Range* current=map;
    while(current && current->dest <= seedValue){
        if( seedValue < current->dest+current->len )
            return seedValue-current->dest+current->src;
        current=current->next;
    }
    return seedValue;
}

int existSeed(Seed* seeds, unsigned long long seedValue){
    Seed* current=seeds;
    while(current && current->start < seedValue){
        if( seedValue<current->start+current->len )
            return 1;
        current=current->next;
    }
    return 0;
}

int exist(Range** maps, int nbMap, Seed * seeds, unsigned long long seedValue){
    for(int i=nbMap-1; i>=0; --i){
        seedValue=reverseMap(maps[i],seedValue);
    }
    return existSeed(seeds, seedValue);
}

int main() {
    struct timeval start, end;
    gettimeofday(&start, NULL);

    FILE *stream;
    stream = fopen("../input.txt", "r");
    if (!stream) return -1;

    Seed* seeds;
    unsigned long long minLength=0;
    getSeeds(stream,&seeds,&minLength);

    Range* maps[7];
    int nbMap=7;
    getMaps(stream,maps,nbMap);

    fclose(stream);

    unsigned long long b=0;
    while(!exist(maps,nbMap,seeds,b)) b+=1;
    printf("b=%llu\n",b);

    gettimeofday(&end, NULL);
    printf("Time taken: %f seconds\n", ((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec))/ 1e6);

    return 0;
}