#include "hrtf_triplet.hpp"

Triplet::Triplet( int* _point_indexes )
{
	point_indexes = _point_indexes;

	calculated_inverse = false;
	H_inverse = NULL;
}

std::vector<Triplet> Triplet::delaunay_triangulation()
{
	// We calculate the convex hull of the points as intermediate step, by adding an extra
	// coordinate to each point which value is x^2 + y^2.
	// NOTE our points use azimuth and elevation as coordinates, but this obviously
	// doesn't matter.
	// Compute the artificial "z" of every point
	int z[ SAMPLES_NUMBER ];

	for( int i = 0; i < SAMPLES_NUMBER; i++ )
		z[ i ] = hrtf_coordinates[ i ][ AZIMUTH ] * hrtf_coordinates[ i ][ AZIMUTH ] + hrtf_coordinates[ i ][ ELEVATION ] * hrtf_coordinates[ i ][ ELEVATION ];

	std::vector<Triplet> result;
	int free_spot = 0;

	for( int i = 0; i < SAMPLES_NUMBER - 2; i++ )
	{
		for( int j = i + 1; j < SAMPLES_NUMBER; j++ )
		{
			for( int k = i + 1; k < SAMPLES_NUMBER; k++ )
			{
				if( j == k )
					continue;

				// ( xn, yn, zn ) is the cross product of the vectors i -> j and j -> k, perpendicular
				// to the triangle determined by i, j and k
				int xn = ( hrtf_coordinates[ j ][ ELEVATION ] - hrtf_coordinates[ i ][ ELEVATION ] ) * ( z[ k ] * z[ i ] ) -
					( hrtf_coordinates[ k ][ ELEVATION ] - hrtf_coordinates[ i ][ ELEVATION ] ) * ( z[ j ] * z[ i ] );

				int yn = ( hrtf_coordinates[ k ][ AZIMUTH ] - hrtf_coordinates[ i ][ AZIMUTH ] ) * ( z[ j ] * z[ i ] ) -
					( hrtf_coordinates[ j ][ AZIMUTH ] - hrtf_coordinates[ i ][ AZIMUTH ] ) * ( z[ k ] * z[ i ] );

				int zn = ( hrtf_coordinates[ j ][ AZIMUTH ] - hrtf_coordinates[ i ][ AZIMUTH ] ) * ( hrtf_coordinates[ k ][ ELEVATION ] - hrtf_coordinates[ i ][ ELEVATION ] ) -
					( hrtf_coordinates[ k ][ AZIMUTH ] - hrtf_coordinates[ i ][ AZIMUTH ] ) * ( hrtf_coordinates[ j ][ ELEVATION ] - hrtf_coordinates[ i ][ ELEVATION ] );

				// Because of convex stuff, this flag permits to determine whether or not the triangle
				// has to be inserted in the Delaunay triangulation
				bool flag;

				if( ( flag = ( zn < 0 ? 1 : 0 ) != 0 ) )
					for( int m = 0; m < SAMPLES_NUMBER; m++ )
						flag = flag && ( (
							( hrtf_coordinates[ m ][ AZIMUTH ] - hrtf_coordinates[ i ][ AZIMUTH ] ) * xn +
							( hrtf_coordinates[ m ][ ELEVATION ] - hrtf_coordinates[ i ][ ELEVATION ] ) * yn +
							( z[ m ] - z[ i ] ) * zn ) <= 0 );

				if( !flag )
					continue;

				int _point_indexes[ 3 ] = { i, j, k };

				result.push_back( Triplet( _point_indexes ) );
			}
		}
	}

	return result;
}

t_float Triplet::calculate_distance( t_float source_coordinates[ 2 ] )
{
	t_float center[ 2 ] = { 0, 0 };
	
	for( int i = 0; i < 3; i++ )
	{
		center[ AZIMUTH ] += (t_float)( hrtf_coordinates[ point_indexes[ i ] ][ AZIMUTH ] ) / 3;
		center[ ELEVATION ] += (t_float)( hrtf_coordinates[ point_indexes[ i ] ][ ELEVATION ] ) / 3;
	}

	// Or -?
	t_float distance = source_coordinates[ AZIMUTH ] * center[ ELEVATION ] + source_coordinates[ ELEVATION ] * center[ AZIMUTH ]; 

	return distance;
}

