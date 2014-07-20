#ifndef HRTF_TRIPLET
#define HRTF_TRIPLET

#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "m_pd.h"

#include "kemar_parameters.hpp"

namespace hrtf
{
	class Triplet
	{
		private:
			int* point_indexes;

			t_float center[ 3 ];
			t_float H_inverse[ 3 ][ 3 ];

			void calculate_center();
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
			t_float calculate_distance( t_float* );
			// Calculates the g coefficents, either positive or negative (the check must
			// be performed on the returned value )
			t_float* coefficients( t_float* );

			// Calculates the HRTF of the triplet, knowing the source
			t_float** calculate_hrtf( t_float[ 3 ], int, int );

            std::string to_string();
	};
}

#endif
