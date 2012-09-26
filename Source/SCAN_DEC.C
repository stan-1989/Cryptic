/*function to perform decryption on a block of data
containing 256 bits*/
/*  Main program */
#include<stdio.h>
#include<math.h>

#define siz 16
char lshift_residual(char c,int d);
char file1[20],file2[20];
char file3[50]={'T','E','M','P','.','D','A','T','\0'};
int times,places,secure;

struct mytag1
{
unsigned char str[2];
}mydata1;

void decrypt();
void decrypt1();
void msa_decryption();

void keygen();
FILE *fp1,*fp2;
unsigned char data2[32];
unsigned char mat[siz][siz],mat1[siz][siz];
void randomization();
void leftshift();
void cycling();
void upshift();
void rightshift();
void downshift();
void decrypt_bit();
void rshift(int);
void lshift(int);
int bleft[128];
int bright[128];
int n=0;

struct tag1
{
unsigned char ch[32];
}data1;

int data[256];
void bit_stream(char s[]);
void bit_wise_xor();
main(int argc,char *argv[])
{
long int i,j,k,l,n1,n2,nl;
if(argc==3)
{
clrscr();

msa_decryption(argv[1],file3);

fp1=fopen(file3,"rb");
fp2=fopen(argv[2],"wb");

fseek(fp1,0,2);
l=ftell(fp1);

n1=l/32;
n2=l%32;
fseek(fp1,0,0);
nl=0;
for(i=0;i<16;i++)
	for(j=0;j<16;j++)
	mat[i][j]= nl++;
clrscr();
keygen();
/* To envoke randomizetion() function secure-times */
for(i=1;i<=secure;i++)
randomization();
for(i=1;i<=n1;i++)
	{
	fread(&data1,sizeof(data1),1,fp1);
	bit_stream(data1.ch);
	decrypt_bit();
	}

if (n2!=0)
	{
	for(i=0;i<n2;i++)
	{
	fscanf(fp1,"%c",&data2[i]);

	data2[i]=lshift_residual(data2[i],5);
	fprintf(fp2,"%c",data2[i]);
	}
	}

fcloseall();
printf("\nData decryption is over.\n\n");
getch();
}
else
printf("\n***Invalid command line arguments***\n");
}


void bit_stream(char s[])
{
int i,k,j;
int mask=128;
n=0;
for(i=0;i<32;i++)
{
j=s[i];
mask=128;
	while(mask!=0)
	{
	k=(j & mask)?1:0;
	data[n++]=k;
	mask=mask>>1;
	}
}
}


void decrypt_bit()
{
	int i,j,x,x1,k,temp=0,s1,c,num[8],p2,sum;

	for(c=times-1;c>=0;c--)
	{
		bit_wise_xor();
		lshift(c);
	}

	s1=255;
	for(i=siz-1;i>=0;i--)
	{
		for(j=siz-1;j>=0;j--)
		{
			x=(int)mat[i][j];
			temp=data[s1];
			data[s1]=data[x];
			data[x]=temp;
			s1=s1-1;
			x=0;
		}
	}
	for(i=0;i<256;i=i+8)
		{
		j=0;
		k=0;
		num[j]=data[i];
		num[j+1]=data[i+1];
		num[j+2]=data[i+2];
		num[j+3]=data[i+3];
		num[j+4]=data[i+4];
		num[j+5]=data[i+5];
		num[j+6]=data[i+6];
		num[j+7]=data[i+7];
		sum=0;
		p2=1;
		for(k=7;k>=0;k--)
			{
			if(num[k]!=0)
				sum=sum+p2;
			p2=p2*2;
			}
		/*printf("%c",sum);*/
		fprintf(fp2,"%c",sum);
		}
}

void rshift(int d)
{
	int x,i,j;
	for(j=0;j<=d;j++)
	{
		x=data[n-1];
		for(i=n-1;i>0;i--)
		data[i]=data[i-1];
		data[0]=x;
	}

}

