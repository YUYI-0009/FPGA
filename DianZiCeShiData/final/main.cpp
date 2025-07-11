#include <cstdio>
#include <iostream>
#include <string>
#include <unistd.h>
#include <signal.h>
#include "serial.h"
#include <setjmp.h>
using namespace std;
typedef unsigned char uchar;
#define error -1
#define check_failed -2

string format(const vector<uchar> &data);			//数据转换为字符串
int check(serial &serial1, int addr);				//设备正常检测
int get_score(serial &serial1, int addr);			//分数获取
void reset(serial &serial1);					//从机复位

int main() {
    serial serial1("/dev/ttyUSB0", B9600);
    int addr, check_ret, score;
    while(1){
    	cout<<"---------------基于RS485的总线评分系统---------------"<<endl;
        cout<<"请输入：\n从机地址:查询分数\n-2:将从机复位\n-1:退出程序"<<endl;
        cin>>addr;
        if(addr == -1) break;
        if(addr == -2) {
            reset(serial1);
            continue;
        }
        check_ret = check(serial1, addr);
        if(check_ret == check_failed) cout<<"数据校验错误"<<endl;
        else if(check_ret == 1){
            cout<<"设备检测正常"<<endl;
            sleep(1);
    	    score = get_score(serial1, addr);
    	    if(score == check_failed)
    	    	cout<<"数据校验错误"<<endl;
    	    else if(score == error)
    	        cout<<"回应错误"<<endl;
    	    else 
    	        cout<<"分数："<<score<<endl;
        }
        else if(check_ret == error){
            cout<<"回应错误"<<endl;
            continue;
        }
	else{
	    cout<<"数据传输错误"<<endl;
	}        
    }
    return 0;
}
//将vector中的数据转为字符串
string format(const vector<uchar> &data) {
    std::string str(2 * data.size() + 1, '\x00');
    for (int i = 0; i < data.size(); i++) {
        sprintf(&str[i * 2], "%02X", data[i]);
    }
    return str;
}
//设备正常检测：5a + 从机地址 + 检测功能码(08) + check_content(13) + 校验码 
int check(serial &serial1,int addr){
	int check_code = 117 + addr, ret;				//校验码为累加和
	vector<uchar> code = {0x5a, 0x08, 0x13};
	vector<uchar> rec;
	code.insert(code.begin()+1, (uchar)addr);			//插入从机地址
	code.push_back((uchar)check_code);				//插入校验码				
	/* 写入并接收回应数据包 */
	serial1.myWrite(code);
	sleep(1);
	rec = serial1.myRead(5);
	if(rec[0] + rec[1] + rec[2] + rec[3] != rec[4]) return check_failed; //校验不通过
	if(format(rec) == format(code))					//检验接收数据包是否与发送相同，相同则从机地址正确
	    ret = 1;
	else if(rec[3] == 0x6f) ret = 0;					//回应错误
	return ret;    
}
//获取从机分数：5a + 00 + 读取功能码03 + 从机地址 + 校验字节 
int get_score(serial &serial1, int addr){
	int check_code = 93 + addr, ret;				//校验码为累加和
	vector<uchar> code = {0x5a, 0x00, 0x03};
	vector<uchar> rec;
	code.insert(code.begin()+3, (uchar)addr);			//插入从机地址
	code.push_back((uchar)check_code);				//插入校验码			
	/* 写入并接收回应数据包 */
	serial1.myWrite(code);
	sleep(1);
	rec = serial1.myRead(5);
	if(rec[0] + rec[1] + rec[2] + rec[3] != rec[4]) return check_failed; //校验不通过
	if(rec[3] == 0x6f) ret = error;					//回应错误
	else
	    ret = (int)rec[3];						//转为数字
	return ret;
}

//从机复位：5a + 广播地址00 + 复位功能码01 + 00 + 校验字节
void reset(serial &serial1){
	vector<uchar> code = {0x5a, 0x00, 0x01,0x00,0x5b};				
	/* 发送数据包 */
	for(int i=0;i<500;i++) {
	    serial1.myWrite(code);
	}
	cout<<"从机已复位"<<endl;
	return;	    
}

