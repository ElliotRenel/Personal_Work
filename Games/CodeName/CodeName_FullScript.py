from tkinter import *
import tkinter.messagebox
import random


##START COLOR GRID##
s=[]
def attribution_couleur():
    global s
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

def color_grid():
    global s
    attribution_couleur()
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

def rand_hard():
    t=t_mots_hard
    for i in range(10):
        random.shuffle(t)
    k = random.randrange(0,nb_mots_hard-25)
    l=[]
    for i in range(k,k+25):
        l.append(t[i])
    return l

def rand_mots():
    t=t_mots
    for i in range(10):
        random.shuffle(t)
    k = random.randrange(0,nb_mots-25)
    l=[]
    for i in range(k,k+25):
        l.append(t[i])
    return l

def word_color(event):
    global s
    name = event.widget.cget('text')
    c = name[len(name)-2]
    event.widget.config(bg=s[int(c)],fg='white')
    if s[int(c)]=='yellow':
        event.widget.config(fg='black')
    event.widget.bind("<Button-1>",word_reset)

def word_reset(event):
    event.widget.config(bg='white',fg='black')
    event.widget.bind("<Button-1>",word_color)

def word_grid():
    m = rand_mots()
    i=0
    for y in range(5):
        for x in range(5):
            square = Button(win2,width=15,height=6,text=m[i]+' ('+str(i)+')',font=("Arial Black", 16),bg="white",fg="black")
            square.bind("<Button-1>",word_color)
            square.grid(row=y,column=x)
            i+=1
def word_grid2():
    m = rand_hard()
    i=0
    for y in range(5):
        for x in range(5):
            square = Button(win2,width=15,height=6,text=m[i]+' ('+str(i)+')',font=("Arial Black", 16),bg="white",fg="black")
            square.bind("<Button-1>",word_color)
            square.grid(row=y,column=x)
            i+=1
##END WORD GRID##

def normal_grid():
    word_grid()
    color_grid()
                        ## fonction pour affichage des 2 grilles
def hard_grid():
    color_grid()
    word_grid2()

win = Tk()
win.title('CodeName__Grille de couleur')
win.geometry('750x750')
top = Frame(win)
top.pack(side=TOP)
bot = Frame(win)
bot.pack(side=BOTTOM)
bcreate_normal = Button(top,width=15,height=2,text="Normal",fg="white",bg="green",command=normal_grid)
bcreate_hard=Button(top,width=15,height=2,text="Hard",fg="white",bg="red",command=hard_grid)
bquit = Button(top,width=15,height=2,text="Quit",fg="white",bg="blue",command=win.destroy)
bcreate_normal.grid(row=0)
bcreate_hard.grid(row=0,column=1)
bquit.grid(row=0,column=2)

win2 = Toplevel()
win2.title('CodeName__Grille de mots')
win2.geometry('800x800')

win.mainloop()
