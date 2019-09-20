#include "stdafx.h"

#include <iostream>
#include <sstream>
#include <limits>
#include <cassert>

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fraction.h //
////////////////

class Fraction
{
public:

   Fraction() = default;
   Fraction(int nominator) : m_nominator(nominator) { } // Implicit constructor is reasonable for this class
   Fraction(int nominator, int denominator);

   int nominator() const { return m_nominator; }
   int denominator() const { return m_denominator; }
   bool operator ! () const { return !m_nominator; };
   explicit operator double() const { return double(m_nominator) / m_denominator; }
   Fraction operator - () const { return{ -m_nominator, m_denominator }; } // unary minus

private:
   int m_nominator = 0;
   int m_denominator = 1;
};

std::ostream& operator << (std::ostream&, const Fraction&);
std::istream& operator >> (std::istream&, Fraction&);

Fraction operator + (const Fraction&, const Fraction&);
Fraction operator - (const Fraction&, const Fraction&);
Fraction operator * (const Fraction&, const Fraction&);
Fraction operator / (const Fraction&, const Fraction&);

bool operator == (const Fraction&, const Fraction&);
bool operator != (const Fraction&, const Fraction&);
bool operator < (const Fraction&, const Fraction&);
bool operator > (const Fraction&, const Fraction&);
bool operator <= (const Fraction&, const Fraction&);
bool operator >= (const Fraction&, const Fraction&);

int reducing_factor(const Fraction&);
inline bool is_reduced(const Fraction& f) { return reducing_factor(f) == 1; }
Fraction reduce(const Fraction&);
inline Fraction inverse(const Fraction& f) { return{ f.denominator(), f.nominator() }; }
std::string to_string(const Fraction&);
Fraction parse_fraction(const std::string&);

////////////////////////////////////////////////////////////////////////////////////////
// main.cpp //
///////////////////////

