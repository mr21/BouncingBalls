#ifndef			MYMATH
# define		MYMATH

# include		<math.h>

# ifndef		M_PI
#  define		M_PI		3.14159265358979323846
#  define		M_PI_2		1.57079632679489661923
# endif

# define		POW2(X)		((X) * (X))
# define		DEGREES(X)	((X) * (180 / M_PI))
# define		RADIANS(X)	((X) * (M_PI / 180))

typedef struct {	int	x, y; }		v2i;
typedef struct {	float	x, y; }		v2f;
typedef struct {	float	a, b, c, d; }	m4f;

double			mycos(double rad);
double			mysin(double rad);

v2i*			v2i_(int x, int y);
v2f*			v2f_(float x, float y);
m4f*			m4f_(float a, float b, float c, float d);

void			v2f_mul_f(v2f*, float);
void			v2f_mul_m4f(v2f*, m4f const*);

#endif
