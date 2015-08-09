/*********************************************
> File  : test_main.h
> Author: NewYork
> Mail  : 2287794993@qq.com 
> Time  : Mon 13 Jul 2015 11:28:16 PM CST
 ********************************************/

#include "MyTask.h"
#include <iostream>
#include "MyConf.h"
#include<unistd.h>
#include "ThreadPool.h"
#include<utility>
#include "MySocket.h"
#include<iostream>
#include "./MySplit.h"
#include "./MyResult.h"

class ThreadPool;

int main(void)
{
#if 1
	std::string path = "./conf.dat";
	MyConf mcf(path); 
	struct sockaddr_in address;
	MySocket msock(mcf);
	//std::cout<<"after MySocket msock(mcf)"<<std::endl;
	char recv_buf[1024];
	ThreadPool threadpool(4, 10);
    threadpool.start();
    MySplit mysplit;

	while(1)
	{
		memset(recv_buf, 0, sizeof(recv_buf));
		msock.recv_info(recv_buf, sizeof(recv_buf));
		std::cout<<"the data : "<<recv_buf<<std::endl;
        std::vector<std::string> recv_vec;
		recv_vec = mysplit.wordSplit(recv_buf);
        
        //std::stringstream ss(recv_buf_str);
        //std::string word;
        std::cout<<"recv_vec.size="<<recv_vec.size()<<std::endl;
        for(auto it: recv_vec)
        {
            std::cout<<it<<", ";
            recv_vec.push_back(it);
        }
        std::cout<<std::endl;
		//MyTask* pTask = new MyTask(mcf, msock.getAddr(), recv_buf_str);
		MyTask* pTask = new MyTask(mcf, msock.getAddr(), recv_vec);
		threadpool.addTask(pTask);
	}
#endif 
	return 0;
}
