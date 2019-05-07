from tkinter import *
import random



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
    

win = Tk()
win.geometry('800x800')
top = Frame(win)
top.pack(side=TOP)

bot = Frame(win)
bot.pack(side=BOTTOM)

bcreate_normal = Button(top,width=15,height=2,text="Normal",fg="white",bg="green",command=show_grid)
bquit = Button(top,width=15,height=2,text="Quit",fg="white",bg="blue",command=win.destroy)
bcreate_hard=Button(top,width=15,height=2,text="Hard",fg="white",bg="red",command=hard_grid)

bcreate_normal.pack(side=LEFT)
bquit.pack(side=RIGHT)
bcreate_hard.pack()


win.mainloop()


