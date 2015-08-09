/*********************************************
  > File  : MyTask.h
  > Author: NewYork
  > Mail  : 2287794993@qq.com 
  > Time  : Mon 13 Jul 2015 11:28:16 PM CST
 ********************************************/
#ifndef _MYTASK_H
#define _MYTASK_H

#include<algorithm>
#include<iostream>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<queue>
#include<set>
#include "Task.h"
#include "./MySocket.h"
#include "./MyResult.h"
#include "./MyCompare.h"
#include "./MyConf.h"
//#include "./MyWebSort.h"

class Task;

class MyTask: public Task
{
    public:
        MyTask(){

        };
        MyTask(MyConf& conf)
            //m_inputStr(""),
            :m_vec(&(conf.m_vec_dict)),
            m_index_task(&conf.m_map_index_vec)
             //std::map<std::string, set<int> >* m_index_task;
    {
        //std::cout<<"m_index_task.size="<<m_index_task->size()<<std::endl;
        memset(&m_addr, 0, sizeof(m_addr));
    }
#if 1
        MyTask(MyConf& conf, const struct sockaddr_in& addr, std::vector<std::string>& vec)
            :m_addr(addr),
            m_inputVec(vec),//存放待查询的单词
            m_vec(&(conf.m_vec_dict)),
            m_index_task(&conf.m_map_index_vec)
             //std::map<std::string, set<int> >* m_index_task;
             //88m_index_task(&(conf.m_map_index_vec))
             //^^!!std::map<std::string, set<int> >* m_index_task;
    {
        std::cout<<"MyTask()"<<std::endl;
        
        for(auto it: *m_vec)
        {
            std::cout<<it.first<<","<<it.second<<std::endl;
        }
    }
#endif
#if 1
        void process()
        {
            MyConf mconf("./conf.dat");
            m_sockfd = socket(AF_INET, SOCK_DGRAM, 0);

            std::string send_buf;//存放最终所有单词的纠错后的单词结果
            //!!std::string getResult(std::string query_word)
            /*处理每一个分词（已去停用词，减少没必要的时间消耗）后单词*/
            //!!m_inputVec(vec),//存放待查询的单词
            for(auto it_vec: m_inputVec)
            {
                send_buf += getResult(it_vec);
            }
            int send_len = send_buf.size();
            int ret_send = sendto(m_sockfd, send_buf.c_str(), 
                    send_len, 0, (struct sockaddr*)&m_addr, sizeof(m_addr));
            std::cout<<"server has send : "<<ret_send<<" bytes data!"<<std::endl;
        }
#endif
#if 0
        void process()
        {
            //std::cout<<"inside process() before*****"<<std::endl;
            MyConf mconf("./conf.dat");
            m_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
            /*纠错后的为一个带有空格的字符串*/
            std::string spellchecked_buf = spellCheck();

            //std::cout<<"after inside void process(): 将客户端的信息纠正后的结果为："<<spellchecked_buf<<std::endl;
            std::cout<<"将客户端的信息纠正后的结果为："<<spellchecked_buf<<std::endl;
            std::string send_buf = mywebsort.handleSearch(spellchecked_buf);
            std::cout<<"8888888888888888888888888888888服务端应该给客户端发送到信息为："<<std::endl;
            std::cout<<send_buf<<std::endl;
            int send_len = send_buf.size();

            std::cout<<"99999999999999999999999999999999务端应该给客户端发送的长度为："<<send_len<<std::endl;

            int ret_send = sendto(m_sockfd, send_buf.c_str(), 
                    send_len, 0, (struct sockaddr*)&m_addr, sizeof(m_addr));
            std::cout<<"server has send : "<<ret_send<<" bytes data!"<<std::endl;

        }
#endif

#if 0
        /*返回带有空格的纠错后的字符串*/
        std::string spellCheck()
        {
            std::cout<<"spellCheck()"<<std::endl;
            std::string spell_buf;//存放最终所有单词的纠错后的单词结果
            //!!std::string getResult(std::string query_word)
            /*处理每一个分词（已去停用词，减少没必要的时间消耗）后单词*/
            //!!m_inputVec(vec),//存放待查询的单词
            for(auto it_vec: m_inputVec)
            {
                //std::cout<<"-----inside spellCheck() for():m_inputVec:"<<it_vec<<std::endl;
                spell_buf += getResult(it_vec);
                //std::cout<<"+++++inside spellCheck() for():spell_buf:"<<spell_buf<<std::endl;
                spell_buf += " ";
            }
            int spell_len = spell_buf.size();
            int ret_spell = sendto(m_sockfd, spell_buf.c_str(), 
                    spell_len, 0, (struct sockaddr*)&m_addr, sizeof(m_addr));
            std::cout<<"server has send : "<<ret_spell<<" bytes data!"<<std::endl;
            return spell_buf;
        }
#endif

