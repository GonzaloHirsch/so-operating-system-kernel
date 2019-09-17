#include <stdint.h>

void * memset(void * destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

void * memcpy(void * destination, const void * source, uint64_t length)
{
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	//si el tamanio de la fuente y el destino son divisibles por
	//el tamanio de un entero de 32 bits, entonces se copia de a 32 bits (4 bytes)
	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	//de lo contrario, se copia de a bytes
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}

// funcion para intercambiar dos numeros
void swap(char *x, char *y) {
	char t = *x; *x = *y; *y = t;
}

// funcion para invertir el buffer
char* reverse(char *buffer, int i, int j)
{
	while (i < j)
		swap(&buffer[i++], &buffer[j--]);

	return buffer;
}

// Implementacion iterativa la funcion de itoa de C
char* itoa(int value, char* buffer, int base)
{
	// entrada invalida
	if (base < 2 || base > 32)
		return buffer;

	// se considera el valor absoluto
	int n = value;

	int i = 0;
	while (n)
	{
		int r = n % base;

		//if (r >= 10) 
		//	buffer[i++] = 65 + (r - 10);
		//else
			buffer[i++] = 48 + r;

		n = n / base;
	}

	// si el numero es 0
	if (i == 0)
		buffer[i++] = '0';

	//Si la base es 10 y el numero es negativo, se
	//lo precede con un signo menos. Para cualquier
	//otra base, el numero se asume positivo
	if (value < 0 && base == 10)
		buffer[i++] = '-';

	buffer[i] = '\0'; // terminar el string

	// invertir el string y devolverlo
	return reverse(buffer, 0, i - 1);
}
