
// // Ascending order
// #include <iostream>
// using namespace std;
// int main()
// {
//     int A[] = {1, 15, 20, 30, 35};
//     int B[] = {3, 13, 20, 31, 42, 50};
//     int sizea = sizeof(A) / sizeof(A[0]);
//     int sizeb = sizeof(B) / sizeof(B[0]);
//     int C[sizea + sizeb];
//     int i = 0, j = 0, k = 0;

//     while (i < sizea && j < sizeb)
//     {
//         if (A[i] < B[j])
//         {
//             C[k] = A[i++];
//         }
//         else
//         {
//             C[k] = B[j++];
//         }
//         k++;
//     }
//     // to add all remaining elements of A
//     while (i < sizea)
//     {
//         C[k++] = A[i++];
//     }
//     // to add all remaining elements of B
//     while (j < sizeb)
//     {
//         C[k++] = B[j++];
//     }
//     for (int indx = 0; indx < sizea + sizeb; indx++)
//     {
//         cout << C[indx] << " ";
//     }
//     cout << endl;
//     return 0;
// }



// // Descending Order
// #include <iostream>
// using namespace std;

// int main()
// {
//     int A[] = {1, 15, 20, 30, 35};
//     int B[] = {3, 13, 20, 31, 42, 50};
//     int sizea = sizeof(A) / sizeof(A[0]);
//     int sizeb = sizeof(B) / sizeof(B[0]);
//     int C[sizea + sizeb];
//     int i = 0, j = 0, k = 0;

//     // Merge in ascending order
//     while (i < sizea && j < sizeb)
//     {
//         if (A[i] < B[j])
//         {
//             C[k++] = A[i++];
//         }
//         else
//         {
//             C[k++] = B[j++];
//         }
//     }

//     // Add remaining elements of A
//     while (i < sizea)
//     {
//         C[k++] = A[i++];
//     }

//     // Add remaining elements of B
//     while (j < sizeb)
//     {
//         C[k++] = B[j++];
//     }

//     // Print in descending order (reverse traversal)
//     for (int indx = sizea + sizeb - 1; indx >= 0; indx--)
//     {
//         cout << C[indx] << " ";
//     }
//     cout << endl;

//     return 0;
// }



// // Total 11 products are available with thier names like P1 , P2 , ... and with thier values 

// #include <iostream>
// using namespace std;
// struct Item
// {
//     string name;
//     int val;
// };
// int main()
// {
//     Item A[5];
//     // A[0].name="P1";
//     // A[0].val=1;
//     // A[1].name="P1";
//     // A[1].val=1;
//     //  {1, 15, 20, 30, 35};
//     for (int aindx = 0; aindx < 5; aindx++)
//     {
//         getline(cin, A[aindx].name);
//         cin >> A[aindx].val;
//     }
//     Item B[6];
//     for (int bindx = 0; bindx < 6; bindx++)
//     {
//         getline(cin, B[bindx].name);
//         cin >> B[bindx].val;
//     }
//     // int B[] = {3, 13, 20, 31, 42, 50};
//     int sizea = sizeof(A) / sizeof(A[0]);
//     int sizeb = sizeof(B) / sizeof(B[0]);
//     Item C[sizea + sizeb];
//     int i = 0, j = 0, k = 0;

//     while (i < sizea && j < sizeb)
//     {
//         if (A[i].val < B[j].val)
//         {
//             C[k] = A[i++];
//         }
//         else
//         {
//             C[k] = B[j++];
//         }
//         k++;
//     }
//     // to add all remaining elements of A
//     while (i < sizea)
//     {
//         C[k++] = A[i++];
//     }
//     // to add all remaining elements of B
//     while (j < sizeb)
//     {
//         C[k++] = B[j++];
//     }
//     for (int indx = 0; indx < sizea + sizeb; indx++)
//     {
//         cout << C[indx].name << C[indx].val << " ";
//     }
//     cout << endl;
//     return 0;
// }



//give code for input : 
//text = ABCABCBCDABBCDDAABC 
//pattern = ABC 
//output : 
//locations : 0 3 16

#include <iostream>
using namespace std;
int main()
{
    string text;
    getline(cin, text);
    string pattern;
    getline(cin, pattern);

    int lt = text.length();
    int lp = pattern.length();

    for (int i = 0; i < lt - lp; i++)
    {
        int match = true;
        for (int j = 0; j < lp; j++)
        {
            if (text[i + j] != pattern[j])
            {
                match = false;
                break;
            }
        }
        if (match)
        {
            cout << "location:" << i << endl;
        }
    }
}
