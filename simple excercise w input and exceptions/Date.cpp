#include "Date.h"
#include <string>
#include <algorithm>

namespace CheckTools
{
	bool checkLeapYear(int year)
	{
		return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
	}

	void checkDate(int day, int month, int year)
	{
		if (year <= 0 || year > 9999)
		{
			throw std::runtime_error("Invalid year: " + std::to_string(year));
		}

		if (month <= 0 || month > 12)
		{
			throw std::runtime_error("Invalid month: " + std::to_string(month));
		}

		if (day <= 0 || day > 31 || ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30))
		{
			throw std::runtime_error("Invalid day: " + std::to_string(day) + " of " + std::to_string(month) + " month" );
		}
		else if (month == 2 && !((checkLeapYear(year) && day <= 29) || (!checkLeapYear(year) && day <= 28)))
		{
			throw std::runtime_error("Invalid February day: " + std::to_string(day) + " of " + std::to_string(year) + " year");
		}
	}
}

Date::Date(int year, int month, int day)
	: Date(std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day))
{}

Date::Date(std::string const& string)
{
	std::string delim = "-";
	int date[2];
	int pos = 0;
	int prevPos = pos;
	{
		std::string currentSubstring;
		for (int i = 0; (pos = string.find(delim, prevPos)) != std::string::npos; i++)
		{
			currentSubstring = string.substr(prevPos, pos - prevPos);
			if (std::any_of(currentSubstring.begin(), currentSubstring.end(), ::isalpha))
			{
				throw std::runtime_error("Letters are not allowed");
			}
			date[i] = std::stoi(currentSubstring);
			prevPos = pos + delim.length();
		}
		

	}
	day = std::stoi(string.substr(prevPos));

	year = date[0];
	month = date[1];

	CheckTools::checkDate(day, month, year);
}

int Date::GetYear() const
{
	return year;
}

int Date::GetMonth() const
{
	return month;
}

int Date::GetDay() const
{
	return day;
}

bool Date::operator<(Date const& rhs) const
{
	return (year, month, day) < (rhs.GetYear(), rhs.GetMonth(), rhs.GetDay());
}

Date ParseDate(std::istream& is)
{
	std::string str;
	std::getline(is, str);
	return Date(str);
}

std::ostream& operator<<(std::ostream& stream, Date const& date)
{
	return stream << "Date: " << date.GetYear() << "-" << date.GetMonth() << "-" << date.GetDay() << std::endl;
}
