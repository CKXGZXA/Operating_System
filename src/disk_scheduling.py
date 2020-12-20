from collections import OrderedDict
from ast import literal_eval

# 磁盘访问请求序列
request_sequense = [55,58,39,18,90,160,150,38,184]
correlation_table = {"FCFS":{"总寻道长度":0,"平均寻道长度":0},"SSTF":{"总寻道长度":0,"平均寻道长度":0},"SCAN":{"总寻道长度":0,"平均寻道长度":0},"CSCAN":{"总寻道长度":0,"平均寻道长度":0}}

def print_sequece(sch, begin = 100):
    """
    用以输出调度序列
    """
    print("从" + str(begin) + "号磁道开始")
    print("被访问的下一个磁道号 移动距离(磁道数)")
    for key, value in sch.items():
        print(key,value)
    sum = 0
    for value in sch.values():
        sum += value
    print("总寻道长度:" + str(sum))
    print("平均寻道长度:" + "{:.1f}".format(sum/len(sch)))
    return {"总寻道长度":sum,"平均寻道长度":sum/len(sch)}

def find_next(list, dict,begin):
    """
    找到下一个距离最近的磁道号
    传入参数分别为请求序列,调度序列字典,磁头当前位置
    """
    min = float('inf')
    for i in range(0, len(list)):
        if list[i] not in dict and abs(list[i] - begin) < abs(min - begin):
            min = list[i]
    return min
  
def sequential_scan(request,begin,index = 0,op = 1):
    """
    按某规则扫描序列,
    如果op为1,则从index顺序扫描,op为0则从index逆序扫描,op为2时从0扫描到index
    返回添加了扫描项和寻道时间键值对的字典
    """
    t1 = begin
    t2 = 0
    dict = OrderedDict()
    if 1 == op:
        for i in range(index, len(request)):
            t2 = abs(request[i] - t1)
            t1 = request[i]
            dict[t1] = t2
    elif 0 == op:
        for i in range(index)[::-1]:
            t2 = abs(request[i] - t1)
            t1 = request[i]
            dict[t1] = t2
    elif 2 == op:
        for i in range(index):
            t2 = abs(request[i] - t1)
            t1 = request[i]
            dict[t1] = t2        
    return dict        

def print_table():
    """
    打印对比表
    """
    print("算法 " + "总寻道长度 " + "平均寻道长度")
    for key,value in correlation_table.items():
        print("{} {:^10} {:^10.1f}".format(key,value["总寻道长度"],value["平均寻道长度"]))
        
def FCFS(begin = 100):
    """
    先来先服务(FIFO), 传入参数为开始磁道号, 默认为100
    """
    t1 = begin
    t2 = 0
    sch_pro = OrderedDict()
    for i in range(len(request_sequense)):
        t2 = abs(request_sequense[i] - t1)
        t1 = request_sequense[i]
        sch_pro[t1] = t2
    correlation_table["FCFS"] = print_sequece(sch_pro,begin)

def SSTF(begin = 100):
    """
    最短寻道时间优先(SSTF), 传入参数为开始磁道号, 默认为100    
    """
    t1 = begin
    t2 = 0
    sch_pro = OrderedDict()
    for i in range(len(request_sequense)):
        t2 = abs(find_next(request_sequense, sch_pro, t1) - t1)
        t1 = find_next(request_sequense, sch_pro, t1)
        sch_pro[t1] = t2
    correlation_table["SSTF"] = print_sequece(sch_pro, begin)

def SCAN(begin = 100):
    """
    扫描算法(SCAN),电梯调度算法, 磁头开始位置默认传入为100
    """
    sorted_sequence = sorted(request_sequense)
    sch_dict = OrderedDict()
    index = 0
    for index in range(len(request_sequense)):
        if request_sequense[index] == begin:
            break
        elif request_sequense[index] > begin:
            index += 1
            break
    if index == 0:
        correlation_table["SCAN"] = print_sequece(sequential_scan(sorted_sequence,begin,index,1), begin)
    elif index == len(request_sequense) - 1:
        correlation_table["SCAN"] = print_sequece(sequential_scan(sorted_sequence,begin,index,0), begin)
    else:
            sch_dict.update(sequential_scan(sorted_sequence,begin,index,1)) 
            sch_dict.update(sequential_scan(sorted_sequence,list(sch_dict.keys())[-1],index,0))
            correlation_table["SCAN"] = print_sequece(sch_dict,begin)
         
def CSCAN(begin = 100):
    """
    循环扫描(CSCAN)算法,默认传入参数为磁头开始位置
    """
    index = 0
    sch_dict = OrderedDict()
    sorted_sequence = sorted(request_sequense)
    for index in range(len(request_sequense)):
        if request_sequense[index] == begin:
            break
        elif request_sequense[index] > begin:
            index += 1
            break
    sch_dict.update(sequential_scan(sorted_sequence,begin,index,1)) 
    sch_dict.update(sequential_scan(sorted_sequence,list(sch_dict.keys())[-1],index,2))
    correlation_table["CSCAN"] = print_sequece(sch_dict,begin)

def print_menu():
    """
    打印菜单
    """
    print("=" * 33)
    print("0.  退出")
    print("1.  FCFS算法实现")
    print("2.  SSTF算法实现")
    print("3.  SCAN算法实现")
    print("4.  CSCAN算法")
    print("5.  显示对比表")
    print("=" * 33)


def main():
    """
    程序入口
    """
    print("*" * 13 + "磁盘调度算法实现" + "*" * 13)
    print_menu()
    choice = int(input("请输入你的选择:"))
    while 0 != choice:
        if 1 == choice:
            print("=" * 15 + "FCFS" + "=" * 15)
            FCFS()
            print("=" * 34)
        elif 2 == choice:
            print("=" * 15 + "SSTF" + "=" * 15)
            SSTF()
            print("=" * 34)
        elif 3 == choice:
            print("=" * 15 + "SCAN" + "=" * 15)
            SCAN()
            print("=" * 34)
        elif 4 == choice:
            print("=" * 15 + "CSCAN" + "=" * 14)
            CSCAN()
            print("=" * 34)
        elif 5 == choice:
            print_table()
        print_menu()
        choice = int(input("请输入你的选择:"))        


main()