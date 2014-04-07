import wx

class MyFrame(wx.Frame):
    """ We simple derive a new class of Frame. """
    def __init__(self,parent, title):
        wx.Frame.__init__(self, parent, title = title, size = ( 200, 100))
        self.control = wx.TextCtrl(self, style = wx.TE_MULTILINE)
        self.Show(True)
        
app = wx.App(False) #create a new app, don't redirect stdout/stderr to a window
frame = MyFrame(None, 'Small Editor')#top level
app.MainLoop()