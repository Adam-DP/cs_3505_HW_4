

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


using namespace std;

// Note:  Our classes were declared in a cs3505 namepsace.
//        Instead of 'using namespace cs3505', I qualify the class names 
//        below with cs3505::

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

  // Loop for reading the file.  Note that it is controlled
  //   from within the loop (see the 'break').
  string next_line;

  while (getline(in, next_line))
    {
      // Read a word (don't worry about punctuation)


      // USED STACK OVERFLOW CODE TO LEARN HOW TO PARSE WHITESPACE STRING TO A VECTOR
      //https://stackoverflow.com/questions/236129/the-most-elegant-way-to-iterate-the-words-of-a-string
      stringstream ss(next_line);
      string buf;
      vector<string> tokens;
      while (ss >> buf)
	tokens.push_back(buf);
	  
      string word = tokens[0];

      //cout << next_line << endl; 

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
		
	  cout << "New Food Item " << UPC << " " << life << " " << name << " " << endl; 
	  	
	}
      else if(word.compare("Warehouse")==0)
	{
		string location = tokens[2];
	  cout << "New warehouse in " << location << endl;
	}
      else if(word.compare("Start")==0)
	{
	  // Move on to commands
	  cout << "START WORKING " << endl;
	  break;
	}

    }
	  
  while (getline(in, next_line))
    {
      // Read a word (don't worry about punctuation)


      // USED STACK OVERFLOW CODE TO LEARN HOW TO PARSE WHITESPACE STRING TO A VECTOR
      //https://stackoverflow.com/questions/236129/the-most-elegant-way-to-iterate-the-words-of-a-string
      stringstream ss(next_line);
      string buf;
      vector<string> tokens;
      while (ss >> buf)
	tokens.push_back(buf);
	  
      string word = tokens[0];

      //cout << next_line << endl; 

      if (in.fail())	break;


      if(word.compare("Receive:")==0)
	{
		string UPC = tokens[1];
		string days = tokens[2];
		string warehouse = tokens[3];
		cout << "Receive " << UPC <<  " " << days << " " << warehouse <<  endl;
	}
	else if(word.compare("Request:")==0)
	{
		string UPC = tokens[1];
		string days = tokens[2];
		string warehouse = tokens[3];
		cout << "Request " << UPC << " " << days <<  " " << warehouse <<  endl;
	}
else if (word.compare("Next") ==0)
{
	cout << "RUN NEXT DAY CALCULATIONS" << endl; 
}
	else if (word.compare("End") ==0)
{
	cout << "THE END" << endl; 
}
	


    }




  in.close();

  return 0;
}

