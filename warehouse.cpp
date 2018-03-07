
#include "warehouse.h"
namespace cs3505
{
	warehouse::warehouse(const std::string & n)
	{
		name = n;
	
	}

	void warehouse::remove_expired(){}

	void warehouse::add_to_inventory(batch b){}

	void warehouse::fulfill_requests(){}

	std::string warehouse::getName(){return name;}
}
