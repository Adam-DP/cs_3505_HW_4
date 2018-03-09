
#include "warehouse.h"
namespace cs3505
{
	warehouse::warehouse(const std::string & n)
	{
		name = n;
	
	}

	void warehouse::remove_expired(){}

	void warehouse::add_to_inventory(batch b)
	{
		// Get the UPC from the batch
		std::string id = b.UPC;
		// Add the batch to the inventory queue at the UPC 
		inventory.at(id)->push(b);
		// std::cout<<  "Warehouse" << name << "Added to Inventory at Address: " << inventory.at(id) << std::endl;
		//std::cout << "top batch: " << inventory.at(id).front().item_name << std::endl;
	}

	bool warehouse::fulfill_requests(std::string UPC, long long req_q)
	{
		bool fulfilled = false;
	// std::cout<< "Warehouse" << name << "Pulled from Inventory at Address: " << inventory.at(UPC) << std::endl;

		while(inventory.at(UPC)->size() > 0 && req_q > 0)
		{
			//std::cout<< "ENTERED LOOP" << std::endl;
			batch & b = inventory.at(UPC)->front(); // gets the first batch
			// TODO THIS WAS CHANGED
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

	std::string warehouse::getName(){return name;}

	void warehouse::initialize_UPC(std::string UPC)
	{
		std::queue<batch> *vec = new std::queue<batch>();
		inventory.insert(std::pair<std::string, std::queue<batch> *>(UPC, vec));

		//std::cout << "Inventory size: " << inventory.size() << " warehouse name: " << name << std::endl;
	}


	void warehouse::update_inventory(boost::gregorian::date current_date, std::vector<std::string> & UPC_list) //TODO  changed
	{
		
		for(int idx = 0; idx < UPC_list.size(); idx++) // TODO changed
		{
			//std::cout << "Updating inventory for: " << name << std::endl;
			if(inventory.at(UPC_list.at(idx))->size() != 0) // Prevents accessing the front element of an empty queue
			{
				batch b = inventory.at(UPC_list.at(idx))->front(); // TODO changed
				std::cout << "This batch is: " << b.item_name << "in: " << name << " with left: " << b.quantity << std::endl << std::endl;
				while((b.expiration_date - current_date) == boost::gregorian::days(0)) //TODO  changed
				{
					inventory.at(UPC_list.at(idx))->pop();
					std::cout << "Batch expired: " << b.item_name << " " << name << std::endl;

					// If there is something left in the inventory queue, then grab it,
					// otherwise, there is nothing left to remove, so we break.
					if(inventory.at(UPC_list.at(idx))->size() != 0)
						b = inventory.at(UPC_list[idx])->front(); //TODO  changed
					else break;
				
				}
			}

		}
	}


}
