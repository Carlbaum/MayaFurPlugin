// 1. Load Plugin into Maya
// 2. Save the following lines as a MEL-script in Maya
  // // Create a hair system node and connect it to the timeslider
  // createNode FurrySystemNode -n furry;
  // connectAttr time1.outTime furry.time;
  // // Create the output shape node of the entire hair system and connect it to the hair system
  // createNode nurbsCurve -n outputcurve1;
  // createNode nurbsCurve -n outputcurve2;
  // connectAttr furry.output_curves[0] outputcurve1.create;
  // connectAttr furry.output_curves[1] outputcurve2.create;
  // // Create a start curve and connect it to the hair system as input shape
  // curve -p 0 10 0 -p 5 0 0 -p 5 0 5 -p 0 0 5;
  // setAttr "curveShape1.intermediateObject" 1;
  // connectAttr curveShape1.local furry.input_curves[0];
  // curve -p 2 5 -1 -p 3 -2 1 -p 3 1 -1 -p 1 5 0;
  // setAttr "curveShape2.intermediateObject" 1;
  // connectAttr curveShape2.local furry.input_curves[1];

#include "furry_system_node.h"
#include <maya/MFnNurbsCurveData.h>

MObject FurrySystemNode::time;
MObject FurrySystemNode::input_curves;
MObject FurrySystemNode::output_curves;
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
  input_curves = typed_attr.create("input_curves", "in",
                                  MFnNurbsCurveData::kNurbsCurve, &stat);
  McheckErr(stat, "Failed to create input curve\n");
  stat = typed_attr.setArray( true );
  McheckErr(stat, "Failed to set input curve array\n");
  typed_attr.setStorable(false);

  output_curves = typed_attr.create("output_curves", "out",
                                   MFnNurbsCurveData::kNurbsCurve, &stat);
  McheckErr(stat, "Failed to create output curve\n");
  stat = typed_attr.setArray( true );
  McheckErr(stat, "Failed to set output curve array\n");
  unit_attr.setStorable(false);
  typed_attr.setStorable(false);

  // Add attributes to our node
  addAttribute(time);
  addAttribute(input_curves);
  addAttribute(output_curves);

  // When time is modified, input curve needs to be recalculated
  attributeAffects(time, output_curves);

  return MS::kSuccess;
}

MStatus FurrySystemNode::compute(const MPlug& plug, MDataBlock& data) {
  if (plug == output_curves) {
    // cout << "compute: Plug output_curves is being handled\n";

    MStatus stat;
    MArrayDataHandle output_array_handle = data.outputArrayValue(output_curves, &stat);
    McheckErr(stat, "\tFailed at getting data.outputValue\n");
    MArrayDataHandle input_array_handle = data.inputArrayValue(input_curves, &stat);
    McheckErr(stat, "\tFailed at getting data.inputValue\n");


    int num_curves = input_array_handle.elementCount();

    // Count the number of CVs
    output_array_handle.jumpToElement(0);
    input_array_handle.jumpToElement(0);
    for (int i = 0 ; i < num_curves; i++ ) {
      MDataHandle input_element_handle = input_array_handle.inputValue(&stat);
      MDataHandle output_element_handle = output_array_handle.outputValue(&stat);
      McheckErr(stat, "\tFailed at getting input_element_handle\n");
      MObject curve_obj = input_element_handle.asNurbsCurve();
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

      //TODO: släng in i output array some-fucking-how

      output_element_handle.set(curve_obj);

      // iterate
      stat = input_array_handle.next();
      McheckErr(stat, "\t\tFailed at iterating through input_array!\n");

      stat = output_array_handle.next();
      McheckErr(stat, "\t\tFailed at iterating through output_array!\n");
    }



    data.setClean(plug);
  } else {
    return MS::kUnknownParameter;
  }
  return MS::kSuccess;
}
