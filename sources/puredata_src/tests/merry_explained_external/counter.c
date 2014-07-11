#include "m_pd.h"

// New "class" and its data space
// t_class is defined inside m_pd.h
static t_class* counter_class;

typedef struct _counter
{
	// Must exist and be the first element of the class
	// used by pure data
	t_object x_obj;  
	// Definition of custom attributes of the class
	t_int i_count;  
} t_counter;  
 
// Method called when the class receives a bang
void counter_bang( t_counter* x )
{
	// Internal types of PD. To ouput something on stderr
	// the post method must be called.
	// It's the same as printf but with a \n added automatically
	// at the end
	t_float f = x->i_count;
	x->i_count++;
	outlet_float( x->x_obj.ob_outlet, f );
}
 
// Class' constructor
void* counter_new( t_floatarg f )
{
	t_counter *x = (t_counter*) pd_new( counter_class );

	x->i_count = f;
	outlet_new( &x->x_obj, &s_float );

	return (void *) x;
}
 
// Called by PD when the library counter is loaded
// Creates an instance of the class and adds a bang
void counter_setup( void )
{
	counter_class = class_new( gensym( "counter" ),
		(t_newmethod) counter_new,
		0, sizeof( t_counter ),
		CLASS_DEFAULT,
		// This is a custom constructor argument for the field
		// i_count
		A_DEFFLOAT, 0 );

	class_addbang( counter_class, counter_bang );
}
