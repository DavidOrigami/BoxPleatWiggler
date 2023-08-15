#include <iostream>
#include <fstream>
#include <iomanip>
#include <conio.h>

int main(int argc, char* argv[])
{
	const double inputPrecision = 0.00005;
	double current = 0;
	int iteration = 1;
	double grid = 1;
	double precision = 0;

	int counter = 0;
	int smallCounter = 0;
	bool previous = false;

	std::string filename = argv[1];
	std::ifstream file(filename);

	if (!file)
	{
		std::cerr << "Error opening file: " << filename << std::endl;
		while (!_kbhit())
			continue;
		return 1;
	}

	std::string fixedFilename = filename.substr(0, filename.length() - 3);
	std::ofstream output(fixedFilename + "_fixed.cp");

	int initGrid = 0;
	int oldCounter = 0;
	std::cout << "Determining Grid size..." << std::endl;

	//Determines the grid size of the model 
	for (int i = 1; i < 14; i++)
	{
		switch (i) {

		case 1:
			initGrid = 1024;
			break;
		case 2:
			initGrid = 1536;
			break;
		case 3:
			initGrid = 1280;
			break;
		case 4:
			initGrid = 1792;
			break;
		case 5:
			initGrid = 1152;
			break;
		case 6:
			initGrid = 1408;
			break;
		case 7:
			initGrid = 1664;
			break;
		case 8:
			initGrid = 1920;
			break;
		case 9:
			initGrid = 1088;
			break;
		case 10:
			initGrid = 1216;
			break;
		case 11:
			initGrid = 1344;
			break;
		case 12:
			initGrid = 1472;
			break;
		case 13:
			initGrid = 1600;
			break;
		case 14:
			initGrid = 1728;
			break;
		}

		iteration = 1;

		while (file >> current)
		{
			precision = inputPrecision * initGrid;

			//scales and moves Vertices to make it easier to work with
			if (!((iteration + 4) % 5 == 0))
			{
				current = current * initGrid;
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

			iteration++;
		}

		file.close();
		file.open(filename);

		if (!file)
		{
			std::cerr << "Error opening file: " << filename << std::endl;
			while (!_kbhit())
				continue;
			return 1;
		}

		if (counter > oldCounter)
		{
			grid = initGrid;
			std::cout << grid << "  " << counter << std::endl;
			oldCounter = counter;
		}

		counter = 0;
		smallCounter = 0;
		previous = false;
	}

	precision = inputPrecision * grid;

	while (file >> current)
	{
		//scales and moves Vertices to make it easier to work with
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
