import sys
import socket
import select
import threading
import time
import tempfile
import string
import random
import os

####################### CLASS ####################### 
class Serveur():
    def __init__(self,port=1459):
        self.socket = socket.socket(socket.AF_INET6,socket.SOCK_STREAM)
        self.socket.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
        if len(sys.argv)!=1:
            port = sys.argv[1]
        self.socket.bind(('',port))
        self.socket.listen(1)

class Client():
    def __init__(self,socket,nickname):
        self.sock = socket
        self.nick = nickname
        self.channels = []
        self.current = ""
        self.admin = []
        nick_to_sock[nickname] = socket
    
    def is_admin(self,chan):
        return chan in self.admin 
    
    def is_active(self,chan):
        return self.current == chan

####################### GLOBAL VARIABLE ####################### 

serveur = Serveur()                   
l = [serveur.socket]
              
sock_to_client = {}           # Dictionary giving Client class from socket         
nick_to_sock = {}             # Gives user's socket from nickname 
d_channel = {}                # Gives list of client from channel's name
file_queue = {}               # Gives file name that was sent to a specific client
archieve={}              # Gives channel chat history from channel name

####################### COMMANDS #######################

def help(s,m):
    """ Show all commands to the client. """
    s.send(b'\n* /HELP: print this message\n')
    s.send(b'* /LIST: list all available channels on server\n')
    s.send(b'* /JOIN <channel>: join (or create) a channel\n')
    s.send(b'* /LEAVE: leave current channel\n')
    s.send(b'* /WHO: list users in current channel\n')
    s.send(b'* <message>: send a message in current channel\n')    
    s.send(b'* /MSG <nick1;nick2;...> <message>: send a private message to several users in current channel\n') 
    s.send(b'* /BYE: disconnect from server\n') 
    s.send(b'* /KICK <nick>: kick user from current channel [admin]\n') 
    s.send(b'* /REN <channel>: change the current channel name [admin]\n')
    s.send(b'* /CURRENT: print current channel name\n')
    s.send(b'* /CURRENT <channel>: set current channel\n')
    s.send(b'* /NICK <nick>: change user nickname on server\n')
    s.send(b'* /GRANT <nick>: grant admin privileges to a user [admin]\n')
    s.send(b'* /REVOKE <nick>: revoke admin privileges [admin]\n')
    s.send(b'* /SEND <nick> </path/to/file>: send a file to a remote user\n')
    s.send(b'* /RECV </path/to/file>: receive a file and save it locally\n')
    s.send(b'* /HISTORY: print history of current channel (saved by server)\n')

    s.send(b'\nNote : Nicknames and Channels cannot contain spaces in their names !\nIf you enter a channel with spaces, they will be replaced with \'_\'.\n') 

def bye(s,m):
    """ Close the socket 's' and remove the client from server memory. """
    if s in sock_to_client and sock_to_client[s].channels==[]:
        c = sock_to_client[s]
        nick_to_sock.pop(c.nick)
        sock_to_client.pop(s)
        s.close()
        l.remove(s)
    elif s in sock_to_client and sock_to_client[s].channels!=[]:
        usage(s)
    else :
        s.close()
        l.remove(s)

def plist(s,m):
    """ Show all available channels. """
    s.send(b'MEE6 : Here is the list of all channels:\n')
    if(d_channel == {}):
        s.send(b'No active channel right now !\n')
    else:
        for chan in d_channel: 
            s.send(chan.encode()+b"\n")
    
def who(s,m):
    """ Show all members of the client's channel. """
    if s in sock_to_client and sock_to_client[s].current!="":
        client_channel = sock_to_client[s].current
        s.send(b'MEE6 : Here is the list of all clients in '+ client_channel.encode() +b':\n')
        for c in d_channel[client_channel]:
            if c.is_admin(client_channel) :
                s.send(b"@"+c.nick.encode() + b"@\n")    
            else:
                s.send(b""+c.nick.encode() + b"\n")
    else :
        usage(s)