        std::size_t nBytesCode(const char ch)
        {
            std::size_t nBytes = 0;
            if(ch & (1 << 7))
            {
                if((ch & 0xF0)==0xC0 || (ch & 0xF0) == 0xD0)
                {
                    nBytes = 2;
                }
                else if((ch & 0xF0) == 0xE0)
                {
                    nBytes = 3;
                }
                else if((ch & 0xF0) == 0xF0)
                {
                    nBytes = 4;
                }
                else if((ch & 0xFF) == 0xFE)
                {
                    nBytes = 6;
                }
            }
            else 
            {
                nBytes = 1;
            }
            return nBytes;
        }

        std::size_t length(const char* str)
        {
            std::size_t len = 0;
            std::size_t nlen = strlen(str);
            for(std::size_t index=0; index != nlen; index++)
            {
                std::size_t nBytes = nBytesCode(str[index]);
                index += (nBytes - 1);
                len++;
            }
            return len;
        }

        //!!int editDistance_1(const std::string& left, const std::string& right)
        int editDistance(const std::string& left, const std::string& right)
        {
            //const char* s1 = m_inputStr.c_str();
            const char* s1 = left.c_str();
            const char* s2 = right.c_str();//***************try the const
            //int len1 = strlen(s1);
            //int len2 = right.size();
            int len1 = length(s1);
            int len2 = length(s2);
            int** d=new int*[len1+1];
            for(int k=0;k<=len1;k++)
                d[k]=new int[len2+1];  
            int i,j;   
            for(i = 0;i <= len1;i++)   
                d[i][0] = i;   
            for(j = 0;j <= len2;j++)   
                d[0][j] = j;   
            for(i = 1;i <= len1;i++)   
                for(j = 1;j <= len2;j++)
                {
                    int cost = s1[i] == s2[j] ? 0 : 1;   
                    int deletion = d[i-1][j] + 1;   
                    int insertion = d[i][j-1] + 1;   
                    int substitution = d[i-1][j-1] + cost;   
                    d[i][j] = min(deletion,insertion,substitution);   
                }   
            //printf("%d\n",d[len1][len2]); 
            for(int k=0;i<=len1;k++)
                delete[] d[k];
            delete[] d;

            return d[len1][len2];
        }

