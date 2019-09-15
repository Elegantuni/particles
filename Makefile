particles1:
	g++ -o particles1 main.cpp `pkg-config --cflags --libs allegro`

clean:
	rm -f particles1