void lshift(int d)
{
	int x,i,j;
	for(j=d;j>=0;j--)
	{
		x=data[0];
		for(i=0;i<n-1;i++)
		data[i]=data[i+1];
		data[n-1]=x;
	}
}


char lshift_residual(char c,int d)
{
	int x,i,j,sum,p2;
	int bit_c[8];
	int k,n1;
	int mask=128;
	n1=0;
	mask=128;
	j=c;
	while(mask!=0)
	{
	k=(j & mask)?1:0;
	bit_c[n1++]=k;
	mask=mask>>1;
	}

	for(j=0;j<d;j++)
	{
		x=bit_c[0];
		for(i=0;i<n1-1;i++)
		bit_c[i]=bit_c[i+1];
		bit_c[n1-1]=x;
	}
        sum=0;
		p2=1;
		for(k=7;k>=0;k--)
			{
			if(bit_c[k]!=0)
				sum=sum+p2;
			p2=p2*2;
			}
return (char)sum;
}

void bit_wise_xor()
{
	int i,x,y,z;
	for(i=0;i<n-1;i=i+2)
	{
		x=data[i];
		y=data[i+1];
		z=(x ^ y);
		data[i]=z;
	}
}

/*start of randomization*/

	void randomization()
	{
	unsigned char ch;
	int  a=0,j,k,i,check,nc1;
	int nl;

	long int nblock;
	int ii,jj,ii1,jj1;


                        		for(a=1;a<=times;a++)
					{
					check=a%5;
						switch(check)
						{

						case 0 :cycling();
							upshift();
							rightshift();
							downshift();
							leftshift();
							downshift();
							rightshift();
							upshift();
							cycling();
							break;
						case 1 :leftshift();
							cycling();
							upshift();
							rightshift();
							downshift();
							rightshift();
							upshift();
							cycling();
                                                        leftshift();
							break;
						case 2 :downshift();
							leftshift();
							cycling();
							upshift();
							rightshift();
							upshift();
							cycling();
							leftshift();
                                                        downshift();
							break;
						case 3 :rightshift();
							downshift();
							leftshift();
							cycling();
							upshift();
							cycling();
							leftshift();
							downshift();
                                                        rightshift();
							break;
						case 4 :upshift();
							rightshift();
							downshift();
							leftshift();
							cycling();
							leftshift();
							downshift();
							rightshift();
                                                        upshift();
							break;

						}
					}
	}

	/*** rightshift() function starts ***/

	void rightshift()
	{
	unsigned char ch1,ch2,ch3;
	int j,k,m,r,c;
	for(j=0;j<siz;j++)
	{
		for(k=0;k<siz;k++)
		{
		ch1=mat[j][k];
		m=((j*siz)+k)+1;
		r=j;
		c=k+m;
			while(c>=siz)
			{
			r=r+1;
				if(r==siz) r=0;
				c=c-siz;
			}
			if(m>((siz*siz)/2))
			{
			c=c+1;
				if(c==siz) r++,c=0;
				if(r==siz) r=0;
			}
		mat1[r][c]=ch1;
		}
	}
	for(r=0;r<siz;r++)
	{
		for(c=0;c<siz;c++)
		{
		mat[r][c]=mat1[r][c];
		}
	}

	}

	void downshift()
	{
	unsigned char ch1,ch2,ch3;
	int j,k,m,r,c;
	for(j=0;j<siz;j++)
	{
		for(k=0;k<siz;k++)
		{
		ch1=mat[k][j];
		m=((j*siz)+k)+1;
		c=j;
		r=k+m;
			while(r>=siz)
			{
			c=c+1;
				if(c==siz) c=0;
				r=r-siz;
			}
			if(m>((siz*siz)/2))
			{
			r=r+1;
				if(r==siz) c++,r=0;
				if(c==siz) c=0;
			}
		mat1[c][r]=ch1;
		}
	}
	for(r=0;r<siz;r++)
	{
		for(c=0;c<siz;c++)
		{
		mat[r][c]=mat1[c][r];
	}
	}
	}

	void leftshift()
	{
	unsigned char ch1,ch2,ch3;
	int j,k,m,r,c;
	for(j=0;j<siz;j++)
	{
		for(k=siz-1;k>=0;k--)
		{
		ch1=mat[j][k];
		m=(j*siz)+(siz-1-k)+1;
		r=j;
		c=k-m;
			while(c<0)
			{
			r=r+1;
				if(r==siz) r=0;
				c=c+siz;
			}
			if(m>((siz*siz)/2))
			{
			c=c-1;
				if(c==-1) r++,c=siz-1;
				if(r==siz) r=0;
			}
		mat1[r][c]=ch1;
		}
	}
	for(r=0;r<siz;r++)
	{
		for(c=0;c<siz;c++)
		{
		mat[r][c]=mat1[r][c];
		}
	}
	}

	void upshift()
	{
	unsigned char ch1,ch2,ch3;
	int j,k,m,r,c;
	for(j=0;j<siz;j++)
	{
		for(k=siz-1;k>=0;k--)
		{
		ch1=mat[k][j];
		m=(j*siz)+(siz-1-k)+1;
		c=j;
		r=k-m;
			while(r<0)
			{
			c=c+1;
			if(c==siz) c=0;
			r=r+siz;
			}
			if(m>((siz*siz)/2))
			{
			r=r-1;
			if(r==-1) c++,r=siz-1;
			if(c==siz) c=0;
			}
		mat1[c][r]=ch1;
		}
	}
		for(r=0;r<siz;r++)
		{
			for(c=0;c<siz;c++)
			{
			mat[r][c]=mat1[c][r];
			}
		}
	}

	void cycling()
	{
	int j=0,k=0,r,c,size,start;
	unsigned char ch1,ch2,ch3;
	size=siz;
	start=siz-size;
	while(1)
	{
		if(start==siz/2) break;
		ch1=mat[j][k];
		if(j==start && k>=j && k!=size-1)   k++,mat1[j][k]=ch1;
		else if(k==size-1 && k>j && j!=size-1)    j++,mat1[j][k]=ch1;
		else if(j==size-1 && j>=k && k!=start)    k--,mat1[j][k]=ch1;
		else if(k==start && j>k && j!=start)     j--,mat1[j][k]=ch1;
		if(j==start && k==start)      start++,size--,k=j=start;
		if(start%2==1)
		while(1)
		{
		ch1=mat[j][k];
		if(k==start && j>=k && j!=size-1)   j++,mat1[j][k]=ch1;
		else if(j==size-1 && j>k && k!=size-1)    k++,mat1[j][k]=ch1;
		else if(k==size-1 && k>=j && j!=start)    j--,mat1[j][k]=ch1;
		else if(j==start && k>j && k!=start)     k--,mat1[j][k]=ch1;
		if(j==start && k==start)      start++,size--,k=j=start;
		if(start%2==0) break;
		}
	}
	for(j=0;j<siz;j++)
	{
		for(k=0;k<siz;k++)
		{
		mat[j][k]=mat1[j][k];
		}
	}
	}

