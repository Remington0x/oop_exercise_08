#include<iostream>
#include <string>
#include <memory>
#include <vector>
#include <thread>
#include <functional>
#include"figure.hpp"
#include"pub_sub.hpp"

using namespace std;

Developer < shared_ptr<Figure> > d;

FILE *file = NULL;

string mrandom() {
	unsigned int size = rand() % 10;
	string s = "";
	char c;
	for (int i = 0; i < size; ++i) {
		c = rand() % 26 + 'a';
		s += c;
	}
	return s;
}

void threadFunction() {

	function<void(shared_ptr<Figure> fig)> Print_fig = [](shared_ptr<Figure> fig) {
		//cout << "THREAD 2: came to PRINT\n";
		//cout << "THREAD 2: figure a is ";
		fig->Print();
		//cout << "THREAD 2: PRINT overcome\n";
	};

	function<void(shared_ptr<Figure> fig)> Write_fig = [](shared_ptr<Figure> fig) {
		//cout << "THREAD 2: came to WRITE\n";
		fig->Write(file);
		//cout << "THREAD 2: WRITE overcome\n";
	};

	vector<function<void(shared_ptr<Figure> fig)>> functions({Print_fig, Write_fig});

	bool a = true;
	while(a) {
		if (!d.Empty()) {

			if (d.Front() == NULL) {
				break;
			}

			file = fopen((mrandom()).c_str(), "wb");

			while (!d.Empty()) {
				shared_ptr<Figure> f_3 = d.Front();
				//cout << "THREAD 2: came to lambdas\n";
				for (auto func : functions) {
					func(f_3);
				}
				//cout << "THREAD 2: lambdas overcome\n";
				d.Pop();
			}
			fclose(file);
		}
	}
}

int main(int argc, char** argv) {

	srand(time(NULL));

	unsigned long long size = stoi(string(argv[1]));
	if (size <= 0) {
		printf("Неверный размер буффера\n");
		return -1;
	}

	thread thread_2(threadFunction);
	vector< shared_ptr<Figure> > figures;
	unsigned long long id;

	printf("Введите 1, чтобы добавить прямоугольник\n");
	printf("Введите 2, чтобы добавить квадрат\n");
	printf("Введите 3, чтобы добавить треугольник\n");

	while (cin >> id) {

		if (!cin || id <= 0 || id > 3) {
			printf("Вы ввели неверную команду\n");
			thread_2.join();
			return -1;
		}

		if (id == ID_RECTANGLE) {
			figures.push_back(Factory<double, Rectangle<double>>:: Create());
		} else {
			if (id == ID_QUAD) {
				figures.push_back(Factory<double, Quad<double>>:: Create());
			} else {
				if (id == ID_TRIANGLE) {
					figures.push_back(Factory<double, Triangle<double>>:: Create());
				}
			}
		}

		//cout << "THREAD 1: CAME TO BUFF LIMIT\n";
		if (figures.size() == size) {
			for (int i = 0; i < figures.size(); ++i) {
				d.Add(figures[i]);
			}
			//cout << "THREAD 1: throwed all figures to queue, waiting...\n";
			while (!d.Empty()) {}
			//cout << "THREAD 1: waiting done, clearing...\n";
			figures.clear();
			//cout << "THREAD 1: clear success!\n";
		}

		printf("Введите 1, чтобы добавить прямоугольник\n");
		printf("Введите 2, чтобы добавить квадрат\n");
		printf("Введите 3, чтобы добавить треугольник\n");

	}

	for (int i = 0; i < figures.size(); ++i) {
		d.Add(figures[i]);
	}
	figures.clear();
	shared_ptr<Figure> null = NULL;
	d.Add(null);
	thread_2.join();

}
