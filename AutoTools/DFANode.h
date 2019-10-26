#pragma once
#include <vector>
#include <string>
#include "Production.h"

struct behave
{
	int behavior = -1;//0 �ƽ��ս��  1 �ƽ����ս��   2 ��Լ
	std::string value;//�ƽ��ķ��� ���� ��Լ����ʽ���󲿷���
	int aim = -1;//�ƽ�֮�󵽴��״̬�͹�Լ�Ĳ���ʽ���
};
class DFANode
{
private:
	static int totalNum;
	DFANode();
	DFANode(vector<Production>* productions);
	int index;
	static DFANode* lastNode;
	vector<Production>* productions;
	vector<behave>* behaviors;
	virtual~DFANode();

public:
	static DFANode* createNewNode();
	static DFANode* createNewNode(vector<Production>* productions);
	int getTotalNum();
	bool theSameAs(DFANode* node);
	vector<Production>* getProductions();
	static void distory(DFANode* node);
	int getIndex();
	void push_behavior(behave b);
	vector<behave>* getBehaviors();
	void show();
};

