#include <iostream>
#include <fstream>
#include <iomanip>
#include <conio.h>

int main(int argc, char* argv[])
//int main()
{
	const double inputPrecision = 0.0013; //Oripa can be off by up to 0.0000075     PezDorado off by 0.0013 wtf
	double current = 0;
	int iteration = 1;
	double grid = 1;
	double precision = 0;

	int counter = 0;
	int smallCounter = 0;
	bool previous = false;

	std::string filename = argv[1];
	//std::string filename = ("C:\\Users\\dwism\\Downloads\\Test.cp"); 
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
	int gridBase = 0;

	//Determines the grid size of the model 
	for (int i = 1; i <= 16; i++)
	{
		switch (i)
		{
		case 1:
			initGrid = 1024; //Base: 1		multiplied by 1024 
			break;
		case 2:
			initGrid = 1536; //Base: 3		multiplied by 512
			break;
		case 3:
			initGrid = 1280; //Base: 5		multiplied by 256
			break;
		case 4:
			initGrid = 1792; //Base: 7		multiplied by 256
			break;
		case 5:
			initGrid = 1152; //Base: 9		multiplied by 128
			break;
		case 6:
			initGrid = 1408; //Base: 11		multiplied by 128
			break;
		case 7:
			initGrid = 1664; //Base: 13		multiplied by 128
			break;
		case 8:
			initGrid = 1920; //Base: 15		multiplied by 128
			break;
		case 9:
			initGrid = 1088; //Base: 17		multiplied by 64
			break;
		case 10:
			initGrid = 1216; //Base: 19		multiplied by 64
			break;
		case 11:
			initGrid = 1344; //Base: 21		multiplied by 64
			break;
		case 12:
			initGrid = 1472; //Base: 23		multiplied by 64
			break;
		case 13:
			initGrid = 1600; //Base: 25		multiplied by 64
			break;
		case 14:
			initGrid = 1728; //Base: 27		multiplied by 64
			break;
		case 15:
			initGrid = 1856; //Base: 29		multiplied by 64
			break;
		case 16:
			initGrid = 1984; //Base: 31		multiplied by 64
			break;
		}

		//order of bases:

		//1		1024

		//9		1088

		//5		1152

		//19	1216

		//3		1280

		//21	1344

		//11	1408

		//23	1472

		//3		1536

		//25	1600

		//13	1664

		//27	1728

		//7		1792

		//29	1856

		//15	1920

		//31	1984

		iteration = 1;
		precision = (inputPrecision * float(initGrid)) / 400;

		while (file >> current)
		{
			//scales and moves Vertex to make it easier to work with
			if (!((iteration + 4) % 5 == 0))
			{
				current = current * initGrid;
				current = current / 400;
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

		if (counter > (float(oldCounter) * 1.1f))
		{
			grid = initGrid;
			oldCounter = counter;
			gridBase = 2 * i - 1;
		}
		counter = 0;
		smallCounter = 0;
		previous = false;
	}

	precision = (inputPrecision * float(grid)) / 400;

	while (file >> current)
		//while (0)
	{
		//scales and moves Vertices to make it easier to work with
		if (!((iteration + 4) % 5 == 0))
		{
			current = current * grid;
			current = current / 400;
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
			current = current * 400;
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

	std::cout << "Done!" << std::endl;
	std::cout << std::endl;
	std::cout << "Fixed " << counter << " Vertices" << std::endl;
	std::cout << "Press any key to exit" << std::endl;

	while (!_kbhit())
		continue;
}
