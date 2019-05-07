from tkinter import *
import tkinter.messagebox
import random


##START COLOR GRID##
def attribution_couleur():
    s=[i for i in range(1,26)]
    for n in range(10):
        random.shuffle(s)
    for i in range(25) :
        if s[i]<=9 :
            s[i]="red"
        elif s[i]<=17:
            s[i]="blue"
        elif s[i]==25:
            s[i]="black"
        else :
            s[i]="yellow"
    return s



def found(event):
    color = event.widget.cget('bg')
    event.widget.config(text=color,bg="white")
    event.widget.bind("<Button-1>",do_nothing)
    event.widget.bind("<Button-3>",reset)

def do_nothing(event):
    a=0
    
def reset(event):
    color = event.widget.cget('text')
    event.widget.config(text="",bg=color)
    event.widget.bind("<Button-1>",found)
    event.widget.bind("<Button-3>",do_nothing)


        

def show_grid():
    s = attribution_couleur()
    i=0
    for y in range(5):
        for x in range(5):
            square = Button(bot,width=20,height=8,bg=s[i],fg="white")
            square.bind("<Button-1>",found)
            square.grid(row=y,column=x)
            i+=1
##END COLOR GRID##

##START WORD GRID##

idf = open("mots_pour_codename.txt",'r')
t_mots = idf.readlines()
idf.close()
nb_mots = len(t_mots)
for i in range(nb_mots):
    t_mots[i] = t_mots[i][0:len(t_mots[i])-1]

idf2 = open("liste_francais.txt",'r')
t_mots_hard = idf2.readlines()
idf2.close()
nb_mots_hard = len(t_mots_hard)
for i in range(nb_mots_hard):
    t_mots_hard[i] = t_mots_hard[i][0:len(t_mots_hard[i])-1]


def rand_mots():
    t=t_mots
    for i in range(10):
        random.shuffle(t)
    k = random.randrange(0,nb_mots-25)
    l=[]
    for i in range(k,k+25):
        l.append(t[i])
    return l

def rand_hard():
    t=t_mots_hard
    for i in range(10):
        random.shuffle(t)
    k = random.randrange(0,nb_mots_hard-25)
    l=[]
    for i in range(k,k+25):
        l.append(t[i])
    return l

def faire_couleur(event):
    event.widget.config(fg="white")
    if event.char=='1':
        event.widget.config(bg="red")
    if event.char=='2':
        event.widget.config(bg="blue")
    if event.char=='3':
        event.widget.config(bg="yellow")
        event.widget.config(fg="black")
    if event.char=='0':
        event.widget.config(bg="black")
    if event.char=='.':
        event.widget.config(bg="white")
        event.widget.config(fg="black")
    win.focus_set()



def focus(event):
    event.widget.focus_set()

def show_grid():
    s = rand_mots()
    i=0
    for y in range(5):
        for x in range(5):
            square = Button(bot,width=15,height=6,text=s[i],font=("Arial Black", 16),bg="white",fg="black")
            square.bind("<Key>",faire_couleur)
            square.bind("<Button-1>",focus)
            square.grid(row=y,column=x)
            i+=1
def hard_grid():
    s = rand_hard()
    i=0
    for y in range(5):
        for x in range(5):
            square = Button(bot,width=20,height=8,text=s[i],font=("Courier", 16),bg="white",fg="black")
            square.bind("<Key>",faire_couleur)
            square.bind("<Button-1>",focus)
            square.grid(row=y,column=x)
            i+=1
def win2():
    win2=Toplevel()
    win2.title('Grille de mots')
    win2.geometry('800x800')
    top = Frame(win2)
    top.pack(side=TOP)

    bot = Frame(win2)
    bot.pack(side=BOTTOM)

    bcreate_normal = Button(top,width=15,height=2,text="Normal",fg="white",bg="green",command=show_grid)
    bquit = Button(top,width=15,height=2,text="Quit",fg="white",bg="blue",command=win.destroy)
    bcreate_hard=Button(top,width=15,height=2,text="Hard",fg="white",bg="red",command=hard_grid)

    bcreate_normal.pack(side=LEFT)
    bquit.pack(side=RIGHT)
    bcreate_hard.pack()
##END WORD GRID##

win = Tk()
win.title('Grille de couleur')
win.geometry('750x750')
top = Frame(win)
top.pack(side=TOP)
bot = Frame(win)
bot.pack(side=BOTTOM)
bcreate = Button(top,width=15,height=2,text="New Grid",fg="white",bg="green",command=show_grid)
bquit = Button(top,width=15,height=2,text="Quit",fg="white",bg="red",command=win.destroy)
bcreate.pack(side=LEFT)
bquit.pack(side=RIGHT)
win2()
win.mainloop()
