#include <iostream>
#include <vector>

#include "Matrix.h"

using namespace std;

double calculate(string s, int index, model m);

int main() {
    model H = model(0.2, 0.3, 0.3, 0.2, 0.5, 0.5, 0.5);
    model L = model(0.3, 0.2, 0.2, 0.3, 0.6, 0.4, 0.5);

    hiddenModel hmm = hiddenModel(H, L);

    string s = "aggatc";

    Matrix m = Matrix(s, hmm);

    return 0;
}

/***
 * Calcualte probability with forward algorithm
 * @param s
 * @param index
 * @return
 */
double calculate(string s, int index, model m) {
    double emission;
    double transition;

    if (s == "a") {
        emission = m.a;
    } else if (s == "c") {
        emission = m.c;
    } else if (s == "t") {
        emission = m.t;
    } else {
        emission = m.g;
    }


    if (index == 0) {
        return emission * m.init;
    }

}


double forward(string s, hiddenModel hmm) {

    return 0;
}
