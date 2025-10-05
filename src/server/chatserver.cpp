#include"chatserver.hpp"
#include"json.hpp"
#include"chatservice.hpp"

#include<string>
#include<functional>
using namespace std;
using namespace placeholders;
using json = nlohmann::json;

ChatServer::ChatServer(EventLoop *loop, const InetAddress &listenAddr, const string &nameArg)
                      :_server(loop,listenAddr,nameArg)
                      ,_loop(loop)
{
    //ע�����ӻص�
    _server.setConnectionCallback(std::bind(&ChatServer::onConnection,this,_1));
    
    //ע����Ϣ�ص�
    _server.setMessageCallback(std::bind(&ChatServer::onMessage,this,_1,_2,_3));
    
    //�����߳�����
    _server.setThreadNum(4);
}

//��������
void ChatServer::start()
{
  _server.start();
}

//�ϱ����������Ϣ�Ļص�����
void ChatServer::onConnection(const TcpConnectionPtr &conn)
{
  //�ͻ��˶Ͽ�����
  if(!conn->connected())
  {
    ChatService::instance()->clientCloseException(conn);
    conn->shutdown();
  }

}

//�ϱ���д�¼������Ϣ�Ļص�����
void ChatServer::onMessage(const TcpConnectionPtr &conn, Buffer *buffer, Timestamp time)
{
  string buf=buffer->retrieveAllAsString();
  //���ݵķ����л�
  json js=json::parse(buf);
  //�ﵽ��Ŀ�ģ���ȫ��������ģ��Ĵ����ҵ��ģ��Ĵ���
  //ͨ��js[��msgid��]��ȡ->ҵ��handler->conn js 
  auto msgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>());
  //�ص���Ϣ�󶨺õ��¼�����������ִ����Ӧ��ҵ����
  msgHandler(conn,js,time);
}

