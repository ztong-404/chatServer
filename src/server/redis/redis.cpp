#include"redis/redis.hpp"
#include<iostream>
using namespace std;


Redis::Redis():_publish_context(nullptr),_subcribe_context(nullptr){}
Redis::~Redis()
{
  if(_publish_context!=nullptr)
  {
    redisFree(_publish_context);
  }
  if(_subcribe_context!=nullptr)
  {
    redisFree(_subcribe_context);
  }
}
  
  //����redis������
  bool Redis::connect()
  {
    //����publish������Ϣ������������
    _publish_context=redisConnect("127.0.0.1",6379);
    if(_publish_context==nullptr)
    {
      cerr<<"connect redis failed!"<<endl;
      return false;
    }
    
    //����subscribe������Ϣ������������
    _subcribe_context=redisConnect("127.0.0.1",6379);
    if(_subcribe_context==nullptr)
    {
      cerr<<"connect redis failed!"<<endl;
      return false;
    }
    
    //�ڵ������߳��У�����ͨ���ϵ��¼�������Ϣ��ҵ�������ϱ�
    thread t([&](){
      observer_channel_message();
    });
    t.detach();
    
    cout<<"connect redis-server success!"<<endl;
    
    return true;
  }
  
  //��redisָ����ͨ��channel������Ϣ
  bool Redis::publish(int channel, string message)
  {
    redisReply *reply=(redisReply *)redisCommand(_publish_context,"PUBLISH %d %s",channel, 
                                                    message.c_str());
    if(reply==nullptr)
    {
      cerr<<"publish command failed!"<<endl;
      return false;
    }
    freeReplyObject(reply);
    return true;
  }
  
  //��redisָ����ͨ��subscribe������Ϣ
  bool Redis::subscribe(int channel)
  {
    //SUBSCRIBE����������߳������ȴ�ͨ�����淢����Ϣ������ֻ������ͨ����������ͨ����Ϣ
    //ͨ����Ϣ�Ľ���ר����observer_channel_message�����еĶ����߳��н���
    //ֻ�������������������redis server��Ӧ��Ϣ�������notifyMsg�߳���ռ��Ӧ��Դ
    if(REDIS_ERR==redisAppendCommand(this->_subcribe_context,"SUBSCRIBE %d",channel))
    {
      cerr<<"subscribe command failed!"<<endl;
      return false;
    }
  
    //redisBufferWrite����ѭ�����ͻ�������ֱ�����������ݷ������(done����Ϊ1)
    int done=0;
    while(!done)
    {
      if(REDIS_ERR==redisBufferWrite(this->_subcribe_context,&done))
      {
        cerr<<"subscribe command failed!"<<endl;
        return false;
      }
    }
    //redisGetReply 
  return  true;  
  }
  
  //��redisָ����ͨ��unsubscribeȡ��������Ϣ
  bool Redis::unsubscribe(int channel)
  {
    if(REDIS_ERR==redisAppendCommand(this->_subcribe_context,"UNSUBSCRIBE %d",channel))
    {
      cerr<<"unsubscribe command failed!"<<endl;
      return false;
    }
    
    //redisBufferWrite����ѭ�����ͻ�������ֱ�����������ݷ������(done����Ϊ1)
    int done=0;
    while(!done)
    {
      if(REDIS_ERR==redisBufferWrite(this->_subcribe_context,&done))
      {
        cerr<<"unsubscribe command failed!"<<endl;
        return false;
      }
    }
    return true;
  }

  //�ڶ����߳��н��ն���ͨ���е���Ϣ
  void Redis::observer_channel_message()
  {
    redisReply *reply=nullptr;
    while(REDIS_OK==redisGetReply(this->_subcribe_context,(void**)&reply))
    {
      //�����յ�����Ϣ��һ������Ԫ�ص�����
      if(reply!=nullptr&&reply->element[2]!=nullptr&&reply->element[2]->str!=nullptr)
      {
        //��ҵ����ϱ�ͨ���Ϸ�������Ϣ
        _notify_message_handler(atoi(reply->element[1]->str),reply->element[2]->str);
      }
      freeReplyObject(reply);
    }    
    cerr<<">>>>>>>>>>>>>>>>>>> observer_channel_message quit <<<<<<<<<<<<<<<<<<<<"<<endl;
  }
  
  //��ʼ����ҵ����ϱ�����Ϣ�Ļص�����
  void Redis::init_notify_handler(function<void(int,string)>fn)
  {
    this->_notify_message_handler=fn;
  }