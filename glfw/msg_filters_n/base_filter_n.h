#pragma once
#include "utilitis_base_def.h"
#include "json.h"
namespace auto_future_utilities
{
	/**
	* @brief  message filter is designed as a chain of responsibility model, \n
	*every filter only handle the special type of message, if the message is \n
	*not what he is interested in, he will simply transfer the message to his \n
	*successor  
	*/
	class AFG_EXPORT base_filter_n
	{
	protected:
		base_filter_n* _psuccessor;
	public:
		base_filter_n();
		~base_filter_n();
		void set_succesor(base_filter_n* psr)
		{
			_psuccessor = psr;
		}
		virtual void load_tactic_from_config(const char* ){}
		virtual void parse_protocol_from_json_unit(Json::Value& junit)=0;
		virtual bool attach_handle(const char* pkey_name, msg_handle fn_obj) = 0;
		virtual bool handle_custom_data(const char* pkey_name, u8* pvalue, int len) = 0;
	};
}