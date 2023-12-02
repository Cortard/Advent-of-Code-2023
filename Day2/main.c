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

    int sum=0;

    int getId=0;
    int quantity=0;

    char letter;
    int waitNextLine=0;
    while(fread(&letter,1,1,stream)) {
        if(letter==':')
            getId=1;
        else if('0'<=letter && letter<='9'){
            if(getId)
                quantity=quantity*10+(letter - '0');
        }else if(letter=='b' || letter=='g' || letter=='r'){
            if(quantity==0) continue;
            if(quantityMax[getRank(letter)]<quantity){
                quantityMax[getRank(letter)]=quantity;
            }
            quantity=0;
        }else if(letter==10){
            int _tmp=1;
            for(int i=0;i<3;++i) {
                _tmp *= quantityMax[i];
                quantityMax[i]=0;
            }
            sum+=_tmp;
            getId=0;
        }
    }
    if(quantityMax[0]){
        int _tmp=1;
        for(int i=0;i<3;++i) {
            _tmp *= quantityMax[i];
            quantityMax[i]=0;
        }
        sum+=_tmp;
        getId=0;
    }

    printf("Sum of power : %d",sum);

    return 0;
}
