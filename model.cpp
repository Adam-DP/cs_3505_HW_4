#include <string>
#include "warehouse.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <map>
#include <queue>  
#include "model.h"   

     


namespace cs3505
{
	/* Class functions */

	/* Constructs the model */
	model::model()
	{
		// TODO does this need initializers? 
	}

	/* A function that adds the requested item to the receive queue */
	void model::add_request(std::string UPC, std::string quantity, std::string warehouse_name)
	{

		long long quant = std::atoll(quantity.c_str());
		warehouse location = warehouses.at(warehouse_name);
	
		requested_order requested_item = {food_items.at(UPC), quant, UPC, location}; 
		request_queue.push(requested_item);

		// Increment Popularity
		item_popularity.at(UPC) = item_popularity.at(UPC) + quant;
		
	
	}

	/* A function that is for each item in the receive queue, creates a batch and updates the warehouses
	* inventory based off their requests from the current day
	*/
	void model::receive_item(std::string UPC, std::string quantity, std::string warehouse_name)
	{
		warehouse wh = warehouses.at(warehouse_name);
		long long quant = atoll(quantity.c_str());
		// Compute expiration date
		boost::gregorian::date d;
		d = current_date + boost::gregorian::days(item_life.at(UPC));
		
		// Create a batch struct
		batch b = {food_items.at(UPC), quant, d, UPC};

		wh.add_to_inventory(b);
		
	}

	/* Processes all the requests from the current day, calls the warehouses to 
	 * update their inventory (remove expired items) and then advances to the next day
	 */
	void model::conclude_day()
	{
	// END OF DAY 

		
	//std::cout << "size of queue: " << request_queue.size() << std::endl;
		
	
		model::process_requests();

		// Increment the current day
		current_date = current_date + boost::gregorian::days(1);
		std::cout << "current date: " << current_date << std::endl;
		
		// Tell each warehouse to remove expired items
		for(int idx = 0; idx < warehouse_names.size(); idx++)
		{
			warehouses.at(warehouse_names[idx]).update_inventory(current_date, UPC_list); // changed
		}
	}

	/* Processes the requests of the current day. Updates the popularity map, fills the requests
	 * and if not enough updates the underfilled string 
	 */
	 void model::process_requests()
	 {

			
			std::cout << "END OF DAY" << std::endl;
		//std::cout << "size of queue: " << request_queue.size() << std::endl;
		int original_size = request_queue.size();
		for(int idx = 0; idx < original_size; idx++)
		{
			requested_order ro = request_queue.front();
			request_queue.pop();

			//std::cout << "UPC #" << idx << ": " << ro.UPC << std::endl;

			/* Create a bool for if the request was filled, access the warehouse from 
			 * the requested_order struct, fulfill the request using the UPC and 	
			 * quantity from the struct */
			bool did_fulfil = ro.location.fulfill_requests(ro.UPC, ro.quantity);

			if(did_fulfil)
				std::cout << "Request for " << ro.UPC << " fulfilled" << std::endl;
			else
				std::cout << "Request for " << ro.UPC << " not fulfilled" << std::endl;

		}

		

	 }

	/* Returns the date of the current day*/
	boost::gregorian::date model::get_date()
	{
		return current_date;
	}

	/* Returns the underfilled string */
	std::string model::get_underfilled()
	{
		return "";
	}

	/* Calculates the well stocked warehouses by checking to see if at least two warehouses contain
	 * an item. If this is the case, this function will print out that item in the form of 
	 * "UPC, itemname"
	 */
	void model::calc_well_stocked()
	{

	}

	/* Adds a food item to the food_items map */
	void model::add_item(std::string UPC, std::string item_name)
	{

	}

	/* Adds a warehouse to the set of warehouses */
	void model::add_warehouse(const std::string & name)
	{

		warehouse location(name);
		// Initialize the inventory of the warehouse & popularity map
		for(int idx = 0; idx < UPC_list.size(); idx++)
		{
			location.initialize_UPC(UPC_list[idx]);
			item_popularity.insert(std::pair<std::string, long long>(UPC_list[idx], 0));
		}
		//Initialize the popularity stack
		

		warehouses.insert(std::pair<std::string, warehouse>(name, location) );
		warehouse_names.push_back(name); // Add name of warehouse to list of all warehouse names
		
		//std::cout << "inserted " << warehouses.at(name).getName() << std::endl;
 
	}

	void model::add_food_type(const std::string & UPC, const std::string & life, const std::string & name)
	{
		//std::cout << "New Food Item " << UPC << " " << life << " " << name << " " << std::endl; 
		food_items.insert(std::pair<std::string, std::string>(UPC, name) );
		//std::cout << "inserted UPC for " << food_items.at(UPC) << std::endl;
		
		// Convert Lifespan to an Integer - store it in the map
		item_life.insert(std::pair<std::string, int>(UPC, atoi(life.c_str())) );
		UPC_list.push_back(UPC); // Add the UPC to the list of UPC's
		
	}

	void model::set_start_date(const std::string & d1)
	{
		std::string month = d1.substr(0,2);
		std::string day = d1.substr(3,2);
		std::string year = d1.substr(6,4);
		
		// We used the link on the discussion for reference for this part
		current_date = boost::gregorian::date_from_iso_string(year+month+day);
		//std::cout << current_date << std::endl;  // Just for debugging
				
	}

	void model::print_statistics()
	{
		std::cout << "PRINT STATISTICS" << std::endl;
		for(int idx = 0; idx < UPC_list.size(); idx++)
		{
			std::string name = food_items.at(UPC_list[idx]);
			std::cout << name << "'s popularity is: " << item_popularity.at(UPC_list[idx]) << std::endl;
		}

	}

	
}


