#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main() {
    struct timeval start, end;
    gettimeofday(&start, NULL);

#define TEST 0
#if TEST==1
    #define nbValue 3
        int time[nbValue]={7,15,30};
        int distance[nbValue]={9,40,200};
#else
#define nbValue 4
    int time[nbValue]={52,94,75,94};
    int distance[nbValue]={426,1374,1279,1216};
#endif

    int sum=1;
    for(int i=0;i<nbValue;++i){
        int cpt=0;
        for(int j=0;j<time[i];++j){
            if((time[i]-j)*j>distance[i])
                ++cpt;
        }
        sum*=cpt;
    }

    printf("Res : %d\n",sum);

    gettimeofday(&end, NULL);
    printf("Time taken: %f seconds\n", ((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec)) / 1e6);

    return 0;
}