#ifndef DB_H
#define DB_H

#include<muduo/base/Logging.h>
#include<mysql.h>
#include<string>
using namespace std;

// ���ݿ������
class MySQL
{
public:
// ��ʼ�����ݿ�����
MySQL();


~MySQL();

// �������ݿ�
bool connect();


// ���²���
bool update(string sql);

// ��ѯ����
MYSQL_RES* query(string sql);

//��ȡ����
MYSQL *getConnection();

private:
MYSQL *_conn;
};

#endif