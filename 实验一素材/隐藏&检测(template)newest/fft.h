
/*��������*/
typedef struct
{
	double re;
	double im;
}COMPLEX;

/*���ٸ��ﰧ�任
TDΪʱ��ֵ,FDΪƵ��ֵ,powerΪ2������*/
void FFT(COMPLEX * TD, COMPLEX * FD, int power);
void DCT(double *f, double *F, int power);
void FFT_REAL(double * td, double * fd, int power);