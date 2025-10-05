#include"chatservice.hpp"
#include"public.hpp"
#include<string>
#include<muduo/base/Logging.h>
#include<vector>

using namespace muduo;
using namespace std;

//��ȡ��������Ľӿں���
  ChatService* ChatService::instance()
  {
    static ChatService service;
    return &service;
  }
  
  //ע����Ϣ�Լ���Ӧ��Handler�ص�����
  ChatService::ChatService()
  {
    _msgHandlerMap.insert({LOGIN_MSG,std::bind(&ChatService::login,this,_1,_2,_3)});
    _msgHandlerMap.insert({LOGOUT_MSG,std::bind(&ChatService::logout,this,_1,_2,_3)});
    _msgHandlerMap.insert({REG_MSG,std::bind(&ChatService::reg,this,_1,_2,_3)});
    _msgHandlerMap.insert({ONE_CHAT_MSG,std::bind(&ChatService::oneChat,this,_1,_2,_3)});
    _msgHandlerMap.insert({ADD_FRIEND_MSG,std::bind(&ChatService::addFriend,this,_1,_2,_3)});
    
    _msgHandlerMap.insert({CREATE_GROUP_MSG,std::bind(&ChatService::createGroup,this,_1,_2,_3)});
    _msgHandlerMap.insert({ADD_GROUP_MSG,std::bind(&ChatService::addGroup,this,_1,_2,_3)});
    _msgHandlerMap.insert({GROUP_CHAT_MSG,std::bind(&ChatService::groupChat,this,_1,_2,_3)});
    
    //����redis������
    if(_redis.connect())
    {
      //�����ϱ���Ϣ�Ļص�
      _redis.init_notify_handler(std::bind(&ChatService::handleRedisSubscribeMessage,this,_1,_2));
    }
  }
  
  //�������쳣��ҵ�����÷���
  void ChatService::reset()
  {
    //��online״̬���û������ó�offline
    _userModel.resetState();
  
  }
  
  
    //��ȡ��Ϣ��Ӧ�Ĵ�����
  MsgHandler ChatService::getHandler(int msgid)
  {
      //��¼������־��msgidû�ж�Ӧ���¼�����ص�
      auto it =_msgHandlerMap.find(msgid);
      if(it==_msgHandlerMap.end())
      {
          //����һ��Ĭ�ϵĴ��������ղ���
          return [=](const TcpConnectionPtr &conn,json &js, Timestamp){
            LOG_ERROR<<"msgid:"<<msgid<<" can not find handler!";
        } ;
      }
      else
      {
        return _msgHandlerMap[msgid];
      }
  }
  
  //�����¼ҵ��
  void ChatService::login(const TcpConnectionPtr &conn, json &js, Timestamp time)
  {
    int id=js["id"].get<int>();
    string pwd=js["password"];
    
    User user=_userModel.query(id);
    if(user.getID()==id && user.getPwd()==pwd)
    {
      if(user.getState()=="online")
      {
        //���û��Ѿ���¼���������ظ���¼
        json response;
        response["msgid"]=LOGIN_MSG_ACK;
        response["error"]=2;
        response["errmsg"]="this count is using, input another!";
        conn->send(response.dump());  
      }
      else
      {
        //��¼�ɹ�����¼�û�������Ϣ
          {
            lock_guard<mutex>lock(_connMutex);
            _userConnMap.insert({id,conn});
          }
          
          //id�û���¼�ɹ�����redis����channel(id)
          _redis.subscribe(id);
      
        //��¼�ɹ��������û�״̬��Ϣstate offline->online
        user.setState("online");
        _userModel.updateState(user);
        
        json response;
        response["msgid"]=LOGIN_MSG_ACK;
        response["error"]=0;
        response["id"]=user.getID();
        response["name"]=user.getName();
        
        // ��ѯ���û��Ƿ���������Ϣ
        vector<string> vec=_offlineMsgModel.query(id);
        if(!vec.empty())
        {
          response["offlinemsg"]=vec;
          //��ȡ���û���������Ϣ�󣬰Ѹ��û�������������Ϣɾ����
          _offlineMsgModel.remove(id);
        }
        //��ѯ���û��ĺ�����Ϣ������
        vector<User>userVec=_friendModel.query(id);
        if(!userVec.empty())
        {
          vector<string>vec2;
          for(User &user:userVec)
          {
            json is;
            js["id"]=user.getID();
            js["name"]=user.getName();
            js["state"]=user.getState();
            vec2.push_back(js.dump());
          }
          response["friends"]=vec2;
        }
        
        //��ѯ�û���Ⱥ����Ϣ
        vector<Group>groupuserVec=_groupModel.queryGroups(id);
        if(!groupuserVec.empty())
        {
          //group:[{groupid:[xxx,xxx,xxx,xxx]}]
          vector<string>groupV;
          for(Group &group:groupuserVec)
          {
            json grpjson;
            grpjson["id"]=group.getID();
            grpjson["groupname"]=group.getName();
            grpjson["groupdesc"]=group.getDesc();
            vector<string>userV;
            for(GroupUser &user:group.getUsers())
            {
              json js;
              js["id"]=user.getID();
              js["name"]=user.getName();
              js["state"]=user.getState();
              js["role"]=user.getRole();
              userV.push_back(js.dump());
            }
            grpjson["users"]=userV;
            groupV.push_back(grpjson.dump());    
          }
          response["groups"]=groupV;
        }
                     
        conn->send(response.dump());
      }     
    }
    else
    {
      //���û������ڣ��û����ڵ���������󣬵�¼ʧ��
      json response;
      response["msgid"]=LOGIN_MSG_ACK;
      response["error"]=1;
      response["errmsg"]="id or password is invalid!";
      conn->send(response.dump());
    }
  }
  
  //����ע��ҵ��
  void ChatService::reg(const TcpConnectionPtr &conn, json &js, Timestamp time)
  {
    string name=js["name"];
    string pwd=js["password"];
    
    User user;
    user.setName(name);
    user.setPwd(pwd);
    bool state=_userModel.insert(user);
    if(state){
      //ע��ɹ�
      json response;
      response["msgid"]=REG_MSG_ACK;
      response["error"]=0;
      response["id"]=user.getID();
      conn->send(response.dump());
    }
    else{
      //ע��ʧ��
         json response;
      response["msgid"]=REG_MSG_ACK;
      response["error"]=1;
      conn->send(response.dump());
    }
  }
  
    //����ע��ҵ��
  void ChatService::logout(const TcpConnectionPtr &conn, json &js, Timestamp time)
  {
    int userid=js["id"].get<int>();
    {   //
      lock_guard<mutex>lock(_connMutex);
      auto it=_userConnMap.find(userid);
      if(it!=_userConnMap.end())
      {
        _userConnMap.erase(it);
      }     
    }    //
    
    //�û�ע�����൱�ھ������ߣ���redis��ȡ������ͨ��
    _redis.unsubscribe(userid );
    
    //�����û���״̬��Ϣ
    User user(userid,"","","offline");
    _userModel.updateState(user);
  }
  
  //����ͻ����쳣�˳�
  void ChatService::clientCloseException(const TcpConnectionPtr &conn)
  {
    User user;
    {//
    lock_guard<mutex>lock(_connMutex);
    for(auto it=_userConnMap.begin();it!=_userConnMap.end();++it)
    {
        if(it->second==conn)
        {
          //��map��ɾ���û���������Ϣ
          user.setID(it->first);
          _userConnMap.erase(it);
          break;
        }
      }
    }//
    
    //�û�ע�����൱�ھ������ߣ���redis��ȡ������ͨ��
    _redis.unsubscribe(user.getID());
    
  //�����û���״̬��Ϣ
  if(user.getID()!=-1)
  {
    user.setState("offline");
    _userModel.updateState(user);
  }
}

  //һ��һ����ҵ��
  void ChatService::oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time)
  {
    int toid=js["toid"].get<int>();
 
    
    {//
      lock_guard<mutex>lock(_connMutex);
      auto it=_userConnMap.find(toid);
      if(it!=_userConnMap.end())
      {
       //toid���ߣ�ת����Ϣ   ����������������Ϣ��toid�û�
        it->second->send(js.dump());
        return;
      } 
    }//
     
     //��ѯtoid�Ƿ�����
     User user=_userModel.query(toid);
     if(user.getState()=="online")
     {
       _redis.publish(toid,js.dump());
       return;
     }
     
    //toid�����ߣ��洢������Ϣ
    _offlineMsgModel.insert(toid,js.dump());
  }
  
    //��Ӻ���ҵ��msgid id friendid
  void ChatService::addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time)
  {
    int userid=js["id"].get<int>();
    int friendid=js["friendid"].get<int>();
    
    //�洢������Ϣ
    _friendModel.insert(userid,friendid);
  
  }
  
  //����Ⱥ��ҵ��
  void ChatService::createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time)
  {
    int userid = js["id"].get<int>();
    string name=js["groupname"];
    string desc=js["groupdesc"];
    
    //�洢�´�����Ⱥ����Ϣ
    Group group(-1,name,desc);
    if(_groupModel.createGroup(group))
    {
      //�洢Ⱥ�鴴������Ϣ
      _groupModel.addGroup(userid,group.getID(),"creator");
    } 
  }
  
  //����Ⱥ��ҵ��
  void ChatService::addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time)
  {
    int userid=js["id"].get<int>();
    int groupid=js["groupid"].get<int>();
    _groupModel.addGroup(userid,groupid,"normal");
  }
  
  // Ⱥ������ҵ��
  void ChatService::groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time)
  {
    int userid=js["id"].get<int>();
    int groupid=js["groupid"].get<int>();
    vector<int>useridVec=_groupModel.queryGroupUsers(userid,groupid);
    
    lock_guard<mutex>lock(_connMutex);
    for(int id:useridVec)
    {
      auto it=_userConnMap.find(id);
      if(it!=_userConnMap.end())
      {
        //ת��Ⱥ��Ϣ
        it->second->send(js.dump());
      }
      else
      {
        //��ѯtoid�Ƿ�����
        User user=_userModel.query(id);
        if(user.getState()=="online")
        {
           _redis.publish(id,js.dump()); 
        }else{
      
        //�洢����Ⱥ��Ϣ
        _offlineMsgModel.insert(id,js.dump());
        }
      }
    }
  }
  
  
  
  // ��redis��Ϣ�����л�ȡ���ĵ���Ϣ
void ChatService::handleRedisSubscribeMessage(int userid, string msg)
{
    lock_guard<mutex> lock(_connMutex);
    auto it = _userConnMap.find(userid);
    if (it != _userConnMap.end())
    {
        it->second->send(msg);
        return;
    }

    // �洢���û���������Ϣ
    _offlineMsgModel.insert(userid, msg);
}
  
  
  
  
  
  
  
  