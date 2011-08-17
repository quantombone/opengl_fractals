all: 
	g++ -O3 generator.cpp -o generator #&& ./generator
	g++ -O3 -framework OpenGL -framework GLUT newton.cpp other.cpp g.cpp -o g
	


