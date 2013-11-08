/// Description: Base abstract class for Date. 
/// Date contains variables for year, day, days per week and months per year.
/// Also contains a set of base functions for handling and modifying these.
/// 
/// Authors: Martin Pettersson, Christoffer Wiss
///
/// Version: 2013-11-03

#include "date.h"

using namespace lab2;

// Destructor (deallocate allocated memory here).
Date::~Date(){}

// Returns the current year.
int Date::year() const
{
	return currentYear_;
}

// Returns the current month nr.
int Date::month() const
{
	return currentMonth_;
}

// Returns the current month day nr.
int Date::day() const
{
	return currentDay_;
}

// Returns the current week day nr.
int Date::week_day() const
{
	return currentWeekday_;
}

// Returns the nr of days per week.
int Date::days_per_week() const
{
	return daysPerWeek_;
}

// Returns the nr of months per year.
int Date::months_per_year() const
{
	return monthsPerYear_;
}

// Prefix ++ operator; Increases date by one day.
Date& Date::operator++()
{
	currentDay_++;
	currentWeekday_++;
	
	// Are we in the next week? 
    if(currentWeekday_ > daysPerWeek_)
	{
        currentWeekday_ = 1;
	}
	
	// Are we in a new month?
	if(currentDay_ > days_this_month())
	{
		currentDay_ = 1;
		currentMonth_++;
		
		// Are we in a new year?
		if(currentMonth_ > monthsPerYear_)
		{
			currentMonth_ = 1;
			add_year();
		}
	}
	return *this;
}

// Prefix -- operator; Decreases date by one day.
Date& Date::operator--()
{
	currentDay_--;
	currentWeekday_--;
	
	// Are we in the previous week?
    if(currentWeekday_ < 1)
	{
        currentWeekday_ = daysPerWeek_;
	}
	
	// Are we in the previous month?
	if(currentDay_ < 1)
	{
		currentMonth_--;
		bool isPrevYear = (currentMonth_ < 1);
		
		if(isPrevYear) currentMonth_ = monthsPerYear_;
		currentDay_ = days_this_month();
		
		// Are we in previous year?
		if(isPrevYear)
		{
			add_year(-1);
		}
	}
	return *this;
}

// Compound += operator; Adds val number of days to current date. 
Date& Date::operator+=(int val)
{	
    if(val < 0)
    {
        operator-=(-val);
    }
    else
    {
        for(int i = 0; i < val; i++)
        {
            operator++();
        }
    }
	return *this;
}

// Compound -= operator; Subtracts val number of days from current date. 
Date& Date::operator-=(int val)
{	
    if(val < 0)
    {
        operator+=(-val);
    }
    else
    {
        for(int i = 0; i < val; i++)
        {
            operator--();
        }
    }
	return *this;
}

// Adds n to current year. 
// n : can be positive or negative.
void Date::add_year(int n)
{
	currentYear_ += n;
}

// Assignment operator. Assigns from date reference. Returns reference to a new date object.
Date& Date::operator=(const Date& date)
{
    currentYear_    = date.currentYear_;
    currentMonth_   = date.currentMonth_;
    currentDay_     = date.currentDay_;
    currentWeekday_ = date.currentWeekday_;
	return *this;
}

// Checks if two dates are the same.
// This is done by first converting both dates to their Julian Day number
// (in order to provide a common base) and then do the comparison.
// Returns true if so, otherwise false.
bool Date::operator==(const Date& rhs) const
{
	return mod_julian_day() == rhs.mod_julian_day();
}

// Checks if two dates are different.
// This is done by first converting both dates to their Julian Day number
// (in order to provide a common base) and then do the comparison.
// Returns true if so, otherwise false.
bool Date::operator!=(const Date& rhs) const
{
	return mod_julian_day() != rhs.mod_julian_day();
}

// Checks if date is less than the right hand side date.
// This is done by first converting both dates to their Julian Day number
// (in order to provide a common base) and then do the comparison.
// Returns true if so, otherwise false.
bool Date::operator<(const Date& rhs) const
{
	return mod_julian_day() < rhs.mod_julian_day();
}

// Checks if date is less than or equal to the right hand side date.
// This is done by first converting both dates to their Julian Day number
// (in order to provide a common base) and then do the comparison.
// Returns true if so, otherwise false.
bool Date::operator<=(const Date& rhs) const
{
	return mod_julian_day() <= rhs.mod_julian_day();
}

// Checks if date is larger than the right hand side date.
// This is done by first converting both dates to their Julian Day number
// (in order to provide a common base) and then do the comparison.
// Returns true if so, otherwise false.
bool Date::operator>(const Date& rhs) const
{
	return mod_julian_day() > rhs.mod_julian_day();
}

// Checks if date is larger than or equal the right hand side date.
// This is done by first converting both dates to their Julian Day number
// (in order to provide a common base) and then do the comparison.
// Returns true if so, otherwise false.
bool Date::operator>=(const Date& rhs) const
{
	return mod_julian_day() >= rhs.mod_julian_day();
}

// Returns the integer differens between two dates.
// This is done by first converting both dates to their Julian Day number
// (in order to provide a common base) and then do the subtraction.
int Date::operator-(const Date& rhs) const
{
	return (mod_julian_day()-rhs.mod_julian_day());
}

// Puts a text string representation of the date 
// to outputstream object. Format: YYYY-MM-DD
ostream& lab2::operator<<(ostream& os, const Date& date)
{
	return (os << date.year() << "-" << ((date.month() < 10) ? "0" : "") << date.month() << "-" << ((date.day() < 10) ? "0" : "") << date.day()); 
}
