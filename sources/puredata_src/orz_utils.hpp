#ifndef __ORZ_UTILS_H__
#define __ORZ_UTILS_H__

#include <cstdlib>
#include <ctime>

#include <vector>

#include "m_pd.h"

int random_int(int, int);
double nth_element(double*, int, int, int);

std::vector<t_float> filter( std::vector<t_float>, std::vector<t_float> );

#endif // __ORZ_UTILS_H__
