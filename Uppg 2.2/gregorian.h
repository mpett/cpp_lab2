/// Description: Implemenation of Date for Gregorian Calender.
/// 
/// Authors: Martin Pettersson, Christoffer Wiss
///
/// Version: 2013-11-01

#pragma once

#include <ostream>
#include <string>
#include "kattistime.h"
#include "date.h"
using std::ostream;
using std::string;

namespace lab2
{
	class Gregorian : public Date
	{
		enum WEEK_DAY_NR
		{
			SUNDAY = 0,
			MONDAY,
			TUESDAY,
			WEDNESDAY,
			THURSDAY,
			FRIDAY,
			SATURDAY,
			DAYS_PER_WEEK
		};
		
		enum MONTH_NR
		{
			JANUARY = 1,
			FEBRUARY,
			MARCH,
			APRIL,
			MAY,
			JUNE,
			JULY,
			AUGUST,
			SEPTEMBER,
			OCTOBER,
			NOVEMBER,
			DECEMBER,
			MONTHS_PER_YEAR = DECEMBER
		};
		
		public:
			// Destructor.
			~Gregorian(){}; 
			
			// Default Constructor (sets date to current date).
			Gregorian();
			
			// Constructor.
			Gregorian(int year, int month, int day);
			
			// Copy constructor.
			Gregorian(const Date &date);
			
			// Construct Gregorian from Date pointer.
			Gregorian(const Date *date);
			
			// Postfix ++ operator; Increases date by one day
			// Julian operator++(int);
			
			// Postfix -- operator; Decreases date by one day
			// Julian operator--(int);
			
			// Returns the nr of days for current month.
			int days_this_month() const;
			
			// Returns the name of the current week day.
			string week_day_name() const;
			
			// Returns the name of the current month.
			string month_name() const;

			// Assignment operator. Assigns from date reference. Returns reference to a new date object.
			Gregorian& operator=(const Date& date);
			
			// Adds n to current year, ensures that new date is valid.
			//
			// n : nr of years to add/subtract
			void add_year(int n = 1);
			
			// Adds n to current month, ensures that new date is valid.
			//
			// n : nr of months to add/subtract
			void add_month(int n = 1);
			
			// Returns Julian day number.
			double mod_julian_day() const;

		private:
			static const int GENERIC_MONTH_D = 30; // Nr of days in a default month, used when one cannot get the same date after adding a month (ex. 31 Jan + 1 month != 31 February)
			int daysPerMonths_[MONTHS_PER_YEAR];
			
			// Checks if current date is a valid Julian date.
			void checkValidDate();
			
			// Sets days per month to standard days per month 
			// if non-leap year or with 29 days per februari if leap-year.
			void initializeDaysPerMonths(bool leapYear);
			
			// Returns true if leap year. Requires (up to) three criterias:
			// 1. Year is evenly divisible by 4.
			// 2. Year is not divisible by 100, if so => (see next criteria)
			// 3. Year is divisble by 400
			bool isLeapYear();
			
			// Set Gregorian date (using a quite horrible formula)
			// given a Julian day number.
			void setDateFromJulian(double julian);

			// Updates the nr of days in february.
			// (29 if leapyear, 28 otherwise)
			void updateFebruaryDays(bool leapYear);
			
			// Calculates the current weekday given year, month and day of the month.
			int calculateCurrentWeekday(int year, int month, int monthDay);
	};
}
