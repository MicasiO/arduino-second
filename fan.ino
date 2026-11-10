#include <Servo.h>
#include <MD_MAX72xx.h>

const int motor_pin = 9;
const int temp_pin = 1;
const int pmeter_pin = 0;

const int cs_led_pin = 11;
const int din_led_pin = 10;
const int clk_led_pin = 13;

#define ADC_VREF_mV 5000.0 // in millivolt
#define ADC_RESOLUTION 1024.0

uint16_t analog_read(uint8_t channel)
{
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC))
		;
	return ADC;
}

Servo motor;

MD_MAX72XX matrix = MD_MAX72XX(MD_MAX72XX::DR0CR1RR0_HW, din_led_pin,
			       clk_led_pin, cs_led_pin, 1);
const uint8_t digit[10][3] = { { 62, 34, 62 }, { 36, 62, 32 }, { 58, 42, 46 },
			       { 42, 42, 62 }, { 14, 8, 62 },  { 46, 42, 58 },
			       { 62, 42, 58 }, { 2, 2, 62 },   { 62, 42, 62 },
			       { 46, 42, 62 } };

void show_number(int n)
{
	matrix.clear();
	int tens = (n / 10) % 10;
	int ones = n % 10;
	for (int i = 0; i < 3; i++) {
		matrix.setColumn(i, digit[ones][2 - i]);
		matrix.setColumn(i + 4, digit[tens][2 - i]);
	}
}

float get_temp()
{
	analog_read(2);
	analog_read(temp_pin);

	long sum = 0;
	for (int i = 0; i < 100; i++) {
		sum += analog_read(temp_pin);
	}

	float res = (sum / 100.0) * 5.0 / 1024.0 * 100.0;
	return res;
}

int get_dial()
{
	analog_read(2);
	analog_read(pmeter_pin);

	long sum = 0;
	for (int i = 0; i < 100; i++) {
		sum += analog_read(pmeter_pin);
	}

	int res = map(sum / 100, 0, 1024, 50, 10);
	return res;
}

void setup()
{
	ADMUX = (1 << REFS0);
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

	Serial.begin(9600);
	motor.attach(motor_pin);

	matrix.begin();
	matrix.clear();
	matrix.control(MD_MAX72XX::INTENSITY, 0);
}

long last_tick = 0;
void loop()
{
	if (millis() - last_tick < 200)
		return;
	last_tick = millis();

	float dial = get_dial();
	float temp = get_temp();
	show_number(dial);

	if (temp > dial) {
		motor.write(0);
	} else {
		motor.write(90);
	}

	Serial.print("Temperature: ");
	Serial.print(temp);
	Serial.print(" C | Dial Target: ");
	Serial.println(dial);
}
