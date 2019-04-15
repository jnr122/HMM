//
// Created by Jonah Rubin on 2019-04-11.
//
#include <iostream>
#include <iomanip>
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
    computeViterbi(hmm.H, seq.size()-1);
    computeViterbi(hmm.L, seq.size()-1);

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


ostream &operator<<(ostream &os, const Matrix &matrix) {
    os << "";
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


// get max of two args
double Matrix::argMax(double d1, double d2) {
    if (d1 > d2) {
        return d1;
    }
    return d2;
}

// get emission probability
double Matrix::getEmission(char s, model m) {
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

// getters
double Matrix::getForwardProb() const {
    return forwardProb;
}

