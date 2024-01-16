#include <iostream>
#include <vector>
//#define MAX 10
#define Radix 8
#define Buckets 255

using namespace std;

/* Use the right most 8 bits as the key only.
   This will limit the bucket number to 256.
   Bucket 0 to Bucket 255. */

void print(vector<unsigned int>);
void radix(vector<unsigned int>&); 

int main(){

    vector<unsigned int>v(5);
    v.at(0) = 221;
    v.at(1) = 312;
    v.at(2) = 322;
    v.at(3) = 215;
    v.at(4) = 319;
    
    cout << "Before sorting:\n";
    print (v);
    radix (v);
    cout << "After sorting:\n";
    print (v);

    return 0;
}
void print(vector<unsigned int> a)
{  
   for (int i = 0; i < static_cast<int>(a.size()); i++)
      cout << a[i] << " ";
   cout << "\n";
}
void radix(vector<unsigned int>& D){
    int i, j, pass;
    vector<int>count(Buckets);
    int Size = D.size();
    unsigned mask;
    vector<unsigned int>S(Size);

/* It will sort 8 bits a time for 4 times.
   It is because we declare those iarrays as
   unsigned arrays */
    mask = (unsigned)Buckets;
    for (pass=0; pass<4; pass++){
        for (j=0; j<Buckets; j++){
            count.at(j) = 0;
        }

        /* Radix = 8
           pass * Radix = multiples of 8 bits
           D[i]>>(pass*Radix)
             = D[i] (pass)th digit now at the right
           (D[i]>>(pass*Radix)) & mask
             = extract the (pass)th digit of D[i]
        */
        for (i=0; i<Size; i++){
            count.at((D.at(i)>>(pass*Radix)) & mask)++;
        }

        for (j=1; j<Buckets; j++){
            count.at(j) += count.at(j-1);
        }

        /* The offset count[i] starts from 1 not 0 */
        for (j=0; j<Buckets; j++){
            count.at(j)--;
        }

        for (i=Size-1; i>=0; i--){
            S.at(count.at((D.at(i)>>(pass*Radix)) & mask)--) = D.at(i);
        }

        for (i=0; i<Size; i++){
            D.at(i) = S.at(i);
        }
    }
    return;
}
