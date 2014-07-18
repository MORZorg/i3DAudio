#include "orz_hrtf~.hpp"

using namespace std;
using namespace hrtf;

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
		t_orz_hrtf_tilde* x = (t_orz_hrtf_tilde*) w[ 1 ];
		
		t_float* inlet_signal = (t_float*) w[ 2 ];

		t_float* outlet_right = (t_float*) w[ 3 ];
		t_float* outlet_left = (t_float*) w[ 4 ];

		int blocksize = (int) w[ 5 ];
	
		// FIXME: check order of azimuth and elevation
		t_float source_position[] = { x->azimuth, x->elevation };
		t_float* g_coefficients;

		// Ordering the triplets and finding the best coefficients
		// Taking the 2% of triangles, in which we except to find the best triplet
		int percentile_offset = (int) ceil( x->dt_triplets.size() * 0.02 );
		
		nth_element( x->dt_triplets.begin(),
						 x->dt_triplets.begin() + percentile_offset,
						 x->dt_triplets.end(),
						 [ &source_position ]( Triplet lhs, Triplet rhs ) -> bool
						 {
							return lhs.calculate_distance( source_position ) < rhs.calculate_distance( source_position );
						 } );

		// Cycling until i'll find the best coefficients, knowing that i'm looking in the
		// most probable part first.
		vector<Triplet>::iterator it;
		
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

		// Assigning the interpolated HRTF to the outlets
		/* outlet_left = current_hrtf[ LEFT_CHANNEL ]; */
		/* outlet_right = current_hrtf[ RIGHT_CHANNEL ]; */

		// Filtering the "in" source with the newly composed filter
		int signal_size = sizeof( inlet_signal ) / sizeof( t_float );
		int kernel_size = sizeof( current_hrtf[ LEFT_CHANNEL ] ) / sizeof( t_float );

		t_float filtered_temp[ 2 ];
		t_float signal_temp[ 2 ];

		// Filter from left to right, using past samples when data are not available
		// for the first chunk, the past samples are all 0
		for( int i = 0; i < signal_size; i++ )
		{
			filtered_temp[ LEFT_CHANNEL ] = 0;
			filtered_temp[ RIGHT_CHANNEL ] = 0;

			for( int j = 0; j < kernel_size; j++ )
			{
				if( i - j > 0 )
				{
					signal_temp[ LEFT_CHANNEL ] = inlet_signal[ i - j ];
					signal_temp[ RIGHT_CHANNEL ] = inlet_signal[ i - j ];
				}
				else
				{
					signal_temp[ LEFT_CHANNEL ] = x->previous_sample[ LEFT_CHANNEL ][ i - j + SAMPLES_LENGTH ];
					signal_temp[ RIGHT_CHANNEL ] = x->previous_sample[ RIGHT_CHANNEL ][ i - j + SAMPLES_LENGTH ];
				}

				filtered_temp[ LEFT_CHANNEL ] += signal_temp[ LEFT_CHANNEL ] * current_hrtf[ LEFT_CHANNEL ][ j ];  
				filtered_temp[ RIGHT_CHANNEL ] += signal_temp[ RIGHT_CHANNEL ] * current_hrtf[ RIGHT_CHANNEL ][ j ];  
			}

			x->previous_sample[ LEFT_CHANNEL ][ i ] = inlet_signal[ i ];
			x->previous_sample[ RIGHT_CHANNEL ][ i ] = inlet_signal[ i ];

			// Assign to outlets
			*outlet_left++ = filtered_temp[ LEFT_CHANNEL ];
			*outlet_right++ = filtered_temp[ RIGHT_CHANNEL ];
		}

		// Returns a pointer to the end of the parameter vector
		return w + 6;
	}

	static void orz_hrtf_tilde_dsp( t_orz_hrtf_tilde* x, t_signal** sp )
	{
		post("Called setup");
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
		post("called new");
		t_orz_hrtf_tilde* x = (t_orz_hrtf_tilde*) pd_new( orz_hrtf_tilde_class );

		post("channels");
		// Registering variables as outlets and inlets
		x->left_channel = outlet_new( &x->x_obj, gensym( "signal" ) );
		x->right_channel = outlet_new( &x->x_obj, gensym( "signal" ) );

		post("inlets");
		floatinlet_new( &x->x_obj, &x->azimuth );
		floatinlet_new( &x->x_obj, &x->elevation );

		post("assign inlets");
		// Assigning the inlets
		x->azimuth = (t_float) _azimuth;
		x->elevation = (t_float) _elevation;

		post("triangulation");
		// The hrtf database is already loaded in the hrtf_data.hpp header
		// Creating the triplets
		x->dt_triplets = Triplet::delaunay_triangulation();

		post("initialization");
		// Initializating the last sample at 0
		for( int i = 0; i < SAMPLES_LENGTH; i++ )
		{
			x->previous_sample[ LEFT_CHANNEL ][ i ] = 0;
			x->previous_sample[ RIGHT_CHANNEL ][ i ] = 0;
		}

		post("return");
		return (void*) x;
	}

	// Called by PD when the library orz_hrtf_tilde~ is loaded
	void orz_hrtf_tilde_setup()
	{
		post("Called setup");
		orz_hrtf_tilde_class = class_new(
			gensym( "orz_hrtf~" ), // Created symbol to use in pd
			(t_newmethod) orz_hrtf_tilde_new, // Constructor method
			0, // Destructor method (0 = don't care)
			sizeof( t_orz_hrtf_tilde ), // Size of the class
			CLASS_DEFAULT, // Graphical representation of the object
			A_DEFFLOAT, A_DEFFLOAT, A_NULL ); // Definition of constructor arguments, terminated by A_NULL

		post("something");
		// The f variable is a dummy one contained in the data space, used to replace the signal inlet (the first) with a float inlet if the signal is missing
		CLASS_MAINSIGNALIN( orz_hrtf_tilde_class, t_orz_hrtf_tilde, f );

		post("method");
		class_addmethod(
			orz_hrtf_tilde_class, // The class to which the method must be added
			(t_method) orz_hrtf_tilde_dsp, // The method's name - the symbol must be dsp
			gensym( "dsp" ), A_NULL ); // The symbol associated to the method and a number of parameters terminated by A_NULL
	}
}
