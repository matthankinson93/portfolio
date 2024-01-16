// From C++ algorithm
#include <stdio.h>
#define MAX 9

void insertion(int *, int); 

int main(){

    int iarray[MAX] = {8,1,4,1,5,9,2,6,5};
    int i;

    printf ("Before sorting:\n");
    for (i=0; i<MAX; i++){
        printf ("<%d> ", iarray[i]);
    }
    printf ("\n");

    printf ("Sorting:\n");
    insertion (iarray, MAX);

    printf ("After sorting:\n");
    for (i=0; i<MAX; i++){
        printf ("<%d> ", iarray[i]);
    }
    printf ("\n");

    return 0;
}
void insertion(int * ptr, int max){
    int i, j, k, tmp;
    for (i=1; i<max; i++){
        tmp = ptr[i];
        j = i;
        while (j>0 && ptr[j-1] > tmp){
            ptr[j] = ptr[j-1];
            j--;
        }
        ptr[j] = tmp;

        for (k=0; k<MAX; k++){
            printf ("<%d> ", ptr[k]);
        }
        printf ("\n");
    }
    return;
}
