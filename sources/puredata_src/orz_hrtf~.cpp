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
	
		// FIXME: check order of azimuth and elevation
		t_float source_position[] = { x->azimuth, x->elevation };
		t_float* g_coefficients;

		// Ordering the triplets and finding the best coefficients
		// Taking the 2% of triangles, in which we except to find the best triplet
		int percentile_offset = (int) ceil( x->dt_triplets.size() * 0.02 );
		
		std::nth_element( x->dt_triplets.begin(),
						 x->dt_triplets.begin() + percentile_offset,
						 x->dt_triplets.end(),
						 [ &source_position ]( Triplet lhs, Triplet rhs ) -> bool
						 {
							return lhs.calculate_distance( source_position ) < rhs.calculate_distance( source_position );
						 } );

		// Cycling until i'll find the best coefficients, knowing that i'm looking in the
		// most probable part first.
		std::vector<Triplet>::iterator it;
		
		for( it = x->dt_triplets.begin(); it < x->dt_triplets.end(); it++ )
		{
			// Calculating the distance
			g_coefficients = it->coefficients( source_position );
			if( g_coefficients[ 0 ] >= 0 && g_coefficients[ 1 ] >= 0 && g_coefficients[ 2 ] >= 0 )
				break;
		}

		// I have my triplet
		// Getting HRTF values of the triplet found
		t_float** current_hrtf = it->calculate_hrtf( source_position );

		// Filtering the "in" source with the newly composed filter
		std::vector<t_float> in_vector( in, in + sizeof( in ) / sizeof( t_float ) );
		std::vector<t_float> hrtf_left( current_hrtf[ 0 ], current_hrtf[ 0 ] + sizeof( current_hrtf[ 0 ] ) / sizeof( t_float ) );
		std::vector<t_float> hrtf_right( current_hrtf[ 1 ], current_hrtf[ 1 ] + sizeof( current_hrtf[ 1 ] ) / sizeof( t_float ) );

		std::vector<t_float> out_left = filter( in_vector, hrtf_left );
		std::vector<t_float> out_right = filter( in_vector, hrtf_right );

		// Assign to outlets
		std::copy( out_left.begin(), out_left.begin() + out_left.size(), out_lx );
		std::copy( out_right.begin(), out_right.begin() + out_right.size(), out_rx );

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
		dsp_add( orz_hrtf_tilde_perform, 5, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[0]->s_n );
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
