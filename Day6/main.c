#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main() {
    struct timeval start, end;
    gettimeofday(&start, NULL);

    #define TEST 0
    #if TEST==1
        int time=71530;
        int distance=940200;
    #else
        unsigned long long time=52947594;
        unsigned long long distance=426137412791216;
    #endif

    unsigned long long sum=0;
    int win=0;
    for(int j=0;j<time;++j){
        if((time-j)*j>distance) {
            if(!win) win=1;
            ++sum;
        }else if(win) break;
    }

    printf("Res : %d\n",sum);

    gettimeofday(&end, NULL);
    printf("Time taken: %f seconds\n", ((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec)) / 1e6);

    return 0;
}