#include "orz_utils.hpp"

/* Private functions */

/**
 * @brief Partitions the given array in two parts, using A[p] as pivot.
 * Values with index smaller than the returned one are smaller or equal than
 * the pivot, values with greater index are greater than the pivot.
 * Taken from the slides of the course `Algoritmi e Strutture Dati`.
 *
 * @param A The array to be partitioned.
 * @param p The first index of the partition.
 * @param r The last index of the partition.
 *
 * @return The new index of A[p].
 */
int partition(double* A, int p, int r)
{
  double x = A[p];
  int i = p-1;
  int j = r+1;
  double temp;

  while (1)
  {
    while (A[--j] > x);
    while (A[++i] < x);

    if (i < j)
    {
      temp = A[i];
      A[i] = A[j];
      A[j] = temp;
    }
    else
      return j;
  }
}

/**
 * @brief Partitions the given array in two parts, using a random pivot.
 * Values with index smaller than the returned one are smaller or equal than
 * the pivot, values with greater index are greater than the pivot.
 * Taken from the slides of the course `Algoritmi e Strutture Dati`.
 *
 * @param A The array to be partitioned.
 * @param p The first index of the partition.
 * @param r The last index of the partition.
 *
 * @return The new index of the value used as pivot.
 */
int randomized_partition(double* A, int p, int r)
{
  int i = random_int(p, r);
  double temp;

  temp = A[i];
  A[i] = A[p];
  A[p] = temp;

  return partition(A, p, r);
}

/**
 * @brief Returns the i-th value of the sorted version of the given array.
 * This function doesn't fully sort the array, but ensures that the values with
 * index smaller than i will be smaller than the returned value and values with
 * greater index will be greater.
 * Taken from the slides of the course `Algoritmi e Strutture Dati`.
 *
 * @param A The input array.
 * @param p The first index to analyze.
 * @param r The last index to analyze.
 * @param i The index of the value to be put in the right position.
 *
 * @return The i-th value of the sorted version of the given array.
 */
double randomized_select(double* A, int p, int r, int i)
{
  if (p == r)
    return A[p];

  int q = randomized_partition(A, p, r);
  int k = q-p+1;
  if (i <= k)
    return randomized_select(A, p, q, i);
  else
    return randomized_select(A, q+1, r, i-k);
}

/* Public functions */

int random_int(int min, int max)
{
  // This method should not be the best (should break uniformity) but seems to
  // work fine.
  return (rand() % (max-min+1)) + min;
}

double nth_element(double* source, int in_size, int n, int n_error)
{
  return randomized_select(source, 0, in_size-1, n);
}

std::vector<t_float> filter( std::vector<t_float> signal, std::vector<t_float> kernel )
{
	std::vector<t_float> filtered( signal.size() );

	// Filter from left to right
	for( int i = 0; i < signal.size(); i++ )
	{
		filtered[ i ] = 0;

		for( int j = 0; j < kernel.size(); j++ )
		{
			filtered[ i ] += signal[ i - j ] * kernel[ j ];  
		}
	}

	return filtered;
}
