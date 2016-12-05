// // createNode FurrySystem -n FurryNode1;
// // connectAttr time1.outTime FurryNode1.time;


// //TODOS
// // 1. Test rebuild

// // READ
// // 1. Shape
// // 2. Data

// //THOUGHTS
// // Strand extend NurbsCurve?

// #include <vector>
// #include <math.h>
// #include <maya/MPxNode.h>
// #include <maya/MFnPlugin.h>
// #include <maya/MIOStream.h>
// #include <maya/MPoint.h>
// #include <maya/MPointArray.h>
// #include <maya/MDoubleArray.h>
// #include <maya/MFnNurbsCurve.h>
// #include "common.h"

// #include <maya/MFnUnitAttribute.h>
// #include <maya/MFnTypedAttribute.h>

// // TODO: Move to separate class with header files and stuff
// class FurrySystem : public MPxNode {
//  public:
//   static MTypeId id; // Some hash to identify plugin with
//   static MObject time;
// 	//static MObject outputCurve;
//   std::vector<Strand*> strands;
//   static MObject curve;

//   FurrySystem() {
//   	cout << "FurrySystem Constructor!!\n";
//     MStatus stat = createStrands();
//     if (stat == MS::kSuccess)
//     	cout << "Successfully created strand!\n";
//   }

//   virtual ~FurrySystem() {
//   	cout << "FurrySystem destructor!!\n";
// 	  while(!strands.empty()) {
// 	  	delete strands.back();
// 	  	strands.pop_back();
// 	  }
//   };

//   static void* creator() {
//   	cout << "CREATOR!!\n";
//   	return new FurrySystem;
//   };
//   // From old doIt
//   static MStatus initialize() {
//     cout << ">> Initializing\n";
//     MStatus stat;

//     MFnTypedAttribute typedAttr;

//     MFnUnitAttribute unitAttr;
//     time = unitAttr.create("time", "tm", MFnUnitAttribute::kTime, 0.0, &stat);
//     McheckErr(stat, "ERROR creating time attribute\n");
//     stat = addAttribute(time);
//     McheckErr(stat, "ERROR adding time attribute\n");
//     if (MS::kSuccess != stat) {
//       cout << "Error creating curve.\n";
//     }

//     return MS::kSuccess;
//   }
// //TODO: Motsvarande createMesh i animCube
//   MStatus createStrands() {
//     // delete strand_0; // DO THIS IN DEINITIALIZE PLUGIN
//     MStatus stat;
//     const unsigned num_segments = 10;
//     float strand_length = 10.f;
//     strands.push_back(new Strand(MPoint(0.0, 15.0, 0.0), num_segments,
//                                  MVector(1, 0, 0), 15.f));
//     return MS::kSuccess;
//   }

//   void UpdateHairSystem() {
//   	// TODO: FIRST THING MUST BE TO MOVE THE ROOT NODE!

//     // cout << ">> Number of strands: " << strands.size() << "\n";
//     // Update forces
//     // For every strand of hair h in hair system
//     for (int h = 0; h < strands.size(); h++) {
//       cout << "\t>> Number of points in strand: " << strands[h]->curve_fn.numCVs()
//            << ", now at strand " << h << "\n";
//     	MStatus stat;
//     	strands[h]->forces[0] = MVector(0,0,0); //reset forces
//       // For every point p in strand h (EXCLUDING ROOOOOOOOT)
//       for (int p = 1; p < strands[h]->curve_fn.numCVs(); p++) {
//         strands[h]->forces[p] = MVector(0,0,0); //reset forces
//         cout << "\t\t>> Number of springs that has this point as endpoint: "
//              << strands[h]->springs[p-1].size()  << ", now at point " << p << "\n";
//         // For every spring s whos endpoint is p

//         MPoint current_position;
//         // MVector new_force = MVector(0,0,0);
//         stat = strands[h]->curve_fn.getCV(p, current_position);

//         for (int s = 0; s < strands[h]->springs[p-1].size(); s++) {
//         	// cout << "\t\t\tCurve position = " << current_position << "\n";
// 	        //addInternalForces(temp);
// 	        MPoint prev_position; //will be related to the spring point closer to the root
// 	      	int prev_point_id 	=	strands[h]->springs[p-1][s]->p1;
// 	      	float stiffness 		= strands[h]->springs[p-1][s]->stiffness;
// 	      	float rest_length		= strands[h]->springs[p-1][s]->rest_length;
// 	      	stat = strands[h]->curve_fn.getCV(prev_point_id, prev_position);

// 	        MVector spring_vector = prev_position - current_position;
// 	        float current_length = spring_vector.length();
// 	        MVector spring_force = stiffness * ( current_length / rest_length - 1 ) * (spring_vector / current_length);

// 	        if (prev_point_id != 0) {
// 	        	strands[h]->forces[prev_point_id] += -spring_force;
// 	        }

// 	        strands[h]->forces[p] += spring_force;

// 	        // MPoint updated_position;
// 	        // stat = strands[h]->curve_fn.setCV(p, updated_position);
//           // cout << "\t>> Strand #" << h << "\tNumber of springs attached" <<
//           // strands[i][] << "\n";
//           // 	for ( int spring = 0; spring < strands[i].size();)
//           // 	for ( int spring = 0; spring < strands[i].size();)
//           // (*(strands[h]->springs[p][s]->p2)).x += 2;
//           /*cout << "\t\t\tPoint position = " << *(strands[h]->springs[p][s]->p2)
//                << "\n";
//                */
//         }
//         //addExternalForces(new_force);
//         // Gravitational force
//         strands[h]->forces[p] += MVector(0, -9.82, 0); // assuming mass=1
//         // Damping force
//         strands[h]->forces[p] -= 0.9 * strands[h]->velocities[p];
//       }
//       stat = strands[h]->curve_fn.updateCurve();
//       if (stat != MS::kSuccess) {
//         cerr << "Jaha det här sket sig ju!\n";
//       }
//     }
//     // Velocities and positions
//     // For every strand of hair h in hair system
//     for (int h = 0; h < strands.size(); h++) {
//     	strands[h]->UpdateVelocitiesAndPositions();
//     	/*for (int p = 1; p < strands[h]->springs.size()+1; p++) {

//     	}*/
//     }
//   }

//   virtual MStatus compute(const MPlug& plug, MDataBlock& data) {
//     cout << ">> Computing ..\n";
//     UpdateHairSystem();
//     return MS::kSuccess;
//   }

//   void addInternalForces(MVector& force) {
//   	force += MVector(0, -9.8, 0);
//   }

//   void addExternalForces(MVector& force) {
//   	force += MVector(0, -9.8, 0);
//   }
// };

// // Initialize static variables
// MObject FurrySystem::time;
// //MObject FurrySystem::outputCurve;
// MTypeId FurrySystem::id( 0x80000 );

// // Globals
// /*MStatus initializePlugin(MObject obj) {
//   cout << ">> Initializing plugin .. \n";
//   MStatus   status;
//   MFnPlugin plugin(obj, "Kebaum, Nova, Kingen", "0.1", "Any");
//   status = plugin.registerNode("FurrySystem", FurrySystem::id,
//                                FurrySystem::creator, FurrySystem::initialize);
//   if (!status) {
//     status.perror("registerNode");
//     return status;
//   }
//   return status;
// }

// MStatus uninitializePlugin(MObject obj) {
//   cout << ">> Uninitializing plugin \n";
//   MStatus   status;
//   MFnPlugin plugin(obj);
//   status = plugin.deregisterNode(FurrySystem::id);
//   if (!status) {
//     status.perror("deregisterNode");
//     return status;
//   }
//   return status;
// }*/
