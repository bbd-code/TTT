#pragma once
#include<functional>


namespace SocksServer
{

	class ServerLoop
	{
		virtual void set_logic(/*func*/);
		void run(); /*while + ���������� bool ������� ��� ���������*/
	};

}