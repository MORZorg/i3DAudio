#include "hrtf_triplet.hpp"
#include "hrtf_data.hpp"

/* #define DEBUG */

namespace hrtf
{
#ifdef DEBUG
	std::ofstream debug("/Users/mauzuc90/debug.txt");
#endif

	Triplet::Triplet( int* _point_indexes )
	{
		point_indexes = _point_indexes;

		calculate_center();
		calculate_H_inverse();
	}

	std::vector<Triplet> Triplet::delaunay_triangulation()
	{
		std::vector<Triplet> result;

		for( int i = 0; i < TRIPLET_SIZE - 2; i++ )
			result.push_back( Triplet( hrtf_triplets[ i ] ) );

		return result;
	}

	void Triplet::calculate_center()
	{
		center[ AZIMUTH ] = 0.0;
		center[ ELEVATION ] = 0.0;
		center[ DISTANCE ] = 0.0;

		for( int i = 0; i < 3; i++ )
		{
			center[ AZIMUTH ] += hrtf_coordinates[ point_indexes[ i ] ][ AZIMUTH ];
			center[ ELEVATION ] += hrtf_coordinates[ point_indexes[ i ] ][ ELEVATION ];
			center[ DISTANCE ] += hrtf_coordinates[ point_indexes[ i ] ][ DISTANCE ];
		}
		center[ AZIMUTH ] /= 3;
		center[ ELEVATION ] /= 3;
		center[ DISTANCE ] /= 3;
	}


	t_float Triplet::calculate_distance( t_float source_coordinates[ 3 ] )
	{
#ifdef DEBUG
		debug << "Distance between: " << std::endl;
		debug << "\t" << center[ 0 ] << ", " << center[ 1 ] << ", " << center[ 2 ] << std::endl;
		debug << "\t" << source_coordinates[ 0 ] << ", " << source_coordinates[ 1 ] << ", " << source_coordinates[ 2 ] << std::endl;
#endif

		/* t_float distance = source_coordinates[ AZIMUTH ] * center[ AZIMUTH ] + source_coordinates[ ELEVATION ] * center[ ELEVATION ]; */
		t_float distance = abs( (t_float) source_coordinates[ AZIMUTH ] - center[ AZIMUTH ] ) + abs( (t_float) source_coordinates[ ELEVATION ] - center[ ELEVATION ] ) + abs( (t_float) source_coordinates[ DISTANCE ] - center[ DISTANCE ] );

#ifdef DEBUG
		debug << "\t\t" << distance << std::endl;
#endif

		return distance;
	}

	t_float* Triplet::coefficients( t_float source_coordinates[ 3 ] )
	{
		t_float* g = (t_float*) malloc( sizeof( t_float ) * 3 );

#ifdef DEBUG
		debug << "Triplet: " << point_indexes[ 0 ] << " " << point_indexes[ 1 ] << " " << point_indexes[ 2 ] << std::endl;
		debug << "Source: " << source_coordinates[ AZIMUTH ] << ", " << source_coordinates[ ELEVATION ] << std::endl;
#endif

		g[ 0 ] = H_inverse[ 0 ][ AZIMUTH ] * source_coordinates[ AZIMUTH ] +
				 H_inverse[ 0 ][ ELEVATION ] * source_coordinates[ ELEVATION ] +
				 H_inverse[ 0 ][ DISTANCE ] * source_coordinates[ DISTANCE ];

		g[ 1 ] = H_inverse[ 1 ][ AZIMUTH ] * source_coordinates[ AZIMUTH ] +
				 H_inverse[ 1 ][ ELEVATION ] * source_coordinates[ ELEVATION ] +
				 H_inverse[ 1 ][ DISTANCE ] * source_coordinates[ DISTANCE ];

		g[ 2 ] = H_inverse[ 2 ][ AZIMUTH ] * source_coordinates[ AZIMUTH ] +
				 H_inverse[ 2 ][ ELEVATION ] * source_coordinates[ ELEVATION ] +
				 H_inverse[ 2 ][ DISTANCE ] * source_coordinates[ DISTANCE ];

#ifdef DEBUG
		debug << "Weights: " << g[ 0 ] << ", " << g[ 1 ] << ", " << g[ 2 ] << std::endl;
#endif

		return g;
	}

