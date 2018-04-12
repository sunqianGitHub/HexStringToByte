
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

void HexStrToByte(const char* source, unsigned char* dest, int sourceLen)  
{  
	short i;  
	unsigned char highByte, lowByte;  

	for (i = 0; i < sourceLen; i += 2)  
	{  
		highByte = toupper(source[i]);  
		lowByte  = toupper(source[i + 1]);  

		if (highByte > 0x39)  
			highByte -= 0x37;  
		else  
			highByte -= 0x30;  

		if (lowByte > 0x39)  
			lowByte -= 0x37;  
		else  
			lowByte -= 0x30;  

		dest[i / 2] = (highByte << 4) | lowByte;  
	}  
	return ;  
}  

int main(void)
{

	//测试16进制与字符串互转
	FILE * patch_fp;

	patch_fp = fopen("patch_RX.bin", "r");

	if(NULL == patch_fp)
	{
		printf("fopen patch_RX.bin ERROR:%m", errno);
		return - 1;
	}

	FILE * hex_fp;

	hex_fp = fopen("patch_HEX.bin", "w+");

	if(NULL == hex_fp)
	{
		printf("fopen patch_HEX.bin ERROR:%m", errno);
		return - 1;
	}

	size_t r_len = 0;
	unsigned char tmp_buf = 0;

	do
	{
		r_len = fread(&tmp_buf, 1, 1, patch_fp);
	}
	while(r_len > 0 && fprintf(hex_fp, "%02X", tmp_buf));

	fclose(hex_fp);
	fclose(patch_fp);

	FILE * srt_fp;

	srt_fp = fopen("str.bin", "w+");

	if(NULL == srt_fp)
	{
		printf("fopen HEX_TO_STR.bin ERROR:%m", errno);
		return - 1;
	}

	hex_fp = fopen("patch_HEX.bin", "r");

	if(NULL == hex_fp)
	{
		printf("fopen patch_HEX.bin ERROR:%m", errno);
		return - 1;
	}
	unsigned short dest = 0;
	unsigned short tmp = 0;


	do
	{
		r_len = fread(&tmp, 2, 1, hex_fp);
		HexStrToByte((char *)&tmp, (char *)&dest, 2);
		fprintf(srt_fp, "%s", (char*)&dest);

		if(0 == dest)
		{
			fseek(srt_fp, 1, SEEK_CUR);
		}
	}while(r_len > 0);
	printf("ferror(srt_fp)=%d\n",ferror(srt_fp));
	printf("feof(hex_fp)=%d\n",feof(hex_fp));
	printf("r_len=%lu\n",r_len);

	fclose(hex_fp);
	fclose(srt_fp);
	printf("ght_process_applypatch success!\n");
	return 0;
}
