import socket
import sys
import select
import threading
import time
import os

ss = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
host = socket.gethostname()
port = 1459
if len(sys.argv)>1:
    if len(sys.argv)==3:
        port = sys.argv[2]
    host = sys.argv[1]

ss.connect((host,port))
l= [sys.stdin, ss]

commands = {
    "/HELP": b'\x00',
    "/LIST": b'\x01',
    "/JOIN": b'\x02',
    "/KICK": b'\x03',
    "/WHO": b'\x04',
    "/MSG": b'\x05',
    "/BYE": b'\x06',
    "/REN": b'\x07',
    "/LEAVE": b'\x08',
    "/CURRENT":b'\x0a',
    "/SEND":b'\x0c',
    "/RECV":b'\x0d',
    "/NICK":b'\x0e',
    "/GRANT":b'\x0f',
    "/REVOKE":b'\x10',
    "/HISTORY":b'\x11',
    "/RECVF":b'\x12'
}

com_code = [b'\x00',b'\x01',b'\x02',b'\x03',b'\x04',b'\x05',b'\x06',b'\x07',b'\x08',b'\x09',b'\x0a',b'\x0b',b'\x0c',b'\x0d',b'\x0e',b'\x0f',b'\x10',b'\x11',b'\x12']
com_len = [6,6,6,6,5,5,5,5,7,0,9,9,6,6,6,7,8,9,7]

def find_command(m):
    """ Return command bytes value given at the beginning of string m. """
    if not ('/') in m[0:1]:
        return b'\x09'
    else :
        c = m.split(" ")
        c = c[0].split('\n')
        c = c[0]
        if c in commands :
            if commands[c] == b'\x0a' and len(m.split(" "))!= 1:
                return b'\x0b'
            return commands[c]
        else:
            return b'\xff'

def send_file(s,path):
    """ Open and send a file loaded from path. """
    print("sending...")
    f = open(path,"rb")
    l = f.read(1024)
    while (l):
        s.sendall(l)
        l = f.read(1024)
    s.sendall(b"\xab\xfb")
    print("done\n")
    f.close()
     
	
def recv_file(s,path):
    """ Recive and save a file in path. """
    print("downloading...")
    f = open(path,"wb")
    s.send(b'\xf5')
    l = s.recv(1024)
    while (not b"\xab\xfb" in l[0:3] and l!=b''):
        f.write(l)
        l = s.recv(1024)
    f.close()
    print("done\n")

def get_args(m):
    """ Return list of string that were seperated by spaces in m. """
    m = m.replace('\n','')
    l = m.split(' ')
    return l


while True:
   l_s, a, b = select.select(l, [], [])
   for s in l_s:
       if s == sys.stdin:
           m = s.readline()
           if len(m)==0:
               ss.close()
               print("\nDisconnected from server\n")
               sys.exit(0)
           com = find_command(m)
           if com==b'\xff':
               print("\nInvalid command.\n")
               break
           m = m[com_len[com_code.index(com)]:len(m)]
           if com==b'\x0c':  
               l_arg = get_args(m)
               if not os.path.isfile(l_arg[1]) :
                   print("\nInvalid file !\n")
                   break
               data = com + l_arg[0].encode()
               ss.send(data)
               time.sleep(0.5)
               res = ss.recv(1024)
               print("Waiting confirmation..")
               if b'\x00' in res[0:2] :
                   send_file(ss,l_arg[1])
               else :
                   print("\nSorry, you can't do that !\n")
           elif com==b'\x0d' :
                l_arg = get_args(m)
                data = com
                ss.send(data)
                print("Waiting confirmation..")
                time.sleep(0.5)
                res = ss.recv(1024)
                if b'\x00' in res[0:2] :
                    recv_file(ss,l_arg[0])
                elif b'\x01' in res[0:2] :
                    print("MEE6 : No file waiting for you !\n")
                else :
                    print("MEE6 : Sorry, but you can't do that here !\n")
           elif com==b'\x12':
                print("\nWarning : If you use /RECVF, you won't be able to do anything for a couple seconds or\nuntil you receive your file.")
                print("\nProceed anyway ? (y or n) : ")
                c = input()
                while not 'y' in c[0:1] and not 'n' in c[0:1]:    
                    print("\nPlease answer with 'y' or 'n' : ")
                    c = input()
                if 'n' in c[0:1] :
                    break
                l_arg = get_args(m)
                data = b'\x0d'
                loop_count = 0
                ss.send(data)
                print("Waiting confirmation..")
                time.sleep(0.5)
                res = ss.recv(1024)
                while not b'\x00' in res[0:2] and not b'\xf0' in res[0:2]:
                    ss.send(data)
                    time.sleep(0.2)
                    res = ss.recv(1024)
                    loop_count+=1
                    if loop_count>50 :
                        res = b'\xf1'
                        break
                if b'\x00' in res[0:1] :
                    recv_file(ss,l_arg[0])
                elif b'\xf1' in res[0:1] :
                    print("\nDemand Timeout : client waited too long for file and has been terminated.\n")
                else :
                    print("MEE6 : Sorry, but you can't do that here !")
           else:
               data = com + m.encode()
               ss.send(data)
       else:
           m = s.recv(1024)
           if len(m)==0:
               ss.close()
               print("\nDisconnected from server\n")
               sys.exit(0)
           print(m.decode("utf-8"))