def join(s,m):
    """ Add client to the channel given in argument inside 'm'."""
    if s in sock_to_client :  
        c = sock_to_client[s]  
        channel = m.replace('\n','')
        channel = channel.replace(' ','_')
        if channel=='':
            s.send(b"MEE6 : Hey! Why would you do that ?\n")
        if channel in c.channels:
            s.send(b"MEE6 : You have already joined "+channel.encode()+ b", if you wish to change channel, use /CURRENT <channel> command.\n")
            return
        c.channels.append(channel)
        c.current = channel
        if channel in d_channel:
            d_channel[channel].append(c)
            s.send(b"MEE6 <"+channel.encode()+ b"> : You joined the channel, welcome !\n")
        else :
            d_channel[channel] = [c]
            c.admin.append(channel)
            s.send(b"MEE6 : You have created '"+ channel.encode() +b"' and have admin rights to this channel.\n")
    else :
        usage(s)

def leave(s,m):
    """ Disconnect client from its channel. """
    if s in sock_to_client and sock_to_client[s].current!="":
        c = sock_to_client[s]
        chan = c.current
        d_channel[chan].remove(c)
        c.channels.remove(chan)
        if c.is_admin(chan) :
            c.admin.remove(chan)
            if d_channel[chan] == []:
                d_channel.pop(chan)
                archieve.pop(chan)
            else :
                if no_admin(chan):
                    d_channel[chan][0].admin.append(chan)
        if c.channels == []:
            c.current = ""
            s.send(b"MEE6 :You left the channel "+chan.encode()+b", you aren't part of any other channel for now.\n")
        else:
            c.current = c.channels[0]
            s.send(b"MEE6 :You left the channel "+chan.encode()+b" and were moved to "+ c.current.encode()+b".\n")

    else :
        usage(s)

def msg(s,m):
    """ Send private message to the client given in argument. """
    if s in sock_to_client and sock_to_client[s].current!="":
        c = sock_to_client[s]
        c_list, msg = get_msg_arg(m)
        for c2 in c_list:
            if c2 in nick_to_sock:    
                toyou = sock_to_client[nick_to_sock[c2]]
                if toyou in d_channel[c.current] :
                    toyou.sock.send(c.nick.encode()+b" whispers: "+ msg.encode() +b"\n")
        s.send(b"You whispered : "+msg.encode()+b"\n")
    else :
        usage(s)

def kick(s,m):
    """ Kick a client if user is admin. """
    if s in sock_to_client and sock_to_client[s].current!="":
        c = sock_to_client[s]
        if not c.is_admin(c.current):
            s.send(b'MEE6 : You do not have admin rights in this channel !\n')
            return
        c2 = m[0:len(m)-1]
        if c2 in nick_to_sock:
            if c2 == c.nick:
                s.send(b"MEE6 : Don't be silly ! Just use /LEAVE\n")
                return
            kicked = sock_to_client[nick_to_sock[c2]]
            if kicked in d_channel[c.current] :
                kicked.sock.send(b"MEE6 : You were kicked by the admin, be nicer next time !\n")
                d_channel[kicked.current].remove(kicked)
                kicked.channels.remove(c.current)
                if kicked.channels == []:
                    kicked.current = ""
                    kicked.sock.send(b"MEE6 : You no longer belong in any channel.\n")
                else :
                    kicked.current = kicked.channels[0]
                    kicked.sock.send(b"MEE6 : You were moved to "+kicked.current.encode()+b".\n")
            else :
                s.send(b"MEE6 : This client is not in your channel.\n")
        else:
            s.send(b'MEE6 : Unvalid username ! Are you sure this client exist ?\n')
    else :
        usage(s)

def ren(s,m):
    """ Rename current channel if user is admin. """
    if s in sock_to_client and sock_to_client[s].current!="":
        c = sock_to_client[s]
        if not c.is_admin(c.current):
            s.send(b'MEE6 : You do not have admin rights in this channel !\n')
            return
        new = m.replace('\n','')
        new = new.replace(' ','_')
        if not new in d_channel:
            old = c.current
            d_channel[new] = d_channel[old]
            d_channel.pop(old)
            for c2 in d_channel[new]:
                c2.current = new
                c2.channels[c2.channels.index(old)] = new
                if c2.is_admin(old):
                    c2.admin[c2.admin.index(old)] = new
            send_message_channel(c,"The channel "+old+" has been renamed "+new+" by the admin.",True)
    else :
        usage(s)

def current(s,m):
    """ Send the name of the current channel to the user """
    if s in sock_to_client:
        chan = sock_to_client[s].current
        if chan =="":
            s.send(b"MEE6 : You aren't in any channel right now.\n")
        else :
            s.send(b"MEE6 : You are in '"+chan.encode()+b"'.\n")
    else:
        usage(s)

