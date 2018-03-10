#ifndef MODEL_H
#define MODEL_H

#include <string>
#include "warehouse.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <map>
#include <queue>          // std::queue
#include <set>


namespace cs3505
{
	struct upc_popularity
	{
		std::string UPC;
		std::string item_name;	
		long long popularity;

		bool operator<(const upc_popularity & rhs) const
		{		
			// We want it in reverse order to be able to pull from the back of the vector (the max value)
			if(this->popularity < rhs.popularity)
			{
				return true;
			}
			else if(this->popularity == rhs.popularity)
			{
				if(this->UPC < rhs.UPC)
					return true;
			}
			return false;
		}	
	};

	struct requested_order
	{
		std::string item_name;
		long long quantity;
		std::string UPC;
		warehouse location;
	};

	 struct underfilled_order
	{
		boost::gregorian::date expiration_date;
		std::string UPC;
		std::string item_name;	

		bool operator<(const underfilled_order & rhs) const
		{	
			long long lhsq = atoll((this->UPC).c_str());
			long long rhsq = atoll((rhs.UPC).c_str());

			if(lhsq > rhsq) // This is intentionally backwards, we want it in reverse order
			{
				return true;
			}
			return false;
		}	
	
	};

	class model
	{
		friend class warehouse;   // This allows functions in warehouse to access
					   //   private data within this class.
		private:
			/* String to keep track of all the orders that are underfilled.
			 * This variable will be added to in the form of "date UPC itemname" 
			 * The end contains a std::endl character */
			std::vector<underfilled_order> underfilled_orders; 

			/* This will on a daily basis hod the underfilled orders for that 
			 * day before they are added to the overall underfilled orders. */
			std::vector<underfilled_order> temp_underfilled;

			std::vector<std::string> warehouse_names;


			// Map of warehouses for the system, 
			std::map<std::string, warehouse> warehouses; 

			// All UPC's
			std::vector<std::string> UPC_list;

			/* Maps for storage of item names, item shelf life, and item popularity */
			std::map<std::string, std::string> food_items; // Maps UPC to item Name


			std::map<std::string, int> item_life; // Maps UPC to Item Life (in days)

			std::map<std::string, long long> item_popularity; // Maps

			// Date variable: holds the current date of the model
			boost::gregorian::date current_date;

			

			/* Queue used to keep track of all the requests done in the current day.
			 * Utilizes the batch struct, however the "Experiation Date" field is irrelevant for this stack
			 */
			std::queue<requested_order> request_queue;  

		public:

			/* Constructor that creates the model for the warehouse system and initializes the fields */
			model();

			/* A function that adds the requested item to the receive queue */
			void add_request(std::string UPC, std::string quantity, std::string location);

			/* A function that is for each item in the receive queue, creates a batch and updates the warehouses
			 * inventory based off their requests from the current day
			 */
			void receive_item(std::string UPC, std::string quantity, std::string warehouse_name);

			/* Processes all the requests from the current day, calls the warehouses to 
			 * update their inventory (remove expired items) and then advances to the next day
			 */
			void conclude_day();

			/* Processes the requests of the current day. Updates the popularity map, fills the requests
			 * and if not enough updates the underfilled string 
			 */
			 void process_requests(); 

			/* Print out the Statistics */
			void print_statistics();
			

			/* Calculates the well stocked warehouses by checking to see if at least two warehouses contain
			 * an item. If this is the case, this function will print out that item in the form of 
			 * "UPC, itemname"
			 */
			void calc_well_stocked();

			/* Adds a warehouse to the set of warehouses */
			void add_warehouse(const std::string & name);

			void add_food_type(const std::string & UPC, const std::string & life, const std::string & name);

			/* Sets the start date */
			void set_start_date(const std::string & date);

			/* Function taken from stack overflow to help convert to a usable 
			 *string. https://stackoverflow.com/questions/7162457/how-to-convert-boostgregoriandate-to-mm-dd-yyyy-format-and-vice-versa */
			std::string dateAsMMDDYYYY( const boost::gregorian::date & date );
			
			
	};

}


#endif
