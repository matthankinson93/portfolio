

#include <iostream>
#include <vector>

using namespace std;

void print(vector<int>);
void merge_helper(vector<int>&, int, int); 
void merge(vector<int>&); 

int main(){

    vector<int>v(10);
    v.at(0) = 2;
    v.at(1) = 3;
    v.at(2) = 5;
    v.at(3) = 7;
    v.at(4) = 8;
    v.at(5) = 9;
    v.at(6) = 6;
    v.at(7) = 4;
    v.at(8) = 1;
    v.at(9) = 0;
    
    cout << "Before sorting:\n";
    print(v);
    merge (v);
    cout << "After sorting:\n";
    print(v);
    
    return 0;
}
void print(vector<int> a)
{  
   for (int i = 0; i < static_cast<int>(a.size()); i++)
      cout << a[i] << " ";
   cout << "\n";
}

void merge(vector<int>& a){
    merge_helper(a, 0, static_cast<int>(a.size()-1));
}

void merge_helper(vector<int>& a, int l, int r){
    int i, j, k, m;
    vector<int> tmp(a.size());

    if (r > l){
        m = (r+l)/2;
        merge_helper(a, l, m);
        merge_helper(a, m+1, r);

        /* tmp[1] to tmp[m], from small to large */ 
        for (i=m+1; i>l; i--){
            tmp.at(i-1) = a.at(i-1);
        }

        /* tmp[m+1] to tmp[r], from large to small */
        for (j=m; j<r; j++){
            tmp.at(r+m-j) = a.at(j+1);
        }

        /* Merge from two ends, small one first */
        for (k=l; k<= r; k++){
            a.at(k) = (tmp.at(i)<tmp.at(j)) ? tmp.at(i++) : tmp.at(j--);
        }
    }
    return;
}
