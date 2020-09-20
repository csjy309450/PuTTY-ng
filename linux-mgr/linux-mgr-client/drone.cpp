#include "drone.h"
#include <cassert>

SysInfoParser::SysInfoParser()
{

}

int SysInfoParser::Exe(const QString & qstr)
{
    if(qstr.size()==0)
    {
        return -1;
    }
    int ret = sscanf(qstr.toStdString().c_str(),"##cpuinfo\nprocessor\t: %u\nmodel name\t: %[^\n]\ncpu MHz\t\t: %lf\nclflush size\t: %u\n"
           , &m_struCpuInfo.processor
           , &m_struCpuInfo.model_name
           , &m_struCpuInfo.cpu_MHz
           , &m_struCpuInfo.clflush_size);
    if(ret==EOF)
    {
        return -1;
    }
    return 0;
}

ProcStatParser::ProcStatParser()
{

}

int ProcStatParser::Exe(const QString & qstr)
{
    if(qstr.size()==0)
    {
        return -1;
    }
    QList<DRONE_PID_STAT> qlProcList;
    int iPosStartLine = 0;
    int iPosEndLine = 0;
    while((iPosEndLine = qstr.indexOf("\n", iPosStartLine))!=-1)
    {
        QString qstrLine = qstr.mid(iPosStartLine, iPosEndLine-iPosStartLine);
        DRONE_PID_STAT struPidStat;
        sscanf(qstrLine.toStdString().c_str(),"%d %[^ ] %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld %ld %ld %llu %lu %ld %[^ ] %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %d %d %u %u %llu %lu %ld %lu %lu %lu %lu %lu %lu %lu %d\n"
               , &struPidStat.Pid
               , &struPidStat.Comm
               , &struPidStat.State
               , &struPidStat.Ppid
               , &struPidStat.Pgrp
               , &struPidStat.Session
               , &struPidStat.tty_nr
               , &struPidStat.tpgid
               , &struPidStat.flags
               , &struPidStat.minflt
               , &struPidStat.cminflt
               , &struPidStat.majflt
               , &struPidStat.cmajflt
               , &struPidStat.utime
               , &struPidStat.stime
               , &struPidStat.cutime
               , &struPidStat.cstime
               , &struPidStat.priority
               , &struPidStat.nice
               , &struPidStat.num_threads
               , &struPidStat.itrealvalue
               , &struPidStat.starttime
               , &struPidStat.vsize
               , &struPidStat.rss
               , &struPidStat.rsslim
               , &struPidStat.startcode
               , &struPidStat.endcode
               , &struPidStat.startstack
               , &struPidStat.kstkesp
               , &struPidStat.kstkeip
               , &struPidStat.signal
               , &struPidStat.blocked
               , &struPidStat.sigignore
               , &struPidStat.sigcatch
               , &struPidStat.wchan
               , &struPidStat.nswap
               , &struPidStat.cnswap
               , &struPidStat.exit_signal
               , &struPidStat.processor
               , &struPidStat.rt_priority
               , &struPidStat.policy
               , &struPidStat.delayacct_blkio_ticks
               , &struPidStat.guest_time
               , &struPidStat.cguest_time
               , &struPidStat.start_data
               , &struPidStat.end_data
               , &struPidStat.start_brk
               , &struPidStat.arg_start
               , &struPidStat.arg_end
               , &struPidStat.env_start
               , &struPidStat.env_end
               , &struPidStat.exit_code);
        qlProcList.push_back(struPidStat);
        iPosStartLine = iPosEndLine+1;
    }
    this->ComputeProcRate(qlProcList,0);
    m_qlProcList = qlProcList;
    return 0;
}

void ProcStatParser::ComputeProcRate(QList<DRONE_PID_STAT> & qlCurrStat, time_t tmCurr)
{
    (void)tmCurr;
    m_qlPidRate.clear();
    int processCpuTime1 = 0, processCpuTime2 = 0;
    for(int i=0;i<qlCurrStat.size();++i)
    {
        processCpuTime1 = 0; processCpuTime2 = 0;
        for(int j=0;j<m_qlProcList.size();++j)
        {
            if(qlCurrStat[i].Pid==m_qlProcList[j].Pid
                    && strcmp(qlCurrStat[i].Comm, m_qlProcList[j].Comm)==0)
            {
                // 说明是同一个进程
                processCpuTime2=m_qlProcList[j].utime
                        +m_qlProcList[j].stime
                        +m_qlProcList[j].cutime
                        +m_qlProcList[j].cstime;
                processCpuTime1=qlCurrStat[i].utime
                        +qlCurrStat[i].stime
                        +qlCurrStat[i].cutime
                        +qlCurrStat[i].cstime;
                break;
            }
        }
        assert(processCpuTime1 >= processCpuTime2);
//        if(processCpuTime1 > processCpuTime2)
//        {
//            printf("");
//        }
        DRONE_PID_RATE pidRate;
        pidRate.deltaProcCpuTime = processCpuTime1 - processCpuTime2;
        m_qlPidRate.push_back(pidRate);
    }
}

