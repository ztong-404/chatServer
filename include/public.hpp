#ifndef PUBLIC_H
#define PUBLIC_H

/*
server��client�����ļ�
*/
enum EnMsgType
{
  LOGIN_MSG=1,     //��¼��Ϣ
  LOGIN_MSG_ACK,   //��¼��Ӧ��Ϣ
  LOGOUT_MSG,       //ע����Ϣ
  REG_MSG,         //ע����Ϣ
  REG_MSG_ACK,     //ע����Ӧ��Ϣ
  ONE_CHAT_MSG,    //������Ϣ
  ADD_FRIEND_MSG,  //��Ӻ�����Ϣ
  
  CREATE_GROUP_MSG, //����Ⱥ��
  ADD_GROUP_MSG,    //����Ⱥ��
  GROUP_CHAT_MSG,   //Ⱥ����
};


#endif