#ifndef FRIENDMODEL_H
#define FRIENDMODEL_H

#include"user.hpp"
#include<vector>
using namespace std;

//ά��������Ϣ�Ĳ����ӿڷ���
class FriendModel
{
public:
  //��Ӻ��ѹ�ϵ
  void insert(int userid,int friendid);

  //�����û������б� friendid 
  vector<User>query(int userid);


};






#endif