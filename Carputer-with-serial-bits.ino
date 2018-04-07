#include <RunningAverage.h>
#include <video_gen.h>
#include <TVout.h>
#include <fontALL.h>

TVout TV;
unsigned char x,y,i,laps;
float gastanksize=17.1;
RunningAverage avgmpg(20);
const int bufsize=16;
char buf[bufsize];
char outbuf[bufsize];
int s;

void setup()  {
  x=0;
  y=0;
  laps=0;
  TV.begin(NTSC);	//for devices with only 1k sram(m168) use TV.begin(_NTSC,128,56)
  TV.select_font(font6x8);
  redraw_screen();
  avgmpg.clear();
  Serial.begin(115200,SERIAL_8N1);
  Serial.println("ATL1");
}

void redraw_screen() {
  TV.clear_screen();
  TV.print(1,1,"MPG");  
  TV.print(TV.hres()/2+21,1,"Avg MPG");        
  TV.print(1,TV.vres()/2+20,"mi. to E");
  TV.print(TV.hres()/2+21,TV.vres()/2+20,outbuf);
  TV.draw_line(0,TV.vres()/2-10,TV.hres(),TV.vres()/2-10,1);
  TV.draw_line(TV.hres()/2,0,TV.hres()/2,TV.vres()-22,1);
  redraw_status();
}

void redraw_status() {
    for (i=TV.vres()-20; i<=TV.vres(); i++) {
      TV.draw_line(0,i,TV.hres(),i,1);
    }
    TV.draw_line(20,TV.vres()-20,20,TV.vres(),0);
}

void clear_icon() {
    for (i=TV.vres()-20; i<=TV.vres(); i++) {
      TV.draw_line(0,i,19,i,1);
    }
}

void loop() { 
  do_serial_io();
  //redraw_screen(); 
  render_mpg();
  render_avg();
  render_mitoe();
  render_voltage();
  draw_stop();
  TV.delay(100); 
}

void do_serial_io() {
  Serial.println("010c");
  s=Serial.readBytesUntil('\n',buf,bufsize);
  if (s>0) {
    strncpy(outbuf,buf,s);
    outbuf[s-1] = '\0';
  }
}

/* 
 *  Get the voltage. This is going to be a funky
 *  formula, so return it as a float and be done
 *  with it.
*/
float get_voltage() {
    return 14.8;
}

/*
 * Vehicle speed in km/h. We'll convert
 * that to MPH in another function. 'merica!
 */
int get_speed() {
    return random(0,70);
}

/*
 * This will give you the 
 */
int get_fuel_level() {
    return 22;
}

float get_fuel_rate() {
  return 16.78;
}

/*
 * Derived from a running average of mpg.
 * It would use an insane amount of memory to keep
 * track a sample every time this loop runs, so
 * a PRNG will select which samples to use.
 * The goal is to grab about 0.1% of mpg readings
 * 
 * If a reading is grabbed, add it to the running total.
 * Add one to the counter.
 * Divide the running total by the new counter value. That
 * is your average mpg.
 */
float derive_avg_mpg() {
  // Do we do anything at all here?
  // Only if this value is in the highest 5%
  if (random(1,65535) > 62258) {
      avgmpg.addValue( derive_mpg() );
  } // Stuff for if
  return avgmpg.getAverage();
} // Close function

/*
 * Derived from avg mpg and fuel level
 * Calculate the amount of gas remaining by multiplying
 * The remaining fuel percentage and the size of the fuel tank (in gallons).
 * Then multiply that by your average mpg to get miles remaining.
 */
int derive_mitoe() {
  int mitoe;
  mitoe=gastanksize * get_fuel_level() * derive_avg_mpg() / 100;
  return mitoe;
}

/*
 * get_speed() is going to give you km/h
 * get_fuel_rate() is going to give you L/h
 * Divide speed by rate and you have km/L.
 * Convert to MPG because this is the USA!
 */
float derive_mpg() {
  float mpg=0;
  float kml=0;
  kml = get_speed() / get_fuel_rate();
  mpg = kml * 2.352;
  return mpg;
}

void render_mpg() {
  TV.print (TV.hres()/4-12,TV.vres()/4-10,derive_mpg());
}

