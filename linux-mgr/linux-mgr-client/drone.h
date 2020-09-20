#ifndef DRONE_H
#define DRONE_H

#include <ctime>
#include <QString>
#include <QList>
#include "DroneClient.h"

/**
  * @brief "/proc/[pid]/stat" data
  */
typedef struct tagDRONE_PID_STAT
{
    int Pid; ///< %d  ：进程ID号；
    char Comm[128]; ///< %[^ ] ：括号内的是可执行文件的文件名；
    char State; ///< %c :  进程状态；(running  sleeping  waiting ...)
    int Ppid; ///< %d : 父进程PID号；
    int Pgrp; ///< %d :j进程的进程组ID；
    int Session; ///< %d :  进程的会话ID ；
    int tty_nr; ///< %d :  进程的控制终端（次设备号包含在31-20和7-0的位，主设备号是15-8位）；
    int tpgid; ///< %d： 终端的进程组的ID ；
    unsigned int flags; ///< %u ： 进程的标志字；
    unsigned long minflt; ///< %lu： 该任务不需要从硬盘拷数据而发生的缺页（次缺页）的次数；
    unsigned long cminflt; ///< %lu：累计的该任务的所有的waited-for进程曾经发生的次缺页的次数目；
    unsigned long majflt; ///< %lu：该任务需要从硬盘拷数据而发生的缺页（主缺页）的次数；
    unsigned long cmajflt; ///< %lu　：累计的该任务的所有的waited-for进程曾经发生的主缺页的次数目；
    unsigned long utime; ///< %lu：该任务在用户态运行的时间，单位为jiffies；
    unsigned long stime; ///< %lu　：该任务在内核态运行的时间，单位为jiffies；
    long cutime; ///< %ld：累计的该任务的所有的waited-for进程曾经在用户态运行的时间，单位为jiffies；
    long cstime; ///< %ld：累计的该任务的所有的waited-for进程曾经在内核态运行的时间，单位为jiffies；
    long priority; ///< %ld：任务的动态优先级；
    long nice; ///< %ld：任务的静态优先级；
    long num_threads; ///< %ld：该进程中线程的个数；
    long itrealvalue; ///< %ld：由于计时间隔导致的下一个 SIGALRM 发送进程的时延，以 jiffy 为单位.
    unsigned long long starttime; ///< %llu：该任务启动的时间，单位为jiffies；
    unsigned long vsize; ///< %lu：该任务的虚拟地址空间大小；
    long rss; ///< %ld：该任务当前驻留物理地址空间的大小；
    char rsslim[64]; ///< %[^ ]：该任务能驻留物理地址空间的最大值；
    unsigned long startcode; ///< %lu  [PT]：该任务在虚拟地址空间的代码段的起始地址；
    unsigned long endcode; ///< %lu  [PT]：该任务在虚拟地址空间的代码段的结束地址；
    unsigned long startstack; ///< %lu  [PT]：该任务在虚拟地址空间的栈的起始地址；
    unsigned long kstkesp; ///< %lu  [PT]：esp(32 位堆栈指针) 的当前值, 与在进程的内核堆栈页得到的一致；
    unsigned long kstkeip; ///< %lu  [PT]：指向将要执行的指令的指针, EIP(32 位指令指针)的当前值
    unsigned long signal; ///< %lu：待处理信号的位图，记录发送给进程的普通信号；
    unsigned long blocked; ///< %lu：阻塞信号的位图；
    unsigned long sigignore; ///< %lu：忽略的信号的位图；
    unsigned long sigcatch; ///< %lu：被俘获的信号的位图；
    unsigned long wchan; ///< %lu  [PT]：如果该进程是睡眠状态，该值给出调度的调用点；
    unsigned long nswap; ///< %lu：被swapped的页数，当前没用上；
    unsigned long cnswap; ///< %lu：所有子进程被swapped的页数的和，当前没用；
    int exit_signal; ///< %d (since Linux 2.1.22）：该进程结束时，向父进程所发送的信号；
    int processor; ///< %d  (since Linux 2.2.8)：最后运行在哪个CPU上；
    unsigned int rt_priority; ///< %u  (since Linux 2.5.19)：实时进程的相对优先级别；
    unsigned int policy; ///< %u  (since Linux 2.5.19)：进程的调度策略，0=非实时进程，1=FIFO实时进程；2=RR实时进程 ；
    unsigned long long delayacct_blkio_ticks; ///< %llu  (since Linux 2.6.18)：聚合块I/O延迟；
    unsigned int guest_time; ///< %lu  (since Linux 2.6.24)：该进程的访客时间（用于运行访客操作系统的VIR实际CPU的时间）；
    long cguest_time; ///< %ld  (since Linux 2.6.24)：该进程中子进程的访客时间；
    unsigned long start_data; ///< %lu  (since Linux 3.3)  [PT]：程序初始化和未初始化（BSS）数据的起始地址。
    unsigned long end_data; ///< %lu  (since Linux 3.3)  [PT]：程序初始化和UNIN化（BSS）数据的结束地址；
    unsigned long start_brk; ///< %lu  (since Linux 3.3)  [PT]：BRK（2）扩展程序堆的起始地址；
    unsigned long arg_start; ///< %lu  (since Linux 3.5)  [PT]：程序命令行参数（ARGV）的起始地址；
    unsigned long arg_end; ///< %lu  (since Linux 3.5)  [PT]：程序命令行参数（ARGV）的结束地址；
    unsigned long env_start; ///< %lu  (since Linux 3.5)  [PT]：程序环境的起始地址；
    unsigned long env_end; ///< %lu  (since Linux 3.5)  [PT]：程序环境的结束地址；
    int exit_code; ///< %d  (since Linux 3.5)  [PT]：线程在WITPID（2）报告的窗体中的退出状态；
}DRONE_PID_STAT, *LPDRONE_PID_STAT;

