import subprocess
import SocketServer
import signal

modlist = {
        "server":[["python","/home/ubuntu/python/testServer.py"]],
        "streams":[["/home/ubuntu/mjpg-streamer/multistream.sh"]]
}
for i in modlist:
        modlist[i].append(False)

def checkstate(mod):
        if modlist[mod][1]==True:
                if(len(modlist[mod])>=3):
                        if modlist[mod][2].poll()!=None:
                                del modlist[mod][2]
                                modlist[mod][1] = False
                else:
                        modlist[mod][1] = False
        elif(len(modlist[mod])>=3):
                del modlist[mod][2]

def startmod(mod, args):
        if modlist[mod][1]==False:
                s = modlist[mod][0]+args
                p = subprocess.Popen(s)
                modlist[mod].append(p)
                modlist[mod][1] = True

def stopmod(mod):
        if modlist[mod][1]==True:
                modlist[mod][2].send_signal(signal.SIGINT)
                modlist[mod][2].kill()
                del modlist[mod][2]
                modlist[mod][1] = False

class MyTCPHandler(SocketServer.BaseRequestHandler):

        def handle(self):
                delim = '\t\0'#'[/TCP]\0'
                print("connected to {}".format(self.client_address[0]))
                self.data = ""
                while True:
                #while self.data!="close":
                        self.data = self.request.recv(1024).rstrip(delim)
                        print("  recieved: " + '"'+self.data+'"')
                        incmd = self.data.split(" ")
                        for i in modlist:
                                checkstate(i)
                        msg = []
                        if incmd[0] == "close":
                                break
                        elif ((len(incmd)>1)and(incmd[1] in modlist)):
                                if incmd[0] == "start":
                                        if modlist[incmd[1]][1]==False:
                                                if len(incmd)>2: s=incmd[2:]
                                                else: s=[]
                                                startmod(incmd[1], s)
                                                msg.append("Module {} has been started.".format(incmd[1]))
                                        else:
                                                msg.append("Module {} is already running.".format(incmd[1]))
                                elif incmd[0] == "stop":
                                        if modlist[incmd[1]][1]==True:
                                                stopmod(incmd[1])
                                                msg.append("Module {} has been stopped.".format(incmd[1]))
                                        else:
                                                msg.append("Module {} is not running.".format(incmd[1]))
                                elif incmd[0] == "query":
                                        if modlist[incmd[1]][1]==True:
                                                msg.append("Module {} is running.".format(incmd[1]))
                                        else:
                                                msg.append("Module {} is not running.".format(incmd[1]))
                                else:
                                        msg.append("Not a valid command!")
                        elif incmd[0] == "startall":
                                for i in modlist:
                                        if modlist[i][1]==False:
                                                startmod(i, [])
                                                msg.append("Module {} has been started.".format(i))
                                        else:
                                                msg.append("Module {} is already running.".format(i))
                        elif incmd[0] == "stopall":
                                for i in modlist:
                                        if modlist[i][1]==True:
                                                stopmod(i)
                                                msg.append("Module {} has been stopped.".format(i))
                                        else:
                                                msg.append("Module {} is not running.".format(i))
                        elif incmd[0] == "queryall":
                                for i in modlist:
                                        if modlist[i][1]==True:
                                                msg.append("Module {} is running.".format(i))
                                        else:
                                                msg.append("Module {} is not running.".format(i))
                        else:
                                msg.append("Not a valid command!")
                        self.request.sendall("\n".join(msg)+delim)
                print("connection closed")

if __name__ == "__main__":
        HOST, PORT = "192.168.112.101", 8888
        server = SocketServer.TCPServer((HOST, PORT), MyTCPHandler)
        server.serve_forever()