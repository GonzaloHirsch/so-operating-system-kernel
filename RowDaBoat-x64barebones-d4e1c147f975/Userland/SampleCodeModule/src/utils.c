#include <utils.h>

// ----------------------------------------------------------------------------------
// Este modulo es el modulo de utilitarios
// Hay funciones de libreria estandar, ademas de otras funciones agregadas por nosotros
// ----------------------------------------------------------------------------------

int pow(int base, int exponent);
int concat(char * to, const char * from);

/*FUNCIONES DE LA LIBRERIA ESTANDAR DE C*/

char getChar(){
	char buff[1];
	sys_read(0, buff, 1);
	return buff[0];
}

void putChar(char c){
	sys_write(0, &c, 1);
}

int scanf(const char * fmt, ...){
    va_list list;
    va_start(list, fmt);


		char str[MAX_BUFFER];       //donde se almacena lo que el usuario escribe
		int len = 0;                //la longitud del str
		char key = 0;               //current key

		//Le habilita al usuario a escribir hasta el enter, solo caracteres mayores a ascii 10

		while((key = getKey() & 0xFF) != '\n'){
			if (key == '\b'){
				str[len - 1] = 0;
				len--;
			} else {
				if (key >= 10 && len < MAX_BUFFER){
					str[len++] = key;
                    putChar(key);
				}
			}
		}

		str[len]=0;
		int i = 0;          //el cursor para el string de fmt
		int pos = 0;        //el cursor para el string str recien obtenido
		int matches = 0;    //cant de params que matchean
		int j = 0;          //cursor para string auxiliar

		int aux;

		//Lee el formato
    while(fmt[i] != 0 && str[pos]!=0){
    	if(fmt[i] == '%'){
        switch (fmt[i+1]) {

            case 'd':
						matches++;
						int * ptrD = va_arg(list, int*);
						char num[15] = {0};
						j = 0;
						//Itera el string que fue input
						while(str[pos] != ' ' && str[pos] != '\n' && str[pos] != 0 && str[pos] != '\t'){
						    if (isNumeric(str[pos])){
								num[j++] = str[pos++];
							} else {
                                printf("%s\n", str);
                                aux = atoi(num, j);
                                *ptrD = aux;
                                return -1;
							}
						}
						aux = atoi(num, j);
						*ptrD = aux;
						i += 2;
                    break;

            case 's':
                        matches++;
						char * ptrS = va_arg(list, char*);
						j = 0;
						char buffer[MAX_BUFFER] = {0};

						while(str[pos] != ' ' && str[pos] != '\n' && str[pos] != 0 && str[pos] != '\t'){
							buffer[j++] = str[pos++];
						}
                        buffer[j]=0; //me aseguro de que termino el str
						strcpy(ptrS, buffer);
                    i += 2;
                    break;

            case 'c':
					matches++;
					char * ptrC = va_arg(list, char*);
					j = 0;

					if(str[pos] != ' ' && str[pos] != '\n' && str[pos] != 0 && str[pos] != '\t'){
						*ptrC = *(str + pos);
						pos++;
					}

					i += 2;
					break;

            default:

                    break;

          }
        }
    	else {
            if(str[pos++]==fmt[i++]);
            else return matches;
    	}
    }

		return matches;
}

void printf(char * str, ...){
    va_list list;
    va_start(list, str);
    int i = 0;

		char newStr[MAX_BUFFER] = {0};
		int len = 0;

    while(str[i] != 0){
    	if(str[i] == '%' && (i == 0 || str[i-1] != '\\')){
            char buffer[MAX_BUFFER] = {0};
            switch (str[i+1]) {
                case 'd':
                    itoa((int) va_arg(list,int), buffer, 10);
                    len += concat((newStr + len), buffer);
                    i += 2;
                    break;
                case 's':
										len += concat((newStr + len), va_arg(list,char*));
                    i += 2;
                    break;
                default:
                    i += 2;
                    break;
            }
        }
        else {
            newStr[len] = str[i];
						len++;
						i++;
        }
    }

		newStr[len] = 0;
		len++;
		sys_write(0, newStr, len);
}