t_float* Triplet::coefficients( t_float source_coordinates[ 2 ] )
{
	if( !calculated_inverse )
		calculate_H_inverse();

	t_float* g = (t_float*) malloc( sizeof( t_float ) * 3 );

	g[ 0 ] = H_inverse[ 0 ][ 0 ] * source_coordinates[ AZIMUTH ] + H_inverse[ 0 ][ 1 ] * source_coordinates[ ELEVATION ];
	g[ 1 ] = H_inverse[ 1 ][ 0 ] * source_coordinates[ AZIMUTH ] + H_inverse[ 1 ][ 1 ] * source_coordinates[ ELEVATION ];
	g[ 2 ] = H_inverse[ 2 ][ 0 ] * source_coordinates[ AZIMUTH ] + H_inverse[ 2 ][ 1 ] * source_coordinates[ ELEVATION ];

	return g;
}

void Triplet::calculate_H_inverse()
{
	if( !calculated_inverse )
	{
		t_int A[ 2 ][ 3 ] = {
			{
				hrtf_coordinates[ point_indexes[ 0 ] ][ AZIMUTH ],
				hrtf_coordinates[ point_indexes[ 1 ] ][ AZIMUTH ],
				hrtf_coordinates[ point_indexes[ 2 ] ][ AZIMUTH ]
			}, {
				hrtf_coordinates[ point_indexes[ 0 ] ][ ELEVATION ],
				hrtf_coordinates[ point_indexes[ 1 ] ][ ELEVATION ],
				hrtf_coordinates[ point_indexes[ 2 ] ][ ELEVATION ]
			} };

		t_int A_t[ 3 ][ 2 ] = {
			{ A[ 0 ][ 0 ], A[ 1 ][ 0 ] },
			{ A[ 0 ][ 1 ], A[ 1 ][ 1 ] },
			{ A[ 0 ][ 2 ], A[ 1 ][ 2 ] } };

		// A_t * A
		t_int B[ 3 ][ 3 ] = {
			{
				A_t[ 0 ][ 0 ] * A[ 0 ][ 0 ] + A_t[ 0 ][ 1 ] * A[ 1 ][ 0 ],
				A_t[ 0 ][ 0 ] * A[ 0 ][ 1 ] + A_t[ 0 ][ 1 ] * A[ 1 ][ 1 ],
				A_t[ 0 ][ 0 ] * A[ 0 ][ 2 ] + A_t[ 0 ][ 1 ] * A[ 1 ][ 2 ]
			}, {
				A_t[ 1 ][ 0 ] * A[ 0 ][ 0 ] + A_t[ 1 ][ 1 ] * A[ 1 ][ 0 ],
				A_t[ 1 ][ 0 ] * A[ 0 ][ 1 ] + A_t[ 1 ][ 1 ] * A[ 1 ][ 1 ],
				A_t[ 1 ][ 0 ] * A[ 0 ][ 2 ] + A_t[ 1 ][ 1 ] * A[ 1 ][ 2 ]
			}, {
				A_t[ 2 ][ 0 ] * A[ 0 ][ 0 ] + A_t[ 2 ][ 1 ] * A[ 1 ][ 0 ],
				A_t[ 2 ][ 0 ] * A[ 0 ][ 1 ] + A_t[ 2 ][ 1 ] * A[ 1 ][ 1 ],
				A_t[ 2 ][ 0 ] * A[ 0 ][ 2 ] + A_t[ 2 ][ 1 ] * A[ 1 ][ 2 ]
			} };

		// Inverse of B
		t_float inv_det_B = B[ 0 ][ 0 ] * ( B[ 1 ][ 1 ] * B[ 2 ][ 2 ] - B[ 2 ][ 1 ] * B[ 1 ][ 2 ] ) -
			B[ 0 ][ 1 ] * ( B[ 2 ][ 2 ] * B[ 1 ][ 0 ] - B[ 2 ][ 1 ] * B[ 2 ][ 0 ] ) +
			B[ 0 ][ 2 ] * ( B[ 1 ][ 0 ] * B[ 2 ][ 1 ] - B[ 1 ][ 1 ] * B[ 2 ][ 0 ] );

		inv_det_B = 1 / inv_det_B;

		t_float B_inv[ 3 ][ 3 ] = {
			{
				inv_det_B * ( B[ 1 ][ 1 ] * B[ 2 ][ 2 ] - B[ 2 ][ 1 ] * B[ 1 ][ 2 ] ),
				-1 * inv_det_B * ( B[ 1 ][ 0 ] * B[ 2 ][ 2 ] - B[ 2 ][ 0 ] * B[ 1 ][ 2 ] ),
				inv_det_B * ( B[ 1 ][ 0 ] * B[ 2 ][ 1 ] - B[ 2 ][ 0 ] * B[ 1 ][ 1 ] )
			}, {
				-1 * inv_det_B * ( B[ 0 ][ 1 ] * B[ 2 ][ 2 ] - B[ 2 ][ 1 ] * B[ 0 ][ 2 ] ),
				inv_det_B * ( B[ 0 ][ 0 ] * B[ 2 ][ 2 ] - B[ 2 ][ 0 ] * B[ 0 ][ 2 ] ),
				-1 * inv_det_B * ( B[ 0 ][ 0 ] * B[ 2 ][ 1 ] - B[ 2 ][ 0 ] * B[ 0 ][ 1 ] )
			}, {
				inv_det_B * ( B[ 0 ][ 1 ] * B[ 1 ][ 2 ] - B[ 1 ][ 1 ] * B[ 0 ][ 2 ] ),
				-1 * inv_det_B * ( B[ 0 ][ 0 ] * B[ 1 ][ 2 ] - B[ 1 ][ 0 ] * B[ 0 ][ 2 ] ),
				inv_det_B * ( B[ 0 ][ 0 ] * B[ 1 ][ 1 ] - B[ 1 ][ 0 ] * B[ 0 ][ 1 ] )
			} };

		// B_inverse * A_t
		H_inverse = (t_float**) malloc( sizeof( t_float ) * 6 );
		
		H_inverse[ 0 ][ 0 ] = B_inv[ 0 ][ 0 ] * A_t[ 0 ][ 0 ] + B_inv[ 0 ][ 1 ] * A_t[ 1 ][ 0 ] + B_inv[ 0 ][ 2 ] * A_t[ 2 ][ 0 ];
		H_inverse[ 0 ][ 1 ] = B_inv[ 0 ][ 0 ] * A_t[ 0 ][ 1 ] + B_inv[ 0 ][ 1 ] * A_t[ 1 ][ 1 ] + B_inv[ 0 ][ 2 ] * A_t[ 2 ][ 1 ];

		H_inverse[ 1 ][ 0 ] = B_inv[ 1 ][ 0 ] * A_t[ 0 ][ 0 ] + B_inv[ 1 ][ 1 ] * A_t[ 1 ][ 0 ] + B_inv[ 1 ][ 2 ] * A_t[ 2 ][ 0 ];
		H_inverse[ 1 ][ 1 ] = B_inv[ 1 ][ 0 ] * A_t[ 0 ][ 1 ] + B_inv[ 1 ][ 1 ] * A_t[ 1 ][ 1 ] + B_inv[ 1 ][ 2 ] * A_t[ 2 ][ 1 ];

		H_inverse[ 2 ][ 0 ] = B_inv[ 2 ][ 0 ] * A_t[ 0 ][ 0 ] + B_inv[ 2 ][ 1 ] * A_t[ 1 ][ 0 ] + B_inv[ 2 ][ 2 ] * A_t[ 2 ][ 0 ];
		H_inverse[ 2 ][ 1 ] = B_inv[ 2 ][ 0 ] * A_t[ 0 ][ 1 ] + B_inv[ 2 ][ 1 ] * A_t[ 1 ][ 1 ] + B_inv[ 2 ][ 2 ] * A_t[ 2 ][ 1 ];

		calculated_inverse = true;
	}
}

t_float** Triplet::calculate_hrtf( t_float source_coordinates[ 2 ] )
{
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
