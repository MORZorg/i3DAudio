/**
 * @file data_reader.cpp
 * @brief Reads HRTF data from a file and writes it to a C++ header file.
 * @note The functions have been roughly taken from earplug's `new` function.
 * This means that they were originally written in C. To avoid errors, file
 * pointers are still used, instead of C++'s iostream library.
 * @author Zucchelli Maurizio
 * @version 0.0.1
 * @date 2014-07-14
 */

#include <string>

#define POINTS 368
#define CHANNELS 2
#define LENGTH 128

using namespace std;

float values[POINTS][CHANNELS][LENGTH];

void read_data(string filename)
{
	int i, j;
	FILE* fp = fopen(filename.c_str(), "r");  

	for (i = 0; i < POINTS; i++) 
	{
		while(fgetc(fp) != 10);

		for (j = 0; j < LENGTH; j++)
			fscanf(fp, "%f %f ", &values[i][0][j], &values[i][1][j]);
	}

	fclose(fp);
}

void write_data(string filename)
{
	int i, j, k;
	FILE* fp = fopen(filename.c_str(), "w");  

	fprintf(fp, "#include \"m_pd.h\"\n\nextern \"C\" t_float hrtf_impulses[368][2][128] =\n{\n");
	for (i = 0; i < POINTS; i++) 
	{
		fprintf(fp, "\t{\n");
		for (j = 0; j < CHANNELS; j++)
		{
			fprintf(fp, "\t\t{\n");
			for (k = 0; k < LENGTH; k++)
			{
				if (k % 8 == 0)
					fprintf(fp, "\t\t\t");
				else
					fprintf(fp, " ");

				fprintf(fp, "%f", values[i][j][k]);

				if (k % 8 == 7)
					fprintf(fp, "\n");
				else
					fprintf(fp, ",");
			}
			fprintf(fp, "\t\t}");

			if (j < CHANNELS-1)
				fprintf(fp, ",");
			fprintf(fp, "\n");
		}
		fprintf(fp, "\t}");

		if (j < POINTS-1)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
	fprintf(fp, "};");

	fclose(fp);
}

int main(int argc, char** argv)
{
	if (argc <= 1)
	{
		printf("Usage: data_reader input [output]\n");
		return 1;
	}

	read_data(argv[1]);

	if (argc <= 2)
		write_data("hrtf_data.hpp");
	else
		write_data(argv[2]);

	return 0;
}
