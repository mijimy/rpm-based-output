
const uint8_t LED_PIN = 2;
const uint8_t relay = 1;
unsigned int input_PIN = 0;
volatile unsigned long rpm_tim, rpm_timold;
bool stat = 0, vtc_on = 0;
volatile bool up = 0;
#define rpm_on 3500
#define offset 200
#define potpin A2
void setup()
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(input_PIN, INPUT_PULLUP);
  PCMSK = 1 << input_PIN;                     // Pin change interrupt on Probe
  GIMSK = GIMSK | 1 << PCIE;              // Enable pin-change interrupt

}


void loop()
{ char buff[5];
  int val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)
  val = map(val, 0, 1023, 100, 800);
  if (up == 1)
  { long  rpm_ave = 30000000 / (rpm_tim);
    if (rpm_ave < 200)
      rpm_ave = 0;
    if (rpm_ave > 9000)
      rpm_ave = 9000;
    if (rpm_ave > val * 10  && vtc_on == 0)
    { digitalWrite(relay, LOW);
      vtc_on = 1;
    }
    else if ( rpm_ave < (val * 10 - offset) && vtc_on == 1)
    { digitalWrite(relay, HIGH);
      vtc_on = 0;
    }
    digitalWrite(LED_PIN, stat);
    stat = !stat;
  }
  else
  { digitalWrite(relay, HIGH);
    digitalWrite(LED_PIN, stat);
    vtc_on = 0;
    stat = !stat;
    delay(200);
  }
  up = 0;
  delay(200);




}

ISR(PCINT0_vect)
{ if (digitalRead(input_PIN) == 1)
  { rpm_tim = micros() - rpm_timold;
    rpm_timold = micros();
  }
  up = 1;
}
