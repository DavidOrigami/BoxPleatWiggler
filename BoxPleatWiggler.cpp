// BoxPleatWiggler.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <conio.h>

using std::cout;
using std::setprecision;
using std::endl;
using namespace std;


int main()
{
	
	string filename = "";
	
	double inputPrecision = 0.00005;
	bool debugShortenLoop = false;
	int debugShortendLoopLength = 100;

	double current = 0;
	int iteration = 1;
	int grid = 0;

	int counter = 0;
	int smallCounter = 0;
	bool previous = false;
	bool isSecond = false;
	
	Start:

	cout << "Enter filename without '.cp':" << endl;
	cin >> filename;

	fstream file (filename + ".cp");
	ofstream output(filename + "_fixed.cp");

	if (file.is_open())
	{
		cout << "Please enter Grid Size:" << endl;
		cin >> grid;	
	}
	else
	{
		cout << "File does not exist" << endl;
		goto Start;
	}

	double precision = inputPrecision * grid;

	
	while (file >> current && ((iteration < debugShortendLoopLength) || !debugShortenLoop))
	{

		//blows up the grid and moves everything to the positive/positive quadrant to make the next step easier
		if (!((iteration + 4) % 5 == 0))
		{
			current = current * grid;
			current = current + 1000000;
		}

		//prescion algo
		if (!((iteration + 4) % 5 == 0))
		{
			//Corrected Verticie Counter Logic
			smallCounter++;
			if (smallCounter > 2)
			{
				previous = false;
				smallCounter = 0;
			}




			//barely under case
			if (int(current + precision) != int(current))
			{
				current = current + precision;
				current = int(current);

				if (!previous)
				{
					previous = true;
					counter++;
				}
			}

			//barely over case
			else if ((float(int(current)) != current) && int(current - precision) != int(current))
			{
				current = int(current);

				if (!previous)
				{
					previous = true;
					counter++;
				}
			}
			
		}
		


		//shrinks grid back down and realigns everything
		if (!((iteration + 4) % 5 == 0))
		{
			current = current - 1000000;
			current = current / grid;
		}


		//repack into .cp
		if (iteration % 5 == 0)
		{
			output << current;
			output << "\n";
		}
		else
		{
			output << setprecision(17) << current;
			output << " ";
		}
		iteration++;
	}

	file.close();
	output.close();
	

	cout << endl;
	cout << "Done!" << endl;
	cout << endl;
	cout << "Fixed " << counter << " Verticies" << endl;
	cout << "Press any key to exit" << endl;


	while (!_kbhit())
		continue;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
