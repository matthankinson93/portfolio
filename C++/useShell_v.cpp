#include <iostream>
#include <vector>

using namespace std;

void shell(vector<int>&); 
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
    shell (v);
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

void shell(vector<int>& a){
    int i, j, h, tmp;
    int max = a.size(); 

    for (h=max/2; h>0; h/=2){
        cout << "h = " << h << "\n";
        for (i=h; i<max; i++){
            tmp = a.at(i);
            j = i;
            while (j>=h && a.at(j-h)>tmp){
                a.at(j) = a.at(j-h);
                j -= h;
            }
            a.at(j) = tmp;
        }
        print(a);
    }
    return;
}
