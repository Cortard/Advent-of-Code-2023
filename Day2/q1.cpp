#include <stdio.h>


int getRank(const char color){
    if(color=='b') return 0;
    if(color=='r') return 1;
    if(color=='g') return 2;
    return -1;
}

int main() {FILE *stream;
    stream= fopen("../input.txt", "r");
    if(!stream) return -1;
    printf("File open\n");

    int quantityMax[3];
    quantityMax[getRank('r')]=12;
    quantityMax[getRank('g')]=13;
    quantityMax[getRank('b')]=14;

    int sum=0;

    int id=0;
    int getId=0;
    int quantity=0;

    char letter;
    int waitNextLine=0;
    while(fread(&letter,1,1,stream)) {
        if(waitNextLine){
            if(letter==10) waitNextLine=0;
            continue;
        }
        else if(letter==':')
            getId=1;
        else if('0'<=letter && letter<='9'){
            if(!getId)
                id=id*10+(letter-'0');
            else
                quantity=quantity*10+(letter - '0');
        }else if(letter=='b' || letter=='g' || letter=='r'){
            if(quantity==0) continue;
            if(quantityMax[getRank(letter)]<quantity){
                id=0;getId=0;quantity=0;
                waitNextLine=1;
            }else{
                quantity=0;
            }
        }else if(letter==10){
            sum+=id;
            id=0;getId=0;quantity=0;
        }
    }
    if(id) sum+=id;

    printf("Sum of possible : %d",sum);

    return 0;
}
