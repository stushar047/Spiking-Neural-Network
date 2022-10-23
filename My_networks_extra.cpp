#include <iostream>
#include <string>
#include<fstream>
#include<vector>
#include<map>
#include<vector>
#include <bits/stdc++.h>


class Neuron

{

  public:  
  int node_id;
  std::string type;
  double threshold;
  double leak;
  double std_potential;
  double refr_potential;
  int abs_period;
  double charge;
  int last_fire;
  int fire_count;
  int abs_time;
  std::vector< std::tuple<int, double, int> > to;
  std::string period_type;
};

class Synapse

{

  public:  
  double weight; 
  int delay;
  int from;
  int to;

};

class create_network

{   

public:
    
Neuron* neuron;

Synapse* synapse;

int num_input_neuron;

int num_synapse;

int num_output_neuron;

int syn_num=0;

int* create_random_number(int start, int stop, int amount)
{
 int* arr=new int[amount];   
 for(int i=0;i<amount;i++)
  {
    arr[i]=std::rand()%(stop+1-start)+start;
  }

 return arr; 
}

void intialize_all_neurons()
{  
   this->neuron=new Neuron[this->num_input_neuron + this->num_output_neuron];

   for (int i=0;i<this->num_input_neuron + this->num_output_neuron;i++)
   {
    this->neuron[i].threshold=0.6;
    this->neuron[i].leak=0.15;
    this->neuron[i].std_potential=0.0;
    this->neuron[i].refr_potential=-0.5;
    this->neuron[i].abs_period=2;
    this->neuron[i].charge = 0.0;
    this->neuron[i].period_type="std";
    this->neuron[i].last_fire = -1;
    this->neuron[i].abs_time= 0 ;
    this->neuron[i].fire_count = 0;

   }
 
}

void create_all_input_neurons()
{

    for (int i=0; i<this->num_input_neuron;i++)
    {
         this->neuron[i].node_id=i;
         this->neuron[i].type="input";
    }
}

void create_all_output_neurons()
{

    for (int i=this->num_input_neuron; i<this->num_input_neuron+this->num_output_neuron;i++)
    {
         this->neuron[i].node_id=i;
         this->neuron[i].type="output";
    }
} 

void create_synapse()
{
this->synapse=new Synapse[this->num_synapse];

int* from=new int[this->num_synapse];  
int* to=new int[this->num_synapse];  

from=create_network::create_random_number(0,this->num_input_neuron+this->num_output_neuron-1,this->num_synapse);
to=create_network::create_random_number(0,this->num_input_neuron+this->num_output_neuron-1,this->num_synapse);

    for (int i=0; i<this->num_synapse;i++)
    {
         this->synapse[i].from=from[i];
         this->synapse[i].to=to[i];
         this->synapse[i].weight=0.0;
         this->synapse[i].delay=0;
         
    }

// this->neuron[int(from)].to.push_back(int(to));

}

void add_synapse(int from, int to, double weight, int delay)
{

  this->synapse[this->syn_num].from=from;
  this->synapse[this->syn_num].to=to;
  this->synapse[this->syn_num].weight=weight;
  this->synapse[this->syn_num].delay=delay;
  this->syn_num = this->syn_num + 1; 
  this->neuron[from].to.push_back(std::make_tuple(to,weight,delay)); 
}

};

