/* Warehouse file containing class functions for a warehouse. By Adam Della-Piana and Parker Stewart */

#include "warehouse.h"
namespace cs3505
{
	warehouse::warehouse(const std::string & n)
	{
		name = n;
	
	}

	// Adds the batch to the inventory map
	void warehouse::add_to_inventory(batch b)
	{
		// Get the UPC from the batch
		std::string id = b.UPC;
		// Add the batch to the inventory queue at the UPC 
		inventory.at(id)->push(b);
	}

	/* Function that fulfills the requests for the given UPC. 
	 * The parameters are the UPC and the quantity for that UPC
	 * and this function removes the corresponding batches until the 
	 * quantity is met, or the inventory runs out */
	bool warehouse::fulfill_requests(std::string UPC, long long req_q)
	{
		bool fulfilled = false;

		while(inventory.at(UPC)->size() > 0 && req_q > 0)
		{
			
			batch & b = inventory.at(UPC)->front(); // gets the first batch
			
			if(b.quantity > req_q)
			{
				b.quantity = b.quantity - req_q;
				fulfilled = true;
				break;
			}
			else
			{
				req_q = req_q - b.quantity;
				inventory.at(UPC)->pop();
				if(req_q == 0)
					fulfilled = true;
			}
			
		}
		return fulfilled;
	}

	/* Initialize's the UPC'queue for the inventorie of each food item in
	 * the warehouse */
	void warehouse::initialize_UPC(std::string UPC)
	{
		std::queue<batch> *vec = new std::queue<batch>();
		inventory.insert(std::pair<std::string, std::queue<batch> *>(UPC, vec));
	}


	/* This function removes the expired items from the warehouse's 
	 * inventory. The UPC_list is passed by reference, to prevent copying
	 * the whole data structure. */ 
	void warehouse::update_inventory(boost::gregorian::date current_date, std::vector<std::string> & UPC_list)
	{
		// for each upc (food item) 
		for(int idx = 0; idx < UPC_list.size(); idx++) 
		{
			if(inventory.at(UPC_list.at(idx))->size() != 0) // Prevents accessing the front element of an empty queue
			{
				batch b = inventory.at(UPC_list.at(idx))->front(); 

				// Remove the batch if it is expired
				while((b.expiration_date - current_date) == boost::gregorian::days(0)) 
				{
					inventory.at(UPC_list.at(idx))->pop();
			
					// If there is something left in the inventory queue, then grab it,
					// otherwise, there is nothing left to remove, so we break.
					if(inventory.at(UPC_list.at(idx))->size() != 0)
						b = inventory.at(UPC_list[idx])->front(); 
					else break;
				
				}
			}

		}
	}

	// Returns true if there is a batch in the inventory for this UPC
	bool warehouse::is_stocked(std::string UPC)
	{
		return inventory.at(UPC)->size() > 0;
	}

}
