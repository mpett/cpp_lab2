/// Description: Base abstract class for Date. 
/// Date contains variables for year, day, days per week and months per year.
/// Also contains a set of base functions for handling and modifying these.
/// 
/// Authors: Martin Pettersson, Christoffer Wiss
///
/// Version: 2013-11-06

#pragma once

#include <ostream>
#include <string>
using std::ostream;
using std::string;

namespace lab2
{
	class Date
	{
		public:
			// Destructor (deallocate allocated memory here).
			virtual ~Date();
		
            // Default Constructor.
			Date() = default; 
			
            // Constructor.
			Date(int year, int month, int day, int weekDay, int daysPerWeek, int monthsPerYear) : 
				currentYear_(year), currentMonth_(month), currentDay_(day), currentWeekday_(weekDay), daysPerWeek_(daysPerWeek), monthsPerYear_(monthsPerYear) {} ;

			// Returns the current year.
			int year() const;
			
			// Returns the current month nr.
			int month() const;
			
			// Returns the current month day nr.
			int day() const;
			
			// Returns the current week day nr.
			int week_day() const;
			
			// Returns the nr of days per week.
			int days_per_week() const;
			
			// Returns the nr of days for current month
			virtual int days_this_month() const = 0;
			
			// Returns the nr of months per year.
			int months_per_year() const;
			
			// Returns the name of the current week day
			virtual string week_day_name() const = 0;
			
			// Returns the name of the current month
			virtual string month_name() const = 0;

			// Assignment operator. Assigns from date reference. Returns reference to a new date object.
			virtual Date& operator=(const Date& date);

			// Prefix ++ operator; Increases date by one day.
			Date& operator++();
			
			// Prefix -- operator; Decreases date by one day.
			Date& operator--();
			
			// Compound += operator; Adds val number of days to current date. 
			Date& operator+=(int val);
			
			// Compound -= operator; Subtracts val number of days from current date. 
			Date& operator-=(int val);

			// Adds n to current year. 
			// n : can be positive or negative.
			virtual void add_year(int n = 1);
			
			// Adds n to current month
			virtual void add_month(int n = 1) = 0;
			
			// Sets the date's year, month and day to input.
			virtual void set_date(int year, int month, int day) = 0;
			
			// Checks if two dates are the same.
			// This is done by first converting both dates to their Julian Day number
			// (in order to provide a common base) and then do the comparison.
			// Returns true if so, otherwise false.
			bool operator==(const Date& rhs) const;
			
			// Checks if two dates are different.
			// This is done by first converting both dates to their Julian Day number
			// (in order to provide a common base) and then do the comparison.
			// Returns true if so, otherwise false.
			bool operator!=(const Date& rhs) const;
			
			// Checks if date is less than the right hand side date.
			// This is done by first converting both dates to their Julian Day number
			// (in order to provide a common base) and then do the comparison.
			// Returns true if so, otherwise false.
			bool operator<(const Date& rhs) const;
			
			// Checks if date is less than or equal to the right hand side date.
			// This is done by first converting both dates to their Julian Day number
			// (in order to provide a common base) and then do the comparison.
			// Returns true if so, otherwise false.
			bool operator<=(const Date& rhs) const;
			
			// Checks if date is larger than the right hand side date.
			// This is done by first converting both dates to their Julian Day number
			// (in order to provide a common base) and then do the comparison.
			// Returns true if so, otherwise false.
			bool operator>(const Date& rhs) const;

			// Checks if date is larger than or equal the right hand side date.
			// This is done by first converting both dates to their Julian Day number
			// (in order to provide a common base) and then do the comparison.
			// Returns true if so, otherwise false.
			bool operator>=(const Date& rhs) const;
			
			// Returns the integer differens between two dates.
			// This is done by first converting both dates to their Julian Day number
			// (in order to provide a common base) and then do the subtraction.
			int operator-(const Date& rhs) const;
			
            // Returns number of days from modified julian day.
            virtual int mod_julian_day() const = 0;
		
		protected:
			int currentYear_;
			int currentMonth_;
			int currentDay_;
			int currentWeekday_;
			int daysPerWeek_;
			int monthsPerYear_;
	};
	
	// Puts a text string representation of the date 
	// to outputstream object. Format: YYYY-MM-DD
	ostream& operator<<(ostream& os, const Date & date);
} 
