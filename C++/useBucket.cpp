#include <stdio.h>
#define MAX 10
#define RADIX 8
#define Buckets 256

/* Use the right most 8 bits as the key only.
   This will limit the bucket number to 256.
   Bucket 0 to Bucket 255. */

void bucket(unsigned * DataArray, int ArraySize); 

int main(){

    unsigned j, iarray[MAX] = {8,1,4,1,5,9,2,6,5};
    int i;

    printf ("Before sorting:\n");
    for (i=0; i<MAX; i++){
        printf ("<%d> ", iarray[i]);
    }
    printf ("\n");

    printf ("Sorting:\n");
    bucket (iarray, MAX);
                                
    printf ("After sorting:\n");
    for (i=0; i<MAX; i++){
        printf ("<%u> ", iarray[i]);
    }
    printf ("\n");

    return 0;
}

void bucket(unsigned * D, int Size){
    int i, j, count[Buckets];
    unsigned S[MAX];

    /* Initialise the count in each bucket */
    for (j=0; j<Buckets; j++){
        count[j] = 0;
    }
    
    /* D[i] contains a number which is taken as the key and data. 
       & with Bucket to get the last eight bits.
       Increase the count in buckets accordingly.*/
    for (i=0; i<Size; i++){
        count[D[i] & (Buckets-1)]++;
    }

    /* Form the offset (starting values) for content of each bucket. */ 
    for (j=1; j<Buckets; j++){
        count[j] += count[j-1];
    }

    /* Adjust the offset count. The offset count[i] starts from 1 not 0 */
    for (j=0; j<Buckets ; j++){
        count[j]--;
    }

    /* Form the sorted array.
       0            = 0...0
       ~0           = 1...1
       ~0<<RADIX    = 1...100000000
       ~(~0<<RADIX) = 0...011111111
                    = 255
                    = FF
    */
    for (i=Size-1; i>=0; i--){
        S[count[(D[i] & ~(~0<<RADIX))]--] = D[i];
    }

    /* Copy and pass it back as a parameter. */
    for (i=0; i<Size; i++){
        D[i] = S[i];
    }
    return;
}
