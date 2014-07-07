playme: main.c hrtf.o
	$(CC) -Os -o $@ -Wall $^ `sdl2-config --cflags --libs`

hrtf.o: hrtf.c hrtf.h
	$(CC) -Os -g -c -Wall $<

.PHONY: clean
clean:
	rm -f \
	  playme \
	  hrtf.o
	rm -rf *.dSYM
