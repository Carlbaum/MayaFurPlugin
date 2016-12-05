// 1. Load Plugin into Maya
// 2. Save the following lines as a MEL-script in Maya
  // // Create a hair system node and connect it to the timeslider
  // createNode FurrySystemNode -n furry;
  // connectAttr time1.outTime furry.time;
  // // Create the output shape node of the entire hair system and connect it to the hair system
  // createNode nurbsCurve -n hairSystemShape;
  // connectAttr furry.output_curve hairSystemShape.create;
  // // Create a start curve and connect it to the hair system as input shape
  // curve -p 0 10 0 -p 5 0 0 -p 5 0 5 -p 0 0 5;
  // setAttr "curveShape1.intermediateObject" 1;
  // connectAttr curveShape1.local furry.input_curve;

#include "furry_system_node.h"
#include <maya/MFnNurbsCurveData.h>

MObject FurrySystemNode::time;
MObject FurrySystemNode::input_curve;
MObject FurrySystemNode::output_curve;
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
  cout << ">> Initializing in FurrySystemNode\n";
  MStatus stat;

  // Create attributes
  MFnUnitAttribute unit_attr;
  MFnTypedAttribute typed_attr;
  time = unit_attr.create("time", "tm", MFnUnitAttribute::kTime, 0.0, &stat);
  input_curve = typed_attr.create("input_curve", "in",
                                  MFnNurbsCurveData::kNurbsCurve, &stat);
  McheckErr(stat, "Failed to create input curve\n");
  output_curve = typed_attr.create("output_curve", "out",
                                   MFnNurbsCurveData::kNurbsCurve, &stat);
  McheckErr(stat, "Failed to create output curve\n");
  unit_attr.setStorable(false);
  typed_attr.setStorable(false);

  // Add attributes to our node
  addAttribute(time);
  addAttribute(input_curve);
  addAttribute(output_curve);

  // When time is modified, input curve needs to be recalculated
  attributeAffects(time, output_curve);

  return MS::kSuccess;
}

MStatus FurrySystemNode::compute(const MPlug& plug, MDataBlock& data) {
  if (plug == output_curve) {
    // cout << "compute: Plug output_curve is being handled\n";

    MStatus stat;
    MDataHandle output_handle = data.outputValue(output_curve, &stat);
    McheckErr(stat, "\tFailed at getting data.outputValue\n");
    MDataHandle input_handle = data.inputValue(input_curve, &stat);
    McheckErr(stat, "\tFailed at getting data.inputValue\n");
    MObject curve_obj = input_handle.asNurbsCurve();
    if (curve_obj.isNull()) {
      cerr << "\tERROR getting the inputValue asNurbsCurve!\n";
    } /*else {
      cout << "\tSucceeded to get the inputValue asNurbsCurve!!\n";
    }*/
    MFnNurbsCurve curve_fn(curve_obj, &stat);
    McheckErr(stat, "\t\tFailed at accessing the curve's function set!\n");

    MPointArray cvs;
    stat = curve_fn.getCVs(cvs, MSpace::kWorld);
    McheckErr(stat, "\tFailed at getting CVs\n");
    for (int i = 0; i < cvs.length(); i++) {
      cvs[i] += MPoint(i, 0, 0);
    }

    stat = curve_fn.setCVs(cvs, MSpace::kWorld);
    McheckErr(stat, "\tFailed at setting CVs\n");
    stat = curve_fn.updateCurve();
    McheckErr(stat, "\tFailed at updating curve!\n");

    output_handle.set(curve_obj);
    data.setClean(plug);
  } else {
    return MS::kUnknownParameter;
  }
  return MS::kSuccess;
}
