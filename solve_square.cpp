#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>

//! SS_INF_NROOTS - value returned by SolveSquare() if root is any number 
const int SS_INF_NROOTS = -1;

//! IE_PRESSISION - pressision of float comparison
const float IE_PRESSISION = 1e-6f;

int SolveSquare(float a, float b, float c, float *x1_p, float *x2_p);
int SolveLineare(float a, float b, float *x_p);
int IsEqual(float a, float b = 0);
int ScanCoefficient(char *name, float *addr);

int main()
{
	printf("Solve sqare equation\n");
	
	float a = NAN, b = NAN, c = NAN;
	
	int errn = 0;
	if(	(errn = ScanCoefficient("x^2",		&a)) ||
		(errn = ScanCoefficient("x",		&b)) || 
		(errn = ScanCoefficient("constant",	&c)) )
		return errn;

	float x1 = NAN, x2 = NAN;
	
	int nRoots = SolveSquare(a, b, c, &x1, &x2);
	switch(nRoots)
	{
		case 0:
			printf("No roots\n");
			break;
		case 1:
			printf("Root of %gx^2 + %gx + %g == 0  is:\n %g\n", a, b, c, x1);
			break;
		case 2:
			printf("Roots of %gx^2 + %gx + %g == 0  are:\n %g %g\n", a, b, c, x1, x2);
			break;
		case SS_INF_NROOTS:
			printf("Any number is a root of %gx^2 + %gx + %g == 0\n", a, b, c);
			break;
		default:
			fprintf(stderr, "Error in main(): Invalid number of roots\n");
			return 2;
	}
	return 0;
}

/*! ScanCoefficient
	@note function to read coefficients from stdin, if input isn`t a number returns error
	@param name name of coefficient
	@param addr addres to write coefficient value
	@return 0 if no error occurs		*/

int ScanCoefficient(char *name, float *addr)
{
        printf("Enter %s coefficient of equation:\n", name);
        if (!scanf("%g", addr))
        {
                fprintf(stderr, "Error in ScanCoefficient(): Invalid coeficient %s\n", name);
                return 1;
        }
	return 0;
}

/*! SolveSquare
	@note function to solve square eqation
	@param a x^2 coefficient
	@param b x coefficient
	@param c constant
	@param x1_p addres of 1-st root
	@param x2_p addres of 2-nd root
	@return number of roots			*/

int SolveSquare(float a, float b, float c, float *x1_p, float *x2_p)
{
	assert(isfinite(a));
	assert(isfinite(b));
	assert(isfinite(c));
	assert(x1_p != NULL);
	assert(x2_p != NULL);
	assert(x1_p != x2_p);
	
	if(IsEqual(a, 0))		/*solve linear equation*/
	{
		return SolveLineare(b, c, x1_p);
	}

	if(IsEqual(c, 0))
	{
		*x1_p = 0;
		return SolveLineare(a, b, x2_p);
	}

	float d = b*b - 4*a*c;		/*solve sqare equation*/
	if(IsEqual(d, 0))
	{
		*x1_p = -b / (2 * a);
		return 1;
	}
	if(d < 0) return 0;
	*x1_p = (- b + sqrt(d)) / (2 * a);
	*x2_p = (- b / a) - *x1_p;
	return 2;
}

/*! SolveLineare
	@note function to solve lineare eqation
	@param a x coefficient
	@param b constant
	@param x_p adress of root
	@return number of roots			*/

int SolveLineare(float a, float b, float *x_p)
{
	assert(isfinite(a));
	assert(isfinite(a));
	assert(x_p != NULL);

	if(IsEqual(a, 0)) return IsEqual(b, 0) ? SS_INF_NROOTS : 0;
	*x_p = -b / a;
	return 1;
}

/*!IsEqual
	@note compare to floats
	@param a first float
	@param b second float
	@return 0 if a != b	*/

int IsEqual(float a, float b)
{
        return fabs(a - b) < IE_PRESSISION;
}
