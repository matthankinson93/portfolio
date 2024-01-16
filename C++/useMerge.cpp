#include <stdio.h>
#define MAX 9

void merge(int *, int leftIndex, int rightIndex); 
void smerge(int *, int rightIndex); 

int main(){

    int iarray[MAX] = {8,1,4,1,5,9,2,6,5};
    int i;

    printf ("Before sorting:\n");
    for (i=0; i<MAX; i++){
        printf ("<%d> ", iarray[i]);
    }
    printf ("\n");

    printf ("Sorting:\n");
    smerge (iarray, 8);

    printf ("After sorting:\n");
    for (i=0; i<MAX; i++){
        printf ("<%d> ", iarray[i]);
    }
    printf ("\n");

    return 0;
}

void smerge(int * ptr, int r){
    merge(ptr, 0, r);
}

void merge(int * ptr, int l, int r){
    int i, j, k, m, tmp[MAX];

    if (r > l){
        m = (r+l)/2;
        merge(ptr, l, m);
        merge(ptr, m+1, r);

        /* tmp[1] to tmp[m], from small to large */ 
        for (i=m+1; i>l; i--){
            tmp[i-1] = ptr[i-1];
        }

        /* tmp[m+1] to tmp[r], from large to small */
        for (j=m; j<r; j++){
            tmp[r+m-j] = ptr[j+1];
        }

        /* Merge from two ends, small one first */
        for (k=l; k<= r; k++){
            ptr[k] = (tmp[i]<tmp[j]) ? tmp[i++] : tmp[j--];
        }
    }
    printf ("l=%d, r=%d\n", l, r);
    for (k=0; k<MAX; k++){
        printf ("<%d> ", ptr[k]);
    }
    printf ("\n");
    return;
}