/*start of keygen function*/

	void keygen()
	{
	unsigned char key_string[50];
	long int s,s1,s2,power,p=0;
	int l,n;
	unsigned char ch;
	FILE *fp1;

	s=0;
	clrscr();
	printf("\n\n\nEnter your secret key string(Max. 16 chars. long) :");
		while(1)
		{
		ch=getch();
		if(ch!=13)
		{
		printf("*");
		key_string[s++]=ch;
		}
		else
		{
		key_string[s]='\0';
		break;
		}
		}
	s=0;
	for(l=0;key_string[l]!='\0';l++)
	{
	power=1;
	for(n=1;n<=l;n++)
	{
		power=power*2;
	}
	s+=power*key_string[l];
	}
	s1=0;
	s2=s;
	while(s2!=0)
	{
	p++;
	s1=s1+((s2%10)*p);
	s2=s2/10;
	}
	secure=s%s1;
	if(secure==0)
	secure=s1;
	for( ;secure>32; )
	secure=secure-32;
	if(secure>32)
	secure=secure/32;
	s1=0;
	s2=s;
	while(s2!=0)
	{
	s1=s1+((s2%10)*p);
	s2=s2/10;
	p--;
	}
	times=s%s1;
	if(times==0)
	times=s1;
	for( ;times>32; )
	times=times-32;
	if(times>32)
	times=times/32;
	printf("\n\nNumber of times of the randomization process will continue :%d\n",times);
	printf("\nNumber of times of Decryption is to be done :%d\n",secure);
	printf("\nPress any key to continue--->");
	getch();
	}

