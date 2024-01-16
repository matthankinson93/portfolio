// From C++ algorithm
#include <stdio.h>
#define MAX 9

void shell(int *, int); 

int main(){

    int iarray[MAX] = {8,1,4,7,5,9,2,6,3};
    int i;

    printf ("Before sorting:\n");
    for (i=0; i<MAX; i++){
        printf ("<%d> ", iarray[i]);
    }
    printf ("\n");

    printf ("Sorting:\n");
    shell (iarray, MAX);

    printf ("After sorting:\n");
    for (i=0; i<MAX; i++){
        printf ("<%d> ", iarray[i]);
    }
    printf ("\n");

    return 0;
}
void shell(int * ptr, int max){
    int i, j, h, k, tmp;

    /* Form the largest shell first.  
    for (h=1; h<=max/9; h=3*h+1);
    */

    /* Start with the largest shell.
       Move on to the smaller one h /= 3
    for (; h>0; h/=3){
    */

    /* modify for summative, start with shells with 4 elements.*/
    for (h=4; h>0; h/=2){
        printf ("h = %d\n", h);
    
        /* Start with first element in the second shell.
           Move till the end of array.*/ 
        for (i=h; i<max; i++){
            tmp = ptr[i];
            j = i;
   
            /* Compare and find the bigger one 
               for same elements in different shells.
               Bigger ones are put on the right. 
               Compare a[5] with a[1].
               Compare a[9] with a[5] and then a[1].*/
            while (j>=h && ptr[j-h]>tmp){
                ptr[j] = ptr[j-h];
                j -= h;
            }
            ptr[j] = tmp;
            for (k=0; k<MAX; k++){
                printf ("<%d> ", ptr[k]);
            }
            printf ("\n");
        }
/*        for (k=0; k<MAX; k++){
            printf ("<%d> ", ptr[k]);
        }
        printf ("\n");
*/    }
    return;
}
