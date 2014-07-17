#include "m_pd.h"
#include "hrtf_triplet.hpp"
#include <cmath>
#include <algorithm>

typedef struct _orz_hrtf_tilde
{
	t_object x_obj;
	t_outlet *left_channel;
	t_outlet *right_channel;

	t_float azimuth;
	t_float elevation;

	t_float f;

	std::vector<Triplet> dt_triplets;
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