        /*待查询单词和这个单词里面的单字对应的所有索引的编辑距离的计算*/
        void compute_vec_edit(std::string query_word, std::set<int>& iset)
        {
            //std::cout<<"compute_vec_edit()"<<std::endl;
            //std::cout<<"000000the compute_vec_edit() m_queue_result.size="<<m_queue_result.size()<<std::endl;
            /*
            while(!m_queue_result.empty())
            {
                m_queue_result.pop();
            }
            */
            //std::cout<<"111111the compute_vec_edit() m_queue_result.size="<<m_queue_result.size()<<std::endl;
            for(auto it: iset)
            {
                int dist;
                //!!std::vector<std::pair<std::string, int> >* m_vec;
                if((dist = editDistance(query_word, m_vec->at(it).first)) < 10)
                {
                    //std::cout<<"inside editDistance() < 3 ?"<<std::endl;
                    MyResult retTmp;
                    retTmp.m_word = m_vec->at(it).first;//注意下标操作符
                    retTmp.m_freq = m_vec->at(it).second;//对应的词频
                    //retTmp.m_freq = atoi((m_vec->at(it).second).c_str());
                    retTmp.m_dist = dist;
                    m_queue_result.push(retTmp);//存入优先队列
                    std::cout<<"m_queu.size="<<m_queue_result.size()<<std::endl;
                }
            }
            //std::cout<<"22222after the compute_vec_edit() m_queue_result.size="<<m_queue_result.size()<<std::endl;
            /*
            std::string tmp_word = m_queue_result.top().m_word;
            int tmp_freq = m_queue_result.top().m_freq;
            double tmp_dist = m_queue_result.top().m_dist;
            */
            /*
            while(!m_queue_result.empty())
            {
                m_queue_result.pop();
            }
            //std::cout<<"22222the compute_vec_edit() m_queue_result.size="<<m_queue_result.size()<<std::endl;
            MyResult retTmp;
            retTmp.m_word = tmp_word;//注意下标操作符
            retTmp.m_freq = tmp_freq;//对应的词频
            //retTmp.m_freq = atoi((m_vec->at(it).second).c_str());
            retTmp.m_dist = tmp_dist;
            m_queue_result.push(retTmp);//存入优先队列
            */
            //std::cout<<"m_queue_result.size="<<m_queue_result.size()<<std::endl;

            //std::cout<<"999999  the last inside the compute_vec_edit() m_queue_result.size="<<m_queue_result.size()<<std::endl;
        }

