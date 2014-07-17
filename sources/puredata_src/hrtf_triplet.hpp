#ifndef HRTF_TRIPLET
#define HRTF_TRIPLET

#include <cstdio>
#include <cstdlib>
#include <vector>

#include "m_pd.h"
#include "hrtf_data.hpp"

class Triplet
{
	private:
		int* point_indexes;

		t_float** H_inverse;
		bool calculated_inverse;
		
		// Calculates the pseudoinverse of the matrix A, which has the points' coordinates
		// as columns. The inverse is ( A_t * A ) ^ -1 * A_t
		void calculate_H_inverse();
	
	public:
		Triplet( int* );

		// Calculates the Delaunay triangulation of all the points in hrtf_data.hpp
		// We use the method that computes the convex hull as intermediate step
		// http://stackoverflow.com/questions/5825089/how-does-this-code-for-delaunay-triangulation-work
		static std::vector<Triplet> delaunay_triangulation();

		// NOTE doesn't calculate the actual distance, omits the arccos
		// as suggested in the paper.
		// As result, the values must be ordered in descending order instead
		// of ascending one
		t_float calculate_distance( t_int* );
		// Calculates the g coefficents, either positive or negative (the check must
		// be performed on the returned value )
		t_float* coefficients( t_int* );
};

#endif
