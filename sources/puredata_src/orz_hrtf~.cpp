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
	
		t_float source_position[ 3 ];
		source_position[ AZIMUTH ] = x->azimuth;
		source_position[ ELEVATION ] = x->elevation;
		source_position[ DISTANCE ] = 1;

		// FIXME: should we also have to check for azimuth bounds? => € [ 0, 360 ]

		// Symmetry of HRTF between channels
		int left_channel;
		int right_channel;
		if( x->azimuth <= 180 )
		{
			left_channel = 0;
			right_channel = 1;
		}
		else
		{ 
			left_channel = 1;
			right_channel = 0;
			x->azimuth = 360.0 - x->azimuth;
		}

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

		// Cycling until I'll find the best coefficients, knowing that I'm looking in the
		// most probable part first.
		vector<Triplet>::iterator it;
		t_float* g_coefficients;
		for( it = x->dt_triplets.begin(); it < x->dt_triplets.end(); it++ )
		{
			// Calculating the coefficients
			g_coefficients = it->coefficients( source_position );
			if( g_coefficients[ 0 ] >= 0 && g_coefficients[ 1 ] >= 0 && g_coefficients[ 2 ] >= 0 )
				break;
		}

		// I have my triplet
		// Getting HRTF values of the triplet found
		if( it < x->dt_triplets.end() )
		{
#ifdef DEBUG
			post( "Found a triplet!" );
#endif

			// Normalizing the coefficients
			t_float g_sum = 0;
			for( int i = 0; i < 3; i++ )
				g_sum += g_coefficients[ i ];
			for( int i = 0; i < 3; i++ )
				g_coefficients[ i ] = g_coefficients[ i ] / g_sum;
			
			x->current_hrtf = it->calculate_hrtf( g_coefficients );

			// Assigning the interpolated HRTF to the outlets
			/* outlet_left = current_hrtf[ LEFT_CHANNEL ]; */
			/* outlet_right = current_hrtf[ RIGHT_CHANNEL ]; */

			// Filtering the "in" source with the newly composed filter

			t_float filtered_temp[ 2 ];
			int blockscale = 8192 / blocksize;

			// Convolution
            while (blocksize--)
            {
				filtered_temp[ left_channel ] = 0;
				filtered_temp[ right_channel ] = 0;

				x->conv_buffer[ x->buffer_pin ] = *(inlet_signal++);
				unsigned scaled_blocksize = blocksize * blockscale;
				unsigned blocksize_delta = 8191 - scaled_blocksize;
				for ( int i = 0; i < SAMPLES_LENGTH; i++ )
				{ 
					filtered_temp[ left_channel ] += ( x->previous_hrtf[ left_channel ][ i ] * x->cross_coef[ blocksize_delta ] + 
													   x->previous_hrtf[ left_channel ][ i ] * x->cross_coef[ scaled_blocksize ] ) * 
						x->conv_buffer[ ( x->buffer_pin - i ) & ( SAMPLES_LENGTH - 1 ) ];
					filtered_temp[ right_channel ] += ( x->previous_hrtf[ right_channel ][ i ] * x->cross_coef[ blocksize_delta ] + 
														x->current_hrtf[ right_channel ][ i ] * x->cross_coef[ scaled_blocksize ] ) * 
						x->conv_buffer[ ( x->buffer_pin - i ) & ( SAMPLES_LENGTH - 1 ) ];

					x->previous_hrtf[ left_channel ][ i ] = x->current_hrtf[ left_channel ][ i ];
					x->previous_hrtf[ right_channel ][ i ] = x->current_hrtf[ right_channel ][ i ];
				}	
				x->buffer_pin = (x->buffer_pin + 1) & ( SAMPLES_LENGTH - 1 );

				*outlet_left++ = filtered_temp[ left_channel ];
				*outlet_right++ = filtered_temp[ right_channel ];
            }

		}

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

		// Initializating the last sample at 0
		for( int i = 0; i < SAMPLES_LENGTH; i++ )
		{
			x->conv_buffer[ i ] = 0;
			x->previous_hrtf[ 0 ][ i ] = 0;
			x->previous_hrtf[ 1 ][ i ] = 0;
		}

		// Coefficients to change smoothly the filter between consecutive blocks
		for( int i = 0; i < 8192 ; i++ )
		{	
			x->cross_coef[i] = 1.0 * i / 8192;
		}
		

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
