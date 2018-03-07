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
	void model::request_item(std::string UPC, int quantity, warehouse location)
	{
	/*
		batch requested_item = {food_items.at(UPC), quantity, current_date, UPC, location}; 
		request_queue.push(requested_item);
	*/
	}

	/* A function that is for each item in the receive queue, creates a batch and updates the warehouses
	* inventory based off their requests from the current day
	*/
	void model::receive_item(batch item_batch, warehouse location)
	{
		location.add_to_inventory(item_batch);
	}

	/* Processes all the requests from the current day, calls the warehouses to 
	 * update their inventory (remove expired items) and then advances to the next day
	 */
	void model::conclude_day()
	{
	/*
		model::process_requests();

		for (warehouse w : warehouses)
		{
			w.update_inventory();
		}

		// create a day with 1 day 
		boost::gregorian::days day(10);

		current_date += day;
	*/
	}

	/* Processes the requests of the current day. Updates the popularity map, fills the requests
	 * and if not enough updates the underfilled string 
	 */
	 void model::process_requests()
	 {
	/*
	 	while(request_queue.size != 0)
		{
			batch requested_item = request_queue.pop();

			// Compute expiration date for the food item
			boost::gregorian::date expiration_date = current_date;
			expiration_date = expiration_date + item_lives.at(requested_item.UPC);

			// Create batch to be added to warehouse inventory
			batch item_batch = {requested_item.iten_name, requested_item.quantity, expiration_date, 
				requested_item.UPC, requested_item.location};

			requested_item.location.fulfill_request(item_batch);

			// TODO: add to the popularity map 
		}
	*/
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
		warehouses.insert(std::pair<std::string, warehouse>(name, location) );
		warehouse_names.push_back(name);
		
		std::cout << "inserted " << warehouses.at(name).getName() << std::endl;
 
	}

	void model::add_food_type(const std::string & UPC, const std::string & life, const std::string & name)
	{
		//std::cout << "New Food Item " << UPC << " " << life << " " << name << " " << std::endl; 
		food_items.insert(std::pair<std::string, std::string>(UPC, name) );
		std::cout << "inserted UPC for " << food_items.at(UPC) << std::endl;
		
		item_life.insert(std::pair<std::string, int>(UPC, atoi(life.c_str())) );
		UPC_list.push_back(UPC);
		
	}

	void model::set_start_date(const std::string & d1)
	{
		std::string month = d1.substr(0,2);
		std::string day = d1.substr(3,2);
		std::string year = d1.substr(6,4);
		

		current_date = boost::gregorian::date_from_iso_string(year+month+day);
		std::cout << current_date << std::endl; 

		// populate the expiration dates based off of the item lifes

		for(int i = 0; i < UPC_list.size(); i++)
		{
			boost::gregorian::date d;
			d = current_date + boost::gregorian::days(item_life.at(UPC_list[i]));

			item_exp.insert(std::pair<std::string, boost::gregorian::date>(UPC_list[i],d) );
		} 
		std::cout << item_exp.at(UPC_list[0]) << std::endl;

				
	}

	
}


