#include "SoftwareSerial.h"
#include "dGPS.h"
#include "SPI.h"
#include "SD.h"

#define FILE_SUFFIX         ".csv"
#define FLOAT_DELTA         0.000001
#define FLOAT_PRECISION     6
#define MAX_FILENAME_LENGTH 13
#define SD_PIN              10

long date = 0;
long time = 0;
float lat = 0;
float lng = 0;
float plat = 0;
float plng = 0;
String sdate;
String stime;
char cfilename[MAX_FILENAME_LENGTH];
dGPS dgps = dGPS();              // Construct dGPS class

void setup() {
  Serial.begin(9600);            // Serial output back to computer.  On.

  if (!SD.begin(SD_PIN)) {
    return;
  }

  dgps.init();                   // Run initialization routine for dGPS.

  delay(1000);                   // Delay 1 sec between pings
}

void loop() {
  getData();
  
  if (checkDelta()) {
    logData();
  }

  delay(1000);
}

void getData() {
  dgps.update(0, 0);

  date = dgps.Date();
  time = dgps.Time();
  lat = dgps.Lat();
  lng = dgps.Lon();
}

boolean checkDelta() {
  boolean ret = (abs(lat - plat) > FLOAT_DELTA) || (abs(lng - plng) > FLOAT_DELTA);
  if (ret) {
    plat = lat;
    plng = lng;
  }
  return ret;
}

void prepareData() {
  sdate = String(date);
  while (sdate.length() < 6) {
    sdate = "0" + sdate;
  }
  sdate = "20" + sdate.substring(4, 6) + sdate.substring(2, 4) + sdate.substring(0, 2);

  stime = String(time);
  while (stime.length() < 6) {
    stime = "0" + stime;
  }

  String filename = sdate.substring(2, 6) + stime.substring(0, 4) + FILE_SUFFIX;
  filename.toCharArray(cfilename, filename.length() + 1);
}

void logData() {
  // prepares date, time, filename for output
  prepareData();

  // write the file
  File logfile = SD.open(cfilename, FILE_WRITE);
  if (logfile) {
    logfile.print(sdate);
    logfile.print(",");
    logfile.print(stime);
    logfile.print(",");
    logfile.print(lat, FLOAT_PRECISION);
    logfile.print(",");
    logfile.print(lng, FLOAT_PRECISION);
    logfile.println("");
    logfile.close();
  }
}
