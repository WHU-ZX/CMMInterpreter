#include "FirstFollowTools.h"
#include "Reader.h"
#include "Production.h"
#include <algorithm>
#include <iterator>

vector<string>* getVns(vector<Production*>* pointer);

struct DependNode//a->b  ��ʾ a U= b
{
	string name;//���ս����
};
void FirstFollowTools::buildFirst_Follow()
{
	firstSetP = new map<string, set<string>>();
	followSetP = new map<string, set<string>>();
	buildFirst();
	//cout << "First��:" << endl << endl;
	//printFirst();
	//cout << endl << endl << endl;
	buildFollow();
	//cout << "Follow��:" << endl << endl;
	//printFollow();
	return;
}
void FirstFollowTools::printFirst()
{
	vector<string>* Vns = getVns(prosPointerVectorP);
	for (int i = 0; i < Vns->size(); i++)
	{
		string Vn = (*Vns)[i];
		cout << Vn << "\t\t\t\t\t\t";
		set<string> firstSet = (*firstSetP)[Vn];
		for (string s : firstSet)
		{
			cout << s << " ";
		}
		cout <<endl;
	}
}

void FirstFollowTools::printFollow()
{
	vector<string>* Vns = getVns(prosPointerVectorP);
	for (int i = 0; i < Vns->size(); i++)
	{
		string Vn = (*Vns)[i];
		cout << Vn << "\t\t\t\t\t\t";
		set<string> followSet = (*followSetP)[Vn];
		for (string s : followSet)
		{
			cout << s << " ";
		}
		cout << endl;
	}
}

bool in(string s, vector<string>* p)
{
	for (int i = 0; i < p->size(); i++)
	{
		if (p->at(i) == s)
		{
			return true;
		}
	}
	return false;
}
vector<string>* getVns(vector<Production*>* pointer)
{
	vector<string>* ret = new vector<string>();
	for (int i = 0; i < pointer->size(); i++)
	{
		ProductionSym* left = pointer->at(i)->getLeft();
		string s = left->getVal();
		if (!in(s, ret))
		{
			ret->push_back(s);
		}
	}
	return ret;
}

void FirstFollowTools::buildFirst()//��First��   �ձ����ʾΪ<empty>
{
	vector<string>* aim_str_vec = getVns(this->prosPointerVectorP);
	for (int i = 0; i < aim_str_vec->size(); i++)
	{
		buildFirstOf(aim_str_vec->at(i));
	}
}

void FirstFollowTools::buildFollow()//��Follow��
{
	vector<string>* aim_str_vec = getVns(this->prosPointerVectorP);
	set<string> startSet;
	startSet.insert("$");
	(*followSetP)["<start>"] = startSet;
	for (int i = 1; i < aim_str_vec->size(); i++) // i��1��ʼ
	{
		buildFollowOf(aim_str_vec->at(i));
	}
}

void FirstFollowTools::buildFirstOf(string aim)//��First��
{
	//map<string, bool> *toEmpty;//�ս���ܷ��Ƴ��գ�
	//cout << "getting first of " << aim << endl;
	for (int i = 0; i < prosPointerVectorP->size(); i++)
	{
		Production* production = prosPointerVectorP->at(i);
		if (production->getLeft()->getVal() == aim)//
		{
			ProductionSym* left = production->getLeft();
			vector<ProductionSym*>* right = production->getRight();
			for (int j = 0; j < right->size(); j++)
			{
				ProductionSym* temp = right->at(j);
				if (!temp->isVn())//�����ǰ����Ϊ�ս��
				{
					set<string> curAimSet = (*firstSetP)[aim];
					curAimSet.insert(temp->getVal());
					(*firstSetP)[aim] = curAimSet;
					//pair<string, set<string>> value(aim, Set);
					//firstSetP->insert(value);
					break;
				}
				else if (temp->getVal() == empty_str)//Ϊ��
				{
					if (j == right->size() - 1)
					{
						set<string> curAimSet = (*firstSetP)[aim];
						curAimSet.insert(empty_str);
						(*firstSetP)[aim] = curAimSet;
					}
				}
				else// Ϊ���ս��
				{
					if (temp->getVal() != aim)//����ֱ����ݹ���ִ��
					{//�󲢼�
						buildFirstOf(temp->getVal());
						set<string> curAimSet = (*firstSetP)[aim];
						set<string> tempSet = (*firstSetP)[temp->getVal()];
						if (tempSet.find(empty_str) != tempSet.end())// ���ڴ�set��
						{
							tempSet.erase(empty_str);
							set<string> theUnion;
							//vector<string> vec;
							//set_union(curAimSet.begin(), curAimSet.end(), tempSet.begin(), tempSet.end(), back_inserter(vec));
							set_union(curAimSet.begin(), curAimSet.end(), tempSet.begin(), tempSet.end(), inserter(theUnion,theUnion.begin()));
							//copy(vec.begin(), vec.end(), theUnion.begin());
							(*firstSetP)[aim] = theUnion;
						}
						else//set�������գ�ֱ������ѭ��
						{
							set<string> theUnion;
							set_union(curAimSet.begin(), curAimSet.end(), tempSet.begin(), tempSet.end(), inserter(theUnion, theUnion.begin()));
							(*firstSetP)[aim] = theUnion;
							break;
						}
					}
					else//��ֱ����ݹ�������
					{
						break;
					}
				}
			}
		}
	}
}

