#include "Exception.h"
#include <iostream>

Exception::Exception(ExType type,int row,const char* detail)
{
	this->row = row;
	this->detail = detail;
	this->type = type;
}


void Exception::printException()
{
	std::cout << "��" << row << "�г���";
	std::cout << '(';
	switch (this->type)
	{
	case LexerEx:
		std::cout << "�ʷ�����";
		break;
	case ParserEx:
		std::cout << "�﷨����";
		break;
	case StaticSemaEx:
		std::cout << "��̬�������";
		break;
	case DynamicSemaEx:
		std::cout << "��̬�������";
		break;
	case OtherEx:
		std::cout << "��������";
		break;
	}
	std::cout << ')';
	std::cout << std::endl;
	std::cout << "    " << detail << std::endl;
}
