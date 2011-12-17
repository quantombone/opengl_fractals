all: 
	g++ -O3 `freetype-config --cflags` `freetype-config --libs` -Ipngwriter-0.5.4/src/ -Lpngwriter-0.5.4/src/ -framework OpenGL -framework GLUT polymath.cpp equation.cpp newton.cpp mycomplex.cpp g.cpp -lpngwriter -lz -lpng -o g


