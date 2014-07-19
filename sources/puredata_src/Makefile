CFLAGS=-O3 -std=c++0x

orz_hrtf_linux: orz_hrtf~.cpp hrtf_triplet.o
	$(CXX) $(CFLAGS) -DPD -Wall -g -fPIC -shared \
		-I/usr/include/pdextended \
		-o orz_hrtf~.pd_linux $^
	cp orz_hrtf~.pd_linux ~/pd-externals/

orz_hrtf_tilde_mac: orz_hrtf~.cpp hrtf_triplet.o hrtf_data.hpp
	$(CXX) $(CFLAGS) -DPD -Wall -g -shared -undefined suppress -flat_namespace -arch i386 \
        -I/Applications/Pd-extended.app/Contents/Resources/include/ \
		$^
	mv a.out orz_hrtf~.pd_darwin
	cp orz_hrtf~.pd_darwin ~/Library/Pd/

hrtf_triplet.o: hrtf_triplet.cpp hrtf_triplet.hpp hrtf_data.hpp
	$(CXX) $(CFLAGS) -Os -DPD -Wall -g -fPIC -arch i386 \
        -I/Applications/Pd-extended.app/Contents/Resources/include/ \
        -c $<

.PHONY: clean
clean:
	rm -rf *.o *.gch *.dSYM \
		orz_hrtf~.pd_linux \
		or_hrtf~.pd_darwin
