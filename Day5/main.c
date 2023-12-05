#include <stdio.h>
#include <malloc.h>
#include <memory.h>

typedef unsigned char bool;
#define false 0
#define true 1

typedef struct Range{
    long long start;
    long long length;
} Range;

int isDigit(char letter){
    return '0'<=letter && letter<='9';
}

bool getSeeds(FILE* stream, Range** seeds, size_t* nbSeeds, size_t* nbSeedsMax){
    char* number= malloc(1);
    number[0]='\0';
    size_t numberSize=0;
    size_t numberSizeMax=1;
    bool end=false;
    bool length=false;
    while(!end){
        size_t bytesRead=fread(number+numberSize, 1, 1, stream);
        ++numberSize;
        if(!bytesRead){
            printf("Plus rien a lire");
            free(number);
            free(seeds);
            return false;
        }
        if(numberSize==numberSizeMax){
            char* _tmp=realloc(number,++numberSizeMax);
            if(_tmp==NULL) {
                printf("Number reallocate error");
                free(number);
                free(*seeds);
                return false;
            }
            number=_tmp;
            number[numberSizeMax-1]='\0';
        }
        if(!isDigit(number[numberSize-1])){
            if(number[numberSize-1]=='\n') end=true;

            if(numberSize>0) {
                if(!length) {
                    if(*nbSeeds==*nbSeedsMax){
                        *nbSeedsMax+=10;
                        Range *_tmp = realloc(*seeds, (*nbSeedsMax) * sizeof(Range));
                        printf("size alloc %lld",*nbSeedsMax);
                        if (_tmp == NULL) {
                            printf("Seeds reallocate error");
                            free(number);
                            free(*seeds);
                            return false;
                        }
                        *seeds = _tmp;
                    }
                    ++(*nbSeeds);
                    (*seeds)[*nbSeeds-1].start=strtoll(number, NULL, 10);
                }
                else (*seeds)[*nbSeeds-1].length=strtoll(number, NULL, 10);
                length=!length;

                for (int i = 0; i < numberSize; ++i) {
                    number[i] = '\0';
                }
                numberSize = 0;
            }
        }
    }
    free(number);

    return true;
}

typedef struct Map{
    long long srcStart;
    long long destStart;
    long long len;
}Map;

Map* add(Map** map, Map* new, size_t* mapSize, size_t* mapSizeMax){
    /* Sad but I am tired and this dichotomy doesn't want to work
    int a=0;
    int b=(int)*mapSize;
    int middle=0;
    while(a<=b){
        middle=(a+b)/2.f;
        if((*map)[middle].srcStart<new->srcStart) a=middle+1;
        else b=middle-1;
    }*///So I did this but I lose some performance :
    int middle;
    for(middle=0;middle<(int)*mapSize;++middle)if(new->srcStart<(*map)[middle].srcStart) break;

    if(*mapSize==*mapSizeMax){
        Map* _tmp= realloc(*map,++(*mapSizeMax)* sizeof(Map));
        if(_tmp==NULL){
            printf("error reallocate map in add");
            free(*map);
            free(new);
            return NULL;
        }
        *map=_tmp;
    }
    for(int i=(int)*mapSize; i>middle; --i)
        memcpy(*map+i,*map+i-1, sizeof(Map));
    ++(*mapSize);
    memcpy(*map+middle,new, sizeof(Map));
    free(new);
    return *map+middle;
}

bool mapApplyOne(Map* map, size_t mapSize,Range* range,Range** seeds,size_t* nbSeeds,size_t* nbSeedsMax){
    for(int i=0;i<mapSize; ++i){
        if(map[i].srcStart<=range->start && range->start<map[i].srcStart+map[i].len){
            if(range->start+range->length < map[i].srcStart+map[i].len){
                range->start+=-map[i].srcStart+map[i].destStart;
                return true;
            }else{
                if(*nbSeeds==*nbSeedsMax) {
                    (*nbSeedsMax)+=10;
                    Range *_tmp = realloc(*seeds, (*nbSeedsMax) * sizeof(Range));
                    printf("size alloc %lld",*nbSeedsMax);
                    if (_tmp == NULL) {
                        printf("Range reallocate error");
                        free(*seeds);
                        return false;
                    }
                    *seeds = _tmp;
                }
                ++*nbSeeds;
                Range* new=*seeds+(*nbSeeds-1);
                new->start=map[i].srcStart+map[i].len;
                new->length=range->length-(new->start-range->start);
                range->start+=-map[i].srcStart+map[i].destStart;
                range->length-=new->length;
            }
        }
    }
    return true;
}

