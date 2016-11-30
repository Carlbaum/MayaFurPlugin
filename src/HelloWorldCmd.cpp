// createNode FurrySystem -n FurryNode1;
// connectAttr time1.outTime FurryNode1.time;

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
	//static MObject outputCurve;
  std::vector<Strand*> strands;

  FurrySystem() {
  	cout << "FurrySystem Constructor!!\n";
    MStatus stat = createStrands();
    if (stat == MS::kSuccess)
    	cout << "Successfully created strand!\n";
  }

  virtual ~FurrySystem() {
  	cout << "FurrySystem destructor!!\n";
	  while(!strands.empty()) {
	  	delete strands.back();
	  	strands.pop_back();
	  }
  };

  static void* creator() { 
  	cout << "CREATOR!!\n";
  	return new FurrySystem; 
  };
  // From old doIt
  static MStatus initialize() {
    cout << ">> Initializing\n";
    MStatus stat;

    MFnUnitAttribute unitAttr;
    time = unitAttr.create("time", "tm", MFnUnitAttribute::kTime, 0.0, &stat);
    McheckErr(stat, "ERROR creating time attribute\n");
    stat = addAttribute(time);
    McheckErr(stat, "ERROR adding time attribute\n");
    if (MS::kSuccess != stat) {
      cout << "Error creating curve.\n";
    }

    return MS::kSuccess;
  }
//TODO: Motsvarande createMesh i animCube
  MStatus createStrands() {
    // delete strand_0; // DO THIS IN DEINITIALIZE PLUGIN
    MStatus stat;
    const unsigned num_segments = 10;
    float strand_length = 10.f;
    Strand* new_strand = new Strand( MPoint( 0.0, 10.0, 0.0 ), num_segments, MVector( 0, -1, 0 ), 15.f ) ;
    strands.push_back( new_strand );
    MObject curve = new_strand->curve.create/*WithEditPoints*/(
        strands.back()->point_positions, strands.back()->knot_sequences, strands.back()->degree,
        MFnNurbsCurve::kOpen, false, false/*, false*/, outputCurve, &stat);
    McheckErr(stat, "ERROR adding time attribute\n");
    return MS::kSuccess;
  }

  void UpdateHairSystem() {
  	cout << ">> Number of strands: " << strands.size() << "\n";
  	// For every strand of hair h in hair system
  	for (int h = 0; h < strands.size(); h++ ) {
  		cout << "\t>> Number of points in strand: " << strands[h]->springs.size() << "\n";
  		// For every point p in strand h
  		for (int p = 0; p < strands[h]->springs.size(); p++) {
  			cout << "\t\t>> Number of springs that has this point as endpoint: " << strands[h]->springs[p].size() << "\n";
  			// For every spring s whos endpoint is p
	  		MStatus stat;
	  		MPoint temp;
	  		stat = strands[h]->curve.getCV(p,temp);
	  		cout << "\t\t\tCurve position = " << temp << "\n";
	  		temp += MPoint(.2,0.0,0.0);
	  		stat = strands[h]->curve.setCV(p,temp);
  			for (int s = 0; s < strands[h]->springs[p].size(); s++) {
		  		// cout << "\t>> Strand #" << h << "\tNumber of springs attached" << strands[i][] << "\n";
		  		// 	for ( int spring = 0; spring < strands[i].size();)
		  		// 	for ( int spring = 0; spring < strands[i].size();)
		  		// (*(strands[h]->springs[p][s]->p2)).x += 2;
		  		cout << "\t\t\tPoint position = " << *(strands[h]->springs[p][s]->p2) << "\n";

  			}
  		}
  		MStatus stat = strands[h]->curve.updateCurve();
  		if (stat != MS::kSuccess) {
  			cerr << "Jaha det här sket sig ju!\n";
  		}
		}
  }

  virtual MStatus compute(const MPlug& plug, MDataBlock& data) {
    cout << ">> Computing ..\n";
    UpdateHairSystem();
    return MS::kSuccess;
  }
};

// Initialize static variables
MObject FurrySystem::time;
//MObject FurrySystem::outputCurve;
MTypeId FurrySystem::id( 0x80000 );

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