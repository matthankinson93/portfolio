#include <stdio.h>

typedef enum {Empty, NotYetMoved, Moved} OccupantStatus;
struct slot {
            OccupantStatus Status;
            int            Proxmap;
            int            InsertionLoc;
            float          Key;
    };

void ProxmapSort (slot *, int);

int MapKey (float);

int main(){
    int n = 13;
    slot A[13];
    float farray[13] = {6.7, 5.9, 8.4, 1.2, 7.3, 3.7, 11.5, 1.1, 4.8, 0.4, 10.5, 6.1, 1.8};
    int i;

    for(i=0; i<n; i++){
        A[i].Key = farray[i];
printf ("%5.1f", A[i].Key);
    }

    ProxmapSort (A, n);

    printf ("\nAfter sorting:\n");
    for(i=0; i<n; i++){
        printf ("%5.1f", A[i].Key);
        printf ("%5d", A[i].Proxmap);
        printf ("%5d", A[i].Status);
        printf ("%5d", A[i].InsertionLoc);
        printf ("\n");
    }

    return 0;
}
void ProxmapSort (slot * A, int n){
    int i, j, RunningTotal, TempInt;
    float KeyToInsert, TempKey;
    bool NotInserted;

    /* Initialize Status and ProxMap */
    for(i=0; i<n; i++){
        A[i].Proxmap = 0;
        A[i].Status = NotYetMoved;
    }

    /* Count hits when keys are mapped into Mapkey locations.
       Use the Proxmap as a temporary storage for hit count. */
    for(i=0; i<n; i++){
        j = MapKey(A[i].Key);
        A[i].InsertionLoc = j;
        A[j].Proxmap++;
    }

    /* Convert hit counts to a Proxmap */
    RunningTotal = 0;
    for(i=0; i<n; i++){
        if (A[i].Proxmap > 0){
            TempInt = A[i].Proxmap;
            A[i].Proxmap = RunningTotal;
            RunningTotal += TempInt;
        }
    }

    /* Compute insertion locations */
    for(i=0; i<n; i++){
        A[i].InsertionLoc = A[A[i].InsertionLoc].Proxmap;
    }
    
    /* Rearrange A[i] in situ in A into ascending order */
    for(i=0; i<n; i++){

        /* Find next key in ascending order of i that is NotYetMoved */  
        if (A[i].Status == NotYetMoved){
            j = A[i].InsertionLoc;
            KeyToInsert = A[i].Key;
            A[i].Status = Empty;
            NotInserted = true;
            while (NotInserted){
                if (A[j].Status == NotYetMoved){
                    TempKey = A[j].Key;
                    A[j].Key = KeyToInsert;
                    KeyToInsert = TempKey;
                    A[j].Status = Moved;
                    j = A[j].InsertionLoc;
                } else if (A[j].Status == Moved){
                    if (KeyToInsert < A[j].Key){
                        TempKey = A[j].Key;
                        A[j].Key = KeyToInsert;
                        KeyToInsert = TempKey;
                    }
                    j++;
                } else {
                    A[j].Key = KeyToInsert;
                    A[j].Status = Moved;
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
