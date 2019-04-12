//
// Created by Jonah Rubin on 2019-04-11.
//

#include "Matrix.h"

// model constructor
model::model(double a, double c, double g, double t, double self, double transition, double init)
        : a(a), c(c), g(g), t(t), self(self), transition(transition), init(init) {
}

// hidden model constructor
hiddenModel::hiddenModel(model m1, model m2) : m1(m1), m2(m2) {}

// Matrix constructor
Matrix::Matrix(std::string seq, hiddenModel hmm) : seq(seq), hmm(hmm) {
    populate();
}



