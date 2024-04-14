/*
    BigInt class.h

    BigInt Library for C++

    MIT License

    Created by Roshan Gupta on 16-10-2020
    Copyright (c) 2020 Roshan Gupta

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <math.h>
#include <limits>

class BigInt {
private:
    std::string str; // only data memeber for strong Big Integer as String. [For signed int, str[0] = '-']


    // Function Definitions for Internal Uses

    static std::string trim(std::string);
    static std::string add(std::string, std::string);
    static std::string subtract(std::string, std::string);
    static std::string multiply(std::string, std::string);
    static std::string divide(std::string, std::string);

    static std::string mod(std::string, std::string);
    static std::string shortDivide(std::string, unsigned long long int);
    static std::string maximum(std::string, std::string);
    static bool is_maximum(std::string, std::string);
    static bool is_strictlyMaximum(std::string, std::string);
    static std::string minimum(std::string, std::string);
    static bool is_minimum(std::string, std::string);
    static bool is_strictlyMinimum(std::string, std::string);
    static bool is_BigInt(std::string);

    // Public Property

    static std::string abs(std::string);
    static std::string pow(std::string, std::string);
    static std::string sqrt(std::string);
    static std::string log2(std::string);
    static std::string log10(std::string);
    static std::string logwithbase(std::string, std::string);
    static std::string antilog2(std::string);
    static std::string antilog10(std::string);
    static void swap(std::string&, std::string&);
    static std::string reverse(std::string);
    static std::string gcd(std::string, std::string);
    static std::string lcm(std::string, std::string);
    static std::string fact(std::string);
    static bool isPalindrome(std::string);
    static bool isPrime(std::string);


public:
    // Constructors for big int.
    BigInt() {
        str = '0';  //default value
    }
    BigInt(std::string s) {
        if(!is_BigInt(s))
            throw std::runtime_error("Invalid Big Integer has been fed.");   // if the input string is not valid number.
        str = s;
    }
    BigInt(long long int n) {
        str = std::to_string(n);
    }
    BigInt(int n) {
        str = std::to_string(n);
    }
    BigInt(long int n) {
        str = std::to_string(n);
    }
    BigInt(const BigInt &n) {
        str = n.str;
    }

    // operator overloading for output stream {<<}
    friend std::ostream &operator << (std::ostream& stream, const BigInt &n) {
        stream << n.str;
        return stream;
    }

    // operator overloading for input stream {>>}
    friend std::istream &operator >> (std::istream& stream, BigInt &n) {
        stream >> n.str;
        return stream;
    }

    /* Operator {+} Overloadings, for different kind of
    parameter for the programmer's convinience  */

    BigInt operator + (BigInt const &n) {
        BigInt ans;
        ans.str = add(str, n.str);
        return ans;
    }
    friend BigInt operator + (BigInt const &n1, int n2) {
        BigInt ans;
        ans.str = add(n1.str, std::to_string(n2));
        return ans;
    }
    friend BigInt operator + (int n1, BigInt const &n2) {
        BigInt ans;
        ans.str = add(n2.str, std::to_string(n1));
        return ans;
    }
    friend BigInt operator + (BigInt const &n1, long int n2) {
        BigInt ans;
        ans.str = add(n1.str, std::to_string(n2));
        return ans;
    }
    friend BigInt operator + (long int n1, BigInt const &n2) {
        BigInt ans;
        ans.str = add(n2.str, std::to_string(n1));
        return ans;
    }
    friend BigInt operator + (BigInt const &n1, long long int n2) {
        BigInt ans;
        ans.str = add(n1.str, std::to_string(n2));
        return ans;
    }
    friend BigInt operator + (long long int n1, BigInt const &n2) {
        BigInt ans;
        ans.str = add(n2.str, std::to_string(n1));
        return ans;
    }

    // Extra shortcut feature

    BigInt& operator += (BigInt const n) {
        (*this).str = add((*this).str, n.str);
        return (*this);
    }



    /* Operator {-} Overloadings, for different kind of
    parameter for the programmer's convinience  */

    BigInt operator - (BigInt const &n) {
        BigInt ans;
        ans.str = subtract(str, n.str);
        return ans;
    }

    friend BigInt operator - (BigInt const &n1, int n2) {
        BigInt ans;
        ans.str = subtract(n1.str, std::to_string(n2));
        return ans;
    }
    friend BigInt operator - (int n1, BigInt const &n2) {
        BigInt ans;
        ans.str = subtract(std::to_string(n1), n2.str);
        return ans;
    }
    friend BigInt operator - (BigInt const &n1, long int n2) {
        BigInt ans;
        ans.str = subtract(n1.str, std::to_string(n2));
        return ans;
    }
    friend BigInt operator - (long int n1, BigInt const &n2) {
        BigInt ans;
        ans.str = subtract(std::to_string(n1), n2.str);
        return ans;
    }
    friend BigInt operator - (BigInt const &n1, long long int n2) {
        BigInt ans;
        ans.str = subtract(n1.str, std::to_string(n2));
        return ans;
    }
    friend BigInt operator - (long long int n1, BigInt const &n2) {
        BigInt ans;
        ans.str = subtract(std::to_string(n1), n2.str);
        return ans;
    }

    // Extra shortcut feature

    BigInt& operator -= (BigInt const n) {
        (*this).str = subtract((*this).str, n.str);
        return (*this);
    }

    /* Operator {*} Overloadings, for different kind of
    parameter for the programmer's convinience  */

    BigInt operator * (BigInt const &n) {
        BigInt ans;
        ans.str = multiply(str, n.str);
        return ans;
    }
    friend BigInt operator * (BigInt const &n1, int n2) {
        BigInt ans;
        ans.str = multiply(n1.str, std::to_string(n2));
        return ans;
    }
    friend BigInt operator * (int n1, BigInt const &n2) {
        BigInt ans;
        ans.str = multiply(std::to_string(n1), n2.str);
        return ans;
    }
    friend BigInt operator * (BigInt const &n1, long int n2) {
        BigInt ans;
        ans.str = multiply(n1.str, std::to_string(n2));
        return ans;
    }
    friend BigInt operator * (long int n1, BigInt const &n2) {
        BigInt ans;
        ans.str = multiply(std::to_string(n1), n2.str);
        return ans;
    }
    friend BigInt operator * (BigInt const &n1, long long int n2) {
        BigInt ans;
        ans.str = multiply(n1.str, std::to_string(n2));
        return ans;
    }
    friend BigInt operator * (long long int n1, BigInt const &n2) {
        BigInt ans;
        ans.str = multiply(std::to_string(n1), n2.str);
        return ans;
    }
    // Extra shortcut feature

    BigInt& operator *= (BigInt const n) {
        (*this).str = multiply((*this).str, n.str);
        return (*this);
    }

    /* Operator {/} Overloadings, for different kind of
    parameter for the programmer's convinience  */

    BigInt operator / (BigInt const &n) {
        BigInt ans;
        ans.str = divide(str, n.str);
        return ans;
    }
    friend BigInt operator / (BigInt const &n1, int n2) {
        BigInt ans;
        ans.str = divide(n1.str, std::to_string(n2));
        return ans;
    }
    friend BigInt operator / (int n1, BigInt const &n2) {
        BigInt ans;
        ans.str = divide(std::to_string(n1), n2.str);
        return ans;
    }
    friend BigInt operator / (BigInt const &n1, long int n2) {
        BigInt ans;
        ans.str = divide(n1.str, std::to_string(n2));
        return ans;
    }
    friend BigInt operator / (long int n1, BigInt const &n2) {
        BigInt ans;
        ans.str = divide(std::to_string(n1), n2.str);
        return ans;
    }
    friend BigInt operator / (BigInt const &n1, long long int n2) {
        BigInt ans;
        ans.str = divide(n1.str, std::to_string(n2));
        return ans;
    }
    friend BigInt operator / (long long int n1, BigInt const &n2) {
        BigInt ans;
        ans.str = divide(std::to_string(n1), n2.str);
        return ans;
    }

    // Extra shortcut feature

    BigInt& operator /= (BigInt const n) {
        (*this).str = divide((*this).str, n.str);
        return (*this);
    }

    /* Operator {%} Overloadings, for different kind of
    parameter for the programmer's convinience  */

    BigInt operator % (BigInt const &n) {
        BigInt ans;
        ans.str = mod(str, n.str);
        return ans;
    }
    friend BigInt operator % (BigInt const &n1, int n2) {
        BigInt ans;
        ans.str = mod(n1.str, std::to_string(n2));
        return ans;
    }
    friend BigInt operator % (int n1, BigInt const &n2) {
        BigInt ans;
        ans.str = mod(std::to_string(n1), n2.str);
        return ans;
    }
    friend BigInt operator % (BigInt const &n1, long int n2) {
        BigInt ans;
        ans.str = mod(n1.str, std::to_string(n2));
        return ans;
    }
    friend BigInt operator % (long int n1, BigInt const &n2) {
        BigInt ans;
        ans.str = mod(std::to_string(n1), n2.str);
        return ans;
    }
    friend BigInt operator % (BigInt const &n1, long long int n2) {
        BigInt ans;
        ans.str = mod(n1.str, std::to_string(n2));
        return ans;
    }
    friend BigInt operator % (long long int n1, BigInt const &n2) {
        BigInt ans;
        ans.str = mod(std::to_string(n1), n2.str);
        return ans;
    }

    // Extra shortcut feature

    BigInt& operator %= (BigInt const n) {
        (*this).str = mod((*this).str, n.str);
        return (*this);
    }

    //-------------------------------------------------
    //------------Increment and Decrement--------------
    //-------------------------------------------------

    /* Operator {++} Overloadings
    for the pre incremention  */
    BigInt& operator ++ () {
        (*this).str = add((*this).str, "1");
        return (*this);
    }
    /* Operator {++} Overloadings
    for the post incremention  */
    BigInt operator ++ (int) {
        (*this).str = add((*this).str, "1");
        return (*this);
    }

    /* Operator {--} Overloadings
    for the pre decremention  */
    BigInt& operator -- () {
        (*this).str = subtract((*this).str, "1");
        return (*this);
    }

    /* Operator {--} Overloadings
    for the post incremention  */
    BigInt operator -- (int) {
        (*this).str = subtract((*this).str, "1");
        return (*this);
    }

    //-------------------------------------------------
    //------------Conditional Operators----------------
    //-------------------------------------------------

    /* Operator {>} Overloadings, for different kind of
    parameter for the programmer's convinience  */

    bool operator > (BigInt const &n) {
        return is_strictlyMaximum(str, n.str);
    }
    friend bool operator > (BigInt const &n1, int n2) {
        return is_strictlyMaximum(n1.str, std::to_string(n2));
    }
    friend bool operator > (int n1, BigInt const &n2) {
        return is_strictlyMaximum(std::to_string(n1), n2.str);
    }
    friend bool operator > (BigInt const &n1, long int n2) {
        return is_strictlyMaximum(n1.str, std::to_string(n2));
    }
    friend bool operator > (long int n1, BigInt const &n2) {
        return is_strictlyMaximum(std::to_string(n1), n2.str);
    }
    friend bool operator > (BigInt const &n1, long long int n2) {
        return is_strictlyMaximum(n1.str, std::to_string(n2));
    }
    friend bool operator > (long long int n1, BigInt const &n2) {
        return is_strictlyMaximum(std::to_string(n1), n2.str);
    }

    /* Operator {<} Overloadings, for different kind of
    parameter for the programmer's convinience  */

    bool operator < (BigInt const &n) {
        return is_strictlyMinimum(str, n.str);
    }
    friend bool operator < (BigInt const &n1, int n2) {
        return is_strictlyMinimum(n1.str, std::to_string(n2));
    }
    friend bool operator < (int n1, BigInt const &n2) {
        return is_strictlyMinimum(std::to_string(n1), n2.str);
    }
    friend bool operator < (BigInt const &n1, long int n2) {
        return is_strictlyMinimum(n1.str, std::to_string(n2));
    }
    friend bool operator < (long int n1, BigInt const &n2) {
        return is_strictlyMinimum(std::to_string(n1), n2.str);
    }
    friend bool operator < (BigInt const &n1, long long int n2) {
        return is_strictlyMinimum(n1.str, std::to_string(n2));
    }
    friend bool operator < (long long int n1, BigInt const &n2) {
        return is_strictlyMinimum(std::to_string(n1), n2.str);
    }

    /* Operator {>=} Overloadings, for different kind of
    parameter for the programmer's convinience  */

    bool operator >= (BigInt const &n) {
        return is_maximum(str, n.str);
    }
    friend bool operator >= (BigInt const &n1, int n2) {
        return is_maximum(n1.str, std::to_string(n2));
    }
    friend bool operator >= (int n1, BigInt const &n2) {
        return is_maximum(std::to_string(n1), n2.str);
    }
    friend bool operator >= (BigInt const &n1, long int n2) {
        return is_maximum(n1.str, std::to_string(n2));
    }
    friend bool operator >= (long int n1, BigInt const &n2) {
        return is_maximum(std::to_string(n1), n2.str);
    }
    friend bool operator >= (BigInt const &n1, long long int n2) {
        return is_maximum(n1.str, std::to_string(n2));
    }
    friend bool operator >= (long long int n1, BigInt const &n2) {
        return is_maximum(std::to_string(n1), n2.str);
    }

    /* Operator {<=} Overloadings, for different kind of
    parameter for the programmer's convinience  */

    bool operator <= (BigInt const &n) {
        return is_minimum(str, n.str);
    }
    friend bool operator <= (BigInt const &n1, int n2) {
        return is_minimum(n1.str, std::to_string(n2));
    }
    friend bool operator <= (int n1, BigInt const &n2) {
        return is_minimum(std::to_string(n1), n2.str);
    }
    friend bool operator <= (BigInt const &n1, long int n2) {
        return is_minimum(n1.str, std::to_string(n2));
    }
    friend bool operator <= (long int n1, BigInt const &n2) {
        return is_minimum(std::to_string(n1), n2.str);
    }
    friend bool operator <= (BigInt const &n1, long long int n2) {
        return is_minimum(n1.str, std::to_string(n2));
    }
    friend bool operator <= (long long int n1, BigInt const &n2) {
        return is_minimum(std::to_string(n1), n2.str);
    }


    /* Operator {==} Overloadings, for different kind of
    parameter for the programmer's convinience  */

    bool operator ==(BigInt const &n) {
        return (*this).str == n.str;
    }
    friend bool operator == (BigInt const &n1, int n2) {
        return n1.str == std::to_string(n2);
    }
    friend bool operator == (int n1, BigInt const &n2) {
        return std::to_string(n1) == n2.str;
    }
    friend bool operator == (BigInt const &n1, long int n2) {
        return n1.str == std::to_string(n2);
    }
    friend bool operator == (long int n1, BigInt const &n2) {
        return std::to_string(n1) == n2.str;
    }
    friend bool operator == (BigInt const &n1, long long int n2) {
        return n1.str == std::to_string(n2);
    }
    friend bool operator == (long long int n1, BigInt const &n2) {
        return std::to_string(n1) == n2.str;
    }


    /* Operator {!=} Overloadings, for different kind of
    parameter for the programmer's convinience  */

    bool operator !=(BigInt const &n) {
        return (*this).str != n.str;
    }
    friend bool operator != (BigInt const &n1, int n2) {
        return n1.str != std::to_string(n2);
    }
    friend bool operator != (int n1, BigInt const &n2) {
        return std::to_string(n1) != n2.str;
    }
    friend bool operator != (BigInt const &n1, long int n2) {
        return n1.str != std::to_string(n2);
    }
    friend bool operator != (long int n1, BigInt const &n2) {
        return std::to_string(n1) != n2.str;
    }
    friend bool operator != (BigInt const &n1, long long int n2) {
        return n1.str != std::to_string(n2);
    }
    friend bool operator != (long long int n1, BigInt const &n2) {
        return std::to_string(n1) != n2.str;
    }

    //-----------------------------------------------------------
    //--------Function Definitions for External Uses-------------
    //-----------------------------------------------------------


    static BigInt _big_max(BigInt &a, BigInt &b) {     // returns the maximum value between two Big Integers.
        BigInt ans;
        ans.str = maximum(a.str, b.str);
        return ans;
    }
    static BigInt _big_min(BigInt &a, BigInt &b) {      // returns the minimum value between two Big Integers.
        BigInt ans;
        ans.str = minimum(a.str, b.str);
        return ans;
    }
    static BigInt _big_abs(BigInt &a) {                 // returns the absolute value of Big Integer.
        BigInt ans;
        ans.str = abs(a.str);
        return ans;
    }
    static BigInt _big_pow(BigInt &a, BigInt &b) {      // returns the power value between two Big Integers i.e., a^b, ^ -> power
        BigInt ans;
        ans.str = pow(a.str, b.str);
        return ans;
    }
    static BigInt _big_sqrt(BigInt &a) {                // returns the square root value of Big Integer.
        BigInt ans;
        ans.str = sqrt(a.str);
        return ans;
    }
    static BigInt _big_log2(BigInt &a) {                // returns the log of Big Integer to the base of 2.
        BigInt ans;
        ans.str = log2(a.str);
        return ans;
    }
    static BigInt _big_log10(BigInt &a) {               // returns the log of Big Integer to the base of 10.
        BigInt ans;
        ans.str = log10(a.str);
        return ans;
    }
    static BigInt _big_logwithbase(BigInt &a, BigInt &b) {  // returns the log of Big Integer(a) to the base of (b).
        BigInt ans;
        ans.str = logwithbase(a.str, b.str);
        return ans;
    }
    static BigInt _big_antilog2(BigInt &a) {        // returns the antilog of Big Integer to the base of 2.
        BigInt ans;
        ans.str = antilog2(a.str);
        return ans;
    }
    static BigInt _big_antilog10(BigInt &a) {       // returns the antilog of Big Integer to the base of 10.
        BigInt ans;
        ans.str = antilog10(a.str);
        return ans;
    }
    static void _big_swap(BigInt &a, BigInt &b) {     // swaps the two Big Integers.
        swap(a.str, b.str);
    }
    static BigInt _big_reverse(BigInt &a) {             //Reverses the Big Integer.
        BigInt ans;
        ans.str = reverse(a.str);
        return ans;
    }
    static BigInt _big_gcd(BigInt &a, BigInt &b) {         // returns the gcd of Big Integers a and b.
        BigInt ans;
        ans.str = gcd(a.str, b.str);
        return ans;
    }
    static BigInt _big_lcm(BigInt &a, BigInt &b) {          // returns the lcm of Big Integers a and b.
        BigInt ans;
        ans.str = lcm(a.str, b.str);
        return ans;
    }
    static BigInt _big_fact(BigInt &a) {                    // returns the factorial of Big Integer.
        BigInt ans;
        ans.str = fact(a.str);
        return ans;
    }
    static bool _big_isPalindrome(BigInt &a) {              // Check if the Big Integer is Palindromic Integer.
        return isPalindrome(a.str);
    }
    static bool _big_isPrime(BigInt &a) {                   // Check if the Big Integer is Prime Integer.
        return isPrime(a.str);
    }


    // to BigInteger functions

    static BigInt _to_BigInt(std::string s) {
        BigInt ans;
        ans.str = s;
        return ans;
    }
    static BigInt _to_BigInt(int n) {
        BigInt ans;
        ans.str = std::to_string(n);
        return ans;
    }
    static BigInt _to_BigInt(long int n) {
        BigInt ans;
        ans.str = std::to_string(n);
        return ans;
    }
    static BigInt _to_BigInt(long long int n) {
        BigInt ans;
        ans.str = std::to_string(n);
        return ans;
    }
};