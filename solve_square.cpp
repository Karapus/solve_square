#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <ctype.h>

//! SS_INF_NROOTS - value returned by SolveSquare() if root is any number 
const int SS_INF_NROOTS = -1;

//! D_TOLERANCE - pressision of double comparison
double D_TOLERANCE = 2.75e-17;

int SolveSquare(double a, double b, double c, double *x1_p, double *x2_p);
int SolveLineare(double a, double b, double *x_p);
int IsEqual(double a, double b = 0.0);
double *GetNewInput(const size_t NELEMS, const char *prompt = nullptr);
int ScanElem(double *f);
static inline double MaxD(double a, double b);
void UnitTest(void);
static inline int CmpTests(int res_t, double x1_t, double x2_t, int res, double x1, double x2);

int main()
{
	UnitTest();

	printf("Solve sqare equation\n");
	
	double *coeffs = GetNewInput(3, "Input x^2, x and constant coefficients");
	assert(coeffs != nullptr);

	double x1 = NAN, x2 = NAN;
	
	D_TOLERANCE *= 10;//* log10(MaxD(coeffs[0], MaxD(coeffs[1], coeffs[2])));

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

void UnitTest(void)
{
	const int NTESTS = 5;
	struct test
	{
		double a;
		double b;
		double c;
		int res;
		double x1;
		double x2;
	};
	struct test tests_arr[NTESTS] =
	{
		{0, 0, 0, SS_INF_NROOTS, NAN, NAN},
		{0, 0, 1, 0, NAN, NAN},
		{0, 1, 0, 1, 0, NAN},
		{2, 0, 1, 0, NAN, NAN},
		{1, -5, 4, 2, 4, 1}
	};

	double x1 = NAN, x2 = NAN; 
	for (int i = 0; i < NTESTS; i++)
	{
		x1 = x2 = NAN;
		int res = SolveSquare(tests_arr[i].a, tests_arr[i].b, tests_arr[i].c, &x1, &x2);
		if (!CmpTests(tests_arr[i].res, tests_arr[i].x1, tests_arr[i].x2, res, x1, x2))
			printf("Unittest %d failed: should be res = %d; x1, x2 == %g, %g; but res == %d; x1, x2 == %g,%g\n",
					i, tests_arr[i].res, tests_arr[i].x1, tests_arr[i].x2, res, x1, x2);
	}
	return;
}

static inline int CmpTests(int res_t, double x1_t, double x2_t, int res, double x1, double x2)
{
	return	(res_t == res) && (
		(!isfinite(x1_t))						||
		(IsEqual(x1_t, x1) && (!isfinite(x2_t) || IsEqual(x2_t, x2)))	|| 
		(IsEqual(x1_t, x2) && (!isfinite(x2_t) || IsEqual(x2_t, x1)))
		);
}

/*!	MaxD
 * 	@note returns maximum of two double arguments
 * 	@param a first double to compare
 * 	@param b second double to compare
 * 	@return maximum of a and b		*/

static inline double MaxD(double a, double b)
{
	return (a > b) ? a : b;
}

/*! GetNewInput
	@note function to read NELEMS doubles from stdin
	@param prompt message to show before reading
	@param NELEMS number of doubles to read
	@return array of NELEMS doubles		*/

double *GetNewInput(const size_t NELEMS, const char *prompt)
{
	if (prompt) puts(prompt);

        double *data = (double *) calloc(NELEMS, sizeof(*data));
	assert(data != nullptr);

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
	assert(f != nullptr);

        const unsigned NTRIES = 10;	/*!maximum number of attempts to read f*/
        for (unsigned i = 0; i < NTRIES; i++)
        {
                if (scanf("%lg", f) && isspace(getchar()))
                {
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

	if (IsEqual(a))		/*solve linear equation*/
	{
		return SolveLineare(b, c, x1_p);
	}

	if (IsEqual(c))
	{
		*x1_p = 0;
		int nRoots = SolveLineare(a, b, x2_p);
		return (nRoots == SS_INF_NROOTS) ? SS_INF_NROOTS : (nRoots + 1);
	}

	double d = b*b - 4*a*c;		/*solve sqare equation*/
	if (IsEqual(d))
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

	if (IsEqual(a)) return IsEqual(b) ? SS_INF_NROOTS : 0;
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
        return fabs(a - b) < D_TOLERANCE;
}
