#ifndef FURRY_SYSTEM_NODE_H
#define FURRY_SYSTEM_NODE_H

#include <vector>
#include <math.h>
#include <maya/MPxNode.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MVectorArray.h>
#include <maya/MFnNurbsCurve.h>
#include "common.h"

class FurrySystemNode : public MPxNode {

public:
  FurrySystemNode();
  ~FurrySystemNode();

  static void* creator();
  static MStatus initialize();
  virtual MStatus compute(const MPlug& plug, MDataBlock& data);

  static MTypeId id;
  static MObject input_curves;
  static MObject input_follicles;
  static MObject input_sphere_matrix;
  static MObject output_curves;
  static MObject time;

  static int current_frame;

  std::vector<std::vector<Spring*> > springs;
	std::vector<MVectorArray> velocities;
	std::vector<MVectorArray> forces;

	int num_hairs;
  int num_hair_points;
  float hair_length;
	float curviness;
  float curliness;
	float delta_time;
};

#endif // FURRY_SYSTEM_NODE_H
