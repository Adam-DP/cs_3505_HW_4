
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
			std::cout<< "ENTERED LOOP" << std::endl;
			batch b = inventory.at(UPC)->front(); // gets the first batch
			
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

/*
	void warehouse::update_inventory(boost::gregorian::date current_date, std::vector<std::string> * UPC_list)
	{
		for(int idx = 0; idx < UPC_list->size(); idx++)
		{
			batch b = inventory.at(UPC_list[idx])->front();
			
			while((b.expirationi_date - current_date) == 0)
			{
				inventory.at(UPC_list[idx])->pop();
				std::cout << "Batch expired: " << b.item_name << std::endl;
				b = inventory.at(*UPC_list[idx])->front();
				
			}

		}
	}
*/

}
