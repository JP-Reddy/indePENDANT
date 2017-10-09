// g510chksum.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <string.h>

static unsigned char buff[1024*1024];

int main(int argc, char* argv[])
{
	FILE *fp,*fnp;
	unsigned short sum;
	unsigned long * leninheader;
	unsigned long length,i;
	size_t hasRead;
	
	if(3 != argc)
	{
		printf("USAGE: %s src dest \n",argv[0]);
		goto e0;
	}

	fp  = fopen(argv[1],"rb+");
	fnp = fopen(argv[2],"wb+");

	if(!fp || !fnp)
	{
		printf("%s open %s fail",argv[0],argv[1]);
		goto e1;
	}

	if(0 != fseek(fp,0,SEEK_END))
	{
		printf("seek error\n");
		goto e1;
	}

	length = ftell(fp);

	if(1024 * 1024 <= length)
	{
		printf("file length error \n");
		goto e1;
	}

	if(0 != fseek(fp,0,SEEK_SET))
	{
		printf("seek error\n");
		goto e1;
	}	

	hasRead = fread(buff,1,length,fp);

	if(hasRead != length)
	{
		printf("file read error %lu %lu\n",hasRead,length);
		goto e1;
	}

	leninheader = (unsigned long *)&buff[8];

	if(0x55 == buff[0] && 
	   0xaa == buff[1] &&
	   *leninheader == length)
	{
		for(sum = 0,i = 32;i < hasRead;i++)
		{
			sum += buff[i];
		}

		memcpy(buff+2,&sum,2);
		
		if(length != fwrite(buff,1,length,fnp))
		{
			printf("write error\n");
			goto e1;
		}

		fclose(fp);
		fclose(fnp);

		return 0;
	}
	else
	{
		printf("file error %x %x %u %u\n",buff[0],buff[1],length,*leninheader);
		goto e1;
	}
	return 0;
e1:
	if(fp)
	{
		fclose(fp);
	}

	if(fnp)
	{
		fclose(fnp);
	}
e0:
	return -1;
}

