#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include "arduinoFFT.h"
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define API_KEY "AIzaSyByTWjYsnZQ4zSUH7ZEYJabD5XQxSppE3w"
#define DATABSE_URL "https://motor-health-analysis-default-rtdb.firebaseio.com/"
char ssid[] = "Dinesh";
char password[] = "18032001";

Adafruit_ADXL345_Unified accel;
arduinoFFT FFT = arduinoFFT();

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseJson json;
FirebaseJsonArray arr;

#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

const int samples = 1024;
int samplingFrequency = 493;
unsigned long StartTime;

double xReal[samples];
double xImag[samples];
//double yReal[samples];
//double yImag[samples];
//double zReal[samples];
//double zImag[samples];







void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(19200);
  //////////////////////////////////////////////////
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
//    for(int j=0;j<10;j++){
//      digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
//      delay(200);                      // Wait for a second
//      digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
//      delay(200); 
//    }
     
    while(1);
  }

  /* Set the range to whatever is appropriate for your project */
  accel.setRange(ADXL345_RANGE_8_G);
  

   
  /////////////////////////////////////////////////////

  // Connect to Wi-Fi////////////////////////////////
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
  
  /////////////////////////////////////////////////////  

  config.api_key = API_KEY;
  config.database_url = DATABSE_URL;
  if(Firebase.signUp(&config, &auth, "", "")){
    Serial.println("signUp OK");
    signupOK = true;
  }else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  
}

void loop() {
  // put your main code here, to run repeatedly:

  //taking data from the adxl///////////////////////////////////
  int i =0;
    digitalWrite(LED_BUILTIN, LOW);
  StartTime=millis();
  while(i<samples){
    sensors_event_t event; 
    accel.getEvent(&event);

      xReal[i]=event.acceleration.x;
      
      xImag[i] = 0.0;
      
      i=i+1;
//      if(millis()-StartTime==1000){
//        samplingFrequency=i;
//        
//      }
    
  }
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.print("Sampling Frequency :-> ");Serial.print(",");Serial.println(samplingFrequency);
  
//  for ( int m = 0; m < samples; m = m + 1) {
//    Serial.print(m);Serial.print("  ");Serial.print("     X: "); Serial.println(xReal[m]);
//    //Serial.print(" Y: "); Serial.println(yss[m]);
//    //Serial.print(" Z: "); Serial.println(zss[m]);
//  }
  //////////////////////////////////////////////////////////////
  
  //PrintVector(xReal, samples, SCL_TIME);
  FFT.Windowing(xReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Weigh data */
  FFT.Compute(xReal, xImag, samples, FFT_FORWARD); /* Compute FFT */
  FFT.ComplexToMagnitude(xReal, xImag, samples); /* Compute magnitudes */

//  FFT.Windowing(yReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Weigh data */
//  FFT.Compute(yReal, xImag, samples, FFT_FORWARD); /* Compute FFT */
//  FFT.ComplexToMagnitude(yReal, xImag, samples); /* Compute magnitudes */
//
//  FFT.Windowing(zReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Weigh data */
//  FFT.Compute(zReal, xImag, samples, FFT_FORWARD); /* Compute FFT */
//  FFT.ComplexToMagnitude(zReal, xImag, samples); /* Compute magnitudes */
  Serial.println("/////////// ");
  
  //PrintVector(xReal, (samples >> 1), SCL_FREQUENCY);
  //double x = FFT.MajorPeak(vReal, samples, samplingFrequency);
  //Serial.println(x, 6);

////////////////////////////////////////////////////////////////////////////////////////////////////////
  if(Firebase.ready() && signupOK && (millis()-sendDataPrevMillis>5000|| sendDataPrevMillis ==0)){
    sendDataPrevMillis = millis();
    
    //------STOrE sensor data to a RTDB
    

//    json.add("name", "Dinu");
//    int j=0;
//    while(j < samples/2){
//      
//     for(int i=j*30;i<30+j*30&&i<(samples>>1);i++){
//      
//       char s1[]="xfft/[";
//       char s2[10];
//       tostring(s2, i);
//       char s3[]="]";
//      
//       strcat(s1,s2);
//       strcat(s1,s3);
//       
//       json.set(s1, xReal[i]);
//     }
//          
//          
      json.add("Sampling Frequency", samplingFrequency);
      json.add("Samples", samples);
      Serial.printf("Sending Sampling Frequency %s\n", Firebase.RTDB.set(&fbdo, "fft/fft_meta", &json) ? "ok" : fbdo.errorReason().c_str());
      json.clear();
     
//     if(Firebase.RTDB.updateNode(&fbdo, "fft/fft_samplingFrequency", &json)){
//  //    //Serial.println(); Serial.print(arrdata);
//      //json.toString(Serial, true);
//        Serial.println();
//        Serial.print(" - successfully saved to : " + fbdo.dataPath());
//        
//     }else{
//        Serial.println("Failed: " + fbdo.errorReason());
//     }
//     
//
//     j++;
//     delay(5000);
//    
//    }


    

      for(i=0;i<100;i++){
        arr.add(xReal[i]);
      }
      Serial.printf("Set array... %s\n", Firebase.RTDB.setArray(&fbdo, "fft/fft_arr/f1", &arr) ? "ok" : fbdo.errorReason().c_str());
      arr.clear();

      for(i=100;i<200;i++){
        arr.add(xReal[i]);
      }
      Serial.printf("Set array... %s\n", Firebase.RTDB.setArray(&fbdo, "fft/fft_arr/f2", &arr) ? "ok" : fbdo.errorReason().c_str());
      arr.clear();

      for(i=200;i<300;i++){
        arr.add(xReal[i]);
      }
      Serial.printf("Set array... %s\n", Firebase.RTDB.setArray(&fbdo, "fft/fft_arr/f3", &arr) ? "ok" : fbdo.errorReason().c_str());
      arr.clear();

      for(i=300;i<400;i++){
        arr.add(xReal[i]);
      }
      Serial.printf("Set array... %s\n", Firebase.RTDB.setArray(&fbdo, "fft/fft_arr/f4", &arr) ? "ok" : fbdo.errorReason().c_str());
       arr.clear();
       
      for(i=400;i<500;i++){
        arr.add(xReal[i]);
      }
      Serial.printf("Set array... %s\n", Firebase.RTDB.setArray(&fbdo, "fft/fft_arr/f5", &arr) ? "ok" : fbdo.errorReason().c_str());
      arr.clear();
      
      for(i=500;i<512;i++){
        arr.add(xReal[i]);
      }
      
    Serial.printf("Set array... %s\n", Firebase.RTDB.setArray(&fbdo, "fft/fft_arr/f6", &arr) ? "ok" : fbdo.errorReason().c_str());
        arr.clear();
    
     
  }
  
Serial.println("////////////");

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
      Serial.print(" Hz");
    Serial.print(",");
    Serial.println(vData[i], 4);
  }
  Serial.println();
}



void tostring(char str[], int num)
{
    int i, rem, len = 0, n;
 
    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}
