/// Description: Implemenation of Date for Gregorian Calender.
/// 
/// Authors: Martin Pettersson, Christoffer Wiss
///
/// Version: 2013-11-01

#include "gregorian.h"
#include <stdexcept>
#include <math.h>

using namespace lab2;

// Constructor.
Gregorian::Gregorian(int year, int month, int day) 
: Date(year, month, day, calculateCurrentWeekday(year, month, day), DAYS_PER_WEEK, MONTHS_PER_YEAR)
{
	initializeDaysPerMonths(isLeapYear());
	checkValidDate();
}

// Default Constructor (sets date to current date).
Gregorian::Gregorian()
{
	// Gets current time
	time_t currTime;
	k_time(&currTime);
	
	struct tm *t = gmtime(&currTime);

	currentYear_    = t->tm_year + 1900;
	currentMonth_   = t->tm_mon + 1;
	currentDay_     = t->tm_mday;
	currentWeekday_ = t->tm_wday;
	daysPerWeek_    = DAYS_PER_WEEK;
	monthsPerYear_  = MONTHS_PER_YEAR;
	initializeDaysPerMonths(isLeapYear());
	checkValidDate();
}

// Copy constructor.
Gregorian::Gregorian(const Date &date)
{
	double tmp = date.mod_julian_day();
	setDateFromJulian(tmp);
	daysPerWeek_   = DAYS_PER_WEEK;
	monthsPerYear_ = MONTHS_PER_YEAR;
	initializeDaysPerMonths(isLeapYear());
	checkValidDate();
}

// Construct Gregorian from Date pointer.
Gregorian::Gregorian(const Date *date)
{
	double tmp = date->mod_julian_day();
	setDateFromJulian(tmp);
	daysPerWeek_   = DAYS_PER_WEEK;
	monthsPerYear_ = MONTHS_PER_YEAR;
	initializeDaysPerMonths(isLeapYear());
	checkValidDate();
}

// Assignment operator. Assigns from date reference. Returns reference to a new date object.
Gregorian& Gregorian::operator=(const Date& date)
{
	double tmp = date.mod_julian_day();
	setDateFromJulian(tmp);
	updateFebruaryDays(isLeapYear());
	checkValidDate();
}

// Set Gregorian date (using a quite horrible formula)
// given a Julian day number.
void Gregorian::setDateFromJulian(double julian)
{
	int Z = (int) (julian - 1721118.5);
	double R = julian - 1721118.5 - Z;
	double G = Z - 0.25;
	
	int A = int(G / 36524.25);
	int B = A - (A / 4);
	
	currentYear_ = (int) ((B+G) / 365.25);
	
	int C = B + Z - (int)(365.25 * currentYear_);
	currentMonth_ = floor((5 * C + 456) / 153.0);
	currentDay_ = C - floor((153 * currentMonth_ - 457) / 5.0) + R;
	if(currentMonth_ > 12) 
	{
		currentYear_++;
       	currentMonth_ -= 12;
	}

	// Set Day of week 
	currentWeekday_ = int(julian+1.5)%7;	
}

// Adds n to current month, ensures that new date is valid.
//
// n : nr of months to add/subtract
void Gregorian::add_month(int n)
{
	int sign = ((n > 0) ? 1 : -1);
	int nrMonths = abs(n);
	int nextMonth;
	// Subtract/add one month at a time
	for(int i = 0; i < nrMonths; i++)
	{
		nextMonth = currentMonth_ + sign;
		// Have we passed a year?
		if((nextMonth > monthsPerYear_) || (nextMonth < 1))
		{
			currentYear_ += sign;                // Increase/decrease year
			updateFebruaryDays(isLeapYear());	  // Update February days
			nextMonth    -= sign*monthsPerYear_; // Increase/decrease month so that it is correct (i.e. January-1 = December)
		}
		
		// Check if there are less days in next month
		if(daysPerMonths_[(nextMonth-1)] < currentDay_)
		{
			(*this) += GENERIC_MONTH_D; // Add month generic amount of days (30) instead
		}
		else
		{
			currentMonth_ = nextMonth;
		}
	}
	checkValidDate(); // This should probably be removed once sure that everything functions correctly
}

// Adds n to current year, ensures that new date is valid.
//
// n : nr of years to add/subtract
void Gregorian::add_year(int n)
{
	bool isLeapYearBefore = isLeapYear();
	currentYear_ += n;
	bool isLeapYearNow = isLeapYear();
	updateFebruaryDays(isLeapYearNow);
	
	// Only allow leap-year day if we came from a leap year
	// and it is currently a leap year
	if(isLeapYearBefore && !isLeapYearNow)
	{
		// Note: In practice only the 29th of February 
		// is affected by this check
		if(daysPerMonths_[(currentMonth_-1)] < currentDay_)
		{
			currentDay_ = daysPerMonths_[(currentMonth_-1)];
		}
	}
	checkValidDate(); // This should probably be removed once sure that everything functions correctly
}

