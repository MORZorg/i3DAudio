test_linux:
	$(CXX) -DPD -Wall -g -shared --export-dynamic \
        -I/usr/include/pdextended \
        -o orz_test.pd_linux orz_test.c
	
test_mac: orz_test.cpp orz_test.hpp
	$(CC) -DPD -Wall -g -shared -undefined suppress -flat_namespace -arch i386 \
        -I/Applications/Pd-extended.app/Contents/Resources/include/ \
        -o orz_test.pd_darwin $<
	cp orz_test.pd_darwin ~/Library/Pd/
	
.PHONY: clean_mac clean_linux
clean_mac:
	rm ~/Library/Pd/orz_test.pd_darwin