namespace
{
// A few testing tools
void ut_assert(bool condition, const std::string& assertionText)
{
   if (condition)
   {
      std::cout << "[OK]: " << assertionText << std::endl;
   }
   else
   {
      std::cout << "[Assertion Failed]: " << assertionText << std::endl;
   }
}

#define UT_ASSERT(condition) ut_assert(condition, #condition)
#define UT_ASSERT_EQUAL(expected, actual) ut_assert((expected) == (actual), #expected " == " #actual)
#define UT_ASSERT_EXCEPTION(expr, type) \
{ \
   bool success = false; \
   try { expr; } \
   catch(const type& ex) \
   { \
      std::cout << "[Expected Exception]: " #expr " -> " #type ": " << ex.what() << std::endl; \
      success = true; \
   } \
   catch(...) {} \
   if (!success) \
   { \
      std::cout << "[Exception Expectations Broken]: " << #type << std::endl; \
   } \
}

void test_fraction_output()
{
   UT_ASSERT_EQUAL("0", to_string(Fraction()));
   UT_ASSERT_EQUAL("42", to_string(Fraction(42)));
   UT_ASSERT_EQUAL("1/2", to_string(Fraction(1, 2)));
   UT_ASSERT_EQUAL("5/10", to_string(Fraction(5, 10)));
   UT_ASSERT_EQUAL("10/5", to_string(Fraction(10, 5)));
   UT_ASSERT_EQUAL("5/-10", to_string(Fraction(5, -10)));
   UT_ASSERT_EQUAL("-5/10", to_string(Fraction(-5, 10)));
   UT_ASSERT_EQUAL("-5/-10", to_string(Fraction(-5, -10)));

   UT_ASSERT_EXCEPTION(Fraction(1, 0), std::logic_error);
}

void test_fraction_input()
{
   UT_ASSERT_EQUAL(Fraction(), parse_fraction("0"));
   UT_ASSERT_EQUAL(Fraction(10), parse_fraction("10"));
   UT_ASSERT_EQUAL(Fraction(10, 2), parse_fraction("10/2"));
   UT_ASSERT_EQUAL(Fraction(10, 2), parse_fraction("10  /  2"));
   UT_ASSERT_EQUAL(Fraction(15, 3), parse_fraction("10/2"));
   UT_ASSERT_EQUAL(Fraction(5), parse_fraction("10/2"));
   UT_ASSERT_EQUAL(5, parse_fraction("10/2"));

   UT_ASSERT_EXCEPTION(parse_fraction("2/0"), std::logic_error);
}

void test_fraction_comparison()
{
   UT_ASSERT_EQUAL(0, Fraction());
   UT_ASSERT_EQUAL(0, Fraction(0));
   UT_ASSERT_EQUAL(0, Fraction(0, 42));
   UT_ASSERT_EQUAL(Fraction(1, 2), Fraction(1, 2));
   UT_ASSERT_EQUAL(Fraction(-1, 2), Fraction(-2, 4));
   UT_ASSERT_EQUAL(Fraction(1, -2), Fraction(-3, 6));
   UT_ASSERT_EQUAL(Fraction(-4, 8), Fraction(3, -6));
   UT_ASSERT_EQUAL(Fraction(-4, -8), Fraction(3, 6));
   UT_ASSERT_EQUAL(Fraction(3, 1), Fraction(6, 2));
   UT_ASSERT_EQUAL(Fraction(3, 1), Fraction(9, 3));
   UT_ASSERT_EQUAL(Fraction(3, 1), Fraction(3));
   UT_ASSERT_EQUAL(Fraction(3, 1), 3);

   UT_ASSERT(Fraction(1, 3) != Fraction(3, 10));
   UT_ASSERT(Fraction(1, 3) > Fraction(3, 10));
   UT_ASSERT(Fraction(1, 3) >= Fraction(3, 10));
   UT_ASSERT(Fraction(3, 10) != Fraction(1, 3));
   UT_ASSERT(Fraction(3, 10) < Fraction(1, 3));
   UT_ASSERT(Fraction(3, 10) <= Fraction(1, 3));
}

void test_fraction_reducing()
{
   // reduced forms
   UT_ASSERT(is_reduced(-10));
   UT_ASSERT(is_reduced(0));
   UT_ASSERT(is_reduced(10));
   UT_ASSERT(is_reduced(Fraction(1, 2)));
   UT_ASSERT(is_reduced(Fraction(-1, 2)));

   // not reduced forms
   UT_ASSERT(!is_reduced(Fraction(1, -2)));
   UT_ASSERT(!is_reduced(Fraction(-1, -2)));
   UT_ASSERT(!is_reduced(Fraction(2, 4)));

   UT_ASSERT_EQUAL(1, reducing_factor(Fraction(-1, 2)));
   UT_ASSERT_EQUAL(-1, reducing_factor(Fraction(1, -2)));
   UT_ASSERT_EQUAL(-1, reducing_factor(Fraction(-1, -2)));
   UT_ASSERT_EQUAL(2, reducing_factor(Fraction(2, 4)));

   UT_ASSERT_EQUAL("-1/2", to_string(reduce(Fraction(1, -2))));
   UT_ASSERT_EQUAL("1/2", to_string(reduce(Fraction(-1, -2))));
   UT_ASSERT_EQUAL("1/2", to_string(reduce(Fraction(2, 4))));
   UT_ASSERT_EQUAL("-1/2", to_string(reduce(Fraction(-2, 4))));
   UT_ASSERT_EQUAL("-1/2", to_string(reduce(Fraction(2, -4))));
   UT_ASSERT_EQUAL("1/2", to_string(reduce(Fraction(-2, -4))));
}

void test_fraction_arithmetics()
{
   UT_ASSERT_EQUAL(1, Fraction(2, 3) + Fraction(1, 3));
   UT_ASSERT_EQUAL(Fraction(5, 6), Fraction(1, 2) + Fraction(1, 3));
   UT_ASSERT_EQUAL(Fraction(13, 3), 4 + Fraction(1, 3));
   UT_ASSERT_EQUAL(Fraction(13, 3), 4 + Fraction(1, 3));

   UT_ASSERT_EQUAL(Fraction(1, 6), Fraction(1, 2) - Fraction(1, 3));
   UT_ASSERT_EQUAL(9 + Fraction(1, 3), 10 - Fraction(6, 9));

   UT_ASSERT_EQUAL(Fraction(-1, 6), Fraction(1, -2) * Fraction(1, 3));
   UT_ASSERT_EQUAL(-2, Fraction(1, 2) / Fraction(5, -20));

   UT_ASSERT_EQUAL("1/3", to_string(Fraction(5, 6) - Fraction(7, 10) + Fraction(1, 5)));
}

} // namespace

