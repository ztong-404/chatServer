#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include<muduo/net/TcpConnection.h>
#include<unordered_map>
#include<functional>
#include<mutex>
using namespace std;
using namespace muduo;
using namespace muduo::net;

#include"redis/redis.hpp"
#include"friendmodel.hpp"
#include"usermodel.hpp"
#include"offlinemessagemodel.hpp"
#include"json.hpp"
#include"groupmodel.hpp"
using json=nlohmann::json;

//��ʾ������Ϣ���¼��ص���������
using MsgHandler=std::function<void(const TcpConnectionPtr &conn, json &js, Timestamp)>;

//���������ҵ����
class ChatService
{
public:
  //��ȡ��������Ľӿں���
  static ChatService* instance();
  //�����¼ҵ��
  void login(const TcpConnectionPtr &conn, json &js, Timestamp time);
  //����ע��ҵ��
  void reg(const TcpConnectionPtr &conn, json &js, Timestamp time);
  //һ��һ����ҵ��
  void oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
  //��Ӻ���ҵ��
  void addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time);
   //����Ⱥ��ҵ��
  void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
  //����Ⱥ��ҵ��
  void addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
   // Ⱥ������ҵ��
  void groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
  //����ע��ҵ��
  void logout(const TcpConnectionPtr &conn, json &js, Timestamp time);
  //��ȡ��Ϣ��Ӧ�Ĵ�����
  MsgHandler getHandler(int msgid);
  //����ͻ����쳣�˳�
  void clientCloseException(const TcpConnectionPtr &conn);
  
  //�������쳣��ҵ�����÷���
  void reset();
  
  //��redis��Ϣ�����л�ȡ���ĵ���Ϣ
  void handleRedisSubscribeMessage(int, string);

private:
  ChatService();
  
  //�洢��Ϣid�����Ӧ��ҵ������
  unordered_map<int,MsgHandler> _msgHandlerMap;
  
  // �洢�����û���ͨ������
  unordered_map<int,TcpConnectionPtr>_userConnMap;

  //���廥��������֤_userConnMap���̰߳�ȫ
  mutex _connMutex;
  
  //���ݲ��������
  UserModel _userModel;
  offlineMsgModel _offlineMsgModel;
  FriendModel _friendModel;
  GroupModel _groupModel;
  
  //redis��������
  Redis _redis;
};


#endif