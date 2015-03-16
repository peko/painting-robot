
Serial port;
String port_name = "/dev/ttyUSB0";

void readSerial(){
  
  if ( port.available() > 0)  {
    
    String val = port.readStringUntil('\n');
    
    if(val == null) return;
    
    print(">>", val);
     
    if(val.equals("RDY\r\n")){
        ready = true;
        print("READY");
    }
    
    if(val.equals("OK\r\n")){
        nextPoint();
    }
   
  } 
}


void initSerial() {
  
  println(Serial.list());
  port = new Serial(this, port_name, 9600); 
  
}

void sendAngles() {
  if(ready && (old_angle1 != angle1 || old_angle2 != angle2 || old_angle3 != angle3)) {
    
    String cmd = "s "+nf(angle1,0,2)+" "+nf(angle2,0,2)+" "+nf(angle3,0,2)+" \r\n";
    println("<< " + cmd);
    port.write(cmd);
    old_angle1 = angle1;
    old_angle2 = angle2;
    old_angle3 = angle3;
  }
  
  if(first_point) delay(200);
}
