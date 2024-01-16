#include <stdio.h>
#define MAX 9

void selection(int *, int); 

int main(){

    int iarray[MAX] = {8,1,4,1,5,9,2,6,5};
    int i;

    printf ("Before sorting:\n");
    for (i=0; i<MAX; i++){
        printf ("<%d> ", iarray[i]);
    }
    printf ("\n");

    printf ("Sorting:\n");
    selection (iarray, MAX);

    printf ("After sorting:\n");
    for (i=0; i<MAX; i++){
        printf ("<%d> ", iarray[i]);
    }
    printf ("\n");

    return 0;
}
void selection(int * ptr, int max){
    int i, j, k, min, tmp;

    for (i=0; i<max-1; i++){
        min = i;
        for (j=i+1; j<max; j++){
            if (ptr[j] < ptr[min]){
                min = j;
            }
        }
        tmp = ptr[i];
        ptr[i] = ptr[min];
        ptr[min] = tmp;
        
        for (k=0; k<MAX; k++){
            printf ("<%d> ", ptr[k]);
        }
        printf ("\n");
    }
    return;
}
