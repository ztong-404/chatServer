#ifndef OFFLINEMESSAGEMODEL_H
#define OFFLINEMESSAGEMODEL_H

#include<vector>
#include<string>
using namespace std;


class offlineMsgModel{



public:
//�洢�û���������Ϣ
  void insert(int userid,string msg);
   //ɾ���û���������Ϣ
  void remove(int userid);
  //��ѯ�û���������Ϣ
  vector<string> query(int userid);
private:

};







#endif