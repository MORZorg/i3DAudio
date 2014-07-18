#ifndef __ORZ_HRTF_TILDE__
#define __ORZ_HRTF_TILDE__

#include <cmath>
#include <algorithm>

#include "m_pd.h"
#include "hrtf_triplet.hpp"
#include "kemar_parameters.hpp"

typedef struct _orz_hrtf_tilde
{
	t_object x_obj;
	t_outlet *left_channel;
	t_outlet *right_channel;

	t_float azimuth;
	t_float elevation;

	t_float f;

	std::vector<hrtf::Triplet> dt_triplets;

	// We must keep track of the past samples to start a new convolution!
	t_float previous_sample[ 2 ][ SAMPLES_LENGTH ]; // Contains the last processed sample
} t_orz_hrtf_tilde;

extern "C"
{
	// Execution methods
	static t_int* orz_hrtf_tilde_perform( t_int* );
	static void orz_hrtf_tilde_dsp( t_orz_hrtf_tilde*, t_signal** );

	// PureData dictated methods
	static void* orz_hrtf_tilde_new( t_floatarg, t_floatarg );
	void orz_hrtf_tilde_setup();
}

#endif  // __ORZ_HRTF_TILDE__
