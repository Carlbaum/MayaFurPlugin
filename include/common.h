#include <cmath>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MFloatArray.h>
#include <maya/MFnNurbsCurve.h>
#include <vector>
#include <maya/MIOStream.h>
#include <maya/MSimple.h>

struct Spring {
	MPoint *p1;
	MPoint *p2;
	float rest_length;
	float stiffness; // could be named spring_constant
	// Contructors
	Spring( MPoint *point_1, MPoint *point_2, float rest_length = 1.f, float spring_constant = 1.f ) :
					p1(point_1),
					p2(point_2),
					rest_length(rest_length),
					stiffness(spring_constant) {}

	// Public methods
/*	void CalculateSpringForce() {
		MVector relative_vector = *p2 - *p1;
		float current_length = relative_vector.length();
		MVector force = -stiffness * std::abs( rest_length - current_length ) * ( relative_vector / current_length );
		p1->AddForce( force );
		p2->AddForce( -force );
	}
*/
};

struct Strand {
  MPointArray point_positions;
  MDoubleArray knot_sequences;
  // MFloatArray masses;
  // std::vector<Spring*> edge_springs;
  const unsigned degree;
  std::vector<std::vector<Spring*> > springs; //this should probably be used

  std::vector<MVector> velocities;
  std::vector<MVector> forces;
  MFnNurbsCurve curve;


	// Constructors
	Strand( MPoint root_point,
					int segments, // number of edge springs
					MVector direction,
					float strand_length = 1.0f,
					const unsigned deg = 1 ) :
							degree(deg)
	{

		cout << "Strand constructor!\n";
		// Create root
		MVector delta_position = ( strand_length / segments ) * direction;
		cout << "Strand constructor2!\n";

		// Create the root mass point
		point_positions.append( root_point );

		knot_sequences.append( (double)0 );
		for (int i = 1 ; i < segments + 1 ; i++) {
			cout << "\tStrand constructor loop " << i << "\n";
			cout << "\t\tLength" << point_positions.length() << "\n";
			point_positions.append( root_point + i * delta_position );
			knot_sequences.append( (double)i );
			cout << "\t\tPos x: " << point_positions[i] << "\n";
			// edge_springs.push_back( new Spring( &point_positions[i-1], &point_positions[i] ) );
			// springs[i].push_back( new Spring( &point_positions[i-1], &point_positions[i] ) );
			std::vector<Spring*> springs_attached_to_point;
			springs_attached_to_point.push_back( new Spring( &point_positions[i-1], &point_positions[i] ) );
			springs.push_back( springs_attached_to_point );
		}
		cout << "Strand constructor, EXIT!\n";
	}

	~Strand() {
		cout << "Strand destructor is called! Strand contains " << springs.size() << " points\n";
		// for each point
		for (int p = 0; p < springs.size(); ++p) {
			cout << "\tPoint #" << p;
			cout << ", has " << springs[p].size() << " springs\n";
			// for each spring
			while(!springs[p].empty()) {
        delete springs[p].back();
        springs[p].pop_back();
        cout << "\t\tSpring has been deleted! It now has " << springs[p].size() << " springs\n";
    	}
			/*for (int s = springs[p].size()-1; s >= 0 ; --s) {
				cout << "\t\tSpring #" << s << " is deleted";
				delete springs[p][s];
				springs[p][s].pop_back();
				cout << ", now has " << springs[p].size() << " springs\n";
			}*/
		}
	}
};

// TODO: destructors for all structs using new