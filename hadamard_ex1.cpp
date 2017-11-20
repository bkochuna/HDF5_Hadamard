#include <iostream>
#include <stdlib.h>
#include <string>
#include <cmath>
#include "H5Cpp.h"
using namespace H5;

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

	/* output matrix to stdout
	for (i=0;i<order;i++)
	{
		for (j=0;j<order;j++)
		{	
			std::cout.width(4);
			std::cout<<data[i*order+j];
		}
		std::cout<<std::endl;
	}
        */

	// Turn off the auto-printing when failure occurs so that we can
	// handle the errors appropriately
	Exception::dontPrint();

	H5File *myh5file = NULL;
	try 
	{
		// Create a new file using the default property lists
		H5std_string    fname("hadamard.h5");
                myh5file = new H5File(fname, H5F_ACC_TRUNC);

		// Create the dataspace for a square matrix
 		hsize_t dims[2];
		dims[0] = order;
		dims[1] = order;
		DataSpace dataspace(2,dims);

		// Create the dataset
		H5std_string	dsetname("H");
                DataSet dataset = myh5file->createDataSet(dsetname, PredType::STD_I32BE, dataspace);

		// Write the matrix to the file
		dataset.write(data, PredType::NATIVE_INT);
	}
	catch (FileIException error)
	{
		//error.printError();
		return -1;
	}
	catch (DataSetIException error)
	{
		error.printError();
		return -1;
	}
	catch (DataSpaceIException error)
	{
		error.printError();
		return -1;
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












