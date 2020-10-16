# Program: Round Robin Sim
# Description:
#   It is a program that simulates the round robin scheduler given a range 
#   of time quanta. At the end it plots a graph showing the difference in
#   the average turnaround time it takes given a certian time quanta
# By: Luis Fernando Lomelín Ibarra A01177015

#Link to an example result graph: shorturl.at/gpuHS 
import matplotlib.pyplot as plt

#Define the context switch cost
cscost = 2
#Define Quantum Time values to be used
stqt = int(input("What is the first Quantum Time (qt) to use\n>> "))
enqt = int(input("What will be the last quantum time to use:\n>> "))
qtused = []

if stqt>enqt:
    aux=stqt
    stqt=enqt
    enqt=aux

#Define the array hold the average tat
avgtatused =[] 
#Get the processes
giveList = input("Want to give a list of processes (y/n)\n>> ")

#Aux variables
i = 0
exectime=0
tat = 0
ctswitch = 0
misstime = 0
procescount = 0
# Decide if get process from user or use a static set
if giveList == "y":
    processList = list(map(int,input("List the values (ex. 2 3 4)\n>> ").split()))
else:
    processList = [2,12,8,1,6,5,13]
print("This is the processes to be used:")
print(processList)
processcount = len(processList)
#Simulate round robin in the given qt time given
while stqt<=enqt:
    print("-----------Start calulation with qt = "+str(stqt)+"--------------\n")
    qtused.append(stqt)
    process = processList[:]
    ctswitch = 0
    exectime = 0
    tat = 0
    
    while process:
        #Check if is necesary to do context switch; True = process finished, false= return to queue
        if process[0] <= stqt:
            exectime+= process[0]
            process.pop(0)
        else:
            misstime = process[0] - stqt
            ctswitch+=1
            exectime+=stqt
            process.pop(0)
            process.append(misstime)
            #exectime+=cscost
            print("\nState of the queue after context switch:")
            print(process)
    exectime+=cscost*ctswitch
    tat = float(exectime)/float(processcount)
    avgtatused.append(tat)
    print("In the quantum time: "+str(stqt)+" the avgtat was: "+str(tat))
    print("\n------------------------------------------------------\n")
    stqt+=1
print("Quantum times used")
print(qtused)
print("Average Turn Around Times")
print(avgtatused)
if len(qtused)<=1:
    print("No plot generated given that there is only one quantum time to analyze")
else:
    plt.plot(qtused,avgtatused)
    plt.xlabel('Quantum Value')
    plt.ylabel('Average Turn Around Time')
    plt.show(block=True)
