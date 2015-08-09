/*********************************************
> File  : MySplit.h
> Author: NewYork
> Mail  : 2287794993@qq.com 
> Time  : Mon 20 Jul 2015 07:20:58 PM CST
********************************************/

#ifndef _MYSPLIT_H
#define _MYSPLIT_H

#include<iostream>
#define LOGGER_LEVEL LL_WARN
#include "./jieba/cppjieba/src/MPSegment.hpp"
#include "./jieba/cppjieba/src/HMMSegment.hpp"
#include "./jieba/cppjieba/src/MixSegment.hpp"
using namespace CppJieba;
const char * const JIEBA_DICT_FILE = "./jieba/cppjieba/dict/jieba.dict.utf8";
const char * const HMM_DICT_FILE = "./jieba/cppjieba/dict/hmm_model.utf8";
const char * const USER_DICT_FILE = "./jieba/cppjieba/dict/user.dict.utf8";
MixSegment seg(JIEBA_DICT_FILE, HMM_DICT_FILE, USER_DICT_FILE);

//#include "./seg_split.h"


class MySplit
{
    public:
    MySplit()
    {
    }
    ~MySplit()
    {
    }
#if 0
    std::vector<std::string>& wordSplit_unuseful(std::string& txt)
    {
        std::vector<std::string> vec;
        const char* sResult;
        sResult = NLPIR_ParagraphProcess(txt.c_str(), 0);
        std::stringstream ss(sResult);
        std::string word;
        while(ss >> word)
        {
            vec.push_back(word);
        }
        return vec;
    }
#endif
    //std::string wordSplit(std::string str)
    std::vector<std::string> wordSplit(std::string str)
    {
        std::vector<std::string> split_words;
        seg.cut(str, split_words);
        return split_words;
    }
};

#endif
