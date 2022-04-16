#pragma warning(disable : 4996) 
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <iomanip>
#include <SFML/Graphics.hpp>
using namespace std;

class Data {
public:
	double x;
	double y;
};

vector<double> Gauss(vector<vector<double>> mass, int n, vector<double> & X) 
{
	for (int i = 0; i < n; ++i) 
	{
		double temp = mass[i][i];
		for (int j = n; j >= i; --j) 
			mass[i][j] /= temp;

		for (int j = i + 1; j < n; ++j) 
		{
			temp = mass[j][i];
			for (int k = n; k >= i; --k) 
				mass[j][k] -= temp * mass[i][k];
		}
	}

	X[n - 1] = mass[n - 1][n];
	for (int i = n - 2; i >= 0; --i) 
	{
		X[i] = mass[i][n];
		for (int j = i + 1; j < n; ++j) 
			X[i] -= mass[i][j] * X[j];
	}

	return X;
}

string toStr(double x)
{
	string s = to_string(x);
	if (s[s.size() - 1] == '0')
		for (size_t i = s.size() - 1; s[i] == '0'; i--)
			s.erase(i, 1);

	if (s[s.size() - 1] == ',')
		s.erase(s.size() - 1, 1);
	return s;
}

template<typename T>
ostream& operator<<(ostream& out, vector<T> v)
{
	for (auto &i : v)
		out << setw(10) << i << " ";
	out << "\n";
	return out;
}

template<typename T>
ostream& operator<<(ostream& out, vector<vector<T>> v)
{
	for (auto &i : v)
	{
		for (auto &j : i)
			out << setw(10) << j << " ";
		out << "\n";
	}
	return out;
}
// - - - - - -- - - - - - -- - - -- - - - - -- - -- - - - - - -
double x;
int I = -1;

vector<double> setD(vector<Data> data, int n)
{
	vector<double> d(n - 2);
	for (int i = 1; i < n - 1; ++i)
		d[i - 1] = (data[i + 1].y - data[i].y) / (data[i + 1].x - data[i].x) - (data[i].y - data[i - 1].y) / (data[i].x - data[i - 1].x);
	return d;
}

vector<vector<double>> setCM(vector<double> d, vector<vector<double>> c, int n)
{
	vector<vector<double>> CM(n - 2, vector<double>(n - 1, 0));

	for (int i = 0; i < n - 2; ++i)
	{
		for (int j = 0; j < n - 1; ++j)
		{
			if (j == n - 2)
				CM[i][j] = d[i];
			else
				CM[i][j] = c[i][j];
		}
	}
	return CM;
}

vector<Data> setData(int n)
{
	vector<Data> data(n);
	for (int i = 0; i < n; ++i)
	{
		cout << "x[" << i << "] = ";
		cin >> data[i].x;
		cout << "y[" << i << "] = ";
		cin >> data[i].y;
		if (data[i].x > x && I == -1)
			I = i;
	}
	return data;
}

void setI(vector<Data> data, double X, int n)
{
	for (int i = 0; i < n; ++i)
	{
		if (data[i].x > X && I == -1)
			I = i;
	}
}

vector<vector<double>> setC(vector<Data> data, int n)
{
	vector<vector<double>> c(n - 2, vector<double>(n - 2, 0));

	for (int i = 0; i < n - 2; ++i)
	{
		for (int j = 0; j < n - 2; ++j)
		{
			if (i == j)
				c[i][j] = ((data[i + 1].x - data[i].x) + (data[i + 2].x - data[i + 1].x)) / 3;
			else if (j == i + 1)
				c[i][j] = ((data[j + 2].x - data[j + 1].x)) / 6;
			else if (j == i - 1)
				c[i][j] = (data[i + 1].x - data[i].x) / 6;
			else if (abs(i - j) > 1)
				c[i][j] = 0;
		}
	}
	return c;
}

vector<double> setM(vector<vector<double>> CM, int n)
{
	vector<double> M(n, 0);
	Gauss(CM, n - 2, M);
	return M;
}

double Splines(vector<Data> data, double x, int n)
{
	double S = 0, h;
	vector<double> d = setD(data, n);
	vector<vector<double>> c = setC(data, n);
	vector<vector<double>> CM = setCM(d, c, n);
	vector<double> M = setM(CM, n);
	h = data[I].x - data[I - 1].x;
	S = (M[I - 2] * ((data[I].x - x) * (data[I].x - x) * (data[I].x - x)) / (6.0 * h)) + (M[I - 1] * pow((x - data[I - 1].x), 3)) / (6.0 * h) + (data[I - 1].y - (M[I - 2] * h * h) / 6.0) * (data[I].x - x) / h + (data[I].y - (M[I - 1] * h * h) / 6.0) * (x - data[I - 1].x) / h;
	cout << "\n(d)\n" << d << "\n(c)\n" << c << "\n(CM)\n" << CM << "\n(M)\n" << M;
	cout << "\nS = " << (M[I - 2] * ((data[I].x - x) * (data[I].x - x) * (data[I].x - x)) / (6.0 * h)) << " + " << (M[I - 1] * pow((x - data[I - 1].x), 3)) / (6.0 * h) << " + " << (data[I - 1].y - (M[I - 2] * h * h) / 6.0) * (data[I].x - x) / h << " + " << (data[I].y - (M[I - 1] * h * h) / 6.0) * (x - data[I - 1].x) / h;
	cout << "\nh = " << h << "\nI = " << I << "\nS = " << S;
	return S;
}

