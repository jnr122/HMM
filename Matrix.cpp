//
// Created by Jonah Rubin on 2019-04-11.
//
#include <iostream>
#include <iomanip>
#include <fstream>
#include "Matrix.h"

using namespace std;

// model constructor
model::model(double a, double c, double g, double t, double self, double transition, double init, std::string name)
        : a(a), c(c), g(g), t(t), self(self), transition(transition), init(init), name(name) {
}

// hidden model constructor
hiddenModel::hiddenModel(model H, model L) : H(H), L(L) {}

// Matrix constructor
Matrix::Matrix(std::string seq, hiddenModel hmm) : seq(seq), hmm(hmm) {
    forwardProb = computeForward(hmm.H, seq.size()-1) + computeForward(hmm.L, seq.size()-1);

    populate();
    multiplePaths = false;
    computeViterbi(hmm.H, seq.size()-1);
    computeViterbi(hmm.L, seq.size()-1);
    probablePath = computeProbablePath();
    probOfProbPath = computeProbOfProbPath();
    backwardProb = 0;
    computeBackward(hmm.H, 0, 4);
    posteriorProb = (forwardProb * backwardProb) / probOfProbPath;
}

// populate a default matrix
void Matrix::populate() {

    for (int i = 0; i < 2; i++) {
        std::vector<double> row;
        for (int j = 0; j < seq.size(); j++) {

            double score = -1;
            if (i == 0 and j == 0) {
                score = 1;
            } else if (i == 0 or j == 0) {
                score = 0;
            }
            row.push_back(score);
        }
        table.push_back(row);
    }
}

// compute forward probability
double Matrix::computeForward(model m, int index) {
    if (index == 0) {
        return getEmission(seq.at(index), m) * m.init;
    }
    if (m.name == "H") {
        return getEmission(seq.at(index), hmm.H) * ((computeForward(m, index - 1) * m.self) + computeForward(hmm.L, index - 1) * hmm.L.transition);
    } else {
        return getEmission(seq.at(index), hmm.L) * ((computeForward(m, index - 1) * m.self) + computeForward(hmm.H, index - 1) * hmm.H.transition);
    }
}

// compute viterbi table
double Matrix::computeViterbi(model m, int index) {
    if (index == 0) {

        if (m.name == "H") {
            double h = getEmission(seq.at(index), m) * m.init;
            table[0][index] = h;
            return h;
        }

        double l = getEmission(seq.at(index), m) * m.init;
        table[1][index] = l;
        return l;
    }

    if (m.name == "H") {
        double h = getEmission(seq.at(index), hmm.H) * argMax((computeViterbi(m, index - 1) * m.self), computeViterbi(hmm.L, index - 1) * hmm.L.transition);
        table[0][index] = h;
        return h;
    } else {
        double l = getEmission(seq.at(index), hmm.L) * argMax((computeViterbi(m, index - 1) * m.self), computeViterbi(hmm.H, index - 1) * hmm.H.transition);
        table[1][index] = l;
        return l;
    }
}

// use viterbi table to comput mpp
string Matrix::computeProbablePath() {
    string s = "";

    for (int i = 0; i < seq.size(); i++) {
        if (table[0][i] > table[1][i]) {
            s += "H";
        } else {
            s += "L";
        }
    }
    return s;
}

// prob of prob path
double Matrix::computeProbOfProbPath() {
    double p = 1;

    if (probablePath[0] == 'H') {
        p *= getEmission(seq[0], hmm.H) * hmm.H.init;
    } else {
        p *= getEmission(seq[0], hmm.L) * hmm.L.init;
    }

    for (int i = 0; i < seq.size()-1; i++) {
        if (probablePath[i] == 'H' and probablePath[i+1] == 'H') {
            p *= getEmission(seq[i+1], hmm.H) * hmm.H.self;
        } else if (probablePath[i] == 'H' and probablePath[i+1] == 'L') {
            p *= getEmission(seq[i+1], hmm.L) * hmm.H.transition;
        } else if (probablePath[i] == 'L' and probablePath[i+1] == 'H') {
            p *= getEmission(seq[i+1], hmm.H) * hmm.L.transition;
        } else {
            p *= getEmission(seq[i+1], hmm.L) * hmm.L.self;
        }
    }

    return p;
}

