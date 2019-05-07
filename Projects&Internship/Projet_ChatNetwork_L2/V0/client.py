import socket
import sys
import select
import threading
import time

ss = socket.socket(socket.AF_INET6,socket.SOCK_STREAM)
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
}

com_code = [b'\x00',b'\x01',b'\x02',b'\x03',b'\x04',b'\x05',b'\x06',b'\x07',b'\x08',b'\x09']
com_len = [6,6,6,6,5,5,5,5,7,0]

def find_command(m):
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

def get_args(m):
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
               print("\nDisconnected from serveur\n")
               sys.exit(0)
           com = find_command(m)
           if com==b'\xff':
               print("\nInvalid command.\n")
               break
           m = m[com_len[com_code.index(com)]:len(m)]
           if com==b'\x0c':  
               l_arg = get_args(m)
               data = com + l_arg[0].encode()
               ss.send(data)
               time.sleep(0.5)
               res = ss.recv(1024)
               print("Waiting confirmation..")
               if b'\x00' in res :
                   send_file(ss,l_arg[1])
               else :
                   print("\nSorry, you can't do that !\n")
           elif com==b'\x0d':
                l_arg = get_args(m)
                data = com
                ss.send(data)
                recv_file(ss,l_arg[0])
           else:
               data = com + m.encode()
               ss.send(data)
       else:
           m = s.recv(1024)
           if len(m)==0:
               ss.close()
               print("\nDisconnected from serveur\n")
               sys.exit(0)
           print(m.decode("utf-8"))