/*end of keygen*/


/* Function To decrypt an encrypted file using
	MSA(Meheboob, Saima and Asoke ) algorithm */

	void msa_decryption(char file1[],char file3[])
	{
	char file4[50]={'T','E','M','P','1','.','D','A','T','\0'};
	long int i,nrec,nloop,flag,n1;
	int a;
	unsigned char ch;
	clrscr();
	nrec=0;

	/*keygen();
	randomization();*/

	fp1=fopen(file1,"rb");
	fp2=fopen(file4,"wb");
	n1=0;
	while(fscanf(fp1,"%c",&ch)>0)
	{
	fprintf(fp2,"%c",ch);
	n1++;
	}
	fcloseall();
	for(a=0;a<secure;a++)
	{
	fp1=fopen(file4,"rb");
	fp2=fopen(file3,"wb");
		while(fscanf(fp1,"%c",&ch)>0)
		nrec++;
	rewind(fp1);
	nloop=nrec/2;
		if((nrec%2)!=0)
		flag=1;
		else
		flag=0;
		for(i=1;i<=nloop;i++)
		{
		fread(&mydata1,sizeof(mydata1),1,fp1);
			if(mydata1.str[0] == mydata1.str[1])
			{
			decrypt1(mydata1.str[0],mat);
			decrypt1(mydata1.str[1],mat);
			}
			else
			decrypt(mydata1.str[0],mydata1.str[1],mat);
		}
		if(flag==1)
		{
		fscanf(fp1,"%c",&ch);
		fprintf(fp2,"%c",ch);
		}
	fcloseall();
	nrec=0;
	fp1=fopen(file3,"rb");
	fp2=fopen(file4,"wb");
		while(fscanf(fp1,"%c",&ch)>0)
		fprintf(fp2,"%c",ch);
	fcloseall();
	}  /*End of Decryption Process*/
	/* system("del TEMP.DAT"); */ /*  Removing temporary file */
	}

	void decrypt(unsigned char ch1,unsigned char ch2,unsigned char mat[siz][siz])
	{
	int j,k,m,n,p,q,c;
    	for(j=0,c=0;(c<2)&&(j<siz);j++)
    		for(k=0;k<siz;k++)
    			if(mat[j][k] == ch1)
    				{
					m=j;
					n=k;
					c++;
				}
    			else if(mat[j][k] == ch2)
    				{
					p=j;
					q=k;
					c++;
				}
			n--;
			q--;
			if(n==-1)
			{
				n=siz-1;
				m--;
				if(m==-1) m=siz-1;
			}
			if(q==-1)
			{
				q=siz-1;
				p--;
				if(p==-1) p=siz-1;
			}
			n+=q;
			q=n-q;
			n-=q;
	fprintf(fp2,"%c%c",mat[p][q],mat[m][n]);
	}

	void decrypt1(unsigned char ch1,unsigned char mat[siz][siz])
	{
	int j,k,m,n,q,c;
    	for(j=0,c=0;(c<1)||(j<siz);j++)
    		for(k=0;k<siz;k++)
    			if(mat[j][k] == ch1)
				{
					m=j;
					n=k;
					q=n-1;
					c++;
				}
	if(q==-1)
	{
	q=siz-1;
	m=m-1;
	if(m==-1) m=siz-1;
	}
	fprintf(fp2,"%c",mat[m][q]);
	}
	/*  End of MSA Algorithm  */



