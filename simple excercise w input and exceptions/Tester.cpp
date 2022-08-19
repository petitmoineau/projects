#include "Date.h"
#include <iostream>
#include <fstream>
#include <vector>

namespace Utils
{
	void readDatabase(const std::string& filename, std::vector<Date>& dates)
	{
		std::string contents;
		std::ifstream database;
		try
		{
			database.open(filename, std::ios::in);
			while (!database.eof())
			{
				try
				{
					dates.push_back(ParseDate(database));
				}
				catch (const std::runtime_error& e) {
					std::cout << "Wrong date argument\n"
						<< "Explanatory string: " << e.what() << '\n';
				}
				catch (const std::invalid_argument& e) {
					std::cout << "Wrong date argument\n"
						<< "Explanatory string: " << e.what() << '\n';
				}
			}
		}
		catch(const std::ifstream::failure& e)
		{
			std::cout << "Could not open the file\n" 
				<< "Explanatory string: " << e.what() << '\n';
		}
		database.close();
	}
}

int main()
{
	std::vector<Date> dates;
	Utils::readDatabase("database.txt", dates);

	for (int i = 0; i < dates.size(); i++)
	{
		std::cout << dates.at(i) << "\n";
	}
}