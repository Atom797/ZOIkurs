#include <highgui.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <imgproc.hpp>
#include <highgui.hpp>
#include <cv.h>

using namespace cv;
using namespace std;

constexpr int m_size = 5; // основной размер маски
constexpr int L5t[] = { 1, 4, 6, 4, 1 }; // транспонированные (повернутые на 90 градусов)
constexpr int E5t[] = { -1, -2, 0, 2, 1 }; // матрицы Лавса
constexpr int S5t[] = { -1, 0, 2,0, -1 };
constexpr int R5t[] = { 1, -4, 6, -4, 1 };

vector<int> ffmask(const int* a, const int* b) { // Составление маски 5х5, используя двух различных векторов
	vector<int> mask;

	for (int i = 0; i < m_size; i++) {
		for (int j = 0; j < m_size; j++)
			mask.push_back(a[i] * b[j]); // вычисление умножением одной матрицы на другую (нетранспонированная на транспонированную) 
	}
	return mask;
}

int main(int argc, char* argv[])
{
	cout << "Enter image name:";
	string pictureName;
	cin >> pictureName;
	Mat image;
	image = imread(pictureName); // ввод изображения
	Mat imageBlur, imageLowInt;
	Mat masked[15]; // массив для изображений, подвергнутых очередной маске

	medianBlur(image, imageBlur, 15); // сглаживание медианным фильтром (среднее значение пикселя в окне, размером 15х15)
	subtract(image, imageBlur, imageLowInt); // вычитание сглаженного изображения из основного - получение низкоинтенсивного изображения
	// (малое значение на пикселях)
	imshow("image", image); // вывод изображения
	imshow("blured", imageBlur);
	imshow("low intencity", imageLowInt);

	vector<int> mask = ffmask(L5t, E5t); // получение маски, используя определенные вектора Лавса
	filter2D(imageLowInt, masked[0], -1, mask); // применение маски путем свёртки
	mask = ffmask(E5t, L5t);
	filter2D(imageLowInt, masked[1], -1, mask);
	add(masked[1], masked[0], masked[1]);
	imshow("1.L5E5/E5L5", masked[1]);

	mask = ffmask(L5t, R5t);
	filter2D(imageLowInt, masked[2], -1, mask);
	mask = ffmask(R5t, L5t);
	filter2D(imageLowInt, masked[3], -1, mask);
	add(masked[2], masked[3], masked[3]);
	imshow("2.L5R5/R5L5", masked[3]);

	mask = ffmask(E5t, S5t);
	filter2D(imageLowInt, masked[4], -1, mask);
	mask = ffmask(S5t, E5t);
	filter2D(imageLowInt, masked[5], -1, mask);
	add(masked[4], masked[5], masked[5]);
	imshow("3.ES5/S5E5", masked[5]);

	mask = ffmask(S5t, S5t);
	filter2D(imageLowInt, masked[6], -1, mask);
	imshow("4.S5S5", masked[6]);

	mask = ffmask(R5t, R5t);
	filter2D(imageLowInt, masked[7], -1, mask);
	imshow("5.R5R5", masked[7]);

	mask = ffmask(L5t, S5t);
	filter2D(imageLowInt, masked[8], -1, mask);
	mask = ffmask(S5t, L5t);
	filter2D(imageLowInt, masked[9], -1, mask);
	add(masked[8], masked[9], masked[9]);
	imshow("6.L5S5/S5L5", masked[9]);

	mask = ffmask(E5t, E5t);
	filter2D(imageLowInt, masked[10], -1, mask);
	imshow("7.E5E5", masked[10]);

	mask = ffmask(E5t, R5t);
	filter2D(imageLowInt, masked[11], -1, mask);
	mask = ffmask(R5t, E5t);
	filter2D(imageLowInt, masked[12], -1, mask);
	add(masked[11], masked[12], masked[12]);
	imshow("8.E5R5/R5E5", masked[12]);

	mask = ffmask(S5t, R5t);
	filter2D(imageLowInt, masked[13], -1, mask);
	mask = ffmask(R5t, S5t);
	filter2D(imageLowInt, masked[14], -1, mask);
	add(masked[13], masked[14], masked[14]);
	imshow("9.S5R5/R5S5", masked[14]);

	waitKey(0);
	destroyAllWindows();
	return 0;
}