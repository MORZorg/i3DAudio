orz_hrtf_linux:
	$(CC) -DPD -Wall -g -shared --export-dynamic \
        -I/usr/include/pdextended \
        -o orz_hrtf.pd_linux orz_hrtf.c

orz_hrtf_mac: orz_hrtf.c orz_hrtf.h
	$(CC) -DPD -Wall -g -shared -undefined suppress -flat_namespace -arch i386 \
        -I/Applications/Pd-extended.app/Contents/Resources/include/ \
        -o orz_hrtf.pd_darwin $<
	cp orz_hrtf.pd_darwin ~/Library/Pd/

test_linux:
	$(CC) -DPD -Wall -g -shared --export-dynamic \
        -I/usr/include/pdextended \
        -o orz_test.pd_linux orz_test.c
	
test_mac: orz_test.c orz_test.h
	$(CC) -DPD -Wall -g -shared -undefined suppress -flat_namespace -arch i386 \
        -I/Applications/Pd-extended.app/Contents/Resources/include/ \
        -o orz_test.pd_darwin $<
	cp orz_test.pd_darwin ~/Library/Pd/
