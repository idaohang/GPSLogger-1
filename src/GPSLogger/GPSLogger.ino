#include "SoftwareSerial.h"
#include "dGPS.h"
#include "SPI.h"
#include "SD.h"

dGPS dgps = dGPS();               // Construct dGPS class

#define MAX_FILENAME_LENGTH 13
char cfilename[MAX_FILENAME_LENGTH];
String sdate;
String stime;

void setup() {
  Serial.begin(9600);            // Serial output back to computer.  On.

  if (!SD.begin(10)) {
    return;
  }

  dgps.init();                   // Run initialization routine for dGPS.

  delay(1000);                   // Delay 1 sec between pings
}

void loop() {
  dgps.update(0, 0);
 
  long date = dgps.Date();
  long time = dgps.Time();
  float lat = dgps.Lat();
  float lng = dgps.Lon();
  
  getFilename(date, time);
  
  File logfile = SD.open(cfilename, FILE_WRITE);
  if (logfile) {
    logfile.print(sdate);
    logfile.print(",");
    logfile.print(stime);
    logfile.print(",");
    logfile.print(lat, 6);
    logfile.print(",");
    logfile.print(lng, 6);
    logfile.println("");
    logfile.close();
  }

  delay(1000);
}

void getFilename(long date, long time) {
  sdate = String(date);
  while (sdate.length() < 6) {
    sdate = "0" + sdate;
  }
  sdate = "20" + sdate.substring(4, 6) + sdate.substring(2, 4) + sdate.substring(0, 2);
  
  stime = String(time);
  while (stime.length() < 6) {
    stime = "0" + stime;
  }
  
  String filename = sdate.substring(2, 6) + stime.substring(0, 4) + ".csv";
  filename.toCharArray(cfilename, filename.length()+1);
}
