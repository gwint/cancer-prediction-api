<h1 align="center">
    Predicting Tumor Malignancy With a Decision Tree Classifier
    <br>
    <br>
    <img src="https://www.researchgate.net/profile/Simone_Ludwig/publication/321707228/figure/fig2/AS:588683954647044@1517364600325/Decision-tree-obtained-from-FDT-classifier-for-the-Ovarian-cancer-data-set.png" alt="Raft logo" width="200">
</h1>

A decision tree created using the ID3 training algorithm.

Data Set: Wisconsin Breast Cancer Diagnostic Data 
Data Set URL: http://archive.ics.uci.edu/ml/machine-learning-databases/breast-cancer-wisconsin/wdbc.data 

## What's a Decision Tree?

A decision tree is a tree-based predictive model that makes a guess as to the value of 
some target variable based on several input variables.  If this target variable
is a real number then the decision tree is known as a regression tree, while one
created to predict a discrete target variable is termed a classification tree.
As you can see from the illustration above of a decision tree that I pulled
off of google images, a series of decisions are made that take one from the root
down to a leaf node, where a determination as to the value of the target variable
can be made.
<br>
<br>
[This article](https://engineering.purdue.edu/~landgreb/SMC91.pdf) has more information on the theory behind decision trees learning.

## How to run:   

First compile using:   
ant -buildfile /api/DecisionTreeClassifier/decisionTreeClassifier/src/build.xml all

Then start server using:
ant -buildfile api/DecisionTreeClassifier/decisionTreeClassifier/src/build.xml runServer -Darg0=9000

*You'll probably need sudo*
