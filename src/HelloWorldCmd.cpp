#include <math.h>
#include <maya/MIOStream.h>
#include <maya/MSimple.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MFnNurbsCurve.h>
#include "common.h"
#include <vector>
DeclareSimpleCommand( doHelix, "Autodesk - Example", "2017");
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
}