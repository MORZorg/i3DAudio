#include "orz_hrtf~.hpp"

static t_class* orz_hrtf_tilde_class;

extern "C"
{
	// Selects the most accurate triplet and performs the filtering of the sample,
	// putting the result on the outlet channels
	// The parameters are given by orz_hrtf_tilde_dsp
	static t_int* orz_hrtf_tilde_perform( t_int* w )
	{
		// Retrieve the parameters, casting them to the correct
		// pointer type
		t_orz_hrtf_tilde* x = (t_orz_hrtf_tilde*) w[1];
		t_float* in = (t_float*) w[2];
		t_float* out_rx = (t_float*) w[3];
		t_float* out_lx = (t_float*) w[4];
		int blocksize = (int) w[5];

		// Ordering the triplets and finding the best coefficents

		// Returns a pointer to the end of the parameter vector
		return w + 6;
	}

	static void orz_hrtf_tilde_dsp( t_orz_hrtf_tilde* x, t_signal** sp )
	{
		// Add a callback function that actually performs what has to be done
		// The function has 5 parameters, the class and the data obtained by the signal
		// All of this stuff is given by puredata automatically, it seems
		// The data from the signal are the input samples, the output left channel, the
		// output right channel, the size of the blocks
		dsp_add( orz_hrtf_tilde_perform, 5, x,  sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[0]->s_n );
	}

	// Class constructor
	// Should receive a sound sample and save it (why?)
	static void* orz_hrtf_tilde_new( t_floatarg _azimuth, t_floatarg _elevation )
	{
		t_orz_hrtf_tilde* x = (t_orz_hrtf_tilde*) pd_new( orz_hrtf_tilde_class );

		// Registering variables as outlets and inlets
		x->left_channel = outlet_new( &x->x_obj, gensym( "signal" ) );
		x->right_channel = outlet_new( &x->x_obj, gensym( "signal" ) );

		floatinlet_new( &x->x_obj, &x->azimuth );
		floatinlet_new( &x->x_obj, &x->elevation );

		// Assigning the inlets
		x->azimuth = (t_float) _azimuth;
		x->elevation = (t_float) _elevation;

		// The hrtf database is already loaded in the hrtf_data.hpp header
		// Creating the triplets
		x->dt_triplets = Triplet::delaunay_triangulation();

		return (void*) x;
	}

	// Called by PD when the library orz_hrtf_tilde~ is loaded
	void orz_hrtf_tilde_setup()
	{
		orz_hrtf_tilde_class = class_new(
			gensym( "orz_hrtf~" ), // Created symbol to use in pd
			(t_newmethod) orz_hrtf_tilde_new, // Constructor method
			0, // Destructor method (0 = don't care)
			sizeof( t_orz_hrtf_tilde ), // Size of the class
			CLASS_DEFAULT, // Graphical representation of the object
			A_DEFFLOAT, A_DEFFLOAT, A_NULL ); // Definition of constructor arguments, terminated by A_NULL

		// The f variable is a dummy one contained in the data space, used to replace the signal inlet (the first) with a float inlet if the signal is missing
		CLASS_MAINSIGNALIN( orz_hrtf_tilde_class, t_orz_hrtf_tilde, f );

		class_addmethod(
			orz_hrtf_tilde_class, // The class to which the method must be added
			(t_method) orz_hrtf_tilde_dsp, // The method's name - the symbol must be dsp
			gensym( "dsp" ), A_NULL ); // The symbol associated to the method and a number of parameters terminated by A_NULL
	}
}
