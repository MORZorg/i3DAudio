orz_hrtf_linux: orz_hrtf~.cpp orz_hrtf~.hpp hrtf_triplet.o
	$(CXX) -DPD -Wall -g -fPIC -shared -I/usr/include/pdextended -o orz_hrtf.pd_linux $<

orz_hrtf_tilde_mac: orz_hrtf~.cpp orz_hrtf~.hpp
	$(CXX) -DPD -Wall -g -shared -undefined suppress -flat_namespace -arch i386 \
        -I/Applications/Pd-extended.app/Contents/Resources/include/ \
        -o orz_hrtf~.pd_darwin $<
	cp orz_hrtf~.pd_darwin ~/Library/Pd/

hrtf_triplet.o: hrtf_triplet.cpp hrtf_triplet.hpp orz_utils.o
	$(CXX) -Os -g -c $<

orz_utils.o: orz_utils.cpp orz_utils.hpp
	$(CXX) -Os -g -c $<
