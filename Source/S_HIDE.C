/*

Program to embed any file to any file from the
backside of the File.  Please note that the size of the message
file must be < 20% of the host file for better performance.
This program uses algorithm to change LSB of a Byte of the host
file by a single bit. This program will give
best performance in uncompressed image file, audio file,video file,
sound file etc. However this method may fail in .jpg or .jpeg file specially
for mixing a big file in a .jpeg file.
*/


#include<stdio.h>
double nbyte,nbit;
struct tag1
{
unsigned char b[8];
}data1;

struct tag2
{
unsigned char ch;
}data2;

struct tag3
{
unsigned char b[8];
}data3;

struct tag11
{
unsigned char b[4];
}data11;

struct tag33
{
unsigned char b[2];
}data33;

void data_hide_4th();
void enter_pass_word();
void bit_pattern();
void bit_32_pattern();
void filesize_embed();

void main()
{
char choice;
int flag;
flag=0;
	while(flag==0)
	{
	clrscr();
	gotoxy(25,9);
	printf("Data Hiding and Retrieval");
	gotoxy(25,11);
	printf("1. To Hide Data");
	gotoxy(25,12);
	printf("2. Exit");
	gotoxy(25,16);
	printf("Enter your choice : 1,2--->");
	choice=getch();
		switch(choice)
		{
			case '1' : data_hide_4th();
				      break;
			case '2' : flag=1;
				   clrscr();
				   gotoxy(30,13);
				   printf("Thank you.");
				   choice=getch();
				   fcloseall();
		}
	}
}


/*Function to enter password in a file */

void enter_pass_word(char file1[])
	{
	FILE *fp1;
	long int n,i;
	char pass_wrd[11],ch;
	clrscr();
	printf("\nEnter password within 10 characters :");
	i=0;
	while(i<10)
	{
	ch=getch();
	if(ch==13)
	break;
	printf("*");
	pass_wrd[i]=ch;
	i++;
	}
	fp1=fopen(file1,"rb+");
	fseek(fp1,-100,2);
	fseek(fp1,0,1);
	for(n=0;n<i;n++)
	{
	ch=pass_wrd[n]^255;
	fprintf(fp1,"%c",ch);
	}
	ch=255;
	fprintf(fp1,"%c%c",ch,ch);
	fclose(fp1);
	}


/* Function to change only 4-th bit of host file. Here One Byte of the source File will be embedded in 8 Bytes in the Host file and	every 4-th bit of the host file will be modified. */

void data_hide_4th()
	{
	double nbyte_per,nbit_per;
	unsigned char ch,ch1;
	long int nhost,nstag,k,i,j,k1,l,p1,n,size1;
	int b1[8],b2[8],b32[32],flag;
	char file1[50],file2[50];
	FILE *fp1,*fp2;
	clrscr();
	gotoxy(10,13);
	printf("Enter Host File name---->");
	gets(file1);
	gotoxy(10,14);
	printf("Enter File name to Hide->");
	gets(file2);
	enter_pass_word(file1);
	fcloseall();
	fp1=fopen(file1,"rb+");
	fp2=fopen(file2,"rb");
	nhost=nstag=0;
	nbit=nbyte=0;
		while(fscanf(fp1,"%c",&ch>0)>0)
		nhost++;
		while(fscanf(fp2,"%c",&ch)>0)
		nstag++;

	/* To justify the size of host */

	size1=8*nstag+2000;
	if(size1>nhost)
	{
	printf("\n\n\n***Size of hidden file is comparable to host file.***\n");
	printf("Embedding not possible.Sorry!\n");
	printf("\nPress any key to continue--->");
	getch();
	fcloseall();
	return;
	}

	bit_32_pattern(nstag,b32);
	fclose(fp1);
	filesize_embed(file1,b32);
      fp1=fopen(file1,"rb+");
	rewind(fp2);

	printf("\nSize of host file: <%s>=%ld Bytes\n",file1,nhost);
	printf("Size of file=<%s>  to be embedded=%ld Bytes\n",file2,nstag);
	n=nhost-8*nstag-100;/* n=Byte position where the file pointer will be moved */
	fseek(fp1,n,0);
	fseek(fp1,0,1);
	k=0;
		for(l=1;l<=nstag;l++)
		{
		fread(&data1,sizeof(data1),1,fp1);
		fread(&data2,sizeof(data2),1,fp2);
		k++;
		ch=data2.ch;
		bit_pattern(ch,b1);

		for(i=0;i<8;i++)
		{
		ch1=data1.b[i];
		bit_pattern(ch1,b2);
			if(b2[4]!=b1[i])
			{
			b2[4]=b1[i];
			nbit=nbit+1;
			nbyte=nbyte+1;
			}
		p1=128;
		k1=0;
			for(j=0;j<8;j++)
			{
			k1=k1+b2[j]*p1;
			p1=p1/2;
			}
		data1.b[i]=k1;
		}
		fseek(fp1,-8,1);
		fseek(fp1,0,1);
		fwrite(&data1,sizeof(data1),1,fp1);
		fseek(fp1,0,1);
		}
        nbyte_per=100*nbyte/(8*nstag);
	nbit_per=100*nbit/(8*8*nstag);
	printf("\nPercentage of Byte(s) changed=%6.2lf %",nbyte_per);
	printf("\nPercentage of Bit(s)  changed=%6.2lf %", nbit_per);
	printf("\nPosition of Byte from where data inserted=%ld Byte\n",n+1);
	printf("\nNumber of Bytes inserted =%ld Bytes\n",k);
	fcloseall();
	printf("\nPress any key to go to main menu-->");
	getch();
	}

	/* Function to embed the size of the embed message into
	host file. The size of the souce file will be embeded
	form the 50-th byte of the host file from the end. */

	void filesize_embed(char file1[],int b32[])
	{
	FILE *fp1;
	long int i,j,n;
	int k,p,c;
	unsigned char ch1[4];
	fp1=fopen(file1,"rb+");
	fseek(fp1,-50,2);
	k=0;
	i=0;
	p=128;
	c=0;
	for(j=0;j<32;j++)
	{
	c++;
	k=k+b32[j]*p;
	p=p>>1;
		if((c%8)==0)
		{
		data11.b[i++]=k;
		k=0;
		p=128;
		}
	}
	fwrite(&data11,sizeof(data11),1,fp1);
	fclose(fp1);
	}

	
	/* Function to convert any unsigned character to 8 bit
	pattern */

	void bit_pattern(unsigned char ch,int bit[])
	{
	int mask,i,j,n,k,t;
	for(i=0;i<8;i++)
	bit[i]=0;
	mask=128;
	j=0;
	n=ch;
		while(mask!=0)
		{
		t=n & mask;
			if(t>0)
			bit[j++]=1;
			else
			bit[j++]=0;
		mask=mask>>1;
		}
	}

	/* Function to convert any long int number to 32 bits binary number */

void bit_32_pattern(long int n,int b32[])
	{
	unsigned long int mask,i,j,k,t;
	for(i=0;i<32;i++)
	b32[i]=0;
	mask=2147483648;
	j=0;
		while(mask!=0)
		{
		t=n & mask;
			if(t>0)
			b32[j++]=1;
			else
			b32[j++]=0;
		mask=mask>>1;
		}
	}