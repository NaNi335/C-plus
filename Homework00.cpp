#include <iostream>
using namespace std;

int main() {
    cout << "Enter the b value: \n";
    int b = 0;
    cin >> b;
    for (int a = 1; a <= 10; a += 1) {
        cout << "For a = " << (a) << " b = " << (b * a) << "\n";
    }
}