void render_avg() {
  TV.print (TV.hres()*3/4-4,TV.vres()/4-10,derive_mpg());  
}

void render_mitoe() {
  TV.print (TV.hres()/4-9,TV.vres()*3/4-20,derive_mitoe());    
}

void render_voltage() {
  TV.print (TV.hres()*3/4-9,TV.vres()*3/4-20,get_voltage());    
}

void draw_stop() {
  TV.draw_line(6,TV.vres()-2,14,TV.vres()-2,0);
  TV.draw_line(5,TV.vres()-3,15,TV.vres()-3,0);
  TV.draw_line(4,TV.vres()-4,16,TV.vres()-4,0);
  TV.draw_line(3,TV.vres()-5,17,TV.vres()-5,0);
  TV.draw_line(2,TV.vres()-6,18,TV.vres()-6,0);
  TV.draw_line(1,TV.vres()-7,19,TV.vres()-7,0);  
  TV.draw_line(1,TV.vres()-8,19,TV.vres()-8,0);
  TV.draw_line(1,TV.vres()-9,19,TV.vres()-9,0);
  TV.draw_line(1,TV.vres()-10,19,TV.vres()-10,0);
  TV.draw_line(1,TV.vres()-11,19,TV.vres()-11,0);
  TV.draw_line(1,TV.vres()-12,19,TV.vres()-12,0);
  TV.draw_line(1,TV.vres()-13,19,TV.vres()-13,0);
  TV.draw_line(1,TV.vres()-14,19,TV.vres()-14,0);
  TV.draw_line(2,TV.vres()-15,18,TV.vres()-15,0);
  TV.draw_line(3,TV.vres()-16,17,TV.vres()-16,0);
  TV.draw_line(4,TV.vres()-17,16,TV.vres()-17,0);
  TV.draw_line(5,TV.vres()-18,15,TV.vres()-18,0);
  TV.draw_line(6,TV.vres()-19,14,TV.vres()-19,0);
  TV.draw_line(7,TV.vres()-18,13,TV.vres()-18,1);
  TV.draw_line(12,TV.vres()-18,17,TV.vres()-13,1);
  TV.draw_line(17,TV.vres()-13,17,TV.vres()-8,1);
  TV.draw_line(16,TV.vres()-7,12,TV.vres()-3,1);
  TV.draw_line(13,TV.vres()-3,7,TV.vres()-3,1);
  TV.draw_line(7,TV.vres()-3,2,TV.vres()-8,1);
  TV.draw_line(2,TV.vres()-8,2,TV.vres()-13,1);
  TV.draw_line(7,TV.vres()-18,2,TV.vres()-13,1);
}

void draw_road() {
  for (i=2; i<=18 ; i++) {
    TV.draw_line(7,TV.vres()-19,i,TV.vres()-3,0);
    TV.draw_line(13,TV.vres()-19,i,TV.vres()-3,0);    
  }
  TV.draw_rect(7,TV.vres()-19,5,5,0,0);
  TV.draw_line(3,TV.vres()-3,8,TV.vres()-19,1);
  TV.draw_line(17,TV.vres()-3,12,TV.vres()-19,1);
  TV.draw_line(10,TV.vres()-17,10,TV.vres()-18,1);
  TV.draw_line(10,TV.vres()-14,10,TV.vres()-11,1);
  TV.draw_line(10,TV.vres()-7,10,TV.vres()-3,1);
}


void draw_warning() {
   for (i=1; i<=17 ; i++) {
    TV.draw_line(9,TV.vres()-19,i,TV.vres()-3,0);
  } 
  TV.draw_line(9,TV.vres()-16,9,TV.vres()-8,1);
  TV.draw_line(10,TV.vres()-15,10,TV.vres()-10,1);
  TV.draw_line(8,TV.vres()-15,8,TV.vres()-10,1);
  TV.draw_line(9,TV.vres()-6,9,TV.vres()-5,1);
  }

void draw_tree() {
  clear_icon();
  for (i=TV.vres()-5; i>=TV.vres()-19; i--) {
    TV.draw_line(10+random(i/2-41,i/35),i,10+random(i/2-41,i/2-35),i,0);
  }
}