class event_processing
 {

public: 

// Neuron neuron; 

create_network net;

double** events; 

int sim_time;


void create_all_events()
{

this->events=new double*[this->sim_time];

for (int i=0;i<this->sim_time;i++)

{

this->events[i]=new double[this->net.num_input_neuron];

for (int j=0;j<this->net.num_input_neuron;j++)

{
this->events[i][j]=0.0;

}

}

} 

void add_events_neuron(int time, int neuron, double spike_value)

{

this->events[time][neuron]=spike_value;

}

void process_events()
{  

for(int i=0; i<this->sim_time; i++)
{

 printf("At Time %d \n", i); 

for (int k=0; k<this->net.num_input_neuron; k++) 

{ 

  printf("The neuron %d is in %s period\n",k,net.neuron[k].period_type.c_str());

  if (this->net.neuron[k].period_type=="std")
  {

  printf("Neuron %d will leak %lf charge and then accumulate %lf charge from current charge: %lf at time %d \n",k, this->net.neuron[k].leak, this->events[i][k], this->net.neuron[k].charge, i);  

  this->net.neuron[k].charge = this->net.neuron[k].charge + this->events[i][k]-this->net.neuron[k].leak;

  printf("Neuron %d charge %lf \n",k, this->net.neuron[k].charge); 

  if (this->net.neuron[k].charge<this->net.neuron[k].std_potential)
  {

  this->net.neuron[k].charge=this->net.neuron[k].std_potential;

  printf("Neuron %d charge %lf below std_potential %lf.\n",k, this->net.neuron[k].charge,this->net.neuron[k].std_potential); 

  printf("Neuron %d charge reset to  %lf.\n",k, this->net.neuron[k].charge,this->net.neuron[k].std_potential);
  
  }

  if (this->net.neuron[k].charge>this->net.neuron[k].threshold)

  {
   printf("Neuron %d charge exceed threshold %lf\n", k, this->net.neuron[k].threshold);

   this->net.neuron[k].period_type="refr";

   printf("The neuron %d will be in %s period\n",k,this->net.neuron[k].period_type.c_str());


  }

  } 

  else if (this->net.neuron[k].period_type=="refr")
  {

    
   if (this->net.neuron[k].charge != this->net.neuron[k].refr_potential)
   
   {

    printf("Neuron %d fires at time %d, Connections: %d \n.", k, i, this->net.neuron[k].to.size());

    std::tuple<int,double,int> tup;

    for (int l=0;l<this->net.neuron[k].to.size();l++)
    {

    tup = this->net.neuron[k].to[l];

    double cs;

    cs=std::get<1>(tup);

    printf("Connection to %d weight %lf Delay %d \n",std::get<0>(tup),std::get<1>(tup),std::get<2>(tup));

    this->events[i+1][std::get<0>(tup)] += 0.76;

    printf(" Event: %lf", this->events[i+1][std::get<0>(tup)]);

    }


    printf("Neuron %d is in absoluate refractory period\n", k);

    this->net.neuron[k].charge=this->net.neuron[k].refr_potential;

    printf("Neuron %d charge reset to %lf\n", k, this->net.neuron[k].charge);
   }

    this->net.neuron[k].abs_time += 1;

    if (this->net.neuron[k].abs_time == this->net.neuron[k].abs_period)
    {
      this->net.neuron[k].period_type="std";

      this->net.neuron[k].abs_time=0;

      this->net.neuron[k].charge=this->net.neuron[k].std_potential;

      printf("End of abs refractory period. Neuron %d charge %lf reset to  %lf.\n",k, this->net.neuron[k].charge,this->net.neuron[k].std_potential); 
    }
  } 

}

}

}

};

int main()
{

    create_network net;
    printf("Initial network created\n");
    net.num_input_neuron=2;
    net.num_output_neuron=1;
    net.num_synapse=3;
    net.intialize_all_neurons();
    net.create_all_input_neurons();
    net.create_all_output_neurons();

    printf("Initialization of all input and output neurons\n");
    for (int i=0;i<net.num_input_neuron+net.num_output_neuron;i++)
    {
  
        printf("Neuron id: %d \n", net.neuron[i].node_id);
        printf("Neuron Type: %s \n", net.neuron[i].type.c_str());
        printf("Threshold: %lf \n",net.neuron[i].threshold);
        printf("Std_potential:, %lf \n", net.neuron[i].std_potential);
        printf("Leak: %lf \n", net.neuron[i].leak);
        printf("Charge: %lf \n", net.neuron[i].charge);
    } 

    net.create_synapse();

    net.add_synapse(0,1,0.50,2);
    
    net.add_synapse(0,2,0.50,3);

    net.add_synapse(1,2,0.75,4);

    std::cout<<"Create random synapse"<<std::endl;

    for (int i=0;i<net.num_synapse;i++)
    {
        std::cout<< "from: " << net.synapse[i].from<<" to: "<<net.synapse[i].to<<"  Weight: "<<net.synapse[i].weight<<"  Delay: "<<net.synapse[i].delay<<std::endl;
    } 


  printf("Create and add events \n");

  event_processing ep;

  ep.net=net;

  ep.sim_time=7;

  ep.create_all_events();

  ep.add_events_neuron(0, 2, 0.75);

  ep.add_events_neuron(1, 0, 5.0);

  ep.add_events_neuron(5, 1, 5.0);


  for(int i=0;i<ep.sim_time;i++)
  {

  printf("At Time %d \n",i);
   
   for(int j=0;j<net.num_input_neuron;j++)

   {
    printf("At neuron %d ",j);

    printf("Apply spike %lf \n", ep.events[i][j]);

   } 

    std::cout<<std::endl; 
  }

 printf("Process those events \n");

 ep.process_events(); 

//  for (int i=0;i<5;i++)
//  {

//  printf("Charge: %lf \n", ep.net.neuron[i].charge);

//  }

}