#include <iostream>
#include <vector>

using namespace std;

void print(vector<int>);
void selection(vector<int>&); 

int main(){

    vector<int>v(5);
    v.at(0) = 5;
    v.at(1) = 6;
    v.at(2) = 1;
    v.at(3) = 9;
    v.at(4) = 2;

    cout << "Before sorting:\n";
    print(v);
    selection (v);
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

void selection(vector<int>& a){
    int i, j, min, tmp;
    int max = a.size(); 
    for (i=0; i<max-1; i++){
        min = i;
        for (j=i+1; j<max; j++){
            if (a.at(j) < a.at(min)){
                min = j;
            }
        }
        tmp = a.at(i);
        a.at(i) = a.at(min);
        a.at(min) = tmp;
        
        print(a);
    }
    return;
}
