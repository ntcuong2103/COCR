/**********************************************************************
obutil.h - Various utility methods.

Copyright (C) 1998-2001 by OpenEye Scientific Software, Inc.
Some portions Copyright (C) 2001-2006 by Geoffrey R. Hutchison

This file is part of the Open Babel project.
For more information, see <http://openbabel.org/>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
***********************************************************************/
#pragma once

#include <openbabel/babelconfig.h>

#include <string>
#include <iosfwd>
#include <ctime>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace OpenBabel
{

  // class introduction in obutil.cpp
  class OB_EXPORT OBStopwatch
  {
    clock_t start; //!< the start of timing
    clock_t stop;  //!< the current time

  public:
    //! Mark the start of "stopwatch" timing
    void  Start()
    {
      start= clock();
    }
    //! \return The time since calling OBStopwatch::Start() in seconds.
    double Lap()
    {
      stop= clock();
      return((stop - start) / (double) CLOCKS_PER_SEC);
    }

    //! \return The time since calling OBStopwatch::Start() in seconds.
    double Elapsed()
    {
      return(Lap());
    }
  };


  //! \class OBSqrtTbl obutil.h <openbabel/obutil.h>
  //! \brief Square Root lookup table - given a distance squared returns distance
  class OB_EXPORT OBSqrtTbl
  {
    double _max,_incr,*_tbl;
  public:
  OBSqrtTbl():
    _max(0.0), _incr(0.0),  _tbl(nullptr)
      { }
    //! \brief Create a square root table to handle up to the square root of @p max
    //! (e.g., if you want the square root of 144, supply 12 for max)
    //! \param max The maximum square root stored in the lookup table
    //! \param incr The floating point resolution of the lookup table
  OBSqrtTbl(const double max, const double incr):
    _max(max*max), _incr(incr), _tbl(nullptr)
      {
        Init(max,incr);
      }
    ~OBSqrtTbl()
      {
        if (_tbl)
          {
            delete [] _tbl;
            _tbl = nullptr;
          }
      }
    //! \brief Fast square root calculation using a lookup table
    //! \return Square root of @p d2
    double Sqrt(double d2) const
    {
      if (_tbl)
        return((d2 < _max) ? _tbl[static_cast<int>(d2*_incr)]:sqrt(d2));
      else
        return 0.0;
    }
    //! \brief Initialize the square root lookup table
    //! \param max The maximum square root stored in the lookup table (e.g., if you want the square root of 144, supply 12 for max)
    //! \param incr The floating point resolution of the lookup table
    void Init(double max,double incr)
    {
      // parameters are potentially unneeded, but let's do this until we can
      // deprecate them
      _max = max * max;
      _incr = incr;

      //array size needs to be large enough to account for fp error
      int i;
      double r;
      _tbl = new double [static_cast<int>((_max/_incr)+10)];
      for (r = (_incr/2.0),i=0;r <= _max;r += _incr,++i)
        _tbl[i] = sqrt(r);

      _incr = 1/_incr;
    }
  };

  //***RMS helper methods***/
#ifndef __KCC
  extern "C" {
  OB_EXPORT void  rotate_coords(double*, double m[3][3], unsigned);
  OB_EXPORT double calc_rms(double*, double*, unsigned int);
  }
#else
  OBAPI void  rotate_coords(double*,double m[3][3],unsigned);
  OBAPI double calc_rms(double*,double*,unsigned int);
#endif

#ifndef SWIG
  //! \name  String conversion utilities
  //@{
  // Documentation in obutil.cpp
  OB_EXPORT void ToUpper(std::string&);
  OB_EXPORT void ToUpper(char*);
  OB_EXPORT void ToLower(std::string&);
  OB_EXPORT void ToLower(char *);
  OB_EXPORT void InvertCase(std::string&, int);
  OB_EXPORT void InvertCase(char *);
  //! "Clean" the supplied atom type
  OB_EXPORT void CleanAtomType(char*);
  //@}

  //! Comparison -- returns true if first parameter less than second
  //! \return True if @p a < @p b, False otherwise.
  OB_EXPORT bool OBCompareInt(const int &a, const int &b);
  //! Comparison -- returns true if first parameter less than second
  //! \return True if @p a < @p b, False otherwise.
  OB_EXPORT bool OBCompareUnsigned(const unsigned int &a, const unsigned int &b);
  /*! "Safe" comparison for floats/doubles: returns fabs(a - b) < epsilon
   * This function really doesn't make any sense w.r.t. floating-point
   * representation, so you should never use it. It is provided only for
   * backwards compatibility.
   * \deprecated Use IsApprox() instead
   */
  OB_EXPORT bool IsNear(const double &, const double &, const double epsilon=2e-6);
  /*! "Safe" comparison for floats/doubles: true if a is less than epsilon
   * This function really doesn't make any sense w.r.t. floating-point
   * representation, so you should never use it. It is provided only for
   * backwards compatibility.
   * \deprecated
   */
  OB_EXPORT bool IsNearZero(const double &, const double epsilon=2e-6);
  OB_EXPORT bool IsNan(const double &);
  /**
   * \return true if \a a is much smaller than \a b. More precisely:
   * @code
   return( fabs(a) <= precision * fabs(b) );
   * @endcode
   */
  OB_EXPORT inline bool IsNegligible(const double & a, const double & b,
                                     const double precision = 1e-11)
  {
    return( fabs(a) <= precision * fabs(b) );
  }
  /*! Safe comparison for floats/doubles: true if
   * fabs(a - b) <= precision * std::min( fabs(a), fabs(b) )
   * The parameter precision plays the role of 10^-N where N is the number of
   * significant digits to consider.
   * This is the correct way to replace operator== for doubles. For new code,
   * use this function instead of the old IsNear() function.
   *
   * \note To check
   * if x is zero, use
   * @code
   IsNegligible( x, 1.0)
   * @endcode
   * instead of
   * @code
   IsApprox( x, 0.0 )
   * @endcode
   */
  OB_EXPORT inline bool IsApprox(const double & a, const double & b,
                                 const double precision = 1e-11)
  {
    return( fabs(a - b) <= precision * std::min<const double>( fabs(a), fabs(b) ) );
  }
  //! Same as IsApprox(), but only for positive numbers. Faster.
  OB_EXPORT inline bool IsApprox_pos(const double &a, const double &b,
                                     const double precision = 1e-11)
  {
    return( fabs(a - b) <= precision * std::min<const double>( a, b ) );
  }
  /*! \brief Tests whether its argument can be squared without triggering
    an overflow or underflow.
  */
  OB_EXPORT bool CanBeSquared(const double &);

  OB_EXPORT bool SafeOpen(std::ifstream &fs, const char *filename);
  OB_EXPORT bool SafeOpen(std::ofstream &fs, const char *filename);
#endif
  // (end part to be skipped by SWIG)

  //******************triple template*************************
  //! \class triple obutil.h <openbabel/obutil.h>
  //! \brief A 3-element templated, based on the design of the STL pair<>
  template <class T1, class T2, class T3>
    struct triple
    {
      //type names for the values
      typedef T1 first_type;
      typedef T2 second_type;
      typedef T3 third_type;

      //member
      T1 first;
      T2 second;
      T3 third;

      /** Default constructor
       *	T1() and T2() and T3() force initialization for built in types
       **/
    triple():
      first(T1()),second(T2()),third(T3())
      {}

      //! Constructor for 3 values
    triple(const T1 &a, const T2 &b, const T3 &c):
      first(a), second(b), third(c)
      {}

      //! Copy constructor with implicit conversions
      template<class U, class V, class W>
        triple(const triple<U,V,W> &t):
        first(t.first), second(t.second), third(t.third)
      {}

    };

  //**************quad template********************
  //! \class quad obutil.h <openbabel/obutil.h>
  //! \brief A 4-element templated, based on the design of the STL pair<>
  template <class T1, class T2, class T3, class T4>
    struct quad
    {
      //type names for the values
      typedef T1 first_type;
      typedef T2 second_type;
      typedef T3 third_type;
      typedef T4 fourth_type;

      //member
      T1 first;
      T2 second;
      T3 third;
      T4 fourth;

      /*! default constructor
       *	T1() and T2() and T3() force initialization for built in types
       */
    quad():
      first(T1()),second(T2()),third(T3()),fourth(T4())
      {}

      //! constructor for 3 values
    quad(const T1 &a, const T2 &b, const T3 &c, const T4 &d):
      first(a), second(b), third(c), fourth(d)
      {}

      //! copy constructor with implicit conversions
      template<class U, class V, class W, class X>
        quad(const quad<U,V,W,X> &q):
        first(q.first), second(q.second), third(q.third), fourth(q.fourth)
      {}

    };

} // end namespace OpenBabel

//! \file obutil.h
//! \brief Various utility methods.