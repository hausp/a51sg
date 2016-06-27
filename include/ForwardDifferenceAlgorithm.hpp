/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#ifndef FORWARD_DIFFERENCE_ALGORITHM_HPP
#define FORWARD_DIFFERENCE_ALGORITHM_HPP

#include <vector>
#include "CurveAlgorithm.hpp"

template<unsigned D>
class ForwardDifferenceAlgorithm : public CurveAlgorithm<D> {
public:
    std::vector<Line<D>> update(double accuracy,
        const std::vector<Matrix<4,1>>& coefs) const override {

        auto e = generateEDelta(accuracy);
        std::vector<Matrix<4,1>> params;
        for (unsigned i = 0; i < D; i++) {
            params.push_back(e * coefs[i]);
        }

        unsigned numLines = 1/accuracy + 1;
        std::vector<Line<D>> lines;
        exec(lines, numLines, params);
        return lines;
    }

    Matrix<4,4> generateEDelta(double accuracy) const {
        std::vector<double> powers(3, accuracy);
        powers[1] *= accuracy;  // accuracy^2
        powers[2] *= powers[1]; // accuracy^3
        Matrix<4,4> e = {
            {0, 0, 0, 1},
            {powers[2], powers[1], powers[0], 0},
            {6 * powers[2], 2 * powers[1], 0, 0},
            {6 * powers[2], 0, 0, 0}
        };
        return e;        
    }

    void exec(std::vector<Line<D>>& lines, unsigned numLines,
        std::vector<Matrix<4,1>>& params) const {

        lines.reserve(lines.size() + numLines + 1);
        for (unsigned i = 0; i < numLines; i++) {
            Point<D> p;
            for (unsigned j = 0; j < D; j++) {
                if (i > 0) {
                    params[j][0][0] += params[j][1][0];
                    params[j][1][0] += params[j][2][0];
                    params[j][2][0] += params[j][3][0];
                }
                p[j] = params[j][0][0];
            }

            if (i > 0) {
                lines.back()[1] = p;
            }
            lines.push_back(Line<D>(p, Point<D>()));
        }
        lines.pop_back();
    }

    std::vector<Line<D>> update(double accuracyS, double accuracyT,
        const std::vector<Matrix<4,4>>& coefs) const override {

        auto es = generateEDelta(accuracyS);
        auto et = generateEDelta(accuracyT);
        auto esT = es.transpose();
        auto etT = et.transpose();

        std::vector<Matrix<4,4>> initialConditions;
        initialConditions.reserve(D);
        for (unsigned i = 0; i < D; i++) {
            initialConditions.push_back(es * coefs[i] * etT);
        }

        double numLinesS = 1/accuracyS + 1;
        double numLinesT = 1/accuracyT + 1;
        std::vector<Line<D>> lines;
        fillLineList(lines, numLinesS, numLinesT, initialConditions);

        for (unsigned i = 0; i < D; i++) {
            initialConditions[i] = initialConditions[i].transpose();
        }
        fillLineList(lines, numLinesT, numLinesS, initialConditions);       
        return lines;
    }

    void fillLineList(std::vector<Line<D>>& lines, unsigned numLinesS,
        unsigned numLinesT, const std::vector<Matrix<4,4>>& initialConditions) const {

        auto conditions = initialConditions;
        for (unsigned i = 0; i < numLinesS; i++) {
            std::vector<Matrix<4,1>> params;
            params.reserve(D);
            for (unsigned j = 0; j < D; j++) {
                Matrix<4,1> tConditions = {
                    {conditions[j][0][0]},
                    {conditions[j][0][1]},
                    {conditions[j][0][2]},
                    {conditions[j][0][3]},
                };
                params.push_back(tConditions);
            }

            exec(lines, numLinesT, params);

            for (unsigned j = 0; j < D; j++) {
                for (unsigned k = 0; k < 4; k++) {
                    conditions[j][0][k] += conditions[j][1][k];
                    conditions[j][1][k] += conditions[j][2][k];
                    conditions[j][2][k] += conditions[j][3][k];                    
                }
            }            
        }
    }
};

#endif /* FORWARD_DIFFERENCE_ALGORITHM_HPP */