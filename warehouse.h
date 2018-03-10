#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <map>
#include <queue>          // std::queue
//#include <vector>


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
		std::string name;
		std::map<std::string, std::queue<batch> * > inventory; // A map of UCP to Lists of Inventory


	public:
		warehouse(const std::string & s);
		void remove_expired();
		void add_to_inventory(batch b);
		bool fulfill_requests(std::string UPC, long long req_q);
		std::string getName();
		void initialize_UPC(std::string UPC);
		void update_inventory(boost::gregorian::date current_date,  std::vector<std::string> & UPC_list);
		bool is_stocked(std::string UPC);
		

	};
}



#endif
