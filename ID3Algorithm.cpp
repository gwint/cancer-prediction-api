#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <queue>
#include <functional>
#include <limits>
#include <cassert>

#include "ID3Algorithm.hpp"
#include "node.hpp"
#include "mytypes.hpp"

Node*
ID3Algorithm::createModel(const my::multiple_sample_features& features,
                                const my::multiple_sample_classes& classes,
                                int maximumTreeHeight) {

    Node* root = new Node(features, classes);

    ID3Algorithm::trainHelper(root, maximumTreeHeight);

    return root;
}

void
ID3Algorithm::trainHelper(Node* treeRoot, int maximumTreeHeight) {
    if(treeRoot == NULL) {
        std::cout << "Decision tree root must not be NULL" << std::endl;
        exit(1);
    }

    const my::multiple_sample_features& features = treeRoot->getFeatures();
    const my::multiple_sample_classes& classes = treeRoot->getClasses();

    if(maximumTreeHeight == 1) {
        ID3Algorithm::labelNode(treeRoot);
        return;
    }

    if(features.size() < ID3Algorithm::MIN_SAMPLES_FOR_SPLIT) {
        ID3Algorithm::labelNode(treeRoot);
        return;
    }
    if(features.empty()) {
        ID3Algorithm::labelNode(treeRoot);
        return;
    }
    if(treeRoot->doIncludedSamplesAllHaveSameClass()) {
        ID3Algorithm::labelNode(treeRoot);
        return;
    }

    int columnToUseToSplitSamples =
        ID3Algorithm::findFeatureProvidingLargestInfoGain(features, classes);
    treeRoot->setIndexOfFeatureToUseToSplitSamplesUp(columnToUseToSplitSamples);

    my::intervals intervals =
                ID3Algorithm::getIntervalsForFeature(
                                  features,
                                  columnToUseToSplitSamples,
                                  ID3Algorithm::NUM_DATA_PARTITIONS);

    std::vector<my::training_data> maxInfoGainPartition =
         ID3Algorithm::getPartitionedData(features,
                                          classes,
                                          intervals,
                                          columnToUseToSplitSamples);

    std::vector<Node*> children =
                ID3Algorithm::createChildren(maxInfoGainPartition,
                                             treeRoot);

    treeRoot->setChildren(children);

    for(Node* child : children) {
        ID3Algorithm::trainHelper(child, maximumTreeHeight-1);
    }
}

void
ID3Algorithm::labelNode(Node* node) {
    if(node == NULL) {
        std::cerr << "Should not be trying to label a NULL node" << std::endl;
        exit(1);
    }

    my::multiple_sample_features features = node->getFeatures();
    if(features.empty()) {
        Node* parent = node->getParent();
        if(parent != NULL) {
            ID3Algorithm::labelNode(parent);
            node->setLabel(parent->getLabel());
        }
    }
    else {
        my::multiple_sample_classes classes = node->getClasses();
        std::unordered_map<int, int> classCounts;
        int numSamples = classes.size();

        int mostFrequentLabel = classes[0];
        int largestFrequency = 1;

        classCounts[mostFrequentLabel] = 1;

        for(int i = 1; i < numSamples; i++) {
            int label = classes.at(i);
            classCounts[label]++;
            if(classCounts[label] > largestFrequency) {
                    mostFrequentLabel = label;
                    largestFrequency = classCounts[label];
            }
        }

        node->setLabel(mostFrequentLabel);
    }
}

double
ID3Algorithm::getProportion(int targetLabel, const my::multiple_sample_classes& classes) {
    int numSamplesWithLabel = 0;
    int numSamples = classes.size();

    for(int i = 0; i < numSamples; i++) {
        int label = classes.at(i);
        if(label == targetLabel) {
            numSamplesWithLabel++;
        }
    }

    return ((double) numSamplesWithLabel) / numSamples;
}

double
ID3Algorithm::getMinimumValueForGivenFeature(const my::multiple_sample_features& features, int relevantColumnIndex) {
    my::single_sample_features* firstSampleFeatures = features[0];
    int numSamples = features.size();
    double minimumFeatureValue = firstSampleFeatures->at(relevantColumnIndex);
    for(int sampleIndex = 1; sampleIndex < numSamples; sampleIndex++) {
        my::single_sample_features* sampleFeatures = features[sampleIndex];
        minimumFeatureValue = std::min(minimumFeatureValue, sampleFeatures->operator[](relevantColumnIndex));
    }

    return minimumFeatureValue;
}

double
ID3Algorithm::getMaximumValueForGivenFeature(const my::multiple_sample_features& features, int relevantColumnIndex) {
    my::single_sample_features* firstSampleFeatures = features[0];
    double maximumFeatureValue = firstSampleFeatures->at(relevantColumnIndex);

    int numSamples = features.size();
    for(int sampleIndex = 1; sampleIndex < numSamples; sampleIndex++) {
        maximumFeatureValue = std::max(maximumFeatureValue, features[sampleIndex]->operator[](relevantColumnIndex));
    }

    return maximumFeatureValue;
}

