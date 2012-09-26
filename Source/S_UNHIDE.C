/*Program to embed any file to any file from the
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


void data_unhide_4th();
int retrieve_pass_word();
long int filesize_retrieve();
void bit_pattern4();
void bit_pattern();



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
	gotoxy(25,12);
	printf("1. To Unhide Data");
	gotoxy(25,13);
	printf("2. Exit");
	gotoxy(25,16);
	printf("Enter your choice : 1,2--->");
	choice=getch();
		switch(choice)
		{
			case '1' : data_unhide_4th();
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


/*Function to retrieve password*/

int retrieve_pass_word(char file1[])
	{
	FILE *fp1;
	long int n,i,k,j;
	unsigned char pass_wrd[11],ch;
	int flag;
	clrscr();
	for(k=1;k<=3;k++)
	{
	flag=1;
	i=0;
	for(j=0;j<11;j++)
	pass_wrd[j]=255;
		printf("\nEnter password within 10 characters :");
		do
		{
		ch=getch();
		if(ch==13)
		break;
		printf("*");
		pass_wrd[i]=ch;
		i++;
		}while(i<10);
	fp1=fopen(file1,"rb+");
	fseek(fp1,-100,2);
	fseek(fp1,0,1);
	n=0;
		do
		{
		fscanf(fp1,"%c",&ch);
		ch=ch^255;
			if(ch==255)
			break;
			if(ch!=pass_wrd[n])
			{
			flag=0;
			printf("\n**incorrect password**\n");
			break;
			}
		n++;
		}while(n<i);
		if(flag==1)
		return 1;
	  }
	  return flag;
	  }

	/* Function to extract file size of embed file from
	host file */

	long int filesize_retrieve(char file1[])
	{
	unsigned long int nsize,i,j,k,p;
	int bit[8];
	FILE *fp1;
	unsigned char ch;
	k=0;
	fp1=fopen(file1,"rb");
	fseek(fp1,-50,2);
	fseek(fp1,0,1);
	fread(&data11,sizeof(data11),1,fp1);
	j=0;
	k=0;
	p=2147483648;
	for(i=0;i<4;i++)
	{
	 ch=data11.b[i];
	 bit_pattern(ch,bit);
	for(j=0;j<8;j++)
		{
		k=k+bit[j]*p;
		p=p>>1;
	     }
	 }
	 printf("\nNumber extracted from Host=%ld\n",k);
	 fclose(fp1);
	 return k;
	 }

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



/* Function to unhide data from 4-th bit of 8 consecutive Bytes from host file */

void data_unhide_4th()
	{
	unsigned char ch;
	long int nhost,k,l,i,n,j,k1;
	int bit[8],tst;
	void bit_pattern1();
	char file1[50],file2[50];
	FILE *fp1,*fp2;

	clrscr();
	gotoxy(10,13);
	printf("Enter File name where data is hidden------------>");
	scanf(" %[^\n]",file1);
	gotoxy(10,14);
	printf("Enter File name where hidden data to be copied-->");
	scanf(" %[^\n]",file2);
	tst=retrieve_pass_word(file1);
	if(tst==0)
	{
	printf("***Your password is wrong.You can not retrive data.***\n");
	printf("\nPress any key-->");
	getch();
	return;
	}
	fp1=fopen(file1,"rb+");
	fp2=fopen(file2,"wb");
	nhost=0;
	while(fscanf(fp1,"%c",&ch>0)>0)
	nhost++;
	fclose(fp1);

	printf("\nSize of host file: <%s>=%ld\n",file1,nhost);

	l=filesize_retrieve(file1);
	fp1=fopen(file1,"rb+");
	n=nhost-l*8-100;/* Position of Byte from where data to be extracted */
	fseek(fp1,n,0);
	fseek(fp1,0,1);

	for(j=1;j<=l;j++)
	{
	fread(&data3,sizeof(data3),1,fp1);
	k=0;
	k1=128;
	bit_pattern4(bit,data3);
		for(i=0;i<8;i++)
		{
		k=k+(bit[i])*k1;
		k1=k1/2;
		}

	fprintf(fp2,"%c",k);
	}

	printf("\nNumber of Bytes copied in <%s> =%ld Bytes\n",file2,l);
	fcloseall();
	printf("\nPress any key to go to main menu-->");
	getch();
	}

/* Function to extract 8 bits from lsb-s of 8 Bytes */

	void bit_pattern1(int bit[],struct tag3 data3)
	{
	int mask,i,j,n,k;
	for(i=0;i<8;i++)
	{
	k=(data3.b[i]&1>0)?1:0;
	bit[i]=k;
	}
	}

	/* Function to extract 8 bits from 4-th bit position of
	8 consecutive Bytes */

	void bit_pattern4(int bit[],struct tag3 data3)
	{
	int mask,i,j,n,k;
	for(i=0;i<8;i++)
	{
	mask=data3.b[i] & 8;
	if(mask>0)
	k=1;
	else
	k=0;
	bit[i]=k;
	}
	}