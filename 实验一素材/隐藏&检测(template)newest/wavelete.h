#ifndef _WAVELETE_KIT_
#define _WAVELETE_KIT_
#endif
#ifdef _WAVELETE_KIT_
/*---------------------------------------------------------------------------*/
// Baseline Wavelet Transform Coder Construction Kit
//
// Geoff Davis
// gdavis@cs.dartmouth.edu
// http://www.cs.dartmouth.edu/~gdavis
//
// Copyright 1996 Geoff Davis 9/11/96
//
// Permission is granted to use this software for research purposes as
// long as this notice stays attached to this software.
//
/*---------------------------------------------------------------------------*/
#ifndef _GLOBAL_
#define _GLOBAL_
/*---------------------------------------------------------------------------*/
// global parameters
/*---------------------------------------------------------------------------*/
//#define DEBUG
// Use PGM images as default (comment this line out to use Raw images)
#define PGM
#define DOUBLE_REAL
//#define FLOAT_REAL

#ifdef DOUBLE_REAL
typedef double Real;
#endif
#ifdef FLOAT_REAL
//typedef float Real;
//typedef Real float;
#define Real float
#endif
/*---------------------------------------------------------------------------*/
// standard #defines
/*---------------------------------------------------------------------------*/
#define TRUE  1
#define FALSE 0

#define BACKSPACE 8
#define BS        8
#define ESC       27
/*---------------------------------------------------------------------------*/
// useful constants
/*---------------------------------------------------------------------------*/
#ifdef DOUBLE_REAL
const Real eps = 1.e-15;
const double MaxReal = 1.0e+100;
#endif
#ifdef FLOAT_REAL
const Real eps = 1.e-8;
const double MaxReal = 1.0e+100;
#endif

//const Real Pi = (Real)3.14159265358979;
#define Pi 3.14159265358979
//const Real TwoPi = 2.0 * Pi;
#define TwoPi 2*3.14159265358979
//const Real Sqrt2 = (Real)sqrt(2.0);
#define Sqrt2 1.41421356237;
//const Real Log2 = (Real)log(2.0)
#define Log2 0.301029995663

/*---------------------------------------------------------------------------*/
// helpful inline functions
/*---------------------------------------------------------------------------*/
#ifndef min
#define min(x,y) (((x)<(y))?(x):(y))
#define max(x,y) (((x)>(y))?(x):(y))
#endif

/*---------------------------------------------------------------------------*/
inline Real mod (Real x, Real N)
{
   Real xmodN = x - N*((int)(x/N));
   if (xmodN < 0) xmodN += N;
   return xmodN;
}

/*---------------------------------------------------------------------------*/
inline Real square (Real x) { return (x*x); }
/*---------------------------------------------------------------------------*/
inline int  isquare (int x) { return (x*x); }
/*---------------------------------------------------------------------------*/
inline int  sign (Real x)   { return (x > 0 ? 1 : x < 0 ? -1 : 0); }
/*---------------------------------------------------------------------------*/
inline int log2 (int x) {
   int count = 0;

   while (x > 1)  {
      x >>= 1;
      count++;
   }
   return count;
}

/*---------------------------------------------------------------------------*/
// functions in global.cc
/*---------------------------------------------------------------------------*/
void init ();
void shut_down ();
volatile void error (char *format, ...);
void warning (char *format, ...);
void no_more_memory ();
/*---------------------------------------------------------------------------*/
#endif
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
// Baseline Wavelet Transform Coder Construction Kit
//
// Geoff Davis
// gdavis@cs.dartmouth.edu
// http://www.cs.dartmouth.edu/~gdavis
//
// Copyright 1996 Geoff Davis 9/11/96
//
// Permission is granted to use this software for research purposes as
// long as this notice stays attached to this software.
//
/*---------------------------------------------------------------------------*/
#ifndef _WAVELET_
#define _WAVELET_
//#include "global.h"
/*---------------------------------------------------------------------------*/

