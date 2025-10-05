#ifndef REDIS_H
#define REDIS_H

#include<hiredis/hiredis.h>
#include<thread>
#include<functional>
using namespace std;


class Redis
{
public:
  Redis();
  ~Redis();
  
  //����redis������
  bool connect();
  
  //��redisָ����ͨ��channel������Ϣ
  bool publish(int channel, string message);
  
  //��redisָ����ͨ��subscribe������Ϣ
  bool subscribe(int channel);
  
  //��redisָ����ͨ��unsubscribeȡ��������Ϣ
  bool unsubscribe(int channel);

  //�ڶ����߳��н��ն���ͨ���е���Ϣ
  void observer_channel_message();
  
  //��ʼ����ҵ����ϱ�����Ϣ�Ļص�����
  void init_notify_handler(function<void(int,string)>fn);

private:
  //hiredisͬ�������Ķ��󣬸���publish��Ϣ
  redisContext *_publish_context;

  //hiredisͬ�������Ķ��󣬸���subscribe��Ϣ
  redisContext *_subcribe_context;

  //�ص��������յ����ĵ���Ϣ����service���ϱ�
  function<void(int,string)>_notify_message_handler;
};

#endif