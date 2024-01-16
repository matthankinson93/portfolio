#include <iostream>
#include <vector>

using namespace std;

typedef enum {Empty, NotYetMoved, Moved} OccupantStatus;

class Slot {
    public:
        OccupantStatus Status;
        int            Proxmap;
        int            InsertionLoc;
        float          Key;
};

void print(vector<Slot> a)
{  
   for (int i = 0; i < static_cast<int>(a.size()); i++)
      cout << a.at(i).Key << "   ";
   cout << "\n";
}
void ProxmapSort (vector<Slot>&);

int MapKey (float);

int main(){
    int n = 13;
    vector<Slot> A(n);
    
    A.at(0).Key = 6.7;
    A.at(1).Key = 5.9;
    A.at(2).Key = 8.4;
    A.at(3).Key = 1.2;
    A.at(4).Key = 7.3;
    A.at(5).Key = 3.7;
    A.at(6).Key = 11.5;
    A.at(7).Key = 1.1;
    A.at(8).Key = 4.8;
    A.at(9).Key = 0.4;
    A.at(10).Key = 10.5;
    A.at(11).Key = 6.1;
    A.at(12).Key = 1.8;
        
    cout << "Before sort:\n";
    print(A); 
    ProxmapSort (A);
    cout << "After sort:\n";
    print(A);
    
    return 0;
}
void ProxmapSort (vector<Slot>& A){
    int i, j, RunningTotal, TempInt;
    float KeyToInsert, TempKey;
    bool NotInserted;

    int n=A.size();

    /* Initialize Status and ProxMap */
    for(i=0; i<n; i++){
        A.at(i).Proxmap = 0;
        A.at(i).Status = NotYetMoved;
    }

    /* Count hits when keys are mapped into Mapkey locations.
       Use the Proxmap as a temporary storage for hit count. */
    for(i=0; i<n; i++){
        j = MapKey(A.at(i).Key);
        A.at(i).InsertionLoc = j;
        A.at(j).Proxmap++;
    }

    /* Convert hit counts to a Proxmap */
    RunningTotal = 0;
    for(i=0; i<n; i++){
        if (A.at(i).Proxmap > 0){
            TempInt = A.at(i).Proxmap;
            A.at(i).Proxmap = RunningTotal;
            RunningTotal += TempInt;
        }
    }

    /* Compute insertion locations */
    for(i=0; i<n; i++){
        A.at(i).InsertionLoc = A.at(A.at(i).InsertionLoc).Proxmap;
    }
    
    /* Rearrange A[i] in situ in A into ascending order */
    for(i=0; i<n; i++){

        /* Find next key in ascending order of i that is NotYetMoved */  
        if (A.at(i).Status == NotYetMoved){
            j = A.at(i).InsertionLoc;
            KeyToInsert = A.at(i).Key;
            A.at(i).Status = Empty;
            NotInserted = true;
            while (NotInserted){
                if (A.at(j).Status == NotYetMoved){
                    TempKey = A.at(j).Key;
                    A.at(j).Key = KeyToInsert;
                    KeyToInsert = TempKey;
                    A.at(j).Status = Moved;
                    j = A.at(j).InsertionLoc;
                } else if (A.at(j).Status == Moved){
                    if (KeyToInsert < A.at(j).Key){
                        TempKey = A.at(j).Key;
                        A.at(j).Key = KeyToInsert;
                        KeyToInsert = TempKey;
                    }
                    j++;
                } else {
                    A.at(j).Key = KeyToInsert;
                    A.at(j).Status = Moved;
                    NotInserted = false;
                }
            }
        }
    }
    return;
}
int MapKey(float f){
    return (int)f;
}