void FirstFollowTools::buildFollowOf(string aim)//��Follow��
{
	int curIndex = 0;
	for (int i = 0; i < prosPointerVectorP->size(); )
	{
		bool find = false;
		Production* production = prosPointerVectorP->at(i);
		ProductionSym* left = production->getLeft();
		vector<ProductionSym*>* right = production->getRight();
		int j = curIndex;
		for (; j < right->size(); j++)
		{
			if (aim == right->at(j)->getVal())// A-> bBcB   ?
			{
				find = true;
				break;
			}
		}
		if (find)
		{
			curIndex = j;
			if (j == right->size() - 1 && aim != left->getVal())//aim �ǲ���ʽ�����ұ�һ������aim�����ʽ�󲿲�ͬ(�����ͬ��ʲô��������)
			{
				if (followSetP->count(left->getVal()) != 1)//map �л�û�� left��follow��
				{
					buildFollowOf(left->getVal());
				}
				set<string> curAimSet = (*followSetP)[aim];
				set<string> leftSet = (*followSetP)[left->getVal()];
				set<string> theUnion;
				set_union(curAimSet.begin(), curAimSet.end(), leftSet.begin(), leftSet.end(), inserter(theUnion, theUnion.begin()));
				(*followSetP)[aim] = theUnion;
				i++;
				curIndex = 0;
			}
			else if (j == right->size() - 1 && aim == left->getVal())//aim �ǲ���ʽ�����ұ�һ������aim�����ʽ����ͬ
			{
				i++;
				curIndex = 0;
			}
			else // aim �ұ߻�������
			{
				for (; j < right->size()-1; j++)
				{
					ProductionSym* temp = right->at(j+1);
					if (!temp->isVn())//������ս��,�����follow��
					{
						set<string> curAimSet = (*followSetP)[aim];
						curAimSet.insert(temp->getVal());
						(*followSetP)[aim] = curAimSet;
						i++;
						curIndex = 0;
						break;
					}
					else//�Ƿ��ս��
					{
						set<string> firstSet = (*firstSetP)[temp->getVal()];
						if (firstSet.find(empty_str) != firstSet.end())//�˷��ս����first������ ��
						{
							//����temp��(first-empty)��follow
							//���ķ�ֻ��<formal_para_list> <real_para_list> <stmts> ���ǵ�follow���Ǽ򵥵��ս�� )  )  }
							cout << "�������鷳�ĵط�,����������ѭ��" << endl;
							//���Ҽ򵥴�����Ϊ���ķ���ʱ�������˷�֧
							if (followSetP->count(temp->getVal()) != 1)//map �л�û�� aim��follow��
							{
								buildFollowOf(temp->getVal());
							}
							set<string> curAimSet = (*followSetP)[aim];
							set<string> tempFollowSet = (*followSetP)[temp->getVal()];
							set<string> tempFirstSet = (*firstSetP)[temp->getVal()];
							tempFirstSet.erase(empty_str);// first(temp) - empty
							set<string> theUnion1,theUnion;
							set_union(curAimSet.begin(), curAimSet.end(), tempFollowSet.begin(), tempFollowSet.end(), inserter(theUnion1, theUnion1.begin()));
							set_union(theUnion1.begin(), theUnion1.end(), tempFirstSet.begin(), tempFirstSet.end(), inserter(theUnion, theUnion.begin()));
							(*followSetP)[aim] = theUnion;
							if (j + 1 >= right->size() - 1)
							{
								curIndex = j + 1;
							}
						}
						else//�˷��ս����first�������� ��, ��ֱ�ӽ�first����aim��follow����
						{
							set<string> followSet = (*followSetP)[aim];
							set<string> theUnion;
							set_union(firstSet.begin(), firstSet.end(), followSet.begin(), followSet.end(), inserter(theUnion, theUnion.begin()));
							(*followSetP)[aim] = theUnion;
							//i++��j�ı������
							//������治����aim  �� i++ ������curIndex = 0
							//������滹��aim ��i���䣬��curIndex = ��һ��aim���ڵ��±�
							bool hasAnother = false;
							for (j = curIndex + 1; j < right->size(); j++)
							{
								if (aim == right->at(j)->getVal())
								{
									hasAnother = true;
									break;
								}
							}
							if (hasAnother) {
								curIndex = j;
							}
							else
							{
								i++;
								curIndex = 0;
							}
							break;
						}
					}
				}
				/*if (j == right->size() - 1)
				{
					ProductionSym* temp = right->at(j);
					if (temp->isVn())
					{
						set<string> firstSet = (*firstSetP)[temp->getVal()];
					}
				}*/
			}
		}
		else
		{
			i++;
			curIndex = 0;
		}
	}
}

bool FirstFollowTools::saveFirst_Follow(const char* fileName)
{
	
	return true;
}

bool FirstFollowTools::saveFirst_Follow(FF* ffPointer)
{
	return true;
}

bool toEmpty(vector<Production*>* productionVector,string s)//s �Ƿ����Ƴ��գ�
{
	return true;
}

FirstFollowTools::FirstFollowTools(string empty_str, const char* fileName)
{
	this->firstSetP = nullptr;
	this->followSetP = nullptr;
	this->empty_str = empty_str;
	this->prosPointerVectorP = Reader::readProductions(fileName);
	buildFirst_Follow();
}

FirstFollowTools::FirstFollowTools(string empty_str,vector<Production*>* prosPointerVectorP)
{
	this->firstSetP = nullptr;
	this->followSetP = nullptr;
	this->empty_str = empty_str;
	this->prosPointerVectorP = prosPointerVectorP;
	buildFirst_Follow();
}

map<string, set<string>>* FirstFollowTools::getFollow()
{
	return this->followSetP;
}