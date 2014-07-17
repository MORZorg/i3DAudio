#ifndef HRTF_TRIPLET
#define HRTF_TRIPLET

#include "m_pd.h"
#include "hrtf_data.hpp"

class Triplet
{
	private:
		int point_indexes[ 3 ];
	
	public:
		Triplet( int* );

		// NOTE doesn't calculate the actual distance, omits the arccos
		// as suggested in the paper.
		// As result, the values must be ordered in descending order instead
		// of ascending one
		t_float calculate_distance( t_int* );
		f_float** calculate_H_inverse();
		t_float* coefficients( t_int* );
};

#endif
