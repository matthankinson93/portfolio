#include <stdio.h>
#include <stdlib.h>
#define MAX 9

void quick(int *, int leftIndex, int rightIndex);
void Squick(int *, int);


int main(){
    int iarray[MAX] = {8,1,4,1,5,9,2,6,5};
    int i;

    printf ("Pivot at left\nBefore sorting:\n");
    for (i=0; i<MAX; i++){
        printf ("<%d> ", iarray[i]);
    }
    printf ("\n");

    printf ("Sorting:\n");
    Squick (iarray, 8);

    printf ("After sorting:\n");
    for (i=0; i<MAX; i++){
        printf ("<%d> ", iarray[i]);
    }
    printf ("\n");

    return 0;
}
void Squick(int * ptr, int r){
     quick(ptr, 0, r);
}
void quick(int * ptr, int l, int r){
    int i, k, first, last, pivot;

    if (r>l){
        first = l;
        last = r;
        i = rand()% (last-first+1) + first;
        pivot = ptr[i];
        ptr[i] = ptr[first];
        ptr[first] = pivot;

        while (last > first){
            if (ptr[last] >= pivot){
                last--;
            } else {
                ptr[first] = ptr[last];
                ptr[last] = ptr[++first];
                ptr[first] = pivot;
            }
        }
        printf ("r = %d, l = %d, Pivot = <%d>\n", r, l, pivot);
        for (k=0; k<MAX; k++){
            printf ("<%d> ", ptr[k]);
        }
        printf ("\n");

        pivot = first;
        quick(ptr, l, pivot-1);
        quick(ptr, pivot+1, r);
    }
    return;
}
