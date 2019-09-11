#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio_ext.h>

//! SS_INF_NROOTS - value returned by SolveSquare() if root is any number 
const int SS_INF_NROOTS = -1;

//! IE_PRESSISION - pressision of double comparison
const double IE_PRESSISION = 1e-6;

int SolveSquare(double a, double b, double c, double *x1_p, double *x2_p);
int SolveLineare(double a, double b, double *x_p);
int IsEqual(double a, double b = 0);
int ScanCoefficient(const char *name, double *addr);
double *GetNewInput(const char *prompt, size_t NELEMS);
int ScanElem(double *f);

int main()
{
	printf("Solve sqare equation\n");
	
/*	double a = NAN, b = NAN, c = NAN;
	
	int errn = 0;
	if (	(errn = ScanCoefficient("x^2",		&a)) ||
		(errn = ScanCoefficient("x",		&b)) || 
		(errn = ScanCoefficient("constant",	&c)) )
		return errn;*/
	double *coeffs = GetNewInput("Input x^2, x and constant coefficients", 3);

	double x1 = NAN, x2 = NAN;
	
	int nRoots = SolveSquare(coeffs[0], coeffs[1], coeffs[2], &x1, &x2);
	switch (nRoots)
	{
		case 0:
			printf("No roots of %gx^2 + %gx + %g == 0\n", coeffs[0], coeffs[1], coeffs[2]);
			break;
		case 1:
			printf("Root  of %gx^2 + %gx + %g == 0 is:\n %g\n", coeffs[0], coeffs[1], coeffs[2], x1);
			break;
		case 2:
			printf("Roots of %gx^2 + %gx + %g == 0 are:\n %g %g\n", coeffs[0], coeffs[1], coeffs[2], x1, x2);
			break;
		case SS_INF_NROOTS:
			printf("Any number is a root of %gx^2 + %gx + %g == 0\n", coeffs[0], coeffs[1], coeffs[2]);
			break;
		default:
			fprintf(stderr, "Error in main(): Invalid number of roots\n");
			return 2;
	}
	free(coeffs);
	return 0;
}

/*! ScanCoefficient
	@note function to read coefficients from stdin, if input isn`t a number returns error
	@param name name of coefficient
	@param addr addres to write coefficient value
	@return 0 if no error occurs		*/

int ScanCoefficient(const char *name, double *addr)
{
        printf("Enter %s coefficient of equation:\n", name);
        if (!scanf("%lg", addr))
        {
                fprintf(stderr, "Error in ScanCoefficient(): Invalid coeficient %s\n", name);
                return 1;
        }
	return 0;
}

/*! GetNewInput
	@note function to read NELEMS doubles from stdin
	@param prompt message to show before reading
	@param NELEMS number of doubles to read
	@return array of NELEMS doubles		*/

double *GetNewInput(const char *prompt, const size_t NELEMS)
{
	puts(prompt);
        double *data = (double *) calloc(NELEMS, sizeof(*data));

        size_t i = 0;
        while (i < NELEMS)
        {
                if (ScanElem(data + i))
                {
                        printf("Can`t scan element number %ld\n", i);
                        break;
                }
                i++;
        }

        return data;
}

/*! ScanElem
	@note function to scan double from stdin
	@param f pointer to double to write in
	@return 0 if double is red		*/

int ScanElem(double *f)
{
        const unsigned NTRIES = 10;	/*!maximum number of attempts to read f*/
        for (unsigned i = 0; i < NTRIES; i++)
        {
                //printf("tryn %d\n", i);
                if (scanf("%lg", f))
                {
                        //puts("Ok");
                        return 0;
                }
                //fflush(stdin);
                __fpurge(stdin);
		printf("Invalid coefficient, type again. %d attempt%s remain%s\n", NTRIES - i - 1, i < NTRIES - 2 ? "s" : "", i < NTRIES - 2 ? "" : "s");
        }
        return 1;
}

/*! SolveSquare
	@note function to solve square eqation
	@param a x^2 coefficient
	@param b x coefficient
	@param c constant
	@param x1_p addres of 1-st root
	@param x2_p addres of 2-nd root
	@return number of roots			*/

int SolveSquare(double a, double b, double c, double *x1_p, double *x2_p)
{
	assert(isfinite(a));
	assert(isfinite(b));
	assert(isfinite(c));
	assert(x1_p != nullptr);
	assert(x2_p != nullptr);
	assert(x1_p != x2_p);
	
	if (IsEqual(a, 0))		/*solve linear equation*/
	{
		return SolveLineare(b, c, x1_p);
	}

	if (IsEqual(c, 0))
	{
		*x1_p = 0;
		return SolveLineare(a, b, x2_p);
	}

	double d = b*b - 4*a*c;		/*solve sqare equation*/
	if (IsEqual(d, 0))
	{
		*x1_p = -b / (2 * a);
		return 1;
	}
	if (d < 0) return 0;
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

int SolveLineare(double a, double b, double *x_p)
{
	assert(isfinite(a));
	assert(isfinite(a));
	assert(x_p != nullptr);

	if (IsEqual(a, 0)) return IsEqual(b, 0) ? SS_INF_NROOTS : 0;
	*x_p = -b / a;
	return 1;
}

/*!IsEqual
	@note compare to doubles
	@param a first double
	@param b second double
	@return 0 if a != b	*/

int IsEqual(double a, double b)
{
        return fabs(a - b) < IE_PRESSISION;
}
