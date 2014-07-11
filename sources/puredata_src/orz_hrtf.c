#include "orz_hrtf.h"

static t_class* orz_hrtf_class;

// Method called when the class receives a bang
/* void orz_hrtf_bang( t_orz_hrtf* x )
{
 	post( "Hello ORZ!" );
} */

// Class constructor
// Should receive a sound sample and save it
void* orz_hrtf_new( t_symbol* s, int argc, t_atom* argv )
{
	t_orz_hrtf* x = (t_orz_hrtf*) pd_new( orz_hrtf_class );

	// Signal as first argument, the other inlets are assigned dynamically
	floatinlet_new( &x->x_obj, &x->x_orizontal );
	floatinlet_new( &x->x_obj, &x->x_depth );

	x->x_out = outlet_new( &x->x_obj, &s_float );

	return (void*) x;
}

// Called by PD when the library orz_hrtf is loaded
void orz_hrtf_setup()
{
	orz_hrtf_class = class_new( gensym( "orz_hrtf" ), // Created symbol to use in pd
		(t_newmethod) orz_hrtf_new, // Constructor method
		0, // Destructor method (0 = don't care)
		sizeof( t_orz_hrtf ), // Size of the class
		CLASS_DEFAULT, // Graphical representation of the object
		A_GIMME, 0 ); // Definition of constructor arguments, terminated by 0

	class_addmethod( orz_hrtf_class, // The class to which the method must be added
		(t_method) orz_hrtf_perform, // The method's name
		gensym( "perform" ), 0); // The symbol associated to the method and a number of parameters terminated by 0
}

// Perform the filtering on the sample (should be added as first inlet)
void orz_hrtf_perform()
{
	// TODO implement
	
	return;
}
