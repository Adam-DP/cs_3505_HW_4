#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <map>
#include <queue>         

/*Warehouse header file declaring the warehouse class. By Parker Stewart and Adam Della-Piana */

namespace cs3505
{

	 struct batch
	{
		std::string item_name;
		long long quantity;
		boost::gregorian::date expiration_date;
		std::string UPC;
	
	};
	 

	class warehouse
	{
	private:
		std::string name; // name of the warehouse

		std::map<std::string, std::queue<batch> * > inventory; // A map of UCP to Lists of Inventory


	public:
		// Constructor
		warehouse(const std::string & s);
		
		// Adds the batch to the inventory map
		void add_to_inventory(batch b);
		
		/* Function that fulfills the requests for the given UPC. 
		 * The parameters are the UPC and the quantity for that UPC
		 * and this function removes the corresponding batches until the 
	     * quantity is met, or the inventory runs out */
		bool fulfill_requests(std::string UPC, long long req_q);
		
		/* Initialize's the UPC'queue for the inventorie of each food item in
		 * the warehouse */
		void initialize_UPC(std::string UPC);

		/* This function removes the expired items from the warehouse's 
		 * inventory. The UPC_list is passed by reference, to prevent copying
		 * the whole data structure. */ 
		void update_inventory(boost::gregorian::date current_date,  std::vector<std::string> & UPC_list);

		// Returns true if there is a batch in the inventory for this UPC
		bool is_stocked(std::string UPC);
		

	};
}



#endif
