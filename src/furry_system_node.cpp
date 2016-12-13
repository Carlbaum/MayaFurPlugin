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
  // cout << "FurrySystemNode constructor!!\n";

  hair_length = 10.f;
  num_hairs = 100;
  num_hair_points = 4;
  curviness = 1.5f;
  curliness = 1.5f;

  //INITIALIZE ARRAYS
  for (int h = 0; h < num_hairs; h++) {
    MVectorArray point_velocities;
    MVectorArray point_forces;
    for (int p = 0; p < num_hair_points; p++) {
      point_velocities.append(MVector(0,0,0));
      point_forces.append(MVector(0,0,0));
    }
    velocities.push_back(point_velocities);
    forces.push_back(point_forces);
  }

  float segment_length = (hair_length / (num_hair_points - 1.f));

  for (int i = 1 ; i < num_hair_points; i++) {
    std::vector<Spring*> springs_attached_to_point;
    springs_attached_to_point.push_back(
        new Spring(i - 1, i, segment_length, 20 * (num_hair_points - i)));
    if (i > 1) {
      springs_attached_to_point.push_back(
          new Spring(i - 2, i, segment_length * curviness , 35 * (num_hair_points - i)));
      // if (i > 2) {
      //   springs_attached_to_point.push_back(
      //       new Spring(i - 3, i, segment_length * curliness , 60 * (num_hair_points - i)));
      // }
    }

    springs.push_back(springs_attached_to_point);
  }
  cout << "FurrySystemNode EXITING constructor!!\n";
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
    cout << "compute: Plug output_curves is being handled\n";

    MStatus stat;
    MArrayDataHandle output_array_handle = data.outputArrayValue(output_curves, &stat);
    McheckErr(stat, "\tFailed at getting data.outputValue\n");

    MArrayDataHandle input_array_handle = data.inputArrayValue(input_curves, &stat);
    McheckErr(stat, "\tFailed at getting data.inputValue\n");


    int num_curves = input_array_handle.elementCount();

    

    // Count the number of CVs

    // UPDATE FORCES
    // For every strand i
    for (int i = 0 ; i < num_curves; i++ ) {
      cout << "Calculating forces for strand " << i;
      // output_array_handle.jumpToElement(i);
      input_array_handle.jumpToElement(i);

      MDataHandle input_element_handle = input_array_handle.inputValue(&stat);
      McheckErr(stat, "\tFailed at getting input_element_handle\n");

      // MDataHandle output_element_handle = output_array_handle.outputValue(&stat);
      // McheckErr(stat, "\tFailed at getting output_element_handle\n");

      MObject curve_obj = input_element_handle.asNurbsCurve();

      if (curve_obj.isNull()) {
        cerr << "\tERROR getting the inputValue asNurbsCurve!\n";
      }
      // else {
        // cout << "\tSucceeded to get the inputValue asNurbsCurve!!\n";
      // }
      MFnNurbsCurve curve_fn(curve_obj, &stat);
      McheckErr(stat, "\t\tFailed at accessing the curve's function set!\n");

      MPointArray cvs;

      stat = curve_fn.getCVs(cvs, MSpace::kWorld);
      McheckErr(stat, "\tFailed at getting CVs\n");

      // for every point in each hair (excluding root point)
      for (int p = 1; p < cvs.length(); p++) {
        cout << " and point " << p << "\n";
        // cvs[p] += MPoint(p, 0, 0);

        forces[i][p] = MVector(0,0,0); //reset forces
        MPoint current_position = cvs[p];
        // For every spring s whos endpoint is p
        for (int s = 0; s < springs[p-1].size(); s++) {
          cout << "\tspring " << s << "\n";
          // INTERNAL FORCES
          MPoint prev_position; //will be related to the spring point closer to the root
          int prev_point_id   = springs[p-1][s]->p1;
          float stiffness     = springs[p-1][s]->stiffness;
          float rest_length   = springs[p-1][s]->rest_length;
          prev_position = cvs[prev_point_id];

          MVector spring_vector = prev_position - current_position;
          float current_length = spring_vector.length();
          // Hooke's law
          MVector spring_force = stiffness * ( current_length / rest_length - 1 ) * (spring_vector / current_length);

          if (prev_point_id > 0) {
            forces[i][prev_point_id] += -spring_force;
          }

          forces[i][p] += spring_force;

         // MPoint updated_position;
         // stat = strands[h]->curve_fn.setCV(p, updated_position);
          // cout << "\t>> Strand #" << h << "\tNumber of springs attached" <<
          // strands[i][] << "\n";
          //   for ( int spring = 0; spring < strands[i].size();)
          //   for ( int spring = 0; spring < strands[i].size();)
          // (*(strands[h]->springs[p][s]->p2)).x += 2;
          //cout << "\t\t\tPoint position = " << *(strands[h]->springs[p][s]->p2)
               // << "\n";
        }
        // EXTERNAL FORCES
        forces[i][p] += MVector(0, -9.82, 0); // assuming mass=1
        // Damping force
        forces[i][p] -= 0.9 * velocities[i][p];
      }
    }

    // Velocities and positions
    // For every strand i
    for (int i = 0 ; i < num_curves; i++ ) {
      cout << "Calculating velocities and positions for strand " << i;
      output_array_handle.jumpToElement(i);
      input_array_handle.jumpToElement(i);

      MDataHandle input_element_handle = input_array_handle.inputValue(&stat);
      McheckErr(stat, "\tFailed at getting input_element_handle\n");

      MDataHandle output_element_handle = output_array_handle.outputValue(&stat);
      McheckErr(stat, "\tFailed at getting output_element_handle\n");

      MObject curve_obj = input_element_handle.asNurbsCurve();

      if (curve_obj.isNull()) {
        cerr << "\tERROR getting the inputValue asNurbsCurve!\n";
      }
      // else {
      //   cout << "\tSucceeded to get the inputValue asNurbsCurve!!\n";
      // }
      MFnNurbsCurve curve_fn(curve_obj, &stat);
      McheckErr(stat, "\t\tFailed at accessing the curve's function set!\n");

      MPointArray cvs;

      stat = curve_fn.getCVs(cvs, MSpace::kWorld);
      McheckErr(stat, "\tFailed at getting CVs\n");

      // for every point in each hair (excluding root point)
      for (int p = 1; p < cvs.length(); p++) {
        // cout << "\t\t\tVel_prev: " << velocities[p];
        velocities[i][p] += 0.01 * forces[i][p] / 1.0; // assumes ∆t=0.1 and mass = 1
        // cout << "\n\t\t\t\tVel_post: " << velocities[p] << "\n";
        MPoint prev_position;
        prev_position = cvs[p]; // TODO: this is the error! Reason that it doesn't update
        // cout << "\t\t\tPos_prev: " << prev_position;
        MPoint new_position = prev_position + 0.01 * velocities[i][p];
        stat = curve_fn.setCV(p, new_position);
        McheckErr(stat, "\tFailed at setting CVs\n");
        // cout << "\n\t\t\t\tPos_post: " << new_position << "\n";
      }
      // stat = curve_fn.setCVs(cvs, MSpace::kWorld);
      // McheckErr(stat, "\tFailed at setting CVs\n");

      stat = curve_fn.updateCurve();
      McheckErr(stat, "\tFailed at updating curve!\n");

      //TODO: släng in i output array some-fucking-how

      output_element_handle.set(curve_obj);

      // iterate
      /*
      stat = input_array_handle.next();
      McheckErr(stat, "\t\tFailed at iterating through input_array!\n");

      stat = output_array_handle.next();
      McheckErr(stat, "\t\tFailed at iterating through output_array!\n");
      */
    }

    data.setClean(plug);
  } else {
    return MS::kUnknownParameter;
  }
  return MS::kSuccess;
}
