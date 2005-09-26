#include "Array.h"
#include "fft.h"

using Array::array1;

FFT::FFT() : Module("New delay"),
	iIn("inpute"), oReal("real"),oImag("imag"), pSize("delay")
 {
    //maksymalne opóŸnienie w próbkach
	AddInput(iIn);
	AddOutput(oReal);
	AddOutput(oImag);	
	pSize.Bound(1, 2*(Module::framesPerBlock - 1), 1);    // ograczniczenie wartosci
	AddParameter(pSize);
	buffor = new float[4095];
	n      = 16;
	i      = 0;
	
}

FFT::~FFT() {

}

void FFT::Process() {
  float* in   = iIn.GetSignal();
  float* outR = oReal.GetSignal();
  float* outI = oImag.GetSignal();	

  

  for (unsigned int poz = 0; poz < Module::framesPerBlock; poz++) {
    buffor[i] = *in++;    
    if (i >= n) {  //jezeli w buforze wystarczajaca liczba probek

    TRACE("n= ",n);
      unsigned int np=n/2+1;
    //  double  *f; //= FFTWdouble(n);
    //  Complex *g;// = FFTWComplex(np);
      
    //  f = new double[n];
    //  g = new Complex[np];
    //         
      size_t align=sizeof(Complex);
  
  array1<double> f(n,align);
  array1<Complex> g(np,align);
  rcfft1d Forward(f,g);
  
      for(unsigned int j=0; j < n; j++) f[j]=buffor[j];
	
      Forward.fft(f,g);
  
      for(unsigned int k=0; k < np; k++) {
        *outR++ = g[k].real();
       *outI++ = g[k].imag();        
      }
    
//     FFTWdelete(g);
//      FFTWdelete(f);
     
      i     = 0;
      n     = (int)pSize.GetValue(); //zmiana d³ugoœci
    } else {
      i++;
    }
  }
 /* n = Module::framesPerBlock;
  unsigned int np=n/2+1;
  double  *f = FFTWdouble(n);
  Complex *g = FFTWComplex(np);
  
  rcfft1d Forward(n,f,g);
  
      for(unsigned int j=0; j < Module::framesPerBlock; j++) f[j]=*in++;
	
      Forward.fft(f,g);
  
      for(unsigned int k=0; k < np; k++) {
        *outR++ = g[k].real();
        *outI++ = g[k].imag();        
      }
      FFTWdelete(g);
      FFTWdelete(f);
    */
}
