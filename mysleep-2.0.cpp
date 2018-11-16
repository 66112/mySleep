#include <iostream>
#include <unistd.h>
#include <signal.h>
using namespace std;

void handler(int signo)         //啥也不做
{
   ; 
}

unsigned int mysleep(size_t sec)
{
    sigset_t newmask,oldmask,suspmask;
    struct sigaction act, oact;        //创建两个结构体
    act.sa_handler = handler;       //定义处理方式 sa_handler = void (*handler)(int)
    sigemptyset(&act.sa_mask);      //清空信号集
    sigaction(SIGALRM,&act,&oact);  //收到 SIGALRM 信号,执行handler动作 ，oact传出该信号原来的动作
    //屏蔽14号
    sigemptyset(&newmask);
    sigaddset(&newmask,SIGALRM);
    sigprocmask(SIG_BLOCK,&newmask,&oldmask);
    alarm(sec);          //设置闹钟  
    suspmask = oldmask;
    //取消屏蔽
    sigdelset(&suspmask,SIGALRM);
    //挂起等待14号
    sigsuspend(&suspmask);     //原子操作,临时解除屏蔽，并挂起
    
    unsigned int ret = alarm(0);
    // pause();                        //挂起 
    sigaction(SIGALRM,&oact,NULL);  //恢复默认信号处理动作(以后收到14号信号，则默认退出)

    sigprocmask(SIG_SETMASK,&oldmask,NULL);  //恢复以前的信号屏蔽字
    // cout << "alarm" << endl;
    return ret;
}

int main()
{
    while (1){
        mysleep(1);
        cout << "i am back!!! "<< getpid() << endl;
    }
    return 0;
}
