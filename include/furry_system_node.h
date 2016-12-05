#ifndef FURRY_SYSTEM_NODE_H
#define FURRY_SYSTEM_NODE_H

#include <vector>
#include <math.h>
#include <maya/MPxNode.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MFnNurbsCurve.h>
#include "common.h"

#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>

// Simple deformer node
class FurrySystemNode : public MPxNode {

public:
  FurrySystemNode();
  ~FurrySystemNode();

  static void* creator();
  static MStatus initialize();
  virtual MStatus compute(const MPlug& plug, MDataBlock& data);

  static MTypeId id;
  static MObject input_curve;
  static MObject output_curve;
  static MObject time;
};

#endif // FURRY_SYSTEM_NODE_H
