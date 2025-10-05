#include"offlinemessagemodel.hpp"
#include"db.h"


  //�洢�û���������Ϣ
  void offlineMsgModel::insert(int userid,string msg)
  {
   //1.��װsql���
  char sql[1024]={0};
  sprintf(sql,"insert into offlinemessage values('%d','%s')",userid,msg.c_str());

  MySQL mysql;
  if(mysql.connect()){
      mysql.update(sql); 
    }
  }

  //ɾ���û���������Ϣ
  void offlineMsgModel::remove(int userid)
  {
    //1.��װsql���
    char sql[1024]={0};
    sprintf(sql,"delete from offlinemessage where userid=%d",userid);

    MySQL mysql;
    if(mysql.connect()){
        mysql.update(sql); 
      }
  }
  
  //��ѯ�û���������Ϣ
  vector<string> offlineMsgModel::query(int userid)
  {
  //1.��װsql���
  char sql[1024]={0};
  sprintf(sql,"select message from offlinemessage where userid = %d",userid);

   vector<string>vec;
  MySQL mysql;
  if(mysql.connect())
  {
    MYSQL_RES *res=mysql.query(sql);
    if(res!=nullptr)
    { 
      //��userid�û�������������Ϣ����vec�з���
      MYSQL_ROW row;
      while((row=mysql_fetch_row(res))!=nullptr)
      {
        vec.push_back(row[0]);
      }
      mysql_free_result(res);
      return vec;
    }
  }
  return vec;
}
  