import time

def getTimeRPC():
    xtime = time.localtime(time.time())
    rpc(remoteAddr,'getTime', xtime[0],xtime[1],xtime[2],xtime[3],xtime[4],xtime[5],xtime[6],xtime[7])
    str = "getTime:" + remoteNode.name
    print str
def registerRPC(location):
    rpc(remoteAddr,'setActive')
    str = "Registered: " + remoteNode.name
    print str
def setActive():
    pass
def getTime():
    pass

def podStatus(time,status,location,speed,ticks, destination):
    #print remoteNode.getFormattedNetAddr()
    #remoteNode.getFormattedNetworkId()
    str = remoteNode.name + "="
    str1 ="Time:", time,"Status:",status,"Location:", location,"Speed:", speed,"Ticks:", ticks,"Destination:", destination
    if destination == 0:
        destination = 8
        rpc(remoteAddr,'setDest',destination)
    
    #logData(
    print str,str1
    
def setDest():
    pass