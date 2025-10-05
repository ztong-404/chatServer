#include"groupmodel.hpp"
#include"db.h"


//����Ⱥ��
bool GroupModel::createGroup(Group &group)
{
  //1.��װsql
  char sql[1024]={0};
  sprintf(sql,"insert into allgroup(groupname,groupdesc) values('%s','%s')",
          group.getName().c_str(),group.getDesc().c_str());
          
  MySQL mysql;
  if(mysql.connect())
  {
    if(mysql.update(sql))
    {
      group.setID(mysql_insert_id(mysql.getConnection()));
      return true;
    }
    
  }
  return false;
}

//����Ⱥ��
void GroupModel::addGroup(int userid,int groupid,string role)
{
  char sql[1024]={0};
  sprintf(sql,"insert into groupuser values(%d,%d,%s)",groupid,userid,role.c_str());
  
  MySQL mysql;
  if(mysql.connect())
  {
    mysql.update(sql);
  }
}

//��ѯ�û�����Ⱥ����Ϣ
vector<Group>GroupModel::queryGroups(int userid)
{
/*
1.����userid��groupuser���в�ѯ�����û�������Ⱥ����Ϣ
2.�ڸ���Ⱥ����Ϣ����ѯ���ڸ�Ⱥ��������û���userid�����Һ�user����ж�����ϲ�ѯ������û�����ϸ��Ϣ
*/
  char sql[1024]={0};
  sprintf(sql,"select a.id,a.groupname,a.groupdesc from allgroup a inner join \
          groupuser b on a.id = b.groupid where b.userid=%d",userid);
  
  vector<Group>groupVec;
  
  MySQL mysql;
  if(mysql.connect())
  {
    MYSQL_RES *res=mysql.query(sql);
    if(res!=nullptr)
    {
      MYSQL_ROW row;
      //���userid���е�Ⱥ����Ϣ
      while((row=mysql_fetch_row(res))!=nullptr) 
      {
        Group group;
        group.setID(atoi(row[0]));
        group.setName(row[1]);
        group.setDesc(row[2]);
        groupVec.push_back(group);
      }
      mysql_free_result(res);
    }
  }
  
  //��ѯȺ����û���Ϣ
  for(Group &group:groupVec)
  {
    sprintf(sql,"select a.id,a.name,a.state,b.grouprole from user a \
            inner join groupuser b on b.userid=a.id where b.groupid=%d", group.getID());
            
    MYSQL_RES *res=mysql.query(sql); 
    if(res!=nullptr)
    {
      MYSQL_ROW row;  
      while((row=mysql_fetch_row(res))!=nullptr)
      {
        GroupUser user;
        user.setID(atoi(row[0]));
        user.setName(row[1]);
        user.setState(row[2]);
        user.setState(row[3]);
        group.getUsers().push_back(user);
      }  
      mysql_free_result(res);  
    }               
  }
  return groupVec;
}

//����ָ����groupid��ѯȺ���û�id�б���userid�Լ�����Ҫ�û�Ⱥ��ҵ���Ⱥ��������ԱȺ����Ϣ
vector<int>GroupModel::queryGroupUsers(int userid, int groupid)
{
  char sql[1024]={0};
  sprintf(sql,"select userid from groupuser where groupid = %d and userid != %d",groupid,userid);
  
  vector<int>idVec;
  MySQL mysql;
  if(mysql.connect())
  {
    MYSQL_RES *res=mysql.query(sql);
    if(res!=nullptr)
    {
      MYSQL_ROW row;
      while((row=mysql_fetch_row(res))!=nullptr)
      {
        idVec.push_back(atoi(row[0]));
      }
      mysql_free_result(res);
    }
  }
  return idVec;
}