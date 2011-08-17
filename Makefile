all: 
	g++ -O3 -framework OpenGL -framework GLUT polymath.cpp equation.cpp newton.cpp mycomplex.cpp g.cpp -o g
	


