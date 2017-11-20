#include <iostream>
#include <stdlib.h>
#include <string>
#include <cmath>
#include <ctime>
#include <unistd.h>
#include "H5Cpp.h"
using namespace H5;

void Setup_Hadamard_matrix(int* data, int N, int order);
std::string getDateTime();
std::string getHost();
std::string getUser();
	
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
		// Open existing file for reading/writing
		H5std_string    fname("hadamard.h5");
                myh5file = new H5File(fname, H5F_ACC_RDWR);
	}
	catch (FileIException error)
	{
		// Create a new file using the default property lists
		H5std_string    fname("hadamard.h5");
                myh5file = new H5File(fname, H5F_ACC_TRUNC);
	}


	Group *group1 = NULL;
	try
	{
                //Create groups
                group1 = new Group(myh5file->createGroup("/Hadamard"));
        }
        catch (FileIException error)
        {
		group1 = new Group(myh5file->openGroup("/Hadamard"));
	}
        try {

		Group * group2 = new Group(group1->createGroup(std::to_string(N)));

		/* Create dataspace for attribute */
		DataSpace attr_dataspace(H5S_SCALAR);
		/* Create new string datatype for attribute */
		StrType str_type(PredType::C_S1, 256);

		//Create attribute for User
		Attribute myatt = group2->createAttribute("User",str_type,attr_dataspace);
		myatt.write(str_type,getUser());

		//Create attribute for Host
		myatt = group2->createAttribute("Host",str_type,attr_dataspace);		
		myatt.write(str_type,getHost());

		//Create attribute for Date
		myatt = group2->createAttribute("Date and Time",str_type,attr_dataspace);		
		myatt.write(str_type,getDateTime());
      
		// Create the dataspace for a square matrix
 		hsize_t dims[2];
		dims[0] = order;
		dims[1] = order;
		DataSpace dataspace(2,dims);

		// Modify dataset creation property to enable chunking
		hsize_t chunk_dims[2];
		chunk_dims[0] = max(4,N/8)
		chunk_dims[1] = chunk_dims[0];
		DSetCreatPropList  *plist = new  DSetCreatPropList;
		plist->setChunk(2, chunk_dims);
		
		// Set ZLIB (DEFLATE) Compression using level 6.
		plist->setDeflate(6);

		// Create the dataset
		H5std_string	dsetname("H");
                DataSet dataset = group2->createDataSet(dsetname, PredType::STD_I32BE, dataspace, *plist);

		// Write the matrix to the file
		dataset.write(data, PredType::NATIVE_INT);

	}
	catch (FileIException error)
	{
		//error.printError();
		return -1;
	}
	catch (GroupIException error)
	{
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


std::string getDateTime()
{
  time_t rawtime;
  struct tm * timeinfo;
  char buffer[80];

  time (&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(buffer,sizeof(buffer),"%d-%m-%Y %I:%M:%S",timeinfo);
  std::string str(buffer);

  return str;
}

std::string getUser()
{
	char username[LOGIN_NAME_MAX];
	getlogin_r(username, LOGIN_NAME_MAX);
	std::string str(username);
	return str;
}

std::string getHost()
{
	char hostname[HOST_NAME_MAX];
	gethostname(hostname, HOST_NAME_MAX);
	std::string str(hostname);
	return str;
}




