BOARD ?= arduino:avr:uno
PORT ?= /dev/ttyACM0

build:
	arduino-cli compile --fqbn $(BOARD) .

upload:
	arduino-cli compile --fqbn $(BOARD) --upload -p $(PORT) .

monitor:
	arduino-cli monitor -p $(PORT) -c baudrate=9600
