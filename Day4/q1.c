#include <stdio.h>
#include <malloc.h>

typedef struct numberList numberList;
struct numberList{
    int* list;
    int size;
    int sizeMax;
};

void add(numberList* list, int value){
    if(list->size==list->sizeMax) {
        list->list = (int*)realloc(list->list, ++list->sizeMax * sizeof(int));
        if(list->list==NULL) exit(EXIT_FAILURE);
    }
    int i;
    for(i=0;i<list->size && list->list[i]<value;++i);
    if(i!=list->size)
        for(int j=list->size; j>=i; --j)
            list->list[j]=list->list[j-1];
    list->list[i]=value;
    ++list->size;
}
int in(numberList* list, int value) {
    int a=0;
    int b=list->size;
    int middle=-1;
    while(a<=b){
        middle=(int)((a+b)/2.f);
        int valueList=list->list[middle];
        if(valueList==value) return 1;
        if(value<valueList) b=middle-1;
        else a=middle+1;
    }
    return 0;
}
numberList* create(int sizeMax){
    numberList* _tmp= malloc(sizeof(numberList));
    _tmp->size=0;
    _tmp->sizeMax=sizeMax;
    _tmp->list= (int*)calloc(sizeMax,sizeof(int));
    return _tmp;
}
void delete(numberList** pNumberList){
    free((*pNumberList)->list);
    free(*pNumberList);
    pNumberList=NULL;
}

int isDigit(char letter){
    return '0'<=letter && letter<='9';
}

int main() {
    FILE *stream;
    stream = fopen("../input.txt", "r");
    if (!stream) return -1;
    printf("File open\n");

    char letter;
    int state=0;
    int sum=0;
    int value=0;
    int valueCard=0;
    numberList* list = create(10);
    while (fread(&letter, 1, 1, stream)) {
        if(letter==':') state=1;
        else if(letter=='|') state=2;
        switch (state) {
            case 1:
                if(isDigit(letter)){
                    value=value*10+letter-'0';
                }else if(letter==' ' && value!=0){
                    add(list, value);
                    value=0;
                }
                break;
            case 2:
                if(isDigit(letter)){
                    value=value*10+letter-'0';
                }else {
                    if(value!=0) {
                        if (in(list, value)) {
                            if (valueCard == 0) ++valueCard;
                            else valueCard *= 2;
                        }
                        value = 0;
                    }
                }
                break;
        }
        if(letter==10){
            sum+=valueCard;
            state=0;
            value=0;
            valueCard=0;
            int _tmp=list->sizeMax;
            list= create(_tmp);
        }
    }
    sum+=valueCard;//last
    delete(&list);

    printf("sum : %d",sum);
    return 0;
}
