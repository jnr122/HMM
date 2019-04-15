#include <iostream>
#include <vector>

#include "Matrix.h"

using namespace std;

double getEmission(char s, model m);
double forward(string s, hiddenModel hmm, model m, int index);

int main() {
    model H = model(0.2, 0.3, 0.3, 0.2, 0.5, 0.5, 0.5, "H");
    model L = model(0.3, 0.2, 0.2, 0.3, 0.6, 0.4, 0.5, "L");

    hiddenModel hmm = hiddenModel(H, L);

    string s = "ggca";

    Matrix m = Matrix(s, hmm);

    double forwardH = forward(s, hmm, hmm.H, s.size()-1);
    double forwardL = forward(s, hmm, hmm.L, s.size()-1);

    cout << "Forward Prob: " << forwardH + forwardL;

    return 0;
}

double forward(string s, hiddenModel hmm, model m, int index) {
    cout << "here" << endl;
    cout << index << endl;
    if (index == 0) {

        return getEmission(s.at(index), m) * m.init;
    }

    if (m.name == "H") {
        return getEmission(s.at(index), hmm.H) * ((forward(s, hmm, m, index - 1) * hmm.H.self) + ((forward(s, hmm, hmm.L, index - 1) * hmm.L.transition)));
    } else {
        return getEmission(s.at(index), hmm.L) * (((forward(s, hmm, m, index - 1) * hmm.L.self) + (forward(s, hmm, hmm.H, index - 1) * hmm.H.transition)));
    }

}

double getEmission(char s, model m) {
    if (s == 'a') {
        return m.a;
    } else if (s == 'c') {
        return m.c;
    } else if (s == 't') {
        return m.t;
    } else {
        return m.g;
    }
}