int main()
try
{
   test_fraction_output();
   test_fraction_input();
   test_fraction_comparison();
   test_fraction_reducing();
   test_fraction_arithmetics();
}
catch (const std::exception& ex)
{
   std::cerr << "[Unhandled exception]: " << ex.what() << std::endl;
}

///////////////////////////////////////////////////////////////////////////
// Fraction.cpp //
//////////////////

namespace
{

int64_t greatest_common_divider(int64_t a, int64_t b)
{
   return b ? greatest_common_divider(b, a % b) : std::abs(a);
}

int least_common_multiply(int a, int b)
{
   const int64_t gcd = greatest_common_divider(a, b);
   const int64_t lcm = a / gcd * int64_t(b);

   if (lcm < std::numeric_limits<int>::min() || std::numeric_limits<int>::max() < lcm)
      throw std::out_of_range("The least common multiply is out of range");

   return int(lcm);
}

int64_t reducing_factor(int64_t nominator, int64_t denominator)
{
   assert(denominator != 0);
   const auto gcd = greatest_common_divider(nominator, denominator);
   return denominator > 0 ? gcd : -gcd;
}

} // namespace

Fraction::Fraction(int nominator, int denominator)
   : m_nominator(nominator)
   , m_denominator(denominator)
{
   if (!denominator)
   {
      throw std::logic_error("Zero denominator");
   }
}

std::ostream& operator << (std::ostream& output, const Fraction& f)
{
   output << f.nominator();
   if (f.denominator() != 1)
   {
      output << '/' << f.denominator();
   }
   return output;
}

std::istream& operator >> (std::istream& input, Fraction& f)
{
   int nominator = 0;
   int denominator = 1;

   if (input >> nominator)
   {
      char symbol = 0;
      if (input >> symbol)
      {
         if (symbol == '/')
         {
            input >> denominator;
         }
         else
         {
            input.unget();
         }
      }
   }
   f = Fraction(nominator, denominator);
   return input;
}

Fraction operator + (const Fraction& lhs, const Fraction& rhs)
{
   const int common_denominator = std::abs(least_common_multiply(lhs.denominator(), rhs.denominator()));
   return reduce ({
      lhs.nominator() * (common_denominator / lhs.denominator()) +
      rhs.nominator() * (common_denominator / rhs.denominator()),
      common_denominator
   });
}

Fraction operator - (const Fraction& lhs, const Fraction& rhs)
{
   return lhs + -rhs;
}

Fraction operator * (const Fraction& lhs, const Fraction& rhs)
{
   int64_t nominator = int64_t(lhs.nominator()) * rhs.nominator();
   int64_t denominator = int64_t(lhs.denominator()) * rhs.denominator();
   int64_t gcd = greatest_common_divider(nominator, denominator);
   if (denominator < 0)
   {
      gcd = -gcd;
   }
   return{ int(nominator / gcd), int(denominator / gcd) };
}

Fraction operator / (const Fraction& lhs, const Fraction& rhs)
{
   return lhs * inverse(rhs);
}

bool operator == (const Fraction& lhs, const Fraction& rhs)
{
   return int64_t(lhs.nominator()) * rhs.denominator() == int64_t(lhs.denominator()) * rhs.nominator();
}

bool operator < (const Fraction& lhs, const Fraction& rhs)
{
   return double(lhs) < double(rhs);
}

bool operator != (const Fraction& lhs, const Fraction& rhs) { return !(lhs == rhs); }
bool operator > (const Fraction& lhs, const Fraction& rhs) { return rhs < lhs; }
bool operator <= (const Fraction& lhs, const Fraction& rhs) { return !(rhs < lhs); }
bool operator >= (const Fraction& lhs, const Fraction& rhs) { return !(lhs < rhs); }

int reducing_factor(const Fraction& f)
{
   return int(reducing_factor(f.nominator(), f.denominator()));
}

Fraction reduce(const Fraction& f)
{
   const int rf = reducing_factor(f);
   return { f.nominator() / rf, f.denominator() / rf };
}

std::string to_string(const Fraction& f)
{
   std::ostringstream output;
   output << f;
   return output.str();
}

Fraction parse_fraction(const std::string& s)
{
   std::istringstream input(s);
   Fraction f;
   input >> f;
   return f;
}

