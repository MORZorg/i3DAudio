#include "orz_hrtf~.hpp"

static t_class* orz_hrtf_tilde_class;

extern "C"
{
	// Class constructor
	// Should receive a sound sample and save it (why?)
	void* orz_hrtf_tilde_new( t_symbol* s, int argc, t_atom* argv )
	{
		t_orz_hrtf_tilde* x = (t_orz_hrtf_tilde*) pd_new( orz_hrtf_tilde_class );

		// Signal as first argument, the other inlets are assigned dynamically
		floatinlet_new( &x->x_obj, &x->x_horizontal );
		floatinlet_new( &x->x_obj, &x->x_depth );

		x->x_out = outlet_new( &x->x_obj, &s_float );

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
			A_GIMME, A_NULL ); // Definition of constructor arguments, terminated by A_NULL

		class_addmethod(
			orz_hrtf_tilde_class, // The class to which the method must be added
			(t_method) orz_hrtf_tilde_perform, // The method's name
			gensym( "perform" ), A_NULL ); // The symbol associated to the method and a number of parameters terminated by A_NULL
	}

	// Perform the filtering on the sample (should be added as first inlet)
	void orz_hrtf_tilde_perform()
	{
		// TODO implement

		return;
	}
}
