#include "hrtf_triplet.hpp"
#include "hrtf_data.hpp"

#define DEBUG

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
		center = { 0, 0 };
		for( int i = 0; i < 3; i++ )
		{
			center[ AZIMUTH ] += hrtf_coordinates[ point_indexes[ i ] ][ AZIMUTH ];
			center[ ELEVATION ] += hrtf_coordinates[ point_indexes[ i ] ][ ELEVATION ];
		}
		center[ AZIMUTH ] /= 3;
		center[ ELEVATION ] /= 3;
	}


	t_float Triplet::calculate_distance( t_float source_coordinates[ 2 ] )
	{
#ifdef DEBUG
		debug << "Distance between: " << std::endl;
		debug << "\t" << center[ 0 ] << ", " << center[ 1 ] << std::endl;
		debug << "\t" << source_coordinates[ 0 ] << ", " << source_coordinates[ 1 ] << std::endl;
#endif

		/* t_float distance = source_coordinates[ AZIMUTH ] * center[ AZIMUTH ] + source_coordinates[ ELEVATION ] * center[ ELEVATION ]; */
		t_float distance = abs( (t_float) source_coordinates[ AZIMUTH ] - center[ AZIMUTH ] ) + abs( (t_float) source_coordinates[ ELEVATION ] - center[ ELEVATION ] );

#ifdef DEBUG
		debug << "\t\t" << distance << std::endl;
#endif

		return distance;
	}

	t_float* Triplet::coefficients( t_float source_coordinates[ 2 ] )
	{
		t_float* g = (t_float*) malloc( sizeof( t_float ) * 3 );

#ifdef DEBUG
		debug << "Source: " << source_coordinates[ AZIMUTH ] << ", " << source_coordinates[ ELEVATION ] << std::endl;
#endif

		g[ 0 ] = H_inverse[ 0 ][ AZIMUTH ] * source_coordinates[ AZIMUTH ] + H_inverse[ 0 ][ ELEVATION ] * source_coordinates[ ELEVATION ];
		g[ 1 ] = H_inverse[ 1 ][ AZIMUTH ] * source_coordinates[ AZIMUTH ] + H_inverse[ 1 ][ ELEVATION ] * source_coordinates[ ELEVATION ];
		g[ 2 ] = H_inverse[ 2 ][ AZIMUTH ] * source_coordinates[ AZIMUTH ] + H_inverse[ 2 ][ ELEVATION ] * source_coordinates[ ELEVATION ];

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

		t_float A[ 2 ][ 3 ] = {
			{
				(t_float) hrtf_coordinates[ point_indexes[ 0 ] ][ AZIMUTH ],
				(t_float) hrtf_coordinates[ point_indexes[ 1 ] ][ AZIMUTH ],
				(t_float) hrtf_coordinates[ point_indexes[ 2 ] ][ AZIMUTH ]
			}, {
				(t_float) hrtf_coordinates[ point_indexes[ 0 ] ][ ELEVATION ],
				(t_float) hrtf_coordinates[ point_indexes[ 1 ] ][ ELEVATION ],
				(t_float) hrtf_coordinates[ point_indexes[ 2 ] ][ ELEVATION ]
			} };

#ifdef DEBUG
		debug << "A:" << std::endl;
		debug << "\t" << A[ 0 ][ 0 ] << ", " << A[ 0 ][ 1 ] << ", " << A[ 0 ][ 2 ] << std::endl;
		debug << "\t" << A[ 1 ][ 0 ] << ", " << A[ 1 ][ 1 ] << ", " << A[ 1 ][ 2 ] << std::endl;
#endif

		t_float A_t[ 3 ][ 2 ] = {
			{ A[ 0 ][ 0 ], A[ 1 ][ 0 ] },
			{ A[ 0 ][ 1 ], A[ 1 ][ 1 ] },
			{ A[ 0 ][ 2 ], A[ 1 ][ 2 ] } };

		// A * A_t
		t_float B[ 2 ][ 2 ] = {
			{
				A[ 0 ][ 0 ] * A_t[ 0 ][ 0 ] + A[ 0 ][ 1 ] * A_t[ 1 ][ 0 ] + A[ 0 ][ 2 ] * A_t[ 2 ][ 0 ],
				A[ 0 ][ 0 ] * A_t[ 0 ][ 1 ] + A[ 0 ][ 1 ] * A_t[ 1 ][ 1 ] + A[ 0 ][ 2 ] * A_t[ 2 ][ 1 ],
			}, {                                               
				A[ 1 ][ 0 ] * A_t[ 0 ][ 0 ] + A[ 1 ][ 1 ] * A_t[ 1 ][ 0 ] + A[ 1 ][ 2 ] * A_t[ 2 ][ 0 ],
				A[ 1 ][ 0 ] * A_t[ 0 ][ 1 ] + A[ 1 ][ 1 ] * A_t[ 1 ][ 1 ] + A[ 1 ][ 2 ] * A_t[ 2 ][ 1 ],
			} };

#ifdef DEBUG
		debug << "B:" << std::endl;
		debug << "\t" << B[ 0 ][ 0 ] << ", " << B[ 0 ][ 1 ] << std::endl;
		debug << "\t" << B[ 1 ][ 0 ] << ", " << B[ 1 ][ 1 ] << std::endl;
#endif

		// Inverse of B
		t_float inv_det_B = B[ 0 ][ 0 ] * B[ 1 ][ 1 ] - B[ 1 ][ 0 ] * B[ 0 ][ 1 ];

#ifdef DEBUG
		debug << "Det B: " << inv_det_B << std::endl;
#endif

		inv_det_B = 1 / inv_det_B;

		t_float B_inv[ 2 ][ 2 ] = {
			{
				inv_det_B * B[ 1 ][ 1 ],
				-1 * inv_det_B * B[ 1 ][ 0 ]
			}, {
				-1 * inv_det_B * B[ 0 ][ 1 ],
				inv_det_B * B[ 0 ][ 0 ]
			} };

		// A_t * B_inverse
		H_inverse[ 0 ][ 0 ] = A_t[ 0 ][ 0 ] * B_inv[ 0 ][ 0 ] + A_t[ 0 ][ 1 ] * B_inv[ 1 ][ 0 ];
		H_inverse[ 0 ][ 1 ] = A_t[ 0 ][ 0 ] * B_inv[ 0 ][ 1 ] + A_t[ 0 ][ 1 ] * B_inv[ 1 ][ 1 ];

		H_inverse[ 1 ][ 0 ] = A_t[ 1 ][ 0 ] * B_inv[ 0 ][ 0 ] + A_t[ 1 ][ 1 ] * B_inv[ 1 ][ 0 ];
		H_inverse[ 1 ][ 1 ] = A_t[ 1 ][ 0 ] * B_inv[ 0 ][ 1 ] + A_t[ 1 ][ 1 ] * B_inv[ 1 ][ 1 ];

		H_inverse[ 2 ][ 0 ] = A_t[ 2 ][ 0 ] * B_inv[ 0 ][ 0 ] + A_t[ 2 ][ 1 ] * B_inv[ 1 ][ 0 ];
		H_inverse[ 2 ][ 1 ] = A_t[ 2 ][ 0 ] * B_inv[ 0 ][ 1 ] + A_t[ 2 ][ 1 ] * B_inv[ 1 ][ 1 ];

#ifdef DEBUG
		debug << "Inverse:" << std::endl;
		debug << "\t" << H_inverse[ 0 ][ 0 ] << ", " << H_inverse[ 0 ][ 1 ] << std::endl;
		debug << "\t" << H_inverse[ 1 ][ 0 ] << ", " << H_inverse[ 1 ][ 1 ] << std::endl;
		debug << "\t" << H_inverse[ 2 ][ 0 ] << ", " << H_inverse[ 2 ][ 1 ] << std::endl;
#endif
	}

	t_float** Triplet::calculate_hrtf( t_float source_coordinates[ 2 ] )
	{
#ifdef DEBUG
		debug << "Found a valid HRTF!\n";
#endif

		// Creating and instantiating the result matrix
		t_float** result = (t_float**) malloc( sizeof( t_float* ) * 2 );
		for( int i = 0; i < 2; i++ )
			result[ i ] = (t_float*) malloc( sizeof( t_float ) * SAMPLES_LENGTH );

		// Retrieving hrtf values from the indexes
		for( int i = 0; i < 2; i++ )
			for( int j = 0; j < SAMPLES_LENGTH; j++ )
				for( int k = 0; k < 3; k++ )
					result[ i ][ j ] += hrtf_impulses[ point_indexes[ k ] ][ i ][ j ];

		return result;
	}
}
