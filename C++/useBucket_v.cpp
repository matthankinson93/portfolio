#include <iostream>
#include <vector>
#define MAX 10
#define RADIX 8
#define Buckets 256

using namespace std;

/* Use the right most 8 bits as the key only.
   This will limit the bucket number to 256.
   Bucket 0 to Bucket 255. */
void bucket(vector<unsigned int>&); 
void print(vector<unsigned int>);

int main(){
    
    vector<unsigned int>v(5);
    v.at(0) = 5;
    v.at(1) = 2;
    v.at(2) = 1;
    v.at(3) = 9;
    v.at(4) = 2;
    
    cout << "Before sorting:\n";
    print(v);
    bucket (v);
    cout << "After sorting:\n";
    print(v);
    
    return 0;
}
void print(vector<unsigned int> a)
{  
   for (int i = 0; i < static_cast<int>(a.size()); i++)
      cout << a[i] << " ";
   cout << "\n";
}

void bucket(vector<unsigned int>& D){
    int i, j;
    vector<int>count(Buckets);
    int Size = D.size();
    vector<unsigned int> S(Size);

    /* Initialise the count in each bucket */
    for (j=0; j<Buckets; j++){
        count.at(j) = 0;
    }
    
    /* D[i] contains a number which is taken as the key and data. 
       & with Bucket to get the last eight bits.
       Increase the count in buckets accordingly.*/
    for (i=0; i<Size; i++){
        count.at(D.at(i) & (Buckets-1))++;
    }

    /* Form the offset (starting values) for content of each bucket. */ 
    for (j=1; j<Buckets; j++){
        count.at(j) += count.at(j-1);
    }

    /* Adjust the offset count. The offset count[i] starts from 1 not 0 */
    for (j=0; j<Buckets ; j++){
        count.at(j)--;
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
        S.at(count.at((D.at(i) & ~(~0<<RADIX)))--) = D.at(i);
    }

    /* Copy and pass it back as a parameter. */
    for (i=0; i<Size; i++){
        D.at(i) = S.at(i);
    }
    return;
}
