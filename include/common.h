#ifndef COMMON_H
#define COMMON_H

#include <cmath>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MVectorArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MFnNurbsCurve.h>
#include <vector>
#include <maya/MIOStream.h>

// Macro for debugging
#define McheckErr(stat, msg)  \
  if (MS::kSuccess != stat) { \
    cerr << msg;              \
    return MS::kFailure;      \
  }

struct Spring {
  unsigned p1;
  unsigned p2;
  float rest_length;
  float stiffness;  // could be named spring_constant
                    // Contructors
  Spring(unsigned point_1,
         unsigned point_2,
         float rest_length = 1.f,
         float spring_constant = 25.f)
      : p1(point_1),
        p2(point_2),
        rest_length(rest_length),
        stiffness(spring_constant) {}

  // Public methods
  /*  void CalculateSpringForce() {
                  MVector relative_vector = *p2 - *p1;
                  float current_length = relative_vector.length();
                  MVector force = -stiffness * std::abs( rest_length -
     current_length ) * ( relative_vector / current_length );
                  p1->AddForce( force );
                  p2->AddForce( -force );
          }
  */
};

struct Strand {
  // MFloatArray masses;
  // std::vector<Spring*> edge_springs;
  std::vector<std::vector<Spring*> > springs;  // this should probably be used

  MVectorArray velocities;
  MVectorArray forces;
  MObject curve_obj;
  MFnNurbsCurve curve_fn;

  // Constructors
  Strand(MPoint root_point,
         int segments,  // number of edge springs
         MVector direction,
         float strand_length = 1.0f,
         const unsigned degree = 1,
         float curviness = 1.5f,
         float curliness = 1.5f) {
    cout << "Strand constructor!\n";
    MStatus stat;
    float segment_length = (strand_length / segments);
    MVector delta_position = segment_length * direction;

    MPointArray point_positions;
    MDoubleArray knot_sequences;
    // Create the root mass point
    point_positions.append(root_point);
    knot_sequences.append((double)0);
    velocities.append(MVector(0, 0, 0));
    forces.append(MVector(0, 0, 0));

    for (int i = 1; i < segments + 1; i++) {
      point_positions.append(root_point + i * delta_position);
      knot_sequences.append((double)i);
      std::vector<Spring*> springs_attached_to_point;
      springs_attached_to_point.push_back(
          new Spring(i - 1, i, segment_length, 20 * (segments + 1 - i)));
      if (/*false &&*/ i > 1) {
        springs_attached_to_point.push_back(new Spring(
            i - 2, i, segment_length * curviness, 35 * (segments + 1 - i)));
        if (false && i > 2) {
          springs_attached_to_point.push_back(new Spring(
              i - 3, i, segment_length * curviness, 60 * (segments + 1 - i)));
        }
      }

      springs.push_back(springs_attached_to_point);

      velocities.append(MVector(0, 0, 0));
      forces.append(MVector(0, 0, 0));
    }
    curve_obj = curve_fn.create /*WithEditPoints*/ (
        point_positions, knot_sequences, degree, MFnNurbsCurve::kOpen, false,
        false /*, false*/, MObject::kNullObj, &stat);
    if (stat != MS::kSuccess) {
      cerr << "Strand Constructor FAILED!!!!!!\n";
    } else {
      cout << "NumSegments: " << segments << ",\tNumPoints" << curve_fn.numCVs()
           << "\n";
    }
    cout << "Strand constructor, EXIT!\n";
  }

  ~Strand() {
    cout << "Strand destructor is called! Strand contains " << springs.size()
         << " points\n";
    // for each point
    for (int p = 0; p < springs.size(); ++p) {
      cout << "\tPoint #" << p;
      cout << ", has " << springs[p].size() << " springs\n";
      // for each spring
      while (!springs[p].empty()) {
        delete springs[p].back();
        springs[p].pop_back();
        cout << "\t\tSpring has been deleted! It now has " << springs[p].size()
             << " springs\n";
      }
    }
  }

  void UpdateVelocitiesAndPositions() {
    for (int i = 1; i < curve_fn.numCVs(); i++) {
      cout << "\t\t\tVel_prev: " << velocities[i];

      velocities[i] += 0.01 * forces[i] / 1;  // assumes ∆t=0.1 and mass = 1

      cout << "\n\t\t\t\tVel_post: " << velocities[i] << "\n";

      MPoint prev_position;
      curve_fn.getCV(i, prev_position);

      cout << "\t\t\tPos_prev: " << prev_position;

      MPoint new_position = prev_position + 0.01 * velocities[i];
      curve_fn.setCV(i, new_position);

      cout << "\n\t\t\t\tPos_post: " << new_position << "\n";
    }
  }
};

#endif  // COMMON_H
