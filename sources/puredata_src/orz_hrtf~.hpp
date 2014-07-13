#include "m_pd.h"

typedef struct _orz_hrtf_tilde
{
	t_object x_obj;

	t_sample f_signal;

	// FIXME Sample input missing
	// Inlets representing the position of the audio source
	t_float x_horizontal, x_depth;

	// Outlet containing the sample modified with the hrtf
	t_outlet* x_out;
} t_orz_hrtf_tilde;

extern "C"
{
	// PureData dictated methods
	/* void orz_hrtf_bang( t_orz_hrtf* ); */
	void* orz_hrtf_tilde_new( t_symbol*, int, t_atom* );
	void orz_hrtf_tilde_setup();

	// Support methods for hrtf implementation
	void orz_hrtf_tilde_perform();
}
