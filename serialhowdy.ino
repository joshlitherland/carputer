const int bufsize=32;
char buf[bufsize];
char outbuf[bufsize];
int s;

void setup() {
  Serial.begin(115200,SERIAL_8N1);
  Serial.println("ATL1");
}

void loop() {
  Serial.println("Say something!\n");
  s=Serial.readBytesUntil('\n',buf,bufsize);
  if (s>0) {
    // We got some bytes!
    strncpy(outbuf,buf,s);
    outbuf[s-1]='\0';
    Serial.print("You said: ");
    Serial.println(outbuf);
  }
  delay(100);
}
