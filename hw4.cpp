

/*
 * This method will parse input from the console to run our warehouse 
 *
 * 
 */

#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <vector>
#include "model.h"
#include <set>


using namespace std;
//using namespace cs3505;


int main(int argc, char* argv[])
{
  // Check to make sure that there is an argument passed in
  if(argc==1)
    {
      std::cout << "Error: need to pass in an argument \n";
      return 0;
    }
  // Check to make sure there aren't too many arguments
  if(argc>=3)
    {
      std::cout << "Error: Too many arguments. Please call the method with only one argument\n";
      return 0;
    }

  ifstream in(argv[1]); // Read file

  if(!in) 
    cout << "There was an issue opening the file" << endl; 


  string next_line;

// Initialize the Model
	cs3505::model m;


// Parse the Header information Line By Line
  while (getline(in, next_line))
    {
      // USED STACK OVERFLOW CODE TO LEARN HOW TO PARSE WHITESPACE STRING TO A VECTOR
      //https://stackoverflow.com/questions/236129/the-most-elegant-way-to-iterate-the-words-of-a-string
      stringstream ss(next_line);
      string buf;
      vector<string> tokens;
      while (ss >> buf)
	tokens.push_back(buf);
	  
      string word = tokens[0];



      if (in.fail())	break;


      if(word.compare("FoodItem")==0)
	{
		string UPC = tokens[4];
		string life = tokens[7];
		int count = 9;
	  	string name = tokens[count];
	  	count++;
	  	while(count < tokens.size())
	    {
	      name.append(" " + tokens[count]);
	      count++;
	    }
		

	  	m.add_food_type(UPC, life, name);	
	}
      else if(word.compare("Warehouse")==0)
	{
		int count = 2;
	  	string location = tokens[count];
	  	count++;
	  	while(count < tokens.size())
	    {
	      location.append(" " + tokens[count]);
	      count++;
	    }

		m.add_warehouse(location);
	}
      else if(word.compare("Start")==0)
	{
	  // Move on to commands
      m.set_start_date(tokens[2]);
	 
	  break;
	}

    }
	  
// LOOP THROUGH THE REQUESTS LINE BY LINE
  while (getline(in, next_line))
    {
      // USED STACK OVERFLOW CODE TO LEARN HOW TO PARSE WHITESPACE STRING TO A VECTOR
      //https://stackoverflow.com/questions/236129/the-most-elegant-way-to-iterate-the-words-of-a-string
      stringstream ss(next_line);
      string buf;
      vector<string> tokens;
      while (ss >> buf)
	tokens.push_back(buf);
	  
      string word = tokens[0]; // Get the first word to decide what will be done
      if (in.fail())	break;


      if(word.compare("Receive:")==0)
	{
		string UPC = tokens[1];
		string quant = tokens[2];
		int count = 3;
	  	string warehouse = tokens[count];
	  	count++;
	  	while(count < tokens.size())
	    {
	      warehouse.append(" " + tokens[count]);
	      count++;
	    }

		m.receive_item(UPC, quant, warehouse);

	}
	else if(word.compare("Request:")==0)
	{
		string UPC = tokens[1];
		string quant = tokens[2];
		int count = 3;
	  	string warehouse = tokens[count];
	  	count++;
	  	while(count < tokens.size())
	    {
	      warehouse.append(" " + tokens[count]);
	      count++;
	    }

		m.add_request(UPC, quant, warehouse);
		

	}
else if (word.compare("Next") ==0)
{	
	// Finish the receives and requests for the day and increments the day
	m.conclude_day(); 
}
	else if (word.compare("End") ==0)
{
	// finish the receives and requests for the day
	// print statistics
	m.process_requests();
	m.print_statistics();

}
	


    }




  in.close();

  return 0;
}

