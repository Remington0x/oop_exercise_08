#ifndef FIGURE_HPP
#define FIGURE_HPP

#include <iostream>
#include <memory>
#include <cmath>

using namespace std;

class Figure {
public:
	virtual void Print() = 0;
	virtual void Write(FILE* out) = 0;
	virtual ~Figure() {}
};

const unsigned long long ID_RECTANGLE = 1;
const unsigned long long ID_QUAD = 2;
const unsigned long long ID_TRIANGLE = 3;

template <typename T>
class Rectangle: public Figure{
	public:
		using type = T;
		pair<T,T> t;
		T a;
		T b;
	Rectangle(){};
	Rectangle(T x,T y,T a1, T b1): t(x,y), a(a1), b(b1){};

	void Print() override {
		pair<double,double> t2(t.first + a, t.second );
		pair<double,double> t3(t.first + a, t.second + b);
		pair<double,double> t4(t.first, t.second + b);
		cout << "Rectangle:" << "{("
		<< t.first << "," << t.second << "), ("
		<< t2.first << "," << t2.second << "), ("
		<< t3.first << "," << t3.second << "), ("
		<< t4.first << "," << t4.second << ")"
		<< "}" << endl;
	}

	void Write(FILE* out) override {
		fwrite(&ID_RECTANGLE, sizeof(unsigned long long), 1, out);
		fwrite(&t.first, sizeof(T), 1, out);
		fwrite(&t.second, sizeof(T), 1, out);
		fwrite(&a, sizeof(T), 1, out);
		fwrite(&b, sizeof(T), 1, out);
	}
};

template <typename T>
class Quad: public Figure{
	public:
		using type = T;
		pair<T,T> t;
		T a;
	Quad(T x,T y,T a1): t(x,y), a(a1){};

	void Print() override {
		//cout << "PRINT IS HERE\n";
		pair<T,T> t2(t.first, t.second + a);
		pair<T,T> t3(t.first + a, t.second + a);
		pair<T,T> t4(t.first + a, t.second);
		cout << "Quad:" << "{("
		<< t.first << "," << t.second << "), ("
		<< t2.first << "," << t2.second << "), ("
		<< t3.first << "," << t3.second << "), ("
		<< t4.first << "," << t4.second << ")"
		<< "}" << endl;
		//cout << "WRITE WORKED SUCCESSFULLY!\n";
	}

	void Write(FILE* out) override {
		//cout << "WRITE IS HERE\n";
		fwrite(&ID_QUAD, sizeof(unsigned long long), 1, out);
		fwrite(&t.first, sizeof(T), 1, out);
		fwrite(&t.second, sizeof(T), 1, out);
		fwrite(&a, sizeof(T), 1, out);
		//cout << "WRITE WORKED SUCCESSFULLY!\n";
	}

};

template <typename T>
class Triangle: public Figure{
	public:
		using type = T;
		pair<T,T> t;
		T a;
	Triangle(T x,T y,T a1): t(x,y), a(a1){};

	void Print() override {
		pair<T,T> t2(t.first + a * 0.5, t.second + a * (std::pow(3, 0.5) / 2));
		pair<T,T> t3(t.first + a, t.second);
		cout << "Triangle:" << "{("
		<< t.first << "," << t.second << "), ("
		<< t2.first << "," << t2.second << "), ("
		<< t3.first << "," << t3.second << ")"
		<< "}" << endl;
	}

	void Write(FILE* out) override {
		fwrite(&ID_TRIANGLE, sizeof(unsigned long long), 1, out);
		fwrite(&t.first, sizeof(T), 1, out);
		fwrite(&t.second, sizeof(T), 1, out);
		fwrite(&a, sizeof(T), 1, out);
	}
};


template<class T, class FIGURE>
class Factory;

template<class T>
class Factory<T, Rectangle<T> > {
public:
	static shared_ptr<Figure> Create() {
		pair <T,T> c;
		T a,b;
		printf("Введите координату нижней левой точки прямоугольника и длины сторон, прилегающих к ней, начиная с горизонтальной\n");
		cin >> c.first >> c.second >> a >> b;
		if((!cin)||(a<=0)||(b<=0)){
			printf("Введены неверные данные, взяты значения по умолчанию: 0 0 1 1\n");
			c.first = 0;
			c.second = 0;
			a = 1;
			b = 1;
		}
		Rectangle<T> * rect = new Rectangle<T>(c.first,c.second, a, b);
		return shared_ptr<Figure>(rect);
	}

	static shared_ptr<Figure> Read(FILE* in) {
		pair <T,T> c;
		T a,b;
		fread(&c.first, sizeof(T), 1, in);
		fread(&c.second, sizeof(T), 1, in);
		fread(&a, sizeof(T), 1, in);
		fread(&b, sizeof(T), 1, in);

		Rectangle<T> * rect = new Rectangle<T>(c.first,c.second, a, b);
		return shared_ptr<Figure>(rect);
	}
};

template<class T>
class Factory<T, Quad<T> > {
public:
	static shared_ptr<Figure> Create() {
		pair <T,T> c;
		T a;
		printf("Введите координату нижней точки квадрата и длину его стороны\n");
		cin >> c.first >> c.second >> a;
		if((!cin)||(a<=0)){
			printf("Введены неверные данные, взяты значения по умолчанию: 0 0 1\n");
			c.first = 0;
			c.second = 0;
			a = 1;
		}
		Quad<T> * quad = new Quad<T>(c.first,c.second, a);
		return shared_ptr<Figure>(quad);
	}

	static shared_ptr<Figure> Read(FILE* in) {
		pair <T,T> c;
		T a,b;
		fread(&c.first, sizeof(T), 1, in);
		fread(&c.second, sizeof(T), 1, in);
		fread(&a, sizeof(T), 1, in);

		Quad<T> * quad = new Quad<T>(c.first, c.second, a);
		return shared_ptr<Figure>(quad);
	}
};

template<class T>
class Factory<T, Triangle<T> > {
public:
	static shared_ptr<Figure> Create() {
		pair <T,T> c;
		T a;
		printf("Введите координату нижней левой точки треугольника и его сторону\n");
		cin >> c.first >> c.second >> a;
		if(a<=0){
			printf("Введены неверные данные, взяты значения по умолчанию 0 0 5\n");
			c.first = 0;
			c.second = 0;
			a = 10;
		}
		Triangle<T> * tr = new Triangle<T>(c.first,c.second, a);
		return shared_ptr<Figure>(tr);
	}

	static shared_ptr<Figure> Read(FILE* in) {
		pair <T,T> c;
		T a;
		fread(&c.first, sizeof(T), 1, in);
		fread(&c.second, sizeof(T), 1, in);
		fread(&a, sizeof(T), 1, in);

		Triangle<T> * tr = new Triangle<T>(c.first,c.second, a);
		return shared_ptr<Figure>(tr);
	}
};

#endif
