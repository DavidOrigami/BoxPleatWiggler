#include <iostream>
#include <fstream>
#include <iomanip>
#include <conio.h>

int main(int argc, char* argv[])
{
	double inputPrecision = 0.00005;
	double current = 0;
	int iteration = 1;
	int grid = 0;

	int counter = 0;
	int smallCounter = 0;
	bool previous = false;

	std::string filename = argv[1];
	std::ifstream file(filename);

	if (!file) {
		std::cerr << "Error opening file: " << filename << std::endl;
		while (!_kbhit())
			continue;
		return 1;
	}

	std::ofstream output(filename + "_fixed.cp");

	std::cout << "Please enter grid size:" << std::endl;
	std::cin >> grid;

	double precision = inputPrecision * grid;

	while (file >> current)
	{

		if (!((iteration + 4) % 5 == 0))
		{
			current = current * grid;
			current = current + 1000000;
		}

		//prescion algo
		if (!((iteration + 4) % 5 == 0))
		{
			//Corrected Vertices Counter Logic
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
			output << std::setprecision(17) << current;
			output << " ";
		}
		iteration++;
	}

	file.close();
	output.close();

	std::cout << std::endl;
	std::cout << "Done!" << std::endl;
	std::cout << std::endl;
	std::cout << "Fixed " << counter << " Vertices" << std::endl;
	std::cout << "Press any key to exit" << std::endl;

	while (!_kbhit())
		continue;
}
