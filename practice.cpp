// #include <iostream>
// using namespace std;

// int main() {
//     double amount;      
//     double gst;         
//     double baseamount;  

//     cout << "Enter the total amount (0 to 20000): ";
//     cin >> amount;

//     cout << "Enter the GST percentage (up to 28.00%): ";
//     cin >> gst;

//     baseamount = amount / (1 + (gst / 100.0));

//     cout << "Base Amount (before GST) = " << baseamount << endl;

//     return 0;
// }


// #include <iostream>
// using namespace std;

// int main() {
//     double bmi;
//     double height;
//     double weight;

//     cout << "Enter height (in meters): ";
//     cin >> height;

//     cout << "Enter weight (in kilograms): ";
//     cin >> weight;

//     bmi = weight / (height * height);

//     cout << "BMI = " << bmi << endl;

//     if (bmi < 18.5) {
//         cout << "Underweight" << endl;
//     }
//     else if (bmi >= 18.5 && bmi <= 24.9) {
//         cout << "Normal" << endl;
//     }
//     else if (bmi >= 25 && bmi <= 29.9) {
//         cout << "Overweight" << endl;
//     }
//     else {
//         cout << "Obese" << endl;
//     }

//     return 0;
// }

// #include <iostream>
// using namespace std;

// int main() {
//     double bill;
//     double tip_percent;
//     double tip;
    
//     cout << "Enter total bill";
//     cin >> bill;

//     cout << "Enter tip percentage";
//     cin >> tip_percent;

//     tip = bill*tip_percent/100;
//     cout << "The tip given is "<<tip;

//     if (tip_percent>30){
//         cout << "High Tip !";
//     }
// }

// create a program to create series with 0, 1, 4, 9 , ...

// #include <iostream>
// using namespace std;

// int main(){
//     double n;

//     cout << "enter n elements";
//     cin >> n;

//     for (int i=0 ; i<n ; i++){
//         cout << (i*i)<< " ";
//     }
// }

// create program to print 0, 1, 4, 9, ... but with elements (upto)

// #include <iostream>
// using namespace std;

// int main (){
//     double n;

//     cout << "enter n element";
//     cin >> n;

//     for (int i=0; (i*i) <= n; i++){
//         cout << (i*i)<< " ";
//     }
//     return 0;
// }

// create a program to have series 0, 1, 5, 15, 37, ....

// #include <iostream>
// using namespace std;

// int main() {
//     int n;
//     cout << "Enter number of terms: ";
//     cin >> n;

//     for (int i = 0; i < n; i++) {
//         int sum = 1;
//         for (int j = 0; j < i; j++) {
//             sum *= 2;
//         }

//         int term = 3 * sum - 2 * i - 3;
//         cout << term;
//         if (i != n - 1) cout << ", ";
//     }
//     cout << endl;
//     return 0;
// }