bool mapApply(Range** seeds, size_t* nbSeeds,size_t* nbSeedsMax, Map* map, size_t mapSize){
    Range* it = *seeds;
    for(int i=0;i<*nbSeeds;++i){
        if(!mapApplyOne(map,mapSize,*seeds+i,seeds,nbSeeds,nbSeedsMax)) return false;
    }
    return true;
}

#include <sys/time.h>
int main() {
    struct timeval start, end;
    gettimeofday(&start, NULL);

    FILE *stream;
    stream = fopen("../input.txt", "r");
    if (!stream) return -1;
    printf("File open\n");

    {
        char* _tmp=malloc(7);
        fread(_tmp,7,1,stream);//skip the "seeds: "
        free(_tmp);
    }

    Range* seeds= malloc(10* sizeof(Range));
    if(seeds==NULL){
        printf("Error seeds malloc");
        return EXIT_FAILURE;
    }
    size_t nbSeeds=0;
    size_t nbSeedsMax=10;

    if(!getSeeds(stream,&seeds,&nbSeeds,&nbSeedsMax)) return EXIT_FAILURE;

    {
        char letter;
        size_t bytesRead;
        do {
            bytesRead = fread(&letter, 1, 1, stream);
            if (letter == ':') break;
        } while (bytesRead); //Go on first data begin
        if (!bytesRead) {
            printf("error go first data start");
            free(seeds);
            return EXIT_FAILURE;
        }
    }

    Map* map= malloc(sizeof(Map));
    size_t mapSize=0;
    size_t mapSizeMax=1;
    Map* current=NULL;
    char* number= malloc(1);
    number[0]='\0';
    size_t numberSize=0;
    size_t numberSizeMax=1;
    char letter;
    while (fread(&letter, 1, 1, stream)){
        if(isDigit(letter)){
            number[numberSize++]=letter;
            if(numberSize==numberSizeMax){
                char* _tmp= realloc(number,++numberSizeMax);
                if(_tmp==NULL){
                    printf("Error reallocate number");
                    free(number);
                    free(seeds);
                    return EXIT_FAILURE;
                }
                number=_tmp;
                number[numberSize]='\0';
            }
        }else{
            if(letter==':'){
                if(!mapApply(&seeds,&nbSeeds,&nbSeedsMax,map,mapSize)){
                    printf("error apply");
                    free(map);
                    return EXIT_FAILURE;
                }
                mapSize=0;
            }else if(numberSize>0){
                if(current==NULL){//first value
                    current= malloc(sizeof(Map));
                    current->destStart=strtoll(number,NULL,10);
                }else if(current<map || map+mapSize<current){//already in map ?
                    current->srcStart=strtoll(number,NULL,10);
                    current= add(&map,current,&mapSize,&mapSizeMax);
                    if(current==NULL) {
                        free(number);
                        return EXIT_FAILURE;
                    }
                }else{
                    current->len= strtoll(number,NULL,10);
                    current=NULL;
                }
                for(int i=0;i<numberSize;++i) number[i]='\0';
                numberSize=0;
            }
        }
    }

    if(!mapApply(&seeds,&nbSeeds,&nbSeedsMax,map,mapSize)){
        printf("error apply");
        free(map);
        return EXIT_FAILURE;
    }

    free(map);
    long long min=seeds[0].start;
    for(int i=1; i<nbSeeds; ++i)
        if(seeds[i].start<min)
            min=seeds[i].start;

    printf("min : %lld\n",min);

    free(seeds);

    gettimeofday(&end, NULL);
    printf("Time taken: %f seconds\n", ((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec))/ 1e6);

    return 0;
}