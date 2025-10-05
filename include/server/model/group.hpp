#ifndef GROUP_H
#define GROUP_H

#include"groupuser.hpp"
#include<string>
#include<vector>
using namespace std;

//User ±íµÄORMÀà
class Group
{
public:
 Group(int ID=-1,string Name="",string Desc=""):id(ID),name(Name),desc(Desc)
 {}

void setID(int id){this->id=id;}
void setName(string name){this->name=name;}
void setDesc(string desc){this->desc=desc;}

int getID(){return id;}
string getName(){return name;}
string getDesc(){return desc;}
vector<GroupUser>&getUsers(){return users;}

private:
  int id;
  string name;
  string desc;
  vector<GroupUser>users;
};

#endif