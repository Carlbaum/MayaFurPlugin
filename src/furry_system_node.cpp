// curve -p 0 10 0 -p 5 0 0 -p 5 0 5 -p 0 0 5;
// createNode FurrySystemNode -n furry;
// connectAttr time1.outTime furry.time;
// connectAttr curveShape1.local furry.input_curve;

#include "furry_system_node.h"

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
  input_curve =
      typed_attr.create("input_curve", "in", MFnData::kNurbsCurve, &stat);
  McheckErr(stat, "Failed to create input curve\n");
  output_curve =
      typed_attr.create("output_curve", "out", MFnData::kNurbsCurve, &stat);
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

    cout << "Computing input_curve\n";

    MStatus stat;
    MDataHandle handle = data.inputValue(input_curve, &stat);
    //MDataHandle handleoutput = data.outputValue(output_curve, &stat);
    McheckErr(stat, "Failed at getting data.inputValue\n");
    MObject curve_obj = handle.asNurbsCurve();
    MFnNurbsCurve curve_fn(curve_obj, &stat);
    McheckErr(stat, "Failed at creating the MFnNurbsCurve\n");

    /*MPointArray cvs;
    stat = curve_fn.getCVs(cvs, MSpace::kWorld);
    for (int i = 0; i < cvs.length(); i++) {
      cvs[i] += MPoint(i, 0, 0);
    }

    curve_fn.setCVs(cvs, MSpace::kWorld);
    curve_fn.updateCurve();
    handle.set(curve_obj);*/
    data.setClean(plug);
  }
  return MS::kSuccess;
}
