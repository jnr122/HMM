//
// Created by Jonah Rubin on 2019-04-11.
//

#ifndef HMM_MATRIX_H
#define HMM_MATRIX_H

#include <vector>
#include <string>
#include <ostream>

struct model {
    double a, c, g, t, self, transition, init;
    std::string name;

    /***
     * Model constructor
     * @param aWeight
     * @param cWeight
     * @param gWeight
     * @param tWeight
     * @param selfWeight
     * @param transitionWeight
     */
    model(double a, double c, double g, double t, double self, double transition, double init, std::string name);

};

struct hiddenModel {
    model H;
    model L;

    /***
     * Hidden model constructor
     * @param m1
     * @param m2
     */
    hiddenModel(model H, model L);
};


class Matrix {
private:
    std::vector<std::vector<double>> table;
    std::string seq, probablePath;
    hiddenModel hmm;
    double forwardProb, probOfProbPath, backwardProb, posteriorProb;
    bool multiplePaths;

    /***
     * Get emission probability
     * @param s
     * @param m
     * @return
     */
    double getEmission(char s, model m);

    /***
     * Populate default matrix for viterbi algorithm
     */
    void populate();

    /***
     * Recursive calculations
     * @param m
     * @param index
     * @return f(i)
     */
    double computeForward(model m, int index);

    /***
     * Table calculations
     * @param m
     * @param index
     * @return nothing, fill table
     */
    double computeViterbi(model m, int index);

    /***
     * Get max
     * @param d1
     * @param d2
     * @return max arg
     */
    double argMax(double d1, double d2);

    /***
     * Use viterbi table to compute most probable path
     * @return most probable path
     */
    std::string computeProbablePath();

    /***
     * @return the probability of the most probable path
     */
    double computeProbOfProbPath();

    /***
     * Backward algorithm
     * @param m
     * @param index
     * @return b(i)
     */
    double computeBackward(model m, int index, int max);

public:

    /***
     * Matrix constructor
     * @param seq
     * @param hmm
     */
    Matrix(std::string seq, hiddenModel hmm);

    friend std::ostream &operator<<(std::ostream &os, const Matrix &matrix);

    // getters
    double getForwardProb() const;
    const std::string &getProbablePath() const;


};


#endif //HMM_MATRIX_H
