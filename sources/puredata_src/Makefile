orz_hrtf_linux:
	$(CXX) -DPD -Wall -g -shared --export-dynamic \
        -I/usr/include/pdextended \
        -o orz_hrtf.pd_linux orz_hrtf.c

orz_hrtf_tilde_mac: orz_hrtf~.cpp orz_hrtf~.hpp
	$(CXX) -DPD -Wall -g -shared -undefined suppress -flat_namespace -arch i386 \
        -I/Applications/Pd-extended.app/Contents/Resources/include/ \
        -o orz_hrtf~.pd_darwin $<
	cp orz_hrtf~.pd_darwin ~/Library/Pd/
