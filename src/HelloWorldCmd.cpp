#include <math.h>
#include <maya/MIOStream.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MFnNurbsCurve.h>
#include "common.h"
#include <vector>

#include <maya/MFnUnitAttribute.h>

// Macro for debugging
#define McheckErr(stat, msg)  \
  if (MS::kSuccess != stat) { \
    cerr << msg;              \
    return MS::kFailure;      \
  }

// TODO: Move to separate class with header files and stuff
class FurrySystem : public MPxNode {
 public:
  static MTypeId id;  // Some hash to identify plugin with
  static MObject time;
  static std::vector<Strand> strands;

  FurrySystem(){};
  virtual ~FurrySystem(){};

  static void* creator() { return new FurrySystem; };
  // From old doIt
  static MStatus initialize() {
    cout << ">> Initializing";
    MStatus stat;

    MFnUnitAttribute unitAttr;
    time = unitAttr.create("time", "tm", MFnUnitAttribute::kTime, 0.0, &stat);
    McheckErr(stat, "ERROR creating time attribute\n");
    stat = addAttribute(time);
    McheckErr(stat, "ERROR adding time attribute\n");
    if (MS::kSuccess != stat) {
      cout << "Error creating curve.\n";
    }
    stat = createStrands(stat);
    return stat;
  }

  static MStatus createStrands(MStatus& stat) {
    // delete strand_0; // DO THIS IN DEINITIALIZE PLUGIN
    const unsigned num_segments = 10;
    float strand_length = 10.f;
    Strand strand_0 =
        Strand(MPoint(0.0, 0.0, 0.0), num_segments, MVector(0, -1, 0), 15.f);
    strands.push_back(strand_0);
    MFnNurbsCurve curveFn;
    MObject curve = curveFn.create(
        strand_0.point_positions, strand_0.knot_sequences, strand_0.degree,
        MFnNurbsCurve::kOpen, false, false, MObject::kNullObj, &stat);
    return stat;
  }

  virtual MStatus compute(const MPlug& plug, MDataBlock& data) {
    cout << ">> Computing ..\n";
    return MS::kSuccess;
  }
};

// Initialize static variables
MObject FurrySystem::time;
MTypeId FurrySystem::id( 0x80000 );
std::vector<Strand> FurrySystem::strands;

// Globals
MStatus initializePlugin(MObject obj) {
  cout << ">> Initializing plugin .. \n";
  MStatus   status;
  MFnPlugin plugin(obj, "Kebaum, Nova, Kingen", "0.1", "Any");
  status = plugin.registerNode("FurrySystem", FurrySystem::id,
                               FurrySystem::creator, FurrySystem::initialize);
  if (!status) {
    status.perror("registerNode");
    return status;
  }
  return status;
}

MStatus uninitializePlugin(MObject obj) {
  cout << ">> Uninitializing plugin \n";
  MStatus   status;
  MFnPlugin plugin(obj);
  status = plugin.deregisterNode(FurrySystem::id);
  if (!status) {
    status.perror("deregisterNode");
    return status;
  }
  return status;
}




