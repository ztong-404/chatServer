#ifndef GROUPMODEL_H
#define GROUPMODEL_H

#include"group.hpp"
#include<string>
#include<vector>
using namespace std;

//ά��Ⱥ����Ϣ�Ĳ����ӿڷ���
class GroupModel
{
public:
  //����Ⱥ��
  bool createGroup(Group &group);
  //����Ⱥ��
  void addGroup(int userid,int groupid,string role);
  //��ѯ�û�����Ⱥ����Ϣ
  vector<Group>queryGroups(int userid);
  //����ָ����groupid��ѯȺ���û�id�б���userid�Լ�����Ҫ�û�Ⱥ��ҵ���Ⱥ��������ԱȺ����Ϣ
  vector<int>queryGroupUsers(int userid, int groupid);  
};

#endif