        //void getResult(char* arr_ret)
        /*处理输入的单词，依据字母索引进行查询*/
        /*只要一个最优结果
         * 返回结果为：单词、词频、最小编辑距离
         * */
        std::string getResult(std::string query_word)
        {
            //std::cout<<"getResult()"<<std::endl;
            std::string ch;
            /*对汉字和英文字符进行处理
             * 对单词里面的一个一个字进行计算
             * */
            int character_cnt = 0, character_original_cnt = 0;//对中文字符的个数计数
            int ch_cnt = 0, ch_original_cnt = 0;//对英文字符的个数计数
            bool character_flag = false;
            bool ch_flag = false;
            bool first_flag = true;//判断是否是待查询单词的第一个字符，以便判断该查询的是英文还是中文
            for(std::size_t index=0; index<query_word.size();)
            {
                char tmp = query_word[index];
                //if(tmp && (1 << 7))
                /*说明是英文字符*/
                if(first_flag)
                {
                    if(isalpha(tmp))
                    {
                        ch_flag = true;
                        ch_cnt = query_word.size();//这个英文单词的字符总数 
                        ch_original_cnt = ch_cnt;
                    }
                    /*说明是中文字符*/
                    else//handle a chinese character
                    {
                        character_flag = true;
                        character_cnt = query_word.size() / 3;//这个中文单词的字符总数
                        character_original_cnt = character_cnt;
                    }
                    first_flag = false;//此循环结束
                }
                if(isalpha(tmp))
                {
                    ch = query_word.substr(index, 1);
                    index++;
                }
                /*说明是中文字符*/
                else//handle a chinese character
                {
                    ch = query_word.substr(index, 3);
                    index += 3;
                }
                /*
                 *仅仅是针对query_word这个单词里面所有的单字进行计算
                 * */
                /*每处理完一个单字便对单字对应的所有单词（序号索引表示）进行编辑距离计算*/
                //!!std::map<std::string, set<int> >* m_index_task;
                if((*m_index_task).find(ch) != (*m_index_task).end())
                {
                    /*两个if只有一个执行*/
                    if(ch_flag)
                    {
                        ch_cnt--;
                    }
                    if(character_flag)
                    {
                        character_cnt--;
                    }
                    compute_vec_edit(query_word, (*m_index_task)[ch]);
                    //std::cout<<"inside getResult() m_queue_result.size="<<m_queue_result.size()<<std::endl;
                }
                /*
                   else
                   {
                   std::cout<<"%%%%%%%%%%%%%%%%所找的单词不用纠错，哈哈哈！"<<std::endl;
                   return query_word;
                   }
                   */
            }

            /*如果未对query_word进行纠错的话就直接返回即可*/
#if 0
            if(ch_flag && (ch_cnt==ch_original_cnt))
            {
                return query_word; 
            }
            if(character_flag && (character_cnt==character_original_cnt))
            {
                return query_word;
            }
#endif
            //std::cout<<"outside getResult() m_queue_result.size="<<m_queue_result.size()<<std::endl;
            //!!void compute_vec_edit(set<int>& iset)
            //!!std::map<std::string, set<int> >* m_index_task;//里面存放的是单字而非单词
            //!!std::vector<std::string> m_inputVec;
            //!!std::cout<<"m_inputVec.size() = "<<m_inputVec.size()<<std::endl;
            //int cnt =1;
            std::size_t index = 0;

            std::string arr_ret;
            if(m_queue_result.empty())
            {
                std::cout<<"queue empty!"<<std::endl;
                exit(-1);
            }
            arr_ret += m_queue_result.top().m_word;

#if 1
            arr_ret += "\t";
            char freq_char[100];
            sprintf(freq_char, "%d", m_queue_result.top().m_freq);
            arr_ret += freq_char;
            arr_ret += "\t";
            memset(freq_char, 0, sizeof(freq_char));
            sprintf(freq_char, "%d", m_queue_result.top().m_dist);
            arr_ret += freq_char;
            arr_ret += "\n";
            //m_queue_result.pop();

            //!!弹出最优结果
            //m_queue_result.pop();

#endif
            return arr_ret;
        }
#if 0
        std::string getResult(std::string query_word)
        {
            std::cout<<"getResult()"<<std::endl;
            std::string ch;
            /*对汉字和英文字符进行处理*/
            for(std::size_t index=0; index<query_word.size();)
            {
                char tmp = query_word[index];
                //if(tmp && (1 << 7))
                if(isalpha(tmp))
                {
                    ch = query_word.substr(index, 1);
                    index++;
                }
                else//handle a chinese character
                {
                    ch = query_word.substr(index, 3);
                    index += 3;
                }
                /*
                 *仅仅是针对query_word这个单词里面所有的单字进行计算
                 * */
                /*每处理完一个单字便对单字对应的所有单词（序号索引表示）进行编辑距离计算*/
                //!!std::map<std::string, set<int> >* m_index_task;
                if((*m_index_task).find(ch) != (*m_index_task).end())
                {
                    compute_vec_edit(query_word, (*m_index_task)[ch]);
                    //std::cout<<"inside getResult() m_queue_result.size="<<m_queue_result.size()<<std::endl;
                }
            }
            //std::cout<<"outside getResult() m_queue_result.size="<<m_queue_result.size()<<std::endl;
            //!!void compute_vec_edit(set<int>& iset)
            //!!std::map<std::string, set<int> >* m_index_task;//里面存放的是单字而非单词
            //!!std::vector<std::string> m_inputVec;
            //!!std::cout<<"m_inputVec.size() = "<<m_inputVec.size()<<std::endl;
            //int cnt =1;
            std::size_t index = 0;

            std::string arr_ret;
            if(m_queue_result.empty())
            {
                std::cout<<"queue empty!"<<std::endl;
                exit(-1);
            }
            arr_ret += m_queue_result.top().m_word;
            arr_ret += "\t";

            char freq_char[10];
            sprintf(freq_char, "%d", m_queue_result.top().m_freq);
            arr_ret += freq_char;
            arr_ret += "\t";
            memset(freq_char, 0, sizeof(freq_char));
            sprintf(freq_char, "%d", m_queue_result.top().m_dist);
            arr_ret += freq_char;
            arr_ret += "\n";

            //!!弹出最优结果
            //m_queue_result.pop();


            return arr_ret;
        }
#endif

        ~MyTask()
        {
            close(m_sockfd);
        }

    private:
        /*单词和对应的词频*/
        std::vector<std::pair<std::string, int> >* m_vec;
        std::map<std::string, set<int> >* m_index_task;
        std::priority_queue<MyResult, std::vector<MyResult>, MyCompare> m_queue_result;
        //std::string  m_inputStr;
        /*存放待查询的单词*/
        std::vector<std::string> m_inputVec;
        struct sockaddr_in m_addr;
        //MyWebSort mywebsort;
        int m_sockfd;
        int min(int a,int b,int c) {int t = a < b ? a : b;   return t < c ? t : c;}   
};

#endif
