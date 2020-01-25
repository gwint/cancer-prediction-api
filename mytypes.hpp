#ifndef MY_TYPES_H
#define MY_TYPES_H

#include <vector>
#include <utility>

namespace my {
    typedef std::vector<double> single_sample_features;
    typedef std::vector<std::vector<double>*> multiple_sample_features;
    typedef std::vector<int> multiple_sample_classes;
    typedef std::pair<my::multiple_sample_features, my::multiple_sample_classes> training_data;
    typedef std::pair<my::multiple_sample_features, my::multiple_sample_classes> testing_data;
    typedef std::pair<double, double> interval;
    typedef std::vector<my::interval> intervals;
    typedef std::pair<std::vector<my::multiple_sample_features>, std::vector<my::multiple_sample_classes>> partitionedDataset;
    typedef struct confusion_matrix {
        int truePositive;
        int falsePositive;
        int trueNegative;
        int falseNegative;
    } confusion_matrix;
};

#endif