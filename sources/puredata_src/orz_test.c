#include "orz_test.h"

static t_class* orz_test_class;

typedef struct _orz_test
{
	t_object x_obj;
} t_orz_test;

void orz_test_bang( t_orz_test* x )
{
	post( "Hello ORZ!" );
}

void* orz_test_new()
{
	t_orz_test* x = (t_orz_test*) pd_new( orz_test_class );

	return (void*) x;
}

void orz_test_setup()
{
	orz_test_class = class_new( gensym( "orz_test" ),
								(t_newmethod) orz_test_new,
								0, sizeof( t_orz_test ),
								CLASS_DEFAULT, 0 );

	class_addbang( orz_test_class, orz_test_bang );
}