// Returns the nr of days for current month.
int Gregorian::days_this_month() const
{
	return daysPerMonths_[(currentMonth_-1)];
}

// Returns the name of the current week day.
string Gregorian::week_day_name() const
{
	string result;
	switch(currentWeekday_)
	{
		case(SUNDAY):
		{
			result = "sunday";
			break;
		}
		case(MONDAY):
		{
			result = "monday";
			break;
		}
		case(TUESDAY):
		{
			result = "tuesday";
			break;
		}
		case(WEDNESDAY):
		{
			result = "wednesday";
			break;
		}
		case(THURSDAY):
		{
			result = "thursday";
			break;
		}
		case(FRIDAY):
		{
			result = "friday";
			break;
		}
		case(SATURDAY):
		{
			result = "saturday";
			break;
		}
	}
	return result;
}

// Returns the name of the current month.
string Gregorian::month_name() const
{
	string result;
	switch(currentMonth_)
	{
		case(JANUARY):
		{
			result = "january";
			break;
		}
		case(FEBRUARY):
		{
			result = "february";
			break;
		}
		case(MARCH):
		{
			result = "march";
			break;
		}
		case(APRIL):
		{
			result = "april";
			break;
		}
		case(MAY):
		{
			result = "may";
			break;
		}
		case(JUNE):
		{
			result = "june";
			break;
		}
		case(JULY):
		{
			result = "july";
			break;
		}
		case(AUGUST):
		{
			result = "august";
			break;
		}
		case(SEPTEMBER):
		{
			result = "september";
			break;
		}
		case(OCTOBER):
		{
			result = "october";
			break;
		}
		case(NOVEMBER):
		{
			result = "november";
			break;
		}
		case(DECEMBER):
		{
			result = "december";
			break;
		}
	}
	return result;
}

// Returns true if leap year. Requires (up to) three criterias:
// 1. Year is evenly divisible by 4.
// 2. Year is not divisible by 100, if so => (see next criteria)
// 3. Year is divisble by 400
bool Gregorian::isLeapYear()
{
	return (currentYear_%4 == 0 && (currentYear_%100 != 0 || (currentYear_%400 == 0)));
}

// Updates the nr of days in february.
// (29 if leapyear, 28 otherwise)
void Gregorian::updateFebruaryDays(bool leapYear)
{
	if(leapYear)
	{
		daysPerMonths_[1] = 29;
	}
	else
	{
		daysPerMonths_[1] = 28;
	}
}

// Sets days per month to standard days per month 
// if non-leap year or with 29 days in February if leap-year.
void Gregorian::initializeDaysPerMonths(bool leapYear)
{
	// Months indices & days
	// 0    1    2  3  4  5  6  7  8  9 10 11
	// 31 28/29 31 30 31 30 31 31 30 31 30 31
	
	// Assign January-July [0-7)
	for(size_t i = 0; i < (MONTHS_PER_YEAR-4); i++)
	{
		i++;
		daysPerMonths_[i]   = 30;
		daysPerMonths_[i-1] = 31;
	}
	// Assign August-November [7-11)
	for(size_t i = (MONTHS_PER_YEAR-5); i < (MONTHS_PER_YEAR-1); i++)
	{
		daysPerMonths_[i]   = 31;
		daysPerMonths_[i+1] = 30;
		i++;
	}
	daysPerMonths_[(MONTHS_PER_YEAR-1)] = 31; // Assign December
	
	updateFebruaryDays(leapYear); // Update February accordingly
}


// Calculates the current weekday given year, month and day of the month.
int Gregorian::calculateCurrentWeekday(int year, int month, int monthDay) 
{
	if(month < 3)
	{
		month += 12;
		year--;
	}
	
	double jdn = monthDay + int((153*month - 457)/5.0) + 365*year + floor(year/4) - floor(year/100) + floor(year/400) + 1721118.5;

	return int(jdn+1.5)%7;	
}

// Returns the Julian Day number.
double Gregorian::mod_julian_day() const
{
	int M = currentMonth_;
	int Y = currentYear_;

	if(M < 3)
	{
		M += 12;
		Y--;
	}
	
	return currentDay_ + int((153*M - 457)/5.0) + 365*Y + floor(Y/4) - floor(Y/100) + floor(Y/400) + 1721118.5;
}

// Checks if current date is a valid Gregorian date.
void Gregorian::checkValidDate() 
{
	if(currentMonth_ > MONTHS_PER_YEAR || currentMonth_ < 1 
		|| currentDay_ > daysPerMonths_[currentMonth_ - 1] || currentDay_ < 1)
	{
		throw std::out_of_range("Date out of range.");
	}
}
