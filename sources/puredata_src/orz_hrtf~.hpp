#ifndef __ORZ_HRTF_TILDE__
#define __ORZ_HRTF_TILDE__

#include <cmath>
#include <algorithm>

#include "m_pd.h"
#include "hrtf_triplet.hpp"
#include "kemar_parameters.hpp"

// Suppresses the "unused function" warnings in XCode.
// This is because the functions are used, just not directly by it.
#pragma clang diagnostic ignored "-Wunused-function"

typedef struct _orz_hrtf_tilde
{
	t_object x_obj;
	t_outlet *left_channel;
	t_outlet *right_channel;

	t_float azimuth;
	t_float elevation;

    t_float cross_coef[ 8192 ]; 
    t_float conv_buffer[ SAMPLES_LENGTH ];
    
    t_float coefficients[ 3 ];
    t_float current_hrtf[ 2 ][ SAMPLES_LENGTH ];
    t_float previous_hrtf[ 2 ][ SAMPLES_LENGTH ];

    t_int buffer_pin;

	std::vector<hrtf::Triplet> dt_triplets;
    
    // Dummy float for DSP
	t_float f;
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