StatParser::StatParser()
{
}

int StatParser::Exe(const QString & qstr)
{
    if(qstr.size()==0)
    {
        return -1;
    }

    // get stat
    time_t currTm;
    QList<DRONE_STAT_ITEM> qlCurrStat;

    int iPosStartLine = 0;
    int iPosEndLine = 0;
    QString qstrLine;
    // 获取时间戳
    if(qstr.startsWith("timestam"))
    {
        iPosEndLine = qstr.indexOf("\n", iPosStartLine);
        if(iPosEndLine == -1)
        {
            return -1;
        }
        qstrLine = qstr.mid(iPosStartLine, iPosEndLine-iPosStartLine);
        sscanf(qstrLine.toStdString().c_str(), "timestam\t: %llu\n", &currTm);
        iPosStartLine = iPosEndLine + 1;
    }

    // 获取cpu状态
    while((iPosEndLine = qstr.indexOf("\n", iPosStartLine))!=-1)
    {
        qstrLine = qstr.mid(iPosStartLine, iPosEndLine-iPosStartLine);
        DRONE_STAT_ITEM struItem;
        sscanf(qstrLine.toStdString().c_str(),"%[^ ] %u %u %u %u %u %u %u %u %u %u\n"
               , &struItem.cpu_name
               , &struItem.user
               , &struItem.nice
               , &struItem.system
               , &struItem.idle
               , &struItem.iowait
               , &struItem.irq
               , &struItem.softirq
               , &struItem.steal
               , &struItem.guest
               , &struItem.guest_nice);
        qlCurrStat.push_back(struItem);
        iPosStartLine = iPosEndLine+1;
    }
    this->ComputeSysRate(qlCurrStat, currTm);
    m_qlStat = qlCurrStat;
    return 0;
}

void StatParser::ComputeSysRate(QList<DRONE_STAT_ITEM> & qlCurrStat, time_t tmCurr)
{
    (void)tmCurr;
    if(qlCurrStat.size()!=m_qlStat.size())
    {
        return;
    }
    m_qlSysRate.clear();
    int cpuTotal1=0, cpuUsed1=0, cpuTotal2=0, cpuUsed2=0;
    for(int i=0;i<qlCurrStat.size();++i)
    {
        cpuTotal1=qlCurrStat[i].user
                +qlCurrStat[i].nice
                +qlCurrStat[i].system
                +qlCurrStat[i].idle
                +qlCurrStat[i].iowait
                +qlCurrStat[i].irq
                +qlCurrStat[i].softirq;
        cpuUsed1=qlCurrStat[i].user
                +qlCurrStat[i].nice
                +qlCurrStat[i].system
                +qlCurrStat[i].irq
                +qlCurrStat[i].softirq;
        cpuTotal2=m_qlStat[i].user
                +m_qlStat[i].nice
                +m_qlStat[i].system
                +m_qlStat[i].idle
                +m_qlStat[i].iowait
                +m_qlStat[i].irq
                +m_qlStat[i].softirq;
        cpuUsed2=m_qlStat[i].user
                +m_qlStat[i].nice
                +m_qlStat[i].system
                +m_qlStat[i].irq
                +m_qlStat[i].softirq;
        DRONE_SYS_RATE sysRate;
        sysRate.deltaProcCpuTime = cpuUsed1 - cpuUsed2;
        sysRate.deltaTotalCpuTime = cpuTotal1 - cpuTotal2;
        m_qlSysRate.push_back(sysRate);
    }
}

TopParser::TopParser()
{}

int TopParser::Exe(const QString & qstr)
{
    if(qstr.size()==0)
    {
        return -1;
    }
    int iPosStart = 0;
    int iPosEnd=0;
    QString qstrWork;

    // get stat
    iPosStart = qstr.indexOf("##stat\n");
    if(iPosStart==-1)
    {
        return-1;
    }
    iPosStart = iPosStart+strlen("##stat")+1;
    iPosEnd = qstr.indexOf("##process list\n");
    if(iPosEnd==-1)
    {
        return-1;
    }
    qstrWork = qstr.mid(iPosStart, iPosEnd-iPosStart);
    m_objSataParser.Exe(qstrWork);

    // get proc list
    iPosStart = iPosEnd + strlen("##process list") + 1;
    qstrWork = qstr.mid(iPosStart);
    m_objProcStatParser.Exe(qstrWork);

    return 0;
}
