#include <iostream>
#include <string>
#include<fstream>
#include<vector>
#include<map>
#include <typeinfo>
#include<cmath>

class data_processing
{
public:
std::string filename;
std::string write_filename,write_filename_std,write_filename_norm;
std::string* feature_name;
int N,m;
double** x;
double** x_std;
double** x_norm;
double* mean;
double* min_value;
double* max_value;
double* std;

void read_csv()
{
this->feature_name=new std::string[this->N];

std::ifstream ip(this->filename);

std::string data;

for (int j=0;j<this->N-1;j++)
{  
std::getline(ip,data,',');

this->feature_name[j]=data;
}



std::getline(ip,data,'\n');

feature_name[this->N-1]=data;

/**/

this->x=new double*[this->m];

for (int i=0;i<this->m;i++)
{
this->x[i]=new double[this->N];

for (int j=0;j<this->N-1;j++)
{  
std::getline(ip,data,',');

this->x[i][j]=std::atof(data.c_str());

}
std::getline(ip,data,'\n');

this->x[i][this->N-1]=std::atof(data.c_str());

}
/**/

}


void standarize()
{
this->mean = new double[N];

this->std = new double[N];

for (int j=0;j<this->N-1;j++)
{
double sum=0.0;  
for (int i=0;i<this->m;i++)
{
sum = sum + this->x[i][j];
}
this->mean[j]=sum/this->m;

// std::cout<<this->mean[j]<<std::endl;
}

for (int j=0;j<this->N-1;j++)
{
double std_var=0.0;  
for (int i=0;i<this->m;i++)
{
std_var = std_var + std::pow((this->x[i][j]-this->mean[j]),2);
}
this->std[j]=std::sqrt(std_var/this->m);
}


this->x_std=new double*[this->m];

for (int i=0;i<this->m;i++)
{
this->x_std[i]=new double[this->N];   

for (int j=0;j<this->N;j++)
{
this->x_std[i][j]=(this->x[i][j]-this->mean[j])/this->std[j];
}
}
}

void normalize()
{

this->min_value = new double[N];

for (int j=0;j<this->N-1;j++)
{
double temp = this->x[0][j]; 
for (int i=0;i<this->m;i++)
{
  if (temp > this->x[i][j])
  {
  temp = this->x[i][j];
  }
}
this->min_value[j]=temp;  
}

for (int j=0;j<this->N-1;j++)
{
double temp = this->x[0][j]; 
for (int i=0;i<this->m;i++)
{
  if (temp < this->x[i][j])
  {
  temp = this->x[i][j];
  }
}  
this->max_value[j]=temp;  
}

this->x_norm=new double*[this->m];

for (int i=0;i<this->m;i++)
{
this->x_norm[i]=new double[this->N];   

for (int j=0;j<this->N;j++)
{
this->x_norm[i][j]=(this->x[i][j]-this->min_value[j])/(this->max_value[j]-this->min_value[j]);
}
}
}

void write_main_csv()
{

std::ofstream myfile;

myfile.open(this->write_filename);

for (int i=0;i<this->m;i++)
{

for (int j=0;j<this->N;j++)
{  

myfile<<this->x[i][j];

myfile<<",";
}
myfile<<"\n";

}

myfile.close();

}

void write_std_csv()
{

std::ofstream myfile;

myfile.open(this->write_filename_std);

for (int i=0;i<this->m;i++)
{

for (int j=0;j<this->N;j++)
{  

myfile<<this->x_std[i][j];

myfile<<",";
}
myfile<<"\n";

}

myfile.close();

}


void write_norm_csv()
{

std::ofstream myfile;

myfile.open(this->write_filename_norm);

for (int i=0;i<this->m;i++)
{

for (int j=0;j<this->N;j++)
{  

myfile<<this->x_norm[i][j];

myfile<<",";
}
myfile<<"\n";

}

myfile.close();

}


};

int main()
{
  data_processing data;
  data.filename="test_csv.csv";
  data.write_filename="main.csv";
  data.write_filename_std="std.csv";
  data.write_filename_norm="norm.csv";
  data.N=6;
  data.m=150;
  data.read_csv(); 
  data.standarize();
  data.normalize();
  std::cout<<data.x[0][2]<<","<<data.mean[2]<<","<<data.std[2]<<","<<data.x_std[0][2]<<std::endl;
  std::cout<<data.x_norm[0][4]<<std::endl;
  data.write_main_csv();
  data.write_std_csv();
  data.write_norm_csv();
}