#ifndef GROUPUSER_H
#define GROUPUSER_H

#include"user.hpp"

//Ⱥ���û�,����һ��role��ɫ��Ϣ,��User��ֱ�Ӽ̳�,����User��������Ϣ
class GroupUser:public User
{
public:
  //GroupUser(string Role):role(Role){}
  void setRole(string Role){role=Role;}
  string getRole(){return role;}
private:
  string role;


};
#endif