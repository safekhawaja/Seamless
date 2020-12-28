#include "arduinoFFT.h"

/* Thank you to arduinoFFT library authors — Seamless wouldn't have been possible without their library! */

arduinoFFT FFT = arduinoFFT();

const uint16_t samples = 64; 
const double signalFrequency = 1000;
const double samplingFrequency = 5000;
const uint8_t amplitude = 100;

double vReal[samples];
double vImag[samples];

#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03

pinMode(A0, INPUT);     // Microphone input
pinMode(A1, OUTPUT);      // Output low f
pinMode(A2, OUTPUT);      // Output high f

void setup()
{
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Ready");
}




void loop() {

  // Build raw data
  
  double cycles = (((samples-1) * signalFrequency) / samplingFrequency); //Number of signal cycles that the sampling will read
  for (uint16_t i = 0; i < samples; i++)
  {
    vReal[i] = int8_t((amplitude * (sin((i * (twoPi * cycles)) / samples))) / 2.0);     // Build data with positive and negative values
    vImag[i] = 0.0;       // Imaginary part must be zeroed in case of looping to avoid wrong calculations and overflows
  }
  
  // Print results
  
  Serial.println("Data:");
  PrintVector(vReal, samples, SCL_TIME);
  FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);        // Weigh data 
  Serial.println("Weighed data:");
  PrintVector(vReal, samples, SCL_TIME);
  FFT.Compute(vReal, vImag, samples, FFT_FORWARD);      // Compute FFT 
  Serial.println("Computed Real values:");
  PrintVector(vReal, samples, SCL_INDEX);
  Serial.println("Computed Imaginary values:");
  PrintVector(vImag, samples, SCL_INDEX);
  FFT.ComplexToMagnitude(vReal, vImag, samples);      // Compute magnitudes 
  Serial.println("Computed magnitudes:");
  PrintVector(vReal, (samples >> 1), SCL_FREQUENCY);
  double x = FFT.MajorPeak(vReal, samples, samplingFrequency);
  Serial.println(x, 6);
  while(1); /* Run Once */
  // delay(2000);       // Repeat 


  l = map(a, 0, 1023, 0, 255);
  h = map(a, 0, 1023, 0, 255);


  analogWrite(A1, l);
  analogWrite(A2, h);

}







void PrintVector(double *vData, uint16_t bufferSize, uint8_t scaleType)
{
  for (uint16_t i = 0; i < bufferSize; i++)
  {
    double abscissa;
    /* Print abscissa value */
    switch (scaleType)
    {
      case SCL_INDEX:
        abscissa = (i * 1.0);
  break;
      case SCL_TIME:
        abscissa = ((i * 1.0) / samplingFrequency);
  break;
      case SCL_FREQUENCY:
        abscissa = ((i * 1.0 * samplingFrequency) / samples);
  break;
    }
    Serial.print(abscissa, 6);
    if(scaleType==SCL_FREQUENCY)
      Serial.print("Hz");
    Serial.print(" ");
    Serial.println(vData[i], 4);
  }
  Serial.println();

}
