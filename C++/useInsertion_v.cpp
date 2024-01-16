#include <iostream>
#include <vector>

using namespace std;

void insertion(vector<int>&); 
void print(vector<int>);

int main(){
    
    vector<int>v(5);
    v.at(0) = 1;
    v.at(1) = 6;
    v.at(2) = 4;
    v.at(3) = 9;
    v.at(4) = 2;
    
    cout << "Before sorting:\n";
    print(v);
    insertion (v);
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
void insertion(vector<int>& a){
    int i, j, tmp;
    int max = a.size();
    
    for (i=1; i<max; i++){
        tmp = a.at(i);
        j = i;
        while (j>0 && a.at(j-1) > tmp){
            a.at(j) = a.at(j-1);
            j--;
        }
        a.at(j) = tmp;
        print(a);        
    }
    return;
}
