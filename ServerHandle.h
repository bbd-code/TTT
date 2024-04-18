#pragma once


#include<initializer_list>
#include<string>




struct ServerHandle final
{
	
	static void filter(std::string& request, std::initializer_list<std::string> filter_strings);



};