	void Triplet::calculate_H_inverse()
	{
#ifdef DEBUG
		debug << "Triplet:" << std::endl;
		debug << point_indexes[ 0 ] << " " << point_indexes[ 1 ] << " " << point_indexes[ 2 ] << std::endl;
#endif

		t_float H[ 3 ][ 3 ] = 
		{
			{
				(t_float) hrtf_coordinates[ point_indexes[ 0 ] ][ 0 ],
				(t_float) hrtf_coordinates[ point_indexes[ 1 ] ][ 0 ],
				(t_float) hrtf_coordinates[ point_indexes[ 2 ] ][ 0 ]
			},
			{
				(t_float) hrtf_coordinates[ point_indexes[ 0 ] ][ 1 ],
				(t_float) hrtf_coordinates[ point_indexes[ 1 ] ][ 1 ],
				(t_float) hrtf_coordinates[ point_indexes[ 2 ] ][ 1 ]
			},
			{
				(t_float) hrtf_coordinates[ point_indexes[ 0 ] ][ 2 ],
				(t_float) hrtf_coordinates[ point_indexes[ 1 ] ][ 2 ],
				(t_float) hrtf_coordinates[ point_indexes[ 2 ] ][ 2 ]
			}
		};

#ifdef DEBUG
		debug << "H:" << std::endl;
		debug << "\t" << H[ 0 ][ 0 ] << ", " << H[ 0 ][ 1 ] << ", " << H[ 0 ][ 2 ] << std::endl;
		debug << "\t" << H[ 1 ][ 0 ] << ", " << H[ 1 ][ 1 ] << ", " << H[ 1 ][ 2 ] << std::endl;
		debug << "\t" << H[ 2 ][ 0 ] << ", " << H[ 2 ][ 1 ] << ", " << H[ 2 ][ 2 ] << std::endl;
#endif

		// Inverse of H
		t_float inv_det_H = H[ 0 ][ 0 ] * ( H[ 1 ][ 1 ] * H[ 2 ][ 2 ] - H[ 2 ][ 1 ] * H[ 1 ][ 2 ] ) -
							H[ 0 ][ 1 ] * ( H[ 2 ][ 2 ] * H[ 1 ][ 0 ] - H[ 2 ][ 1 ] * H[ 2 ][ 0 ] ) +
		  					H[ 0 ][ 2 ] * ( H[ 1 ][ 0 ] * H[ 2 ][ 1 ] - H[ 1 ][ 1 ] * H[ 2 ][ 0 ] );	

#ifdef DEBUG
		debug << "Det H: " << inv_det_H << std::endl;
#endif

		inv_det_H = 1 / inv_det_H;

		H_inverse[ 0 ][ 0 ] =  inv_det_H * ( H[ 1 ][ 1 ] * H[ 2 ][ 2 ] - H[ 2 ][ 1 ] * H[ 1 ][ 2 ] );
		H_inverse[ 0 ][ 1 ] = -inv_det_H * ( H[ 1 ][ 0 ] * H[ 2 ][ 2 ] - H[ 2 ][ 0 ] * H[ 1 ][ 2 ] );
		H_inverse[ 0 ][ 2 ] =  inv_det_H * ( H[ 1 ][ 0 ] * H[ 2 ][ 1 ] - H[ 2 ][ 0 ] * H[ 1 ][ 1 ] );
		H_inverse[ 1 ][ 0 ] = -inv_det_H * ( H[ 0 ][ 1 ] * H[ 2 ][ 2 ] - H[ 2 ][ 1 ] * H[ 0 ][ 2 ] );
		H_inverse[ 1 ][ 1 ] =  inv_det_H * ( H[ 0 ][ 0 ] * H[ 2 ][ 2 ] - H[ 2 ][ 0 ] * H[ 0 ][ 2 ] );
		H_inverse[ 1 ][ 2 ] = -inv_det_H * ( H[ 0 ][ 0 ] * H[ 2 ][ 1 ] - H[ 2 ][ 0 ] * H[ 0 ][ 1 ] );
		H_inverse[ 2 ][ 0 ] =  inv_det_H * ( H[ 0 ][ 1 ] * H[ 1 ][ 2 ] - H[ 1 ][ 1 ] * H[ 0 ][ 2 ] );
		H_inverse[ 2 ][ 1 ] = -inv_det_H * ( H[ 0 ][ 0 ] * H[ 1 ][ 2 ] - H[ 1 ][ 0 ] * H[ 0 ][ 2 ] );
		H_inverse[ 2 ][ 2 ] =  inv_det_H * ( H[ 0 ][ 0 ] * H[ 1 ][ 1 ] - H[ 1 ][ 0 ] * H[ 0 ][ 1 ] );

#ifdef DEBUG
		debug << "Inverse:" << std::endl;
		debug << "\t" << H_inverse[ 0 ][ 0 ] << ", " << H_inverse[ 0 ][ 1 ] << ", " << H_inverse[ 0 ][ 2 ] << std::endl;
		debug << "\t" << H_inverse[ 1 ][ 0 ] << ", " << H_inverse[ 1 ][ 1 ] << ", " << H_inverse[ 1 ][ 2 ] << std::endl;
		debug << "\t" << H_inverse[ 2 ][ 0 ] << ", " << H_inverse[ 2 ][ 1 ] << ", " << H_inverse[ 2 ][ 2 ] << std::endl;
#endif
	}

	t_float** Triplet::calculate_hrtf( t_float g_coefficients[ 3 ], int left_channel, int right_channel )
	{
#ifdef DEBUG
		debug << "Found a valid HRTF!\n";
        debug << "Coefficients: " << g_coefficients[ 0 ] << ", " << g_coefficients[ 1 ] << ", " << g_coefficients[ 2 ] << std::endl;
#endif

		// Creating and instantiating the result matrix
		t_float** result = (t_float**) malloc( sizeof( t_float* ) * 2 );
		for( int i = 0; i < 2; i++ )
			result[ i ] = (t_float*) malloc( sizeof( t_float ) * SAMPLES_LENGTH );

		// Retrieving hrtf values from the indexes
		for( int i = 0; i < SAMPLES_LENGTH; i++ )
			for( int j = 0; j < 3; j++ )
			{
				result[ left_channel ][ i ] += g_coefficients[ j ] * hrtf_impulses[ point_indexes[ j ] ][ left_channel ][ i ];
				result[ right_channel ][ i ] += g_coefficients[ j ] * hrtf_impulses[ point_indexes[ j ] ][ right_channel ][ i ];
			}

		return result;
	}
}
