#include <iostream>
#include <vector>

#include "Matrix.h"

using namespace std;

int main() {
    model H = model(0.2, 0.3, 0.3, 0.2, 0.5, 0.5, 0.5, "H");
    model L = model(0.3, 0.2, 0.2, 0.3, 0.6, 0.4, 0.5, "L");

    hiddenModel hmm = hiddenModel(H, L);

    string s = "ggcactgaa";

    Matrix m = Matrix(s, hmm);

    cout << "Forward Prob: " << m.getForwardProb() << endl;
    cout << m;

    return 0;
}

