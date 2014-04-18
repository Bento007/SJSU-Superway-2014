"""Provide GUI window display for MulticastCounter in Portal
"""
import time
# import sqlite3
# conn = sqlite3.connect('SuperwayDB.db')


    
def getTimeRPC():
    xtime = time.localtime(time.time())
    rpc(remoteAddr,'getTime', xtime[0],xtime[1],xtime[2],xtime[3],xtime[4],xtime[5],xtime[6],xtime[7])
def registerRPC(location):
    rpc(remoteAddr,'setActive')
 
def setActive():
    pass
def getTime():
    pass
     