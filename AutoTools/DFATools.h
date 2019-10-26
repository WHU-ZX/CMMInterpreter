#pragma once
#include "DFANode.h"
#include "structs.cpp"
#include <map>
#include <set>
struct DicIndex//����mapʹ�� : (state,sym) -> string
{
	int state;
	std::string sym;
	friend bool operator < (const struct DicIndex& d1, const struct DicIndex& d2);
};

class DFATools
{
private:
	vector<Production*>* productions;
	vector<DFANode*>* nodes;
	int nodeNum = 0;
	map<DicIndex,string>* SLR1_table;
	map<string, set<string>>* followSet;
	string empty_str;
	

	void dealWithEmpty();
	int getReduceIndex(Production* pro);//���ݲ���ʽ�������productions�е��±�
	void readRules(const char* ruleFileName);


public:
	DFATools(const char* productionfileName,string empty_str);
	int alreadyInDFAGroup(DFANode* node);//������Nodes�У��򷵻�-1�����򷵻���Nodes�е��±�
	void buildNodes();
	void achieve(DFANode* node);//���i����Ϊ�����ƽ���Լ�ȵ�,  ����Ҫ�����µ���Ŀ
	void derive(DFANode* node);//���������Ѿ��в���ʽ��I ����������

	void getSLR1Table(const char* ruleFileName);
	void writeSLR1Table(const char* targetFileName, const char* ruleFileName);//��SLR1��־û�
	void show();
	static map<DicIndex, string>* readSLR1Table(const char* sourceFileName);
};