double Splines(vector<Data> data, double x)
{
	int n = data.size();
	double S = 0, h;
	vector<double> d = setD(data, n);
	vector<vector<double>> c = setC(data, n);
	vector<vector<double>> CM = setCM(d, c, n);
	vector<double> M = setM(CM, n);
	h = data[I].x - data[I - 1].x;
	S = (M[I - 2] * ((data[I].x - x) * (data[I].x - x) * (data[I].x - x)) / (6.0 * h)) + (M[I - 1] * pow((x - data[I - 1].x), 3)) / (6.0 * h) + (data[I - 1].y - (M[I - 2] * h * h) / 6.0) * (data[I].x - x) / h + (data[I].y - (M[I - 1] * h * h) / 6.0) * (x - data[I - 1].x) / h;
	return S;
}

int main()
{
	
	
	setlocale(0, "");
	int n;
	
	cout << "Введите x: ";
	cin >> x;
	cout << "Введите n: ";
	cin >> n;
	vector<Data> data = setData(n);	

	double S = Splines(data, x, n);
	
	sf::RenderWindow window(sf::VideoMode(1200, 800), "I hate SFML");
	sf::RectangleShape fon;
	fon.setSize({ 1200, 800 });
	fon.setFillColor(sf::Color(0xFF,0xFF,0xFF));

	sf::Font font;//шрифт 
	font.loadFromFile("CyrilicOld.ttf");//передаем нашему шрифту файл шрифта

	while (window.isOpen())
	{
		window.clear();
		window.draw(fon);
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		sf::VertexArray line(sf::Lines, 4);
		line[0].position = sf::Vector2f(0, 400);
		line[1].position = sf::Vector2f(1200, 400);
		line[2].position = sf::Vector2f(600, 0);
		line[3].position = sf::Vector2f(600, 800);
		line[0].color = sf::Color(0x0, 0x0, 0x0);
		line[1].color = sf::Color(0x0, 0x0, 0x0);
		line[2].color = sf::Color(0x0, 0x0, 0x0);
		line[3].color = sf::Color(0x0, 0x0, 0x0);
		

		sf::VertexArray graf(sf::LinesStrip, data.size()+1);
		graf[0].position = sf::Vector2f(600, 400);
		graf[0].color = sf::Color(0x0, 0x0, 0x0);
		sf::Text *text = new sf::Text[data.size()];
		
		sf::Text ttext("", font, 12);
		for (int i = 0; i < data.size(); ++i)
		{
			graf[i + 1].color = sf::Color(0x0, 0x0, 0x0);
			graf[i + 1].position = sf::Vector2f(600 + data[i].x * 20, 400 - data[i].y * 20);

			ttext.setPosition(600 + data[i].x * 20, 400);
			ttext.setColor(sf::Color::Red);
			ttext.setString(toStr(data[i].x));
			window.draw(ttext);

			ttext.setPosition(598 + data[i].x * 20, 391);
			ttext.setString("|");
			window.draw(ttext);

			ttext.setPosition(598 + data[i].x * 20, 387 - data[i].y * 20);
			ttext.setString("|\n|");
			window.draw(ttext);

			ttext.setPosition(600, 387 - data[i].y * 20);
			ttext.setColor(sf::Color::Red);
			ttext.setString(toStr(data[i].y));
			window.draw(ttext);

			ttext.setPosition(594 + data[i].x * 20, 387 - data[i].y * 20);
			ttext.setString("__");
			window.draw(ttext);

			ttext.setPosition(596, 387 - data[i].y * 20);
			ttext.setString("_");
			window.draw(ttext);
		}
		ttext.setColor(sf::Color::Blue);

		ttext.setPosition(600 + x * 20, 400);
		ttext.setString(toStr(x));
		window.draw(ttext);

		ttext.setPosition(600, 387 - S * 20);
		ttext.setString(toStr(S));
		window.draw(ttext);

		ttext.setPosition(598 + x * 20, 391);
		ttext.setString("|");
		window.draw(ttext);

		ttext.setPosition(598 + x * 20, 387 - S * 20);
		ttext.setString("|\n|");
		window.draw(ttext);

		ttext.setPosition(594 + x * 20, 387 - S * 20);
		ttext.setString("__");
		window.draw(ttext);

		ttext.setPosition(596, 387 - S * 20);
		ttext.setString("_");
		window.draw(ttext);

		sf::VertexArray grafic(sf::LinesStrip, (data[n-1].x-data[1].x)*10+1);
		grafic[0].position = sf::Vector2f(600, 400);
		grafic[0].color = sf::Color::Green;
		int j = 1;
		for(double i = data[1].x; i < data[n-1].x-0.1; i+=0.1)
		{
			I = -1;
			setI(data, i, n);
			double y = Splines(data, i);
			grafic[j].position = sf::Vector2f(600 + i*20, 400 - y*20);
			grafic[j].color = sf::Color::Green;
			++j;
		}
		window.draw(grafic);
		window.draw(line);
		window.draw(graf);
		window.display();
	}
}
//15 5 1 1 5 5 9 1 13 -3 17 1
//7 5 1 1 5 5 9 1 13 -3 17 1
//8.9 5 1 1 5 5 9 1 13 -3 17 1
//6 5 1 1 5 5 9 1 13 -3 17 1
//5 5 1 1 5 5 9 1 13 -3 17 1