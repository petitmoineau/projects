#pragma once
#include <iostream>

class Date
{
public:
	Date(int year, int month, int day);
	Date(std::string const& string);
	int GetYear() const;
	int GetMonth() const;
	int GetDay() const;
	bool operator<(Date const& rhs) const;
private:
	int year, month, day;
};

Date ParseDate(std::istream& is);
std::ostream& operator<<(std::ostream& stream, Date const& date);