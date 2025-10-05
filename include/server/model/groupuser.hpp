#ifndef GROUPUSER_H
#define GROUPUSER_H

#include"user.hpp"

//群组用户,多了一个role角色信息,从User类直接继承,复用User的其它信息
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