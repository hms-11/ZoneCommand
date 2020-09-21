# ZoneCommand
6 - Zone Control Board based on ATMEGA328P IC
Open Source 6 Zone Control Board designed to be easily configurable for a wide range of applications. 
This designed to be powered off of any 12V supply with its own on board voltage regulator (7805 footprint) to supply 5V to the board.
All inputs have an ADC as well as on-board RC filters for noise mitigation. 
Each zone has a corresponding indicator LED to let you know if the zone is active.
Outputs can be easily configured between active HIGH (5V) and active LOW (0V) in code.
Output had dedicated 5V and GND pins for outputs requiring power. 
Programming through TXD/RXD pins using a USB/Serial adapter.
All settings aim to be easily settable by the non-familiar with coding. All configuration settings aim to be well labled in the code.
