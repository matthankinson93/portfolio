#include <iostream>
#include <vector>

using namespace std;

void bubble(vector<int>&); 
void print(vector<int>);

int main(){
    
    vector<int>v(5);
    v.at(0) = 5;
    v.at(1) = 6;
    v.at(2) = 1;
    v.at(3) = 9;
    v.at(4) = 2;
    
    cout << "Before sorting:\n";
    print(v);
    bubble (v);
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
void bubble(vector<int>& a){
    int i, j, tmp;
    int max = a.size();
    for (i=max-1; i>=1; i--){
        for (j=1; j<=i; j++){
            if (a.at(j-1) > a.at(j)){
                tmp = a.at(j);
                a.at(j) = a.at(j-1);
                a.at(j-1) = tmp;
            }
        }
        print (a);
    }
    return;
}
