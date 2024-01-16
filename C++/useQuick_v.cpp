#include <iostream>
#include <vector>

using namespace std;

void quick(vector<int>&);
void quick_helper(vector<int>&, int, int);  
void print(vector<int>);

int main(){
    
    vector<int>v(10);
    v.at(0) = 8;
    v.at(1) = 3;
    v.at(2) = 7;
    v.at(3) = 2;
    v.at(4) = 5;
    v.at(5) = 9;
    v.at(6) = 1;
    v.at(7) = 4;
    v.at(8) = 0;
    v.at(9) = 6;
  
    cout << "Before sorting:\n";
    print(v);
    quick (v);
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
void quick(vector<int>& a){ 
    quick_helper(a, 0, static_cast<int>(a.size() - 1));
}
void quick_helper(vector<int>& a, int l, int r){
    int i, first, last, pivot;

    if (r>l){
        first = l;
        last = r;
        i = (l+r)/2; 
        pivot = a.at(i);
        a.at(i) = a.at(first);
        a.at(first) = pivot;

        while (last > first){
            if (a.at(last) >= pivot){
                last--;
            } else {
                a.at(first) = a.at(last);
                a.at(last) = a.at(++first);
                a.at(first) = pivot;
            }
        }
       
        pivot = first;
        quick_helper(a, l, pivot-1);
        quick_helper(a, pivot+1, r);
    }
    return;
}
