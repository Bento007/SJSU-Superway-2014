import time

startup =1     #// initializes everything
ready   =2     #// waiting for direction
error   =3     #// error detected or initialize failed
roam    =4     #// no directions received driving around track
pickup  =5     #// directions received going to pick up location
load    =6     #// at stations loading
dropoff =7     #// in route to drop off
unload  =8     #// at destination and unloading passengers
emergency = 9  #// an emergency has occured
    
def getTimeRPC():
    xtime = time.localtime(time.time())
    rpc(remoteAddr,'getTime', xtime[0],xtime[1],xtime[2],xtime[3],xtime[4],xtime[5],xtime[6],xtime[7])
    str0 = remoteNode.name + ": getTime" 
    print str0 #TODO: log this
def registerRPC(location):
    rpc(remoteAddr,'setActive')
    str0 = remoteNode.name +": Registered " +"@Location: " + str(location)
    print str0 #TODO: log this

def podStatus(time,status,location,speed,ticks, destination):
    #print remoteNode.getFormattedNetAddr()
    #remoteNode.getFormattedNetworkId()
    str0 = remoteNode.name + "= "
    str0 = str0 + "Time: "+str(time)
    str0 = str0 +", Status: " + str(status)
    str0 = str0 +", Location: "+ str(location)
    str0 = str0 +", Speed:"+ str(speed)
    str0 = str0 +", Ticks:"+ str(ticks)
    str0 = str0 +", Destination:", str(destination)
    
    #logData(
    print str0 #TODO: log this
    
def setRoute():
    pass
def setActive():
    pass
def getTime():
    pass