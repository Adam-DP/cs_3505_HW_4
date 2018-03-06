#include <string>
#include "warehouse.H"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <map>
#include <queue>  
#include "model.h"        


/* Class functions */

/* Constructs the model */
model::model()
{
	// TODO does this need initializers? 
}

/* A function that adds the requested item to the receive queue */
void model::request_item(std::string UPC, int quantity, warehouse location)
{
	batch requested_item = {food_items.at(UPC), quantity, current_date, UPC, location}; 
	request_queue.push(requested_item);
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
	model::process_requests();

	for (warehouse w : warehouses)
	{
		w.update_inventory();
	}

	// create a day with 1 day 
	boost::gregorian::days day(10);

	current_date += day;

}

/* Processes the requests of the current day. Updates the popularity map, fills the requests
 * and if not enough updates the underfilled string 
 */
 void model::process_requests()
 {
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
 }

/* Returns the date of the current day*/
boost::gregorian::date model::get_date();

/* Returns the underfilled string */
std::string model::get_underfilled();

/* Calculates the well stocked warehouses by checking to see if at least two warehouses contain
 * an item. If this is the case, this function will print out that item in the form of 
 * "UPC, itemname"
 */
void model::calc_well_stocked();

/* Adds a food item to the food_items map */
void model::add_item(std::string UPC, std::string item_name);


