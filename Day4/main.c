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
numberList* createNumberList(int sizeMax){
    numberList* _tmp= malloc(sizeof(numberList));
    _tmp->size=0;
    _tmp->sizeMax=sizeMax;
    _tmp->list= (int*)calloc(sizeMax,sizeof(int));
    return _tmp;
}
void deleteNumberList(numberList** pNumberList){
    free((*pNumberList)->list);
    free(*pNumberList);
    pNumberList=NULL;
}

typedef struct Link Link;
struct Link{
    int value;
    int number;
    Link* next;
};
void deleteLink(Link* link){
    if(link->next==NULL) return;
    deleteLink(link->next);
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
    Link* valueCard= malloc(sizeof(Link*));
    valueCard->next=NULL;
    valueCard->value=0;
    valueCard->number=1;
    numberList* list = createNumberList(10);
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
                            ++valueCard->value;
                        }
                        value = 0;
                    }
                }
                break;
            default:
                break;
        }
        if(letter==10){
            Link* temp=valueCard;
            for(int i=0;i<valueCard->value;++i){
                if(temp->next==NULL) {
                    temp->next = malloc(sizeof(Link));
                    temp->next->value=0;
                    temp->next->number=1;
                    temp->next->next=NULL;
                }
                temp=temp->next;
                temp->number+=valueCard->number;
            }
            sum+=valueCard->number;
            state=0;
            value=0;
            int _tmp=list->sizeMax;
            list= createNumberList(_tmp);

            temp=valueCard;
            if(valueCard->next==NULL){
                valueCard->next=malloc(sizeof(Link));
                valueCard->next->value=0;
                valueCard->next->number=1;
                valueCard->next->next=NULL;
            }
            valueCard=valueCard->next;
            free(temp);
        }
    }
    sum+=valueCard->number;//last
    deleteNumberList(&list);

    printf("sum : %d",sum);
    return 0;
}
