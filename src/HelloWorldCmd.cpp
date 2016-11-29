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
#define McheckErr(stat,msg)         \
    if ( MS::kSuccess != stat ) {   \
        cerr << msg;                \
        return MS::kFailure;        \
    }

// TODO: Move to separate class with header files and stuff
class FurrySystem : public MPxNode {
  public:
    static MTypeId id; // Some hash to identify plugin with
    static MObject time;

    FurrySystem() {};
    virtual ~FurrySystem() {};
    static void* creator() {
      return new FurrySystem;
    };

    // From old doIt
    static MStatus initialize() {
      MStatus stat;
      const unsigned num_segments = 10;
      float strand_length = 10.f;
      cout << "hej hoppppppppp.\n";

      MFnUnitAttribute unitAttr;
      time = unitAttr.create( "time", "tm",
                                          MFnUnitAttribute::kTime,
                                          0.0, &stat);
      McheckErr(stat, "ERROR creating time attribute\n");
      stat = addAttribute(time);
      McheckErr(stat, "ERROR adding time attribute\n");

      Strand* strand_0 = new Strand(
          MPoint( 0.0, 0.0, 0.0 ),
          num_segments,
          MVector( 0,-1,0 ),
          15.f );
      cout << "KALAS!!.\n";
      MFnNurbsCurve curveFn;
      MObject curve = curveFn.create( strand_0->point_positions,
          strand_0->knot_sequences, strand_0->degree, MFnNurbsCurve::kOpen, false, false, MObject::kNullObj, &stat );
      cout << "FANTASTISKT.\n";
      if ( MS::kSuccess != stat )
          cout << "Error creating curve.\n";
      //delete strand_0; // DO THIS IN DEINITIALIZE PLUGIN
      return stat;
      //return MS::kSuccess;
    }

    virtual MStatus compute(const MPlug& plug, MDataBlock& data) {
      std::cout << "Computing .." << std::endl;
      return MS::kSuccess;
    }
};

// Initialize static variables
MObject FurrySystem::time;
MTypeId FurrySystem::id( 0x80000 );


// Globals
MStatus initializePlugin(MObject obj) {
  std::cout << ">> Initializing plugin .. " << std::endl;
  MStatus   status;
  MFnPlugin plugin(obj, "Kebaum, Nova, Kingen", "0.1", "Any");
  status = plugin.registerNode("FurrySystem", FurrySystem::id, FurrySystem::creator, FurrySystem::initialize);
  if (!status) {
    status.perror("registerNode");
    return status;
  }
  return status;
}

MStatus uninitializePlugin(MObject obj) {
  std::cout << ">> Uninitializing plugin " << std::endl;
  MStatus   status;
  MFnPlugin plugin(obj);
  status = plugin.deregisterNode(FurrySystem::id);
  if (!status) {
    status.perror("deregisterNode");
    return status;
  }
  return status;
}




