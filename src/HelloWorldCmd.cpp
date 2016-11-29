#include <math.h>
#include <maya/MIOStream.h>
//#include <maya/MSimple.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MFnNurbsCurve.h>
#include "common.h"
#include <vector>
/*DeclareSimpleCommand( doHelix, "Autodesk - Example", "2017");
MStatus doHelix::doIt( const MArgList& )
{
    MStatus stat;
    const unsigned num_segments = 10;
    float strand_length = 10.f;
    cout << "hej hoppppppppp.\n";
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
    delete strand_0;
    cout << "Fur plugin exited normally!\n";
    return stat;
}*/

class FurrySystem : public MPxNode {
  public:
    // Some hash to identify plugin with
    static MTypeId id;

    FurrySystem() {};
    virtual ~FurrySystem() {};
    static void* creator() {
      return new FurrySystem;
    };

    static MStatus initialize() {
      /*MFnUnitAttribute unitAttr;
      MFnTypedAttribute typedAttr;
      MStatus returnStatus;
      animCube::time = unitAttr.create( "time", "tm",
                                            MFnUnitAttribute::kTime,
                                            0.0, &returnStatus );
      McheckErr(returnStatus, "ERROR creating animCube time attribute\n");
      animCube::outputMesh = typedAttr.create( "outputMesh", "out",
                                                   MFnData::kMesh,
                                                   &returnStatus ); 
      McheckErr(returnStatus, "ERROR creating animCube output attribute\n");
      typedAttr.setStorable(false);
      returnStatus = addAttribute(animCube::time);
      McheckErr(returnStatus, "ERROR adding time attribute\n");
      returnStatus = addAttribute(animCube::outputMesh);
      McheckErr(returnStatus, "ERROR adding outputMesh attribute\n");
      returnStatus = attributeAffects(animCube::time,
                                      animCube::outputMesh);
      McheckErr(returnStatus, "ERROR in attributeAffects\n");*/
      return MS::kSuccess;
    }

};

MTypeId FurrySystem::id( 0x80000 );

MStatus initializePlugin(MObject obj) {
  std::cout << ">> Initializing plugin .. " << std::endl;
  MStatus   status;
  MFnPlugin plugin(obj, "Kebaum, Nova, Kingen", "0.1", "Any");
  status = plugin.registerNode("animCube", FurrySystem::id, FurrySystem::creator, FurrySystem::initialize);
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




