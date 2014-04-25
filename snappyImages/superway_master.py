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
    str0 = "getTime:" + remoteNode.name
    print str0 #TODO: log this
def registerRPC(location):
    rpc(remoteAddr,'setActive')
    str0 = "Registered: " + remoteNode.name +" @Location:" + str(location)
    str1 = str(location)
    str2 = str0 + str1
    print str #TODO: log this

def podStatus(time,status,location,speed,ticks, destination):
    #print remoteNode.getFormattedNetAddr()
    #remoteNode.getFormattedNetworkId()
    str0 = remoteNode.name + "="
    str1 ="Time:", time,"Status:",status,"Location:", location,"Speed:", speed,"Ticks:", ticks,"Destination:", destination
    if destination == 0 & status == ready:#TODO when is destination zero other than startup
        #TODO: find a new destination for the pod
        pickup = 8
        dropoff = 1
        rpc(remoteAddr,'setRoute',pickup, dropoff)
    
    #logData(
    print str0,str1 #TODO: log this
    
def setRoute():
    pass
def setActive():
    pass
def getTime():
    pass