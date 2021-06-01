#include <stdio.h>


int main()
{
	int i,j,k;
	char* methods[] = { "GET", 
		"POST", 
		"HEAD",
		"PUT",
		"DELETE",
		"CONNECT",
		"TRACE",
		"PATCH"
	};

	for(;i<8;i++)
	{
		k = 0;
		for(j = 0; methods[i][j] != '\0'; j++)
		{
			k += methods[i][j];
		}
		printf("#define CSUM_%s %i\n", methods[i], k);
	}


}
