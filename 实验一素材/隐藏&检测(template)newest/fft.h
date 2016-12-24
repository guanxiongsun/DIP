
/*复数定义*/
typedef struct
{
	double re;
	double im;
}COMPLEX;

/*快速付里哀变换
TD为时域值,FD为频域值,power为2的幂数*/
void FFT(COMPLEX * TD, COMPLEX * FD, int power);
void DCT(double *f, double *F, int power);
void FFT_REAL(double * td, double * fd, int power);