#pragma once
#include "header.h"
#include <string>
#include <vector>
#include <iostream>
//#include "..//AutoTools/Exception.cpp"
using namespace std;
enum ExType { LexerEx, ParserEx, StaticSemaEx, DynamicSemaEx, OtherEx };

struct SourceCode
{
	int length = 0;
	char* buffer = nullptr;
};
struct Exception 
{
	int row;
	const char* detail;
	ExType type;
	Exception(ExType type, int row, const char* detail)
	{
		this->type = type;
		this->row = row;
		this->detail = detail;
	}
	void print()
	{
		std::cout << "��" << this->row << "�г���";
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
		exit(0);
	}
};
struct Symbol 
{
	int row = 0;
	string value = "";
	Token token;
};

class MyLexer
{
private:
	SourceCode* mySourceCode;
	int curSymIndex = 0;
	vector<Symbol*>* symVecPointer = nullptr;
	static string keyWords[];
	static Token keyTokens[];
	static int keyNum;

	SourceCode* readSourceCode(const char* fileName);
	void buildSymbols();
	static Token inKey(string str);


public:
	MyLexer(const char* fileName);
	virtual ~MyLexer();



	void printResult();
	vector<Symbol*>* getSymbols();
};

