//
// Created by Jonah Rubin on 2019-04-11.
//

#include "Matrix.h"

// model constructor
model::model(double a, double c, double g, double t, double self, double transition, double init, string name)
        : a(a), c(c), g(g), t(t), self(self), transition(transition), init(init), name(name) {
}

// hidden model constructor
hiddenModel::hiddenModel(model H, model L) : H(H), L(L) {}

// Matrix constructor
Matrix::Matrix(std::string seq, hiddenModel hmm) : seq(seq), hmm(hmm) {
    populate();
}


// populate a default matrix
void Matrix::populate() {

    for (int i = 0; i <= seq.size(); i++) {
        std::vector<double> row;
        for (int j = 0; j <= 2; j++) {

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
