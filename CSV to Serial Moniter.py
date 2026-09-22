import serial
import time

ser = serial.Serial('COM4','9600') # open port
print("Serial port Opened")
time.sleep(1.75) # give the arduino time to start

 # theoretically data will be coming in fast enough that the with open look will never end unless the sensors stop
with open('mag_field.csv','r') as infile, open('dipole_moment.csv','w') as outfile:
        for line in infile:
           
            ser.write(line.encode('utf-8')) #Writes to arduino # not sure if it will process the headers - may have to add to outfile manually
            #time.sleep(0.5) # Slows it down enought for me to read - does not need - rn set for t_s val
            dipoleOutput = ser.readline().decode('utf-8')
            outfile.write(str(dipoleOutput)) # this should output the dipole moment to a saved file
            outfile.flush()
            print(dipoleOutput)
            #print("here")


          

ser.close() # close port