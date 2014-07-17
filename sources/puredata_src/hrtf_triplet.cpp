#include "hrtf_triplet.hpp"

Triplet::Triplet( int* _point_indexes )
{
	point_indexes = _point_indexes;

	calculated_inverse = false;
	H_inverse = NULL;
}

t_float Triplet::calculate_distance( t_int source_coordinates[ 2 ] )
{
	t_float center[ 2 ] = { 0, 0 }
	
	for( int i = 0; i < 3; i++ )
	{
		center[ 0 ] += (t_float)( hrtf_coordinates[ point_indexes[ i ][ 0 ] ] ) / 3;
		center[ 1 ] += (t_float)( hrtf_coordinates[ point_indexes[ i ][ 1 ] ] ) / 3;
	}

	// Or -?
	t_float distance = source_coordinates[ 0 ] * center[ 1 ]
		+ source_coordinates[ 1 ] * center[ 0 ]; 

	return distance;
}

t_float* Triplet::coefficients( t_int source_coordinates[ 2 ] )
{
	if( !calculated_inverse )
		calculate_H_inverse();

	f_float* g = malloc( sizeof( t_float ) * 3 );

	g = {
		H_inverse[ 0 ][ 0 ] * source_coordinates[ 0 ] + H_inverse[ 0 ][ 1 ] * source_coordinates[ 1 ],
		H_inverse[ 1 ][ 0 ] * source_coordinates[ 0 ] + H_inverse[ 1 ][ 1 ] * source_coordinates[ 1 ],
		H_inverse[ 2 ][ 0 ] * source_coordinates[ 0 ] + H_inverse[ 2 ][ 1 ] * source_coordinates[ 1 ]
	};

	return g;
}

void Triplet::calculate_H_inverse()
{
	if( !calculated_inverse )
	{
		t_int A[ 2 ][ 3 ] = {
			{
				hrtf_coordinates[ point_indexes[ 0 ] ][ 0 ],
				hrtf_coordinates[ point_indexes[ 1 ] ][ 0 ],
				hrtf_coordinates[ point_indexes[ 2 ] ][ 0 ]
			}, {
				hrtf_coordinates[ point_indexes[ 0 ] ][ 1 ],
				hrtf_coordinates[ point_indexes[ 1 ] ][ 1 ],
				hrtf_coordinates[ point_indexes[ 2 ] ][ 1 ]
			} };

		t_int A_t[ 3 ][ 2 ] = {
			{ A[ 0 ][ 0 ], A[ 1 ][ 0 ] }
			{ A[ 0 ][ 1 ], A[ 1 ][ 1 ] }
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

		t_float inv_det_B = 1 / inv_det_B;

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
		H_inverse = malloc( sizeof( t_float ) * 6 );
		
		H_inverse = {
			{
				B_inv[ 0 ][ 0 ] * A_t[ 0 ][ 0 ] + B_inv[ 0 ][ 1 ] * A_t[ 1 ][ 0 ] + B_inv[ 0 ][ 2 ] * A_t[ 2 ][ 0 ],
				B_inv[ 0 ][ 0 ] * A_t[ 0 ][ 1 ] + B_inv[ 0 ][ 1 ] * A_t[ 1 ][ 1 ] + B_inv[ 0 ][ 2 ] * A_t[ 2 ][ 1 ]
			}, {
				B_inv[ 1 ][ 0 ] * A_t[ 0 ][ 0 ] + B_inv[ 1 ][ 1 ] * A_t[ 1 ][ 0 ] + B_inv[ 1 ][ 2 ] * A_t[ 2 ][ 0 ],
				B_inv[ 1 ][ 0 ] * A_t[ 0 ][ 1 ] + B_inv[ 1 ][ 1 ] * A_t[ 1 ][ 1 ] + B_inv[ 1 ][ 2 ] * A_t[ 2 ][ 1 ]
			}, {
				B_inv[ 2 ][ 0 ] * A_t[ 0 ][ 0 ] + B_inv[ 2 ][ 1 ] * A_t[ 1 ][ 0 ] + B_inv[ 2 ][ 2 ] * A_t[ 2 ][ 0 ],
				B_inv[ 2 ][ 0 ] * A_t[ 0 ][ 1 ] + B_inv[ 2 ][ 1 ] * A_t[ 1 ][ 1 ] + B_inv[ 2 ][ 2 ] * A_t[ 2 ][ 1 ]
			} };

		calculated_inverse = true;
	}
}
