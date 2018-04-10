#include <OneWire.h>


OneWire ds(14);
uint8_t data[12];
uint8_t addr[8];

int start_conversion(void)
{
	// The DallasTemperature library can do all this work for you!

	ds.reset();
	ds.select(addr);
	ds.write(0x44, 1);	// start conversion, with parasite power on at the end
}

// asynchrounous read result
float read_conversion_result(void)
{
	byte present = ds.reset();
	ds.select(addr);
	ds.write(0xBE);		// Read Scratchpad

	byte data[9];
	for (byte i = 0; i < 2; i++)	// we need 9 bytes
	{
		data[i] = ds.read();
	}
	ds.reset_search();
	float temp = (data[1] << 8) | data[0];	// just want temperature

	float Temperature = temp / 16;

	return Temperature;
}

/* *************************************************************** DS18B20 ****************************************************************** */
float DS18B20_Captura_temperatura(void)
{
	static float temperatura = 0;
	static long anterior = millis();


		if (!ds.search(addr)) {
			//no more sensors on chain, reset search
			ds.reset_search();
			return -1000;
		}

		if (OneWire::crc8(addr, 7) != addr[7])
			return -1000;

		if (addr[0] != 0x10 && addr[0] != 0x28)
			return -1000;
		ds.reset();
		ds.select(addr);
		ds.write(0x44, 1);

		ds.reset();
		ds.select(addr);
		ds.write(0xBE);

		for (int i = 0; i < 9; i++)
			data[i] = ds.read();
		ds.reset_search();

		uint8_t MSB = data[1];
		uint8_t LSB = data[0];

		float TRead = ((MSB << 8) | LSB);
		temperatura = TRead / 16;

	return temperatura;

}
