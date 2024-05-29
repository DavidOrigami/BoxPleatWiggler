#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>



using namespace std;

#define DEBUG 0
#define DISPLAYGRIDNUMBER 0


float BPCutOffValue = .75;
int counter_BP = 0;
long long totalLineNumber = 0;
bool failed = false;


std::vector<double> data_22_5;
std::vector<double> prevDataPoint;

std::string filename = "";
std::string filename_sans_cp = "";
std::string fixedFilename_BP = "";





void fix_BP()
{
	const double inputPrecision_BP = 0.0013; // Oripa can be off by up to 0.0000075     PezDorado.cp by Fernando was off by 0.0013 wtf
	double current_BP = 0;
	long long iteration_BP = 1;
	double grid_BP = 1;
	double precision_BP = 0;

	short smallCounter_BP = 0;
	bool alreadyFixed_BP = false;

	int initGrid_BP = 0;
	long long oldCounter_BP = 0;

	// Opens file that needs to be fixed
	std::ifstream file_BP(filename);
	if (!file_BP)
	{
		std::cerr << "Error opening file: " << filename << std::endl;
		failed = true;
		return;
	}


	// Created fixed [filename]_BP temp file
	std::ofstream output_BP(fixedFilename_BP, std::ios::trunc);
	if (!output_BP)
	{
		std::cerr << "Error creating BP output file: " << fixedFilename_BP << std::endl;
		failed = true;
		return;
	}


	// Determines the grid size of the model 
	for (int i = 1; i <= 12; i++)
	{
		switch (i)
		{
			// Ordered by likelyhood
		case 1:
			initGrid_BP = 1152; //Base: 9		multiplied by 128	includes base 1 & 3
			break;
		case 2:
			initGrid_BP = 1920; //Base: 15		multiplied by 128	includes base 5 
			break;
		case 3:
			initGrid_BP = 1344; //Base: 21		multiplied by 256	includes base 7
			break;
		case 4:
			initGrid_BP = 1408; //Base: 11		multiplied by 128
			break;
		case 5:
			initGrid_BP = 1664; //Base: 13		multiplied by 128
			break;
		case 6:
			initGrid_BP = 1088; //Base: 17		multiplied by 64
			break;
		case 7:
			initGrid_BP = 1216; //Base: 19		multiplied by 64
			break;
		case 8:
			initGrid_BP = 1472; //Base: 23		multiplied by 64
			break;
		case 9:
			initGrid_BP = 1600; //Base: 25		multiplied by 64
			break;
		case 10:
			initGrid_BP = 1728; //Base: 27		multiplied by 64
			break;
		case 11:
			initGrid_BP = 1856; //Base: 29		multiplied by 64
			break;
		case 12:
			initGrid_BP = 1984; //Base: 31		multiplied by 64
			break;
		}

		iteration_BP = 1;
		precision_BP = (inputPrecision_BP * double(initGrid_BP)) / 400;

		while (file_BP >> current_BP)
		{
			//Skips if it's a line type indicator
			if ((((iteration_BP + 4) % 5) == 0))
			{
				totalLineNumber++;
				goto skip_Line_Type_Indicator_GridSearch;
			}


			// Scales and moves vertex to make it easier to work with
			current_BP = current_BP / 400;
			current_BP = current_BP * initGrid_BP;
			current_BP = current_BP + 1000000;


			// Corrected lines counter logic
			smallCounter_BP++;
			if (smallCounter_BP > 4)
			{
				alreadyFixed_BP = false;
				smallCounter_BP = 0;
			}

			// Barely under case
			if (int(current_BP + precision_BP) != int(current_BP))
			{
				current_BP = current_BP + precision_BP;
				current_BP = int(current_BP);

				if (!alreadyFixed_BP)
				{
					alreadyFixed_BP = true;
					counter_BP++;

				}
			}

			// Barely over case
			// The double brackets are necessary, idk why exactly
			else if (int(current_BP) != int((current_BP - precision_BP)))
			{
				current_BP = int(current_BP);
				if (!alreadyFixed_BP)
				{
					alreadyFixed_BP = true;
					counter_BP++;
				}
			}

		skip_Line_Type_Indicator_GridSearch:
			iteration_BP++;
		}


		// Reopens file to get back to the beginning
		file_BP.close();
		file_BP.open(filename);
		if (!file_BP)
		{
			std::cerr << "Error opening file: " << filename << std::endl;
			failed = true;
			return;
		}


		// Only overwrites old grid solution if the new one has 10% more matches (arbitrary value)
		if (counter_BP > (float(oldCounter_BP) * 1.1f))
		{
			grid_BP = initGrid_BP;
			oldCounter_BP = counter_BP;
		}

#if DEBUG
		std::cout << initGrid_BP << "  counter: " << counter_BP << "\n";
#endif


		// Ends grid search prematurely if it finds a close match
		if ((totalLineNumber * BPCutOffValue) < counter_BP)
		{

#if DISPLAYGRIDNUMBER
			std::cout << "Found grid: " << grid_BP << "\n" << "counter: " << counter_BP << " of " << totalLineNumber << "\n";
#endif

			counter_BP = 0;
			smallCounter_BP = 0;
			alreadyFixed_BP = false;
			totalLineNumber = 0;
			goto endGridSearch;
		}

		// Resets values for next iteration
		counter_BP = 0;
		smallCounter_BP = 0;
		alreadyFixed_BP = false;
		totalLineNumber = 0;
	}


endGridSearch:


#if DISPLAYGRIDNUMBER
	std::cout << "Using grid: " << grid_BP << "\n";
#endif


	iteration_BP = 1;
	// Fixing algo
	while (file_BP >> current_BP)
	{
		// Skips if it's a line type indicator
		if (((iteration_BP + 4) % 5 == 0))
		{
			totalLineNumber++;
			goto skip_Line_Type_Indicator;
		}

		// Scales and moves vertex to make it easier to work with
		current_BP = current_BP / 400;
		current_BP = current_BP * grid_BP;
		current_BP = current_BP + 1000000;


		// Corrected lines counter logic
		smallCounter_BP++;
		if (smallCounter_BP > 4)
		{
			alreadyFixed_BP = false;
			smallCounter_BP = 0;
		}


		// Barely under case
		if (int(current_BP + precision_BP) != int(current_BP))
		{
			current_BP = current_BP + precision_BP;
			current_BP = int(current_BP);

			if (!alreadyFixed_BP)
			{
				alreadyFixed_BP = true;
				counter_BP++;
			}
		}


		// Barely over case
		// Double bracket is necessary for some reason, idk why exactly
		else if (int(current_BP) != int((current_BP - precision_BP)))
		{
			current_BP = int(current_BP);

			if (!alreadyFixed_BP)
			{
				alreadyFixed_BP = true;
				counter_BP++;
			}
		}


		// Shrinks grid back down and realigns everything		
		current_BP = current_BP - 1000000;
		current_BP = current_BP * 400;
		current_BP = current_BP / grid_BP;



	skip_Line_Type_Indicator:

		// Repacks vertex back into .cp
		if (iteration_BP % 5 == 0)
			output_BP << std::setprecision(17) << current_BP << "\n";
		else
			output_BP << std::setprecision(17) << current_BP << " ";

		iteration_BP++;
	}

	file_BP.close();
	output_BP.close();
	output_BP.flush();

#if DEBUG

#endif


#if DEBUG
	std::cout << "\nBP end\n";
#endif

}


int main(int argc, char* argv[])
{
	filename = argv[1];
	std::cout << "Filename: " << filename << "\n" << "\n";

	// Creates various filenames
	filename_sans_cp = filename.substr(0, filename.length() - 3);
	fixedFilename_BP = filename_sans_cp + ("_BP");


	// Removes _fixed.cp file if it already exists
	std::string toRemove = filename_sans_cp;
	toRemove.append("_fixed.cp");
	std::remove(toRemove.c_str());


	// Main operations
	fix_BP();

	// Creates new file name
	std::string newName = filename_sans_cp;
	newName.append("_fixed.cp");

	//if any check failed end program
	if (failed)
	{
		std::remove(fixedFilename_BP.c_str());
		std::cout << "Press any key to exit" << "\n";
		std::cin.get();
		return 2;
	}


	// Removes and renames files
	
		//IO and Removes 22.5 File
		std::cout << "\n" << "Done!" << "\n" << "\n" << "Fixed " << counter_BP << " lines" << "\n";
		// Renames BP file
		std::rename(fixedFilename_BP.c_str(), newName.c_str());


	// Closing ceremony
	std::cout << "Press any key to exit" << "\n";
	std::cin.get();
	return 1;
}