my::intervals
ID3Algorithm::getIntervalsForFeature(const my::multiple_sample_features& features, int featureColumnIndex, int numIntervals) {
    std::vector<my::interval> intervals;

    double minimumFeatureValue =
             ID3Algorithm::getMinimumValueForGivenFeature(features, featureColumnIndex);
    double maximumFeatureValue =
             ID3Algorithm::getMaximumValueForGivenFeature(features, featureColumnIndex);

    double intervalSize =
      (maximumFeatureValue - minimumFeatureValue) / (numIntervals-2);

    intervals.push_back(std::make_pair(-std::numeric_limits<double>::max(), minimumFeatureValue));

    for(int numIntervalsMade = 0; numIntervalsMade < numIntervals-2; numIntervalsMade++) {
        double start = minimumFeatureValue + (intervalSize * numIntervalsMade);
        double end = start + intervalSize;
        intervals.push_back(std::make_pair(start, end));
    }

    intervals.push_back(std::make_pair(maximumFeatureValue, std::numeric_limits<double>::max()));

    return intervals;
}

std::vector<my::training_data>
ID3Algorithm::getPartitionedData(const my::multiple_sample_features& features,
                                 const my::multiple_sample_classes& classes,
                                 const my::intervals& intervals,
                                 int indexOfFeatureUsedToSplitSamples) {
    int numIntervals = intervals.size();
    int numSamples = features.size();

    std::vector<my::training_data> partitionedData(numIntervals);

    for(int sampleIndex = 0; sampleIndex < numSamples; sampleIndex++) {
        double featureVal = features[sampleIndex]->operator[](indexOfFeatureUsedToSplitSamples);
        for(int intervalIndex = 0; intervalIndex < numIntervals; intervalIndex++) {
            if(featureVal <= intervals.at(intervalIndex).first) {
                partitionedData[intervalIndex].features.push_back(features[sampleIndex]);
                partitionedData[intervalIndex].classes.push_back(classes[sampleIndex]);
                break;
            }
        }
    }

    return partitionedData;
}

double
ID3Algorithm::calculateEntropy(const my::multiple_sample_classes& classes) {
    double class1Proportion = ID3Algorithm::getProportion(0, classes);
    double class2Proportion = ID3Algorithm::getProportion(1, classes);

    return (-class1Proportion * (log10(class1Proportion) / log10(2))) -
           (class2Proportion * (log10(class2Proportion) / log10(2)));
}

int
ID3Algorithm::findFeatureProvidingLargestInfoGain(const my::multiple_sample_features& features, const my::multiple_sample_classes& classes) {
    int maxInformationGainFeature = 0;
    while(Node::attributesAlreadyUsedToSplitANode.find(maxInformationGainFeature) !=
                            Node::attributesAlreadyUsedToSplitANode.end()) {
        maxInformationGainFeature++;
    }

    double maxInformationGain = -std::numeric_limits<double>::max();

    double entropy = ID3Algorithm::calculateEntropy(classes);

    int numColumns = features.at(0)->size();
    for(int featureIndex = 0; featureIndex < numColumns; featureIndex++) {
        if(Node::attributesAlreadyUsedToSplitANode.find(featureIndex) ==
           Node::attributesAlreadyUsedToSplitANode.end()) {
            my::intervals intervals =
                 ID3Algorithm::getIntervalsForFeature(
                                features,
                                featureIndex,
                                ID3Algorithm::MIN_SAMPLES_FOR_SPLIT);


            std::vector<my::training_data> partitionedData =
                     ID3Algorithm::getPartitionedData(features,
                                                      classes,
                                                      intervals,
                                                      featureIndex);

            double resultantInformationGain =
              ID3Algorithm::calculateInformationGain(partitionedData, entropy);

            if(resultantInformationGain > maxInformationGain) {
                maxInformationGain = resultantInformationGain;
                maxInformationGainFeature = featureIndex;
            }
        }
    }

    return maxInformationGainFeature;
}

double
ID3Algorithm::calculateInformationGain(const std::vector<my::training_data>& partitionedData, double entropy) {
    double totalPartitionEntropy = 0.0;
    unsigned int datasetSize = partitionedData.size();

    if(datasetSize) {
        return 0.0;
    }

    for(unsigned int i = 0; i < datasetSize; i++) {
        my::multiple_sample_classes classes = partitionedData[i].classes;

        int partitionSize = classes.size();
        double partitionEntropy = ID3Algorithm::calculateEntropy(classes);

        totalPartitionEntropy += (partitionSize * partitionEntropy);
        datasetSize += partitionSize;
    }

    return entropy - (totalPartitionEntropy / datasetSize);
}

std::vector<Node*>
ID3Algorithm::createChildren(const std::vector<my::training_data>& partitionedData, const Node* parent) {
    std::vector<Node*> children;
    int numPartitions = partitionedData.size();

    for(int i = 0; i < numPartitions; i++) {
        my::multiple_sample_features features = partitionedData[i].features;
        my::multiple_sample_classes classes = partitionedData[i].classes;
        children.push_back(new Node(features, classes));
    }

    return children;
}
