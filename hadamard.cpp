#include <iostream>
#include <stdlib.h>
#include <string>
#include <cmath>

void Setup_Hadamard_matrix(int* data, int N, int order);
	
int main (int argc, char *argv[])
{

	if (argc!=2) // argc should be 2 for correct execution
	{
		std::cout<<"Usage: "<<argv[0]<<" needs one command line argument for the Hadamard matrix order"<<std::endl;
		std::cout<<"The argument can be any non-negative integer. \n";
		return 1;
	}
	
	int N=atoi(argv[1]); //get integer from command line argument. 
	int order=std::pow(2,N); //calculate the order of the Hadamard matrix. 	

   	/* Data initialization */
   	
	int i,j;
   	int* data=new int[order*order];
   
	/* Setup the Hadamard matrix */
	Setup_Hadamard_matrix(data,N,order);

	/* output matrix to stdou */
	for (i=0;i<order;i++)
	{
		for (j=0;j<order;j++)
		{	
			std::cout.width(4);
			std::cout<<data[i*order+j];
		}
		std::cout<<std::endl;
	}

	// if successfully terminated. 
	/* Clean up the memory allocated */
	delete [] data;
	
	return 0;
}

void Setup_Hadamard_matrix(int* data, int N, int order)
{
	int i,j,c,temp;
	data[0]=1;
	for (c=1;c<=N;c++)
	{
		temp=std::pow(2,c-1);
		for (i=0;i<temp;i++)
		{	
			for (j=temp;j<2*temp;j++)
				data[i*order+j]=data[i*order+j-temp];
		}
		for (i=temp;i<temp*2;i++)
		{
			for (j=0;j<temp;j++)
				data[i*order+j]=data[(i-temp)*order+j];
		}
		for (i=temp;i<temp*2;i++)
		{	
			for (j=temp;j<2*temp;j++)
				data[i*order+j]=-data[(i-temp)*order+j-temp];
		}
	}
}












