//c/c++
#include<string.h>
#include<stdio.h>

//linux

//user define
#include"../logger/logger.h"
#include"client.h"
using namespace WSMQ;

//全局变量 
Logger producerLogger;



int main(int argc,char *argv[])
{
    if(InitConf(CONF_FILE_PATH)!=0)
    {
        printf("init conf failed\n");
        return -1;
    }
    char pLogPath[MQ_MAX_PATH_LEN]=DEFAULT_CLI_LOG_PATH;
    if(producerLogger.Init(pLogPath)!=Logger::SUCCESS)
    {
        printf("logger init failed!");
        return -1;
    }
    Producer *pProducter=new Producer();
    if(pProducter->BuildConnection()!=Producer::SUCCESS)
    {
        
        producerLogger.WriteLog(mq_log_err,"Build Connection failed!errMsg is %s",pProducter->GetErrMsg());
    }
    if(pProducter->CreateExchange("exchange1",EXCHANGE_TYPE_FANOUT)!=Producer::SUCCESS)
    {
       producerLogger.WriteLog(mq_log_err, "Create exchange failed!errMsg is %s",pProducter->GetErrMsg());
    }
    if(pProducter->CreateQueue("queue6")!=Producer::SUCCESS)
    {
        producerLogger.WriteLog(mq_log_err, "Create queue failed!errMsg is %s",pProducter->GetErrMsg());
    }
    if(pProducter->CreateQueue("queue7")!=Producer::SUCCESS)
    {
        producerLogger.WriteLog(mq_log_err,"Create queue failed!errMsg is %s",pProducter->GetErrMsg());
    }
    if(pProducter->CreateBinding("exchange1","queue7","mq.normal.test1")!=Producer::SUCCESS)
    {
        producerLogger.WriteLog(mq_log_err,"Create binding failed!errMsg is %s",pProducter->GetErrMsg());
    }
    if(pProducter->CreateBinding("exchange1","queue6","mq.normal.test2")!=Producer::SUCCESS)
    {
        producerLogger.WriteLog(mq_log_err,"Create binding failed!errMsg is %s",pProducter->GetErrMsg());
    }

    int i=1;
    char pVal[100];
    while(i<10000)
    {
        memset(pVal,0,sizeof(pVal));
        sprintf(pVal,"%d",i);
        string strVal=pVal;
        string strMsg="normal producer send msg "+strVal;
        int ret=pProducter->PuslishMessage("exchange1","mq.normal.test",strMsg);
        if(ret!=Producer::SUCCESS)
        {
            break;
        }
        producerLogger.WriteLog(mq_log_info,"send the %d msg:%s\n",i,strMsg.c_str());
        ++i;
    }
    producerLogger.WriteLog(mq_log_err,"Run finished!errMsg is %s",pProducter->GetErrMsg());
    // if(pProducter->DeleteQueue("queue1")!=Client::SUCCESS)
    // {
    //     LOG_INFO(0, 0,"Delete queue failed!errMsg is %s",pProducter->GetErrMsg());
    //     producerLogger.Print(mq_log_err,"Delete queue failed!errMsg is %s",pProducter->GetErrMsg());
    // }
    if(pProducter->DeleteExchange("exchange1")!=Client::SUCCESS)
    {
        producerLogger.WriteLog(mq_log_err,"Delete exchange failed!errMsg is %s",pProducter->GetErrMsg());
    }
    delete pProducter;
    pProducter=NULL;
}