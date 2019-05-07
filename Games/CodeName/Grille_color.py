from tkinter import *
import tkinter.messagebox
import random

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


win = Tk()
win.geometry('750x750')
top = Frame(win)
top.pack(side=TOP)
bot = Frame(win)
bot.pack(side=BOTTOM)
bcreate = Button(top,width=15,height=2,text="New Grid",fg="white",bg="green",command=show_grid)
bquit = Button(top,width=15,height=2,text="Quit",fg="white",bg="red",command=win.destroy)
bcreate.pack(side=LEFT)
bquit.pack(side=RIGHT)
win.mainloop()
