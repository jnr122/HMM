#include <iostream>
#include <vector>
#include <fstream>

#include "Matrix.h"

using namespace std;

int main() {
    string s;

    ifstream infile ("4.in");
    if (infile.is_open())
    {
        getline(infile, s, '\n');
        getline(infile, s);

        infile.close();
    }

    model H = model(0.2, 0.3, 0.3, 0.2, 0.5, 0.5, 0.5, "H");
    model L = model(0.3, 0.2, 0.2, 0.3, 0.6, 0.4, 0.5, "L");

    hiddenModel hmm = hiddenModel(H, L);

    Matrix m = Matrix(s, hmm);

    cout << m;

    return 0;
}

