#ifndef USERMODEL_H
#define USERMODEL_H

#include"user.hpp"

//User������ݲ�����
class UserModel{
public:
  //User������ӷ���
  bool insert(User &user);

  //�����û������ѯ�û���Ϣ
  User query(int id);
  
  //�����û���״̬��Ϣ
  bool updateState(User user);

  //�����û���״̬��Ϣ
  void resetState();

};
#endif
