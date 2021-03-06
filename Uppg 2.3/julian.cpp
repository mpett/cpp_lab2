/// Description: Implemenation of Date for Julian Calender.
/// 
/// Authors: Martin Pettersson, Christoffer Wiss
///
/// Version: 2013-11-06

#include "julian.h"
#include <stdexcept>
#include <math.h>

using namespace lab2;

// Constructor.
Julian::Julian(int year, int month, int day) 
: Date(year, month, day, calculateCurrentWeekday(year, month, day), DAYS_PER_WEEK, MONTHS_PER_YEAR)
{
	initializeDaysPerMonths(isLeapYear());
	checkValidDate();
}

// Default Constructor (sets date to current date).
Julian::Julian()
{
	// Gets current time
	time_t currTime;
	k_time(&currTime);
	
	struct tm *t = gmtime(&currTime);
	convertFromGregorianToJulian(t->tm_year + 1900,t->tm_mon + 1,t->tm_mday);
	daysPerWeek_   = DAYS_PER_WEEK;
	monthsPerYear_ = MONTHS_PER_YEAR;
	initializeDaysPerMonths(isLeapYear());
	checkValidDate();
}

// Copy constructor.
Julian::Julian(const Date &date)
{
    double tmp = date.mod_julian_day();
	setDateFromJulian(tmp);
	daysPerWeek_   = DAYS_PER_WEEK;
	monthsPerYear_ = MONTHS_PER_YEAR;
	initializeDaysPerMonths(isLeapYear());
	checkValidDate();
}

// Construct Julian from Date pointer.
Julian::Julian(const Date *date)
{
    double tmp = date->mod_julian_day();
	setDateFromJulian(tmp);
	daysPerWeek_   = DAYS_PER_WEEK;
	monthsPerYear_ = MONTHS_PER_YEAR;
	initializeDaysPerMonths(isLeapYear());
	checkValidDate();
}

// Assignment operator. Assigns from date reference. Returns reference to a new date object.
Julian& Julian::operator=(const Date& date)
{
    double tmp = date.mod_julian_day();
	setDateFromJulian(tmp);
	updateFebruaryDays(isLeapYear());
	checkValidDate();
    return (*this);
}

// Used for converting a Gregorian date (given by current time) to a Julian Date.
void Julian::convertFromGregorianToJulian(int year, int month, int monthDay)
{
	if(month < 3)
	{
		month += 12;
		year--;
	}	
	// Calculate Julian Day Number using Gregorian conversion formula
    double jdn = monthDay + int((153*month - 457)/5.0) + 365*year + floor(year/4) - floor(year/100) + floor(year/400) + 1721118 - 2400000;
	setDateFromJulian(jdn);
}

// Set Julian date (using a quite dreadful formula) 
// given a Julian day number.
void Julian::setDateFromJulian(double modjulian)
{
    double julian = modjulian + 2400000.5;

	int Z = (int) (julian - 1721116.5);
	double R = julian - 1721116.5 - Z;
	currentYear_ = (int) ((Z - 0.25) / 365.25);
	int C = Z - (int)(365.25 * currentYear_);
	currentMonth_ = floor((5 * C + 456) / 153.0);
	currentDay_ = C - floor((153 * currentMonth_ - 457) / 5.0) + R;
	if(currentMonth_ > 12) 
	{
		currentYear_++;
       	currentMonth_ -= 12;
	}

    int res = int(julian+1.5)%7;
    if(res == 0) res = 7;

	// Set Day of week
    currentWeekday_ = res;
}

// Sets the date's year, month and day to input.
void Julian::set_date(int year, int month, int day)
{
	currentYear_  = year;
	currentMonth_ = month;
	currentDay_   = day;
	updateFebruaryDays(isLeapYear());
	currentWeekday_ = calculateCurrentWeekday(currentYear_, currentMonth_, currentDay_);
	checkValidDate(); 
}

// Postfix ++ operator; Increases date by one day.
// Returns a copy of self (before change was applied)
Julian Julian::operator++(int)
{
    Julian res = (*this);
    Date::operator++();
    return res;
}

// Postfix -- operator; Decreases date by one day.
// Returns a copy of self (before change was applied)
Julian Julian::operator--(int)
{
    Julian res = (*this);
    Date::operator--();
    return res;
}

// Adds n to current month, ensures that new date is valid.
//
// n : nr of months to add/subtract
void Julian::add_month(int n)
{
    int sign = ((n > 0) ? 1 : -1);
	int nrMonths = abs(n);
    int nextMonth = currentMonth_;
	// Subtract/add one month at a time
	for(int i = 0; i < nrMonths; i++)
	{
        nextMonth += sign;
		// Have we passed a year?
		if((nextMonth > monthsPerYear_) || (nextMonth < 1))
		{
			currentYear_ += sign;                // Increase/decrease year
            updateFebruaryDays(isLeapYear());	 // Update February days
			nextMonth    -= sign*monthsPerYear_; // Increase/decrease month so that it is correct (i.e. January-1 = December)
		}

		// Check if there are less days in next month
		if(daysPerMonths_[(nextMonth-1)] < currentDay_)
		{
            operator+=(sign*GENERIC_MONTH_D); // Add month generic amount of days (30) instead
			nextMonth = currentMonth_;
		}
		else
		{
			currentMonth_ = nextMonth;
		}
	}
    currentWeekday_ = calculateCurrentWeekday(currentYear_, currentMonth_, currentDay_);
	checkValidDate(); // This should probably be removed once sure that everything functions correctly
}

// Adds n to current year, ensures that new date is valid.
//
// n : nr of years to add/subtract
void Julian::add_year(int n)
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
    currentWeekday_ = calculateCurrentWeekday(currentYear_, currentMonth_, currentDay_);
	checkValidDate(); // This should probably be removed once sure that everything functions correctly
}

// Returns the nr of days for current month.
int Julian::days_this_month() const
{
	return daysPerMonths_[(currentMonth_-1)];
}

// Returns the name of the current week day.
string Julian::week_day_name() const
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
string Julian::month_name() const
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

// Returns true if leap year (year dividable by 4) otherwise false.
bool Julian::isLeapYear()
{
	return (currentYear_%4 == 0);
}

// Updates the nr of days in february.
// (29 if leapyear, 28 otherwise)
void Julian::updateFebruaryDays(bool leapYear)
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
void Julian::initializeDaysPerMonths(bool leapYear)
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
int Julian::calculateCurrentWeekday(int year, int month, int monthDay)
{
	if(month < 3)
	{
		month += 12;
		year--;
	}
	
    double jdn = (int) (365.25 * (year + 4716)) + (int)(30.6001 * (month + 1)) + monthDay + 0.5 - 1524.5;

    int res = int(jdn+1.5)%7;
    if(res == 0) res = 7;

    return res;
}

// Returns Julian day number.
int Julian::mod_julian_day() const
{
	int M = currentMonth_;
	int Y = currentYear_;

	if(M < 3)
	{
		M += 12;
		Y--;
	}
	
    return (int) (365.25 * (Y + 4716)) + (int)(30.6001 * (M + 1)) + currentDay_ - 1524 - 2400000 -1;
}

// Checks if current date is a valid Julian date.
void Julian::checkValidDate() 
{
    if(currentMonth_ > MONTHS_PER_YEAR || currentMonth_ < 1
        || currentDay_ > daysPerMonths_[currentMonth_ - 1] || currentDay_ < 1
        || currentYear_ < MIN_YEAR || currentYear_ > MAX_YEAR)
    {
        throw std::out_of_range("Date out of range.");
    }
}
