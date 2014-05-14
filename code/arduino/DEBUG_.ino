void printLineReadings(){
  int readings[8];
  getReadings(readings);
  for(int i =0; i<8;i++){
    Serial.print(readings[i]);
    Serial.print('\t');
  }
  Serial.println();
}

void printRearLineReadings(){
  int readings[5];
  getRearReadings(readings);
  for(int i = 0; i< 5; i++){
    Serial.print(readings[i]);
    Serial.print('\t');
  }
  Serial.println();
}

void printFiltered4(){
  Serial.println(getRear4LineReading(100));
  Serial.println();
}

