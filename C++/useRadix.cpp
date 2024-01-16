#include <stdio.h>
#define MAX 10
#define Radix 8
#define Buckets 255

void radix(unsigned * DataArray, int ArraySize); 
//void radix(unsigned * DataArray, unsigned * SortedArray, int ArraySize); 

int main(){

    unsigned j;
    unsigned iarray2[MAX] = {304,307,309,301,308,302,306,303,5,0};
    
    printf ("Before sorting:\n");
    for (int i=0; i<MAX; i++){
        printf ("<%d> ", iarray2[i]);
    }
    printf ("\n");
    printf ("Sorting:\n");
    
    radix (iarray2, MAX);
                                    
    printf ("After sorting:\n");
    
    for (int i=0; i<MAX; i++){
        printf ("<%u> ", iarray2[i]);
    }
    printf ("\n");
/*    for (i=0; i<MAX; i++){
        j = iarray3[i];
        carray[i][0]=(j>>16) & ~(~0<<8);
        carray[i][1]=(j>>8) & ~(~0<<8);
        carray[i][2]=(j) & ~(~0<<8);
        printf ("%5s", carray[i]);
    }
    printf ("\n");
*/
    return 0;
}

void radix(unsigned * D, int Size){
    int i, j, pass, k, count[Buckets];
    unsigned mask, S[MAX];

/* It will sort 8 bits a time for 4 times.
   It is because we declare those iarrays as
   unsigned arrays */
    mask = (unsigned)Buckets;
    for (pass=0; pass<4; pass++){
        for (j=0; j<Buckets; j++){
            count[j] = 0;
        }

        /* Radix = 8
           pass * Radix = multiples of 8 bits
           D[i]>>(pass*Radix)
             = D[i] (pass)th digit now at the right
           (D[i]>>(pass*Radix)) & mask
             = extract the (pass)th digit of D[i]
        */
        for (i=0; i<Size; i++){
            count[(D[i]>>(pass*Radix)) & mask]++;
        }

        for (j=1; j<Buckets; j++){
            count[j] += count[j-1];
        }

        /* The offset count[i] starts from 1 not 0 */
        for (j=0; j<Buckets; j++){
            count[j]--;
        }

        for (i=Size-1; i>=0; i--){
            S[count[(D[i]>>(pass*Radix)) & mask]--] = D[i];
        }

        for (i=0; i<Size; i++){
            D[i] = S[i];
        }
    }
    return;
}
