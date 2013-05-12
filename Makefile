all:
	make clean
	make leds
clean:
	rm -f leds

leds:
	g++ -L/home/jake/builds/rtmidi-2.0.0 -lrtmidi leds.cc -o leds
