// Resources
// Serial Input Stuff: https://forum.arduino.cc/t/serial-input-basics-updated/382007
// Mapping Function: https://forum.arduino.cc/t/floating-point-using-map-function/348113

const byte numChars = 100;
char receivedChars[numChars];  // buffer for entire incoming line
boolean newData = false;

// pointers to parsed fields
char *field0;
char *field1;
char *field2;
char *t_s;

float prev_t_s; // I think this variable is unused
float prevfield0;
float prevfield1;
float prevfield2;

float dipoleMomentX;
float dipoleMomentY;
float dipoleMomentZ;

float k = 10000; // defined constent determined from simulations


void setup() {
    Serial.begin(9600);
    Serial.println("<Arduino is ready>");
}

void loop() {
    recvWithEndMarker();
    if (newData == true){ 
      setNewData();
      showNewData();
      detumblingDipoleMoment();
      printDipoleMomenttoCSV();
    }
}

void setPrevField(){
    prev_t_s = String(t_s).toFloat();
    prevfield0 = String(field0).toFloat();
    prevfield1 = String(field1).toFloat();
    prevfield2 = String(field2).toFloat();
}

void showPrevField(){
        Serial.println("Previous Data: ");
        if (prevfield0 != NULL) Serial.println(prevfield0,5);
        if (prevfield1 != NULL) Serial.println(prevfield1,5);
        if (prevfield2 != NULL) Serial.println(prevfield2,5);
} // Unused currently


void recvWithEndMarker() {
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;

            if (ndx >= numChars) {
                ndx = numChars - 1;   // prevent overflow
            }
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
            // time2 = time1; // save time the previous data was read
            // time1 = micros();// save time the new data was read
        }
    }
} // Processes the CSV File

void showNewData() {
    if (newData == true) {
        // tokenize AFTER full line is received
        t_s = strtok(receivedChars, ",");
        field0 = strtok(NULL, ",");
        field1 = strtok(NULL, ",");
        field2 = strtok(NULL, ",");

        Serial.println("Recieved Data: ");
        if (field0 != NULL) Serial.println(field0);
        if (field1 != NULL) Serial.println(field1);
        if (field2 != NULL) Serial.println(field2);
        newData = false;
        setPrevField();  
    }
}// Currently Unused Function

void setNewData(){
  if (newData == true) {
        // tokenize AFTER full line is received
        t_s = strtok(receivedChars, ",");
        field0 = strtok(NULL, ",");
        field1 = strtok(NULL, ",");
        field2 = strtok(NULL, ",");
        newData = false;
  }
}

void detumblingDipoleMoment(){
 dipoleMomentX = (-k) * ((String(field0).toFloat() - prevfield0) / (0.5)) * 1e-9; // this should be 0.5 since each time step is 0.5
 dipoleMomentY = (-k) * ((String(field1).toFloat() - prevfield1) / (0.5)) * 1e-9;
 dipoleMomentZ = (-k) * ((String(field2).toFloat() - prevfield2) / (0.5)) * 1e-9;
}

void calculateCurrent(){
 //XCurrent = dipoleMomentX / (n * area);
 //YCurrent
 //ZCurrent = 
    // each dipole moment is a different current output - 1 fro each torquerod 
    // for mapping current limit is -0.2 - 0.2 Am
    // 3.3 V limit
} // Unfinished Function

void printDipoleMomenttoCSV(){ //Optimise this function eventually
 Serial.print("Dipole: ");
 Serial.print(t_s);
 Serial.print(",");
 Serial.print(dipoleMomentX);
 Serial.print(",");
 Serial.print(dipoleMomentY);
 Serial.print(",");
 Serial.println(dipoleMomentZ);
 {
  // All the Mapping stuff is being tested still 
  //Serial.print("Mapped data: "); 
  //Serial.print (round(mapf(dipoleMomentX,-1,1,0,255)));
  //Serial.print(",");
  //Serial.print(round(mapf(dipoleMomentY,-1,1,0,255)));
  //Serial.print(",");
  //Serial.println(round(mapf(dipoleMomentZ,-1,1,0,255)));
 }
}

float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
     float result;
     result = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
     return result;
}