/* ------------------------------- */
/* FUNCIONES HELPER */

int concat(char * to, const char * from){
	int i = 0;
	while(from[i] != 0){
		to[i] = from[i];
		i++;
	}
	return i;
}

// intercambiar dos numeros
void swap(char *x, char *y) {
	char t = *x; *x = *y; *y = t;
}

// invertir el contenido de buffer
char* reverse(char *buffer, int i, int j)
{
	while (i < j)
		swap(&buffer[i++], &buffer[j--]);

	return buffer;
}

int atoi(const char* buffer, int len){
	int i = 0;
	int result = 0;


	while(buffer[i] != 0){
		result += (pow(10, --len) * (buffer[i] - 48));
		i++;
	}
	return result;
}

int pow(int base, int exponent){
	int result = 1;
	for (int i = 0; i < exponent; i++){
		result = result * base;
	}
	return result;
}

// Implementacion iterativa de itoa
char* itoa(int value, char* buffer, int base)
{
	// numero invalido
	if (base < 2 || base > 32)
		return buffer;

	// se considera el valor absoluto del numero
	int n = value;

	int i = 0;
	while (n)
	{
		int r = n % base;

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

/* ------------------------------- */

int isNumeric(char c){
	return '0' <= c && c <= '9';
}

char getKey(void){
	char buff;
	sys_get_key(0, &buff);
	return buff;
}

void getTime(char * buff){
	int seconds = sys_time(SECONDS);
	int minutes = sys_time(MINUTES);
	int hours = sys_time(HOURS);

	if (hours < 10){
		itoa(0, buff, 10);
		itoa(hours, buff + 1, 10);
	} else{
		itoa(hours, buff, 10);
	}

	*(buff + 2) = ':';

	if (minutes < 10){
		itoa(0, buff + 3, 10);
		itoa(minutes, buff + 4, 10);
	} else{
		itoa(minutes, buff + 3, 10);
	}

	*(buff + 5) = ':';

	if (seconds < 10){
		itoa(0, buff + 6, 10);
		itoa(seconds, buff + 7, 10);
	} else{
		itoa(seconds, buff + 6, 10);
	}
	*(buff + 8) = 0;
}

void getDate(char * buff){
	int day = sys_time(DAY_OF_MONTH);
	int month = sys_time(MONTH);
	int year = sys_time(YEAR);

	if (day < 10){
		itoa(0, buff, 10);
		itoa(day, buff + 1, 10);
	} else{
		itoa(day, buff, 10);
	}

	*(buff + 2) = '/';

	if (month < 10){
		itoa(0, buff + 3, 10);
		itoa(month, buff + 4, 10);
	} else{
		itoa(month, buff + 3, 10);
	}

	*(buff + 5) = '/';

	year = 2000 + year;
	itoa(year, buff + 6, 10);

	*(buff + 10) = 0;
}

void makeSound(int freq, int time){
	sys_beep(freq, time);
}

void goToSleep(int ticks){
	sys_sleep(ticks);
}

void writeKey(char * key){
	sys_write_key(0, key);
}

int strcmp(const char * stra, const char * strb){
	int result = 0;
	int i;
	for (i = 0; *(stra + i) != 0 && *(strb + i) != 0 && !result; i++){
		if (*(stra + i) < *(strb + i)){
			return -1;
		} else if (*(stra + i) > *(strb + i)){
			return 1;
		}
	}
	if (*(stra + i) == 0 && *(strb + i) == 0 && result != 0){
		return 0;
	} else if (*(stra + i) != 0 && *(strb + i) == 0){
		return 1;
	} else if (*(stra + i) == 0 && *(strb + i) != 0){
		return -1;
	}
	return result;
}

void strcpy(char * dst, char * src){
    while((*(dst++) = *(src++)) != 0);
}

int strlen(const char * str){
	int i = 0;
	for(;str[i] != 0; i++);
	return i;
}

void print(const char * str){
  sys_write(1, str, strlen(str));
}

void printN(const char * str, int length){
  sys_write(1,str,length);
}