def change_current(s,m):
    """ Change the current channel of the client. """
    if s in sock_to_client:
        c = sock_to_client[s]
        new = get_arg(m)
        if c.current=="":
            s.send(b"MEE6 : You aren't in any channel right now.\n")
        else:
            if new in d_channel and new in c.channels:
                c.current = new
                s.send(b"MEE6 : You are now in '"+new.encode()+b"'.\n")
            else:
                s.send(b"MEE6 : You're not a member of "+new.encode()+b" yet, use /JOIN to join or create this channel.\n")
    else:
        usage(s)

def send_message_channel(client,message,bot=False):
    """ Send the string 'message' to all members of the client's channel. """
    if bot :
        data = b"MEE6 <"+client.current.encode()+b"> : " + message.encode() + b"\n"
    else :
        data = client.nick.encode() + b" <"+client.current.encode()+b"> : " + message.encode() + b"\n"
    if client.current in archieve:
        archieve[client.current] = archieve[client.current] + data
    else :
        archieve[client.current]=data
    for c in d_channel[client.current]:
        if c.is_active(client.current):
            c.sock.send(data)

def change_nick(s,m):
    """ Change the nickname of the client. """
    if s in sock_to_client:
        c = sock_to_client[s]
        old_nick = c.nick
        new_nick = get_arg(m)
        if not new_nick in nick_to_sock:
            c.nick = new_nick
            nick_to_sock[c.nick] = s
            nick_to_sock.pop(old_nick)
            s.send(b"MEE6 : Your name is now "+new_nick.encode()+b", great name !\n")
        else:
            s.send(b'MEE6 : Sorry, but this nickname is already taken. Try another one.\n')
    else:
        usage(s)

def grant(s,m):
    """ Grant admin rights to another user given in arguments. """
    if s in sock_to_client:
        c = sock_to_client[s]
        if c.is_admin(c.current):
            nick2 = get_arg(m)
            if nick2 in nick_to_sock:
                s_tmp = nick_to_sock[nick2]
                c2 = sock_to_client[s_tmp]
                if c2 in d_channel[c.current]:
                    if not c2.is_admin(c.current):
                        c2.admin = c2.admin + [c.current]
                        s_tmp.send(b"MEE6 <"+c.current.encode()+b"> : "+c.nick.encode()+b" granted you admin rights.\n")
                    else:
                        s.send(b"MEE6 : Sorry, but "+nick2.encode()+b" is already an admin!.\n")
                else:
                    s.send(b"MEE6 : Sorry, but "+nick2.encode()+b" is not in this channel!\n")
            else:
                s.send(b"MEE6 : Sorry, but "+nick2.encode()+b" has not been found!\n")
        else:
            s.send(b'MEE6 : Sorry, but you need admin rights to do this command!.\n')
    else:
        usage(s)

def revoke(s,m):
    """ Revoke admin rights from another user given in arguments. """
    if s in sock_to_client:
        c = sock_to_client[s]
        if c.is_admin(c.current):
            nick2 = get_arg(m)
            if nick2 in nick_to_sock:
                s_tmp = nick_to_sock[nick2]
                c2 = sock_to_client[s_tmp]
                if c2 in d_channel[c.current]:
                    if c2.is_admin(c.current):
                        (c2.admin).remove(c.current)
                        s_tmp.send(b"MEE6 <"+c.current.encode()+ b"> : "+c.nick.encode()+ b" revoked your admin rights.\n")
                    else:
                        s.send(b"MEE6 : Sorry, but "+nick2.encode()+ b" is not an admin!.\n")
                else:
                    s.send(b"MEE6 : Sorry, but "+nick2.encode()+ b" is not in this channel!\n")
            else:
                s.send(b"MEE6 : Sorry, but "+nick2.encode()+ b" has not been found!\n")
        else:
            s.send(b'MEE6 : Sorry, but you need admin rights to do this command!.\n')
    else:
        usage(s)

def no_command(s,m):
    """ Determine if the message is for the nickname or a regulare message, and acte upon this information. """
    if not s in sock_to_client :
        nick = m.replace('\n','')
        nick = nick.replace(' ','_')
        if not nick in nick_to_sock:
            sock_to_client[s] = Client(s,nick)
            s.send(b'MEE6 : Nice to meet you '+nick.encode()+b'. Go ahead and create or join a channel.\n' )
            plist(s,m)
        else:
            s.send(b'MEE6 : Sorry, but this nickname is already taken. Try another one.')
    elif sock_to_client[s].current!="" :
        send_message_channel(sock_to_client[s],m)
    else :
        usage(s)