// backward algorithm
double Matrix::computeBackward(model m, int index, int max) {

    double init;

    double b;

    if (index == max) {
        return 1;
    }

    if (m.name == "H") {
        if (index == 0) {
            init = m.init;
        } else {
            init = 1;
        }
        return init * ((getEmission(seq.at(index), hmm.H) * hmm.H.self) * computeBackward(hmm.H, index + 1, max) + (getEmission(seq.at(index), hmm.L) * hmm.L.transition) * computeBackward(hmm.L, index + 1, max));

    } else {
        if (index == 0) {
            init = m.init;
        } else {
            init = 1;
        }

        return init * ((getEmission(seq.at(index), hmm.L) * hmm.L.self) * computeBackward(hmm.L, index + 1, max) + (getEmission(seq.at(index), hmm.H) * hmm.H.transition) * computeBackward(hmm.H, index + 1, max));
    }

}

// get max of two args
double Matrix::argMax(double d1, double d2) {
    if (d1 > d2) {
        return d1;
    } else if (d1 == d2) {
        multiplePaths = true;
    }
    return d2;
}

// get emission probability
double Matrix::getEmission(char s, model m) {
    if (s == 'A') {
        return m.a;
    } else if (s == 'C') {
        return m.c;
    } else if (s == 'T') {
        return m.t;
    } else {
        return m.g;
    }
}


ostream &operator<<(ostream &os, const Matrix &matrix) {
    os << "Forward Prob: " << matrix.forwardProb << endl;
    cout << "Probable Path: " << matrix.probablePath << endl;
    cout << "Prob of Probable Path: " << matrix.probOfProbPath << endl;
    cout << "Multiple Paths: " << matrix.multiplePaths << endl;
    cout << "Backward Prob: " << matrix.backwardProb << endl;
    cout << "Posterior Prob: " << matrix.posteriorProb << endl;

    ofstream out1("4.01");
    if (out1.is_open()) {
        out1 << matrix.forwardProb;
        out1.close();
    }

    ofstream out2("4.02");
    if (out2.is_open()) {
        out2 << "- 0";
        for (int i = 0; i < matrix.seq.size(); i++) {
            out2 << " " <<  matrix.seq[i];
        }

        out2 << "\n";

        for (int i = 0; i < matrix.table[1].size() + 2; i++) {
            if (i == 1) {
                out2 << "1 ";

            } else {
                out2 << "0 ";

            }
        }
        out2 << "\n";

        for (int i = 0; i < matrix.table.size(); i++) {
            for (int j = 0; j < matrix.table[i].size(); j++) {
                if (i == 0 and j == 0) {
                    out2 << "H";
                } else if (i == 1 and j == 0){
                    out2 << "L";
                }

                if ( j == 0) {
                    out2 << " 0";
                }
                out2 << " " << matrix.table[i][j];
            }
            out2 << endl;
        }

        out2.close();
    }

    ofstream out3("4.03");
    if (out3.is_open()) {
        out3 << matrix.probablePath;
        out3.close();
    }

    ofstream out4("4.04");
    if (out4.is_open()) {
        out4 << matrix.probOfProbPath;
        out4.close();
    }

    ofstream out5("4.05");
    if (out5.is_open()) {
        if (matrix.multiplePaths) {
            out5 << "YES";
        } else {
            out5 << "NO";
        }
        out5.close();
    }

    ofstream out6("4.06");
    if (out6.is_open()) {
        out6 << matrix.posteriorProb << endl;
        out6 << 1 - matrix.posteriorProb;
        out6.close();
    }

    for (int i = 0; i < matrix.seq.size(); i++) {
        cout << setw(10) <<  matrix.seq[i];
    }

    cout << "\n";
    for (int i = 0; i < matrix.table.size(); i++) {
        for (int j = 0; j < matrix.table[i].size(); j++) {
            if (i == 0 and j == 0) {
                cout << "H ";
            } else if (i == 1 and j == 0){
                cout << "L ";
            }
            cout << setw(10) << matrix.table[i][j] << " ";
        }
        cout << endl;
    }
    return os;
}


// getters
double Matrix::getForwardProb() const {
    return forwardProb;
}

const string &Matrix::getProbablePath() const {
    return probablePath;
}
