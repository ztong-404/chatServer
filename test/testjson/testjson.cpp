#include"json.hpp"
using json = nlohmann::json;

#include<iostream>
#include<vector>
#include<map>
#include<string>
using namespace std;

//json���л�1
void func1()
{
  json js;
  js["msg_type"]=2;
  js["from"]="zhang san";
  js["to"]="li si";
  js["msg"]="hello, what are you doing now?";
  
 // string sendBuf=js.dump();
 // cout<<sendBuf.c_str()<<endl;
  cout<<js<<endl;
}

void func2()
{
json js;
// �������
js["id"] = {1,2,3,4,5};
// ���key-value
js["name"] = "zhang san";
// ��Ӷ���
js["msg"]["zhang san"] = "hello world";
js["msg"]["liu shuo"] = "hello china";
// �����ͬ���������һ��������������
js["msg"] = {{"zhang san", "hello world"}, {"liu shuo", "hello china"}};
cout << js << endl;
}

int main()
{
  func2();
  
  return 0;
}