def send_file(s,m):
    """ Send a file to another user, path and name given in argument. """
    if s in sock_to_client and sock_to_client[s].current!="":
        c = sock_to_client[s]
        rec = get_arg(m)
        if rec in nick_to_sock and sock_to_client[nick_to_sock[rec]].is_active(c.current):
            fname = rand_name()+".tmp"
            f = open(fname,"wb")  
            s.send(b'\x00')            
            data = s.recv(1024)
            while (not b"\xab\xfb" in data[0:3]):
                f.write(data)
                data = s.recv(1024)
            if rec in file_queue:
                file_queue[rec].append(fname)
            else :
                file_queue[rec] = [fname]
            nick_to_sock[rec].send(b"MEE6 : "+c.nick.encode()+ b" sent you a file, use /RECV to download it.\n")
            f.close()
        else:
            s.send(b'\xf0')
    else:
        s.send(b'\xf1')

def recv_file(s,m):
    """ Recive a file that has been sent to the client. """
    if s in sock_to_client and sock_to_client[s].current!="":
        c = sock_to_client[s]
        if c.nick in file_queue:
            fname = file_queue[c.nick][0]
            f = open(fname,"rb")
            s.send(b'\x00')
            data = f.read(1024)
            while (data):
                s.send(data)
                data = f.read(1024)
            s.send(b"\xab\xfb")
            file_queue[c.nick].pop(0)
            if len(file_queue[c.nick])==0:
                file_queue.pop(c.nick)            
            f.close()
            os.remove(fname)
        else :
            s.send(b'\x01')
            
def history(s,m):
    """ Send history of channel message to the client. """
    if s in sock_to_client and sock_to_client[s].current!="":
        chan = sock_to_client[s].current
        s.send(archieve[chan])

####################### ANNEXE #######################

def usage(s):
    """ For error message. """
    s.send(b'MEE6 : The command you typed cannot be used in your current state.\nUse the command /HELP if you\'re not sure when to use each command\n')

def get_arg(m):
    """ Return the string of the second word seperated by intervals inside 'm'. """
    m1 = m.split(' ')
    m2 = m1[0].split('\n')
    return m2[0]

def get_msg_arg(m):
    """ Return a list of all user's name given in argument after a /MSG command. """
    m1 = m.split(';')
    size = len(m1)
    arg_len = 0
    for i in range(size-1):
        arg_len += len(m1[i])+1
    lm = m1[size-1].split(' ')
    for s in lm:
        if s!='':
            m1[size-1] = s
            break
        arg_len+=1
    arg_len+= len(m1[size-1])
    for i in range(len(m1)):
        m1[i] = m1[i].replace(' ','')
    return m1 , m[arg_len:len(m)]


def rand_name(namelen=10):
    """Generate a random name of fixed length """
    letters = string.ascii_lowercase
    return ''.join(random.choice(letters) for i in range(namelen))

def no_admin(channel):
    """ Check if 'channel' has any admin left. """
    for client in d_channel[channel]:
        if client.is_admin(channel):
            return False
    return True

####################### SWITCHMAN #######################

commands = {
    b'\x00': help,
    b'\x01': plist,
    b'\x02': join,
    b'\x03': kick,
    b'\x04': who,
    b'\x05': msg,
    b'\x06': bye,
    b'\x07': ren,
    b'\x08': leave,
    b'\x09': no_command,
    b'\x0a': current,
    b'\x0b': change_current,
    b'\x0c': send_file,
    b'\x0d': recv_file,
    b'\x0e': change_nick,
    b'\x0f': grant,
    b'\x10': revoke,
    b'\x11': history
}

def switchman(s,m,c):
    """ Determine which command to execute. """
    if len(m)==0:
        bye(s,m)
    else :
        if c in commands:
            commands[c](s,m[1:len(m)])

####################### MAIN LOOP #######################

while True:
    l_s, a, b = select.select(l, [], [])
    for s in l_s:
        if s==serveur.socket:
            s2, addr = s.accept()
            l.append(s2)
            s2.send(b'Welcome to the server !\nPlease enter a nickname before doing anything else.\nYou can use the command /HELP to see all commands\n')
        else:
            data = s.recv(1024)
            m = data.decode("utf-8")
            switchman(s,m,data[0:1])       

