#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <conio.h>

using std::cout;
using std::setprecision;
using std::endl;
using namespace std;


int main(int argc, char* argv[])
{
	
	
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

	string filename = argv[1];
	std::ifstream file(filename);

	if (!file) {
		std::cerr << "Error opening file: " << filename << std::endl;
		return 1;
	}

	ofstream output(filename + "_fixed.cp");

	cout << "Please enter Grid Size:" << endl;
	cin >> grid;	


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
