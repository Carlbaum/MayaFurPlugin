#include "furry_system_node.h"

MObject FurrySystemNode::time;
MTypeId FurrySystemNode::id(0x80000);

FurrySystemNode::FurrySystemNode() {
  cout << "FurrySystemNode constructor!!\n";
}

FurrySystemNode::~FurrySystemNode() {
  cout << "FurrySystemNode destructor!!\n";
};

void* FurrySystemNode::creator() {
 cout << "CREATOR!!\n";
 return new FurrySystemNode;
};


MStatus FurrySystemNode::initialize() {
  cout << ">> Initializing\n";
  return MS::kSuccess;
}
