package classifier;

import util.Linkable;
import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;
import java.util.Set;
import java.util.HashSet;
import java.util.Set;
import java.util.Iterator;
import util.NDArray;
import util.Interval;
import visitors.VisitorI;
import util.Dataset;

public class Node implements Linkable, Cloneable {
  private static final int INVALID_NODE_ID = -1;
  private static final int NO_LABEL_ASSIGNED = -1;
  private static int nodeCount = 0;
  private int nodeId;
  private List<Node> children;
  private List<Integer> sampleIndices;
  private double label;
  private Integer splitAttributeIndex;
  private Node parent;
  private Dataset dataset;
  private Set<Integer> usedAttributes;
  private List<Interval> splitAttributeIntervals;

  public Node(List<Integer> sampleIndicesIn,
              Dataset datasetIn,
              Set<Integer> usedAttributesIn) {
    if(sampleIndicesIn == null) {
      throw new IllegalArgumentException("List of sample indices must not be null");
    }

    this.sampleIndices = sampleIndicesIn;
    this.children = null;
    this.nodeId = Node.nodeCount++;
    this.label = Node.NO_LABEL_ASSIGNED;
    this.dataset = datasetIn;
    this.usedAttributes = usedAttributesIn;
  }

  public Set<Integer> getUsedAttributes() {
    return this.usedAttributes;
  }

  public void setSplitAttributeIntervals(List<Interval> intervalsIn) {
    if(intervalsIn == null) {
      throw new IllegalArgumentException("List of intervals must not be null");
    }
    this.splitAttributeIntervals = intervalsIn;
  }

  public List<Interval> getSplitAttributeIntervals() {
    return this.splitAttributeIntervals;
  }

  public Integer getSplitAttributeIndex() {
    return this.splitAttributeIndex;
  }

  public void setSplitAttribute(Integer splitAttributeIn) {
    if(splitAttributeIn == null) {
      throw new IllegalArgumentException("Cannot set split attribute to null");
    }
    this.splitAttributeIndex = splitAttributeIn;
    this.addUsedAttribute(splitAttributeIn);
  }

  private void addUsedAttribute(Integer newAttributeIn) {
    if(newAttributeIn == null) {
      throw new IllegalArgumentException("Integer representing used attribute cannot be null");
    }
    if(this.getUsedAttributes().contains(newAttributeIn)) {
      throw new IllegalArgumentException("No attribute should be used twice within the same branch");
    }
    this.usedAttributes.add(newAttributeIn);
  }

  public static int getNodeCount() {
    return Node.nodeCount;
  }

  public void setParent(Node parentNode) {
    this.parent = parentNode;
  }

  public void setLabel(double labelIn) {
    this.label = labelIn;
  }

  public double getClassNum() {
    return this.label;
  }

  public List<Integer> getSampleIndices() {
    if(this.sampleIndices == null) {
      throw new UnsupportedOperationException("Cannot get the sample indices of a leaf node");
    }
    return this.sampleIndices;
  }

  public Node getParent() {
    return this.parent;
  }

  public boolean isHomogenous() {
    List<Integer> sampleIndices = this.getSampleIndices();

    boolean areIdentical = false;
    Double onlyClassPresent = null;
    for(int sampleIndexIndex = 0; sampleIndexIndex < sampleIndices.size() - 1;
        sampleIndexIndex++) {
      Integer indexObj = sampleIndices.get(sampleIndexIndex);
      if(indexObj == null) {
        throw new IllegalStateException("No sample Index should be null");
      }
      int i = indexObj.intValue();

      if(onlyClassPresent == null) {
        onlyClassPresent = this.dataset.getClassLabel(i);
      }

      Double elem = this.dataset.getClassLabel(i);
      if(elem == null) {
        throw new IllegalStateException("No class label should be null");
      }
      if(!elem.equals(onlyClassPresent)) {
        areIdentical = false;
        break;
      }
    }
    return areIdentical;
  }

  @Override
  public String toString() {
    return "";
  }

  @Override
  public void finalize() {}

  @Override
  public int hashCode() {
    return Node.getNodeCount();
  }

  /**
   Two Node objects are equal if they have the same b-number and contain
   the same classes in the same order.
   @return true if Nodes are equal, false otherwise.
   */
  @Override
  public boolean equals(Object aNode) {
    return false;
  }

  public List<Node> getChildren() {
    return this.children;
  }

  public void setChildren(List<Node> childNodes) {
    if(childNodes == null) {
      throw new IllegalArgumentException("Cannot set list of child nodes to null.");
    }
    this.children = childNodes;
  }

  /**
   Determines if Node is valid (meaning it has a valid b-number) or if it is
   a dummy Node.
   @return true if this Node has a valid 3 digit b-number, false otherwise
   */
  public boolean isDummy() {
    return this.nodeId == Node.INVALID_NODE_ID;
  }

  public Dataset getDataset() {
    return this.dataset;
  }

  public void accept(VisitorI visitor) {
    visitor.visit(this);
  }
}
