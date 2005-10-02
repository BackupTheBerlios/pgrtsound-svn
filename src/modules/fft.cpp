//#include "Array.h"
#include "fft.h"

//using Array::array1;

FFT::FFT() :
	iIn("inpute"), oReal("real"),oImag("imag"), pSize("delay")
 {
    //maksymalne opóŸnienie w próbkach
	AddInput(iIn);
	AddOutput(oReal);
	AddOutput(oImag);	
	pSize.Bound(1, 2*(Module::framesPerBlock - 1), 1);    // ograczniczenie wartosci
	AddParameter(pSize);
	bufforIn   = new float[4096];
  bufforOut1 = new float[4096];
  bufforOut2 = new float[4096];
	n          = 256;
	i          = 0;
	o          = 0;
	
}

FFT::~FFT() {

}

void FFT::Process() {
  float* in   = iIn.GetSignal();

  for (unsigned int poz = 0; poz < Module::framesPerBlock; poz++) {
    //wejscie do bufora wejsciowego
    bufforIn[i] = *in++;    
    if (i >= n) {  //jezeli w buforze wejsciowym wystarczajaca liczba probek
      unsigned int np=n;///2+1;
      double  *f= FFTWdouble(n);
      Complex *g = FFTWComplex(np);
      
      rcfft1d Forward(n,f,g);
  
      for(unsigned int j=0; j < n; j++) 
        f[j]=bufforIn[j];
	    
	    //wyliczenie FFT
      Forward.fft(f,g);
  
     
      for(unsigned int k=0; k < np; k++) {
        //FFT do bufora wyjsciowego
        bufforOut1[o]  = g[k].real();
        bufforOut2[o]  = g[k].imag();
        //buffor wyjsciowy na wyjscie
        if (o >= Module::framesPerBlock) {  //jezeli w buforze wyjsciowym wystarczajaca liczba probek
          float* outR = oReal.GetSignal();
          float* outI = oImag.GetSignal();
           for(unsigned int j=0; j < Module::framesPerBlock; j++) {
            *outR++ = bufforOut1[j];
            *outI++ = bufforOut2[j];   
          }
      /**/    o = 0;     
        } else {
          o++;
        }
      }    
      
      //czyszczenie zmiennych pomocniczych
      FFTWdelete(g);
      FFTWdelete(f);
     
      i     = 0;
      n     = (int)pSize.GetValue(); //pobranie dlugoscu z parametru
    } else {
      i++;
    }
  }
}
