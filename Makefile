CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

cfd: *.cpp *.hpp
	g++ $(CFLAGS) -o cfd *.cpp $(LDFLAGS)

.PHONY: test clean

test: cfd
	./cfd

clean:
	rm -f cfd