typedef struct tagDRONE_CPU_INFO
{
    unsigned int processor; ///< %u, 2
    char model_name[128]; ///< Intel(R) Core(TM) i5-4590 CPU @ 3.30GHz
    double cpu_MHz; ///< 3301.000
    unsigned int clflush_size; ///< 64
}DRONE_CPU_INFO, *LPDRONE_CPU_INFO;

typedef struct tagDRONE_STAT_ITEM
{
    char cpu_name[12];
    unsigned int user;
    unsigned int nice;
    unsigned int system;
    unsigned int idle;
    unsigned int iowait;
    unsigned int irq;
    unsigned int softirq;
    unsigned int steal;
    unsigned int guest;
    unsigned int guest_nice;
}DRONE_STAT_ITEM, *LPDRONE_STAT_ITEM;

typedef struct tagDRONE_PID_RATE
{
    int deltaProcCpuTime;
}DRONE_PID_RATE, *LPDRONE_PID_RATE;

typedef struct tagDRONE_SYS_RATE
{
    int deltaProcCpuTime;
    int deltaTotalCpuTime;
}DRONE_SYS_RATE, *LPDRONE_SYS_RATE;

class SysInfoParser
{
public:
    SysInfoParser();
    int Exe(const QString & qstr);
public:
    DRONE_CPU_INFO m_struCpuInfo;
};

class ProcStatParser
{
public:
    ProcStatParser();
    int Exe(const QString & qstr);
    void ComputeProcRate(QList<DRONE_PID_STAT> & qlCurrStat, time_t tmCurr);
public:
    time_t m_tm;
    QList<DRONE_PID_STAT> m_qlProcList;
    QList<DRONE_PID_RATE> m_qlPidRate;
};

class StatParser
{
public:
    StatParser();
    int Exe(const QString & qstr);
    void ComputeSysRate(QList<DRONE_STAT_ITEM> & qlCurrStat, time_t tmCurr);

public:
    time_t m_tm;
    QList<DRONE_STAT_ITEM> m_qlStat;
    QList<DRONE_SYS_RATE> m_qlSysRate;
};

class TopParser
{
public:
    TopParser();
    int Exe(const QString & qstr);
public:
    StatParser m_objSataParser;
    ProcStatParser m_objProcStatParser;
};

#endif // DRONE_H
