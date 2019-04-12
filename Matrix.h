//
// Created by Jonah Rubin on 2019-04-11.
//

#ifndef HMM_MATRIX_H
#define HMM_MATRIX_H

#include <vector>
#include <string>

struct model {
    double a, c, g, t, self, transition, init;

    /***
     * Model constructor
     * @param aWeight
     * @param cWeight
     * @param gWeight
     * @param tWeight
     * @param selfWeight
     * @param transitionWeight
     */
    model(double a, double c, double g, double t, double self, double transition, double init);

};

struct hiddenModel {
    model m1;
    model m2;

    /***
     * Hidden model constructor
     * @param m1
     * @param m2
     */
    hiddenModel(model m1, model m2);
};


class Matrix {
private:
    std::vector<std::vector<double>> table;
    std::string seq;
    hiddenModel hmm;

public:
    /***
     * Matrix constructor
     * @param seq
     * @param hmm
     */
    Matrix(std::string seq, hiddenModel hmm);

    /***
     * Populate default matrix
     */
    void populate();

};


#endif //HMM_MATRIX_H
