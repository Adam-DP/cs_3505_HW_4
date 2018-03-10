#include <string>
#include "warehouse.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <map>
#include <queue>  
#include "model.h"  
#include <algorithm> 

     


namespace cs3505
{

	/* Helper function to override the comparator for sorting UPC 
		bool myfunction (std::string lhs,std::string rhs); */
	bool myfunction (std::string lhs,std::string rhs) 
	{ 
		long long lhsq = atoll((lhs).c_str());
		long long rhsq = atoll((rhs).c_str());

		if(lhsq < rhsq) // This is intentionally backwards, we want it in reverse order
		{
			return true;
		}
		return false;
	}

/* Class functions */

	/* Constructs the model */
	model::model()
	{

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
	
		model::process_requests();

		// Increment the current day
		current_date = current_date + boost::gregorian::days(1);
		
		// Tell each warehouse to remove expired items
		for(int idx = 0; idx < warehouse_names.size(); idx++)
		{
			warehouses.at(warehouse_names[idx]).update_inventory(current_date, UPC_list); 
		}
	}

	/* Processes the requests of the current day. Updates the popularity map, fills the requests
	 * and if not enough updates the underfilled string 
	 */
	 void model::process_requests()
	 {
		int original_size = request_queue.size();
		for(int idx = 0; idx < original_size; idx++)
		{
			requested_order ro = request_queue.front();
			request_queue.pop();

			/* Create a bool for if the request was filled, access the warehouse from 
			 * the requested_order struct, fulfill the request using the UPC and 	
			 * quantity from the struct */
			bool did_fulfil = ro.location.fulfill_requests(ro.UPC, ro.quantity);

			if(!did_fulfil)
			{
				underfilled_order under = {current_date, ro.UPC, ro.item_name};
				temp_underfilled.push_back(under);
			}

		}

		// UPDATE ALL THE UNDERFILLED REQUESTS
		// Sort todays underfilled orders by UPC
		std::sort (temp_underfilled.begin(), temp_underfilled.end());
		int temp_size = temp_underfilled.size();
		// Add the today's underfilled orders to the overall underfilled orders list

		std::string previous = "";
		for(int i = temp_underfilled.size()-1; i>=0; i--)
		{
			underfilled_order u = temp_underfilled[i];
			if(previous.compare(u.UPC) != 0)
				underfilled_orders.push_back(temp_underfilled[i]);
			
			previous = u.UPC;
			temp_underfilled.pop_back();

		}

	 }


	/* Calculates the well stocked warehouses by checking to see if at least two warehouses contain
	 * an item. If this is the case, this function will print out that item in the form of 
	 * "UPC, itemname"
	 */
	void model::calc_well_stocked()
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
		
 
	}

	void model::add_food_type(const std::string & UPC, const std::string & life, const std::string & name)
	{
		food_items.insert(std::pair<std::string, std::string>(UPC, name) );
		
		// Convert Lifespan to an Integer - store it in the map
		item_life.insert(std::pair<std::string, int>(UPC, atoi(life.c_str())) );
		UPC_list.push_back(UPC); // Add the UPC to the list of UPC's
		
	}

	/* Sets the start date */
	void model::set_start_date(const std::string & d1)
	{
		std::string month = d1.substr(0,2);
		std::string day = d1.substr(3,2);
		std::string year = d1.substr(6,4);
		
		// We used the link on the discussion for reference for this part
		current_date = boost::gregorian::date_from_iso_string(year+month+day);
				
	}

	void model::print_statistics()
	{
		std::cout << "Report by Adam Della-Piana and Parker Stewart" << std::endl;
		std::cout << std::endl;

		std::cout << "Underfilled orders:" << std::endl; 
		for(int idx = 0; idx < underfilled_orders.size(); idx++)
		{
			underfilled_order & order = underfilled_orders[idx];
			boost::gregorian::date d = order.expiration_date;

			
			// Format the date correctly 
			std::string date_string = model::dateAsMMDDYYYY(d);
		
			std::cout << date_string << " " << order.UPC << " " << order.item_name << "\n";
			
		}
		std::cout << std::endl;

		std::cout << "Well-Stocked Products:" << std::endl;
		// calculate well stocked products
		std::sort (UPC_list.begin(), UPC_list.end(), myfunction);
		
		for(int outer_idx = 0; outer_idx < UPC_list.size(); outer_idx++)
		{
			int count = 0;
			for(int inner_idx = 0; inner_idx < warehouse_names.size(); inner_idx++)
			{
				if(warehouses.at(warehouse_names[inner_idx]).is_stocked(UPC_list[outer_idx]))
				{
					count++;
					if(count == 2)
					{
						std::cout << UPC_list[outer_idx] << " " << food_items[UPC_list[outer_idx]] << std::endl;
						break;
					}
				}

			}
		}
		std::cout << std::endl;

		std::cout << "Most Popular Products:" << std::endl;
		

		std::vector<upc_popularity> pop_vec;
		
		// Populate list of UPC Pop structures
		for(int idx = 0; idx < UPC_list.size(); idx++)
		{
			std::string name = food_items.at(UPC_list[idx]);
			long long popularity = item_popularity.at(UPC_list[idx]);
			upc_popularity next_val = {UPC_list[idx], name, popularity};
			pop_vec.push_back(next_val);

		}
		// Sort list by Value
		std::sort (pop_vec.begin(), pop_vec.end());
		// Pull Top 3
		int count = 0;
		while(count < 3 && count < pop_vec.size())
		{
			upc_popularity current = pop_vec[pop_vec.size()-(1+count++)];
			std::cout << current.popularity << " " << current.UPC << " " << current.item_name << std::endl;
		}


	}


	// We found this off of stack over flow. This creates a string based off the date 
	// https://stackoverflow.com/questions/7162457/how-to-convert-boostgregoriandate-to-mm-dd-yyyy-format-and-vice-versa
	const std::locale fmt(std::locale::classic(),
		                  new boost::gregorian::date_facet("%m/%d/%Y"));
	std::string model::dateAsMMDDYYYY( const boost::gregorian::date & date )
	{
		std::ostringstream os;
		os.imbue(fmt);
		os << date;
		return os.str();
	}

	
}


