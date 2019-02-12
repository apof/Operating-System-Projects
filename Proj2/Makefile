.PHONY: shapes clean

shapes: main.cc ring.cc circle.cc semicircle.cc ellipse.cc square.cc 
	g++ -o shapes main.cc
	g++ -o circle circle.cc
	g++ -o semicircle semicircle.cc
	g++ -o square square.cc
	g++ -o ellipse ellipse.cc
	g++ -o ring ring.cc

clean:
	rm shapes
	rm circle
	rm ring
	rm semicircle
	rm ellipse
	rm square