class Filter {
public:
  int size, firstIndex, center;
  Real *coeff;

  Filter () {coeff = NULL; size = 0; firstIndex = 0;};
  Filter (int size, int firstIndex = 0, Real *coeff = NULL)
            { init (size, firstIndex, coeff); };
  Filter (const Filter &filter) {coeff=NULL; copy(filter);};
  ~Filter ();
  
  void init (int size, int firstIndex, Real *coeff);
  Filter& operator= (const Filter &filter) {copy(filter); return *this;};
  Real& operator[] (int index) {return coeff[index-firstIndex];};

protected:
  void copy (const Filter &filter);
};

/*---------------------------------------------------------------------------*/

class FilterSet {
public:
  FilterSet () {symmetric = FALSE; analysisLow = analysisHigh =
		synthesisLow = synthesisHigh = NULL;};
  FilterSet (int symmetric, 
	     Real *anLow, int anLowSize, int anLowFirst,  
	     Real *synLow = NULL, int synLowSize = 0, int
	     synLowFirst = 0);
  FilterSet (const FilterSet &filterset);
  ~FilterSet ();

  void init (int symmetric, 
	     Real *anLow, int anLowSize, int anLowFirst, 
	     Real *synLow, int synLowSize, int synLowFirst);
  FilterSet& operator= (const FilterSet filterset);

  int symmetric;
  Filter *analysisLow, *analysisHigh, *synthesisLow,
    *synthesisHigh;

protected:
  void copy (const FilterSet& filterset);
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

class Wavelet {
public:
  Wavelet (FilterSet *filterset);
  ~Wavelet ();
  
  void transform1d (Real *input, Real *output, int size,
		     int nsteps, int sym_ext = -1);
  void invert1d (Real *input, Real *output, int size,
		  int nsteps, int sym_ext = -1);
  
  void transform2d (Real *input, Real *output, int hsize, int vsize,
		     int nsteps, int sym_ext = -1);
  void invert2d (Real *input, Real *output, int hsize, int vsize, 
		  int nsteps, int sym_ext = -1);

  Filter *analysisLow, *analysisHigh;    // H and G
  Filter *synthesisLow, *synthesisHigh;  // H~ and G~
  int symmetric;  // TRUE if filter set is symmetric

  void symmetric_extension (Real *output, int size, int left_ext, int
			    right_ext, int symmetry);
  void periodic_extension (Real *output, int size);
  int npad;
  
 protected:
  void transform_step (Real *input, Real *output, int size, int sym_ext);
  void invert_step (Real *input, Real *output, int size, int sym_ext);

  // copy length elements from p1 to p2
  void copy (const Real *p1, Real *p2, const int length)
  {int temp = length; while(temp--) *p2++ = *p1++;}
  void copy (const Real *p1, const int stride1, Real *p2, 
	     const int length)
  {int temp = length; while(temp--) {*p2++ = *p1; p1 += stride1;}}
  void copy (const Real *p1, Real *p2, const int stride2,
	     const int length)
  {int temp = length; while(temp--) {*p2 = *p1++; p2 += stride2;}}
};

/*---------------------------------------------------------------------------*/
//enum { SPLINE, HAAR, ADELSON, DAUB4, DAUB6, DAUB8 };
//extern const Wavelet *(wavelets[]);
//extern const char *(wavelet_name[]);

extern FilterSet Haar, Daub4, Daub6, Daub8, Antonini, Villa, Adelson,
  Brislawn, Brislawn2, Villa1, Villa2, Villa3, Villa4, Villa5, Villa6, 
  Odegard,Villa1810;
/*---------------------------------------------------------------------------*/
#endif
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
// Baseline Wavelet Transform Coder Construction Kit
//
// Geoff Davis
// gdavis@cs.dartmouth.edu
// http://www.cs.dartmouth.edu/~gdavis
//
// Copyright 1996 Geoff Davis 9/11/96
//
// Permission is granted to use this software for research purposes as
// long as this notice stays attached to this software.
//
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
#endif
