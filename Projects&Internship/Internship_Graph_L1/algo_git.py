from graphes import *

# constantes

PATH="test graph/"
DEBUG = False
EVIL = False
RELOU = True

# variables globales et de test

gv1=ouvrirGraphe(PATH + "oriente_v1.dot")
gv2=ouvrirGraphe(PATH + "oriente_v2.dot")
gv3=ouvrirGraphe(PATH + "oriente_v3.dot")
gv4=ouvrirGraphe(PATH + "oriente_v4.dot")
gv5=ouvrirGraphe(PATH + "oriente_v5.dot")
gv6=ouvrirGraphe(PATH + "oriente_v6.dot")

relou0 = ouvrirGraphe(PATH + "oriente_relou-témoin.dot")
relou1 = ouvrirGraphe(PATH + "oriente_relou.dot")
relou2 = ouvrirGraphe(PATH + "oriente_relou-2.dot")
relou3 = ouvrirGraphe(PATH + "oriente_relou-3.dot")
relou4 = ouvrirGraphe(PATH + "oriente_relou-4.dot")
relou5 = ouvrirGraphe(PATH + "oriente_relou-5.dot")
relou6 = ouvrirGraphe(PATH + "oriente_relou-6.dot")
relou7 = ouvrirGraphe(PATH + "oriente_relou-7.dot")
relou50 = ouvrirGraphe(PATH + "oriente_relou50v.dot")
relou0_50 = ouvrirGraphe(PATH + "oriente_relou-témoin50v.dot")
relou2_50 = ouvrirGraphe(PATH + "oriente_relou50v_2.dot")
relou3_50 = ouvrirGraphe(PATH + "oriente_relou50v_3.dot")

if not RELOU :
    g = [gv1,gv2,gv3,gv4,gv5,gv6]
else :
    g = [relou0,relou1,relou2,relou3,relou4,relou5,relou6,relou7,
         relou0_50,relou50,relou2_50, relou3_50]
    
nb_step = 0
######################## Fonctions utilisées pour la mise en place de l'erreur ########################

def set_error(g,s0):
    """ Initialise les marquages des successeurs de s0 """
    
    if len(listeSuccesseurs(s0))!=0:
        # en fait, la boucle ci dessus ne sert à rien, un for sur une liste vide n'a pas d'effet.
        for s in listeSuccesseurs(s0):
            s.mark = True
            set_error(g,s)

def reset(g):
    """ Démarque et décolorie tous les sommets de g """
    for s in listeSommets(g):
        s.mark = False
        s.color = "white"
    

def reset_all():
    """ Reset tous les graphes à disposition et remet le compteur nb_step à 0 """
    global g
    global nb_step

    nb_step = 0
    
    for h in g :
        reset(h)

def rand_error(g):
    """ Initialise une erreur au hazard dans le graphe g """
    reset(g)
    s0 = random.choice([listeSommets(g)[i] for i in range(1,len(listeSommets(g)))])
    marquerSommet(s0)
    set_error(g,s0)
    print(s0.label)
        
######################## Fonctions auxiliaires ########################
    
def reverse(l):
    """ Inverse le sens d'une liste """
    for i in range(len(l)//2):
        tmp=l[i]
        l[i]=l[len(l)-i-1]
        l[len(l)-i-1]=tmp

def find_path(g):
    """ Retourne une chemin dans le graphe orienté g (plus tard utiliser pour la recherche) """
    s=g.nodes[0]
    chemin=[s]
    while len(listeSuccesseurs(s))!=0:
        s = listeSuccesseurs(s)[0]
        chemin.append(s)
    return chemin

def find_rand_path(g):
    """ Retourne un chemin aléatoire dans le graphe orienté g (plus tard utilisé pour la recherche) """
    s=g.nodes[0]
    chemin=[s]
    while len(listeSuccesseurs(s))!=0:
        s=random.choice(listeSuccesseurs(s))
        chemin.append(s)
    return chemin

def color_pred(s,c):
    """ Colorie tous les predecesseurs de s blanc en la couleur c """
    for si in listePredecesseurs(s):
        if si.color == "white":
            colorierSommet(si,c)
            color_pred(si,c)

def color_suc(s,c):
    """ Colorie tous les successeurs de s blanc en la couleur c """
    for si in listeSuccesseurs(s):
        if si.color == "white":
            colorierSommet(si,c)
            color_suc(si,c)

def color_suc2(s,c):
    """ Colorie tous les successeurs de s en la couleur c, même si ils sont déjà coloré"""
    for si in listeSuccesseurs(s):
        colorierSommet(si,c)
        color_suc2(si,c)

def color_change(g,c_in,c_out):
    for s in listeSommets(g):
        if s.color == c_in:
            s.color = c_out
            
def nb_edges(g):
    """ Compte le nombre d'arêtes d'un graphe """
    nb=0
    for s in g.nodes:
        for e in s.edges:
            if not e.mark :
                nb+=1
                e.mark=True
    for s in g.nodes:
        for e in s.edges:
            e.mark=False
    return nb

######################## Fonctions faulty ########################

def is_faulty(s):
    """ Retourne le markage du sommet s """
    
    global nb_step
    nb_step += 1
    if DEBUG:
        print("appel is_faulty sur", s, "répond", s.mark)
    return s.mark

def evil_is_faulty(g,s):
    """ Retourne la mark fictive du sommet s amenant le plus de recherche possible par search_error """
    global nb_step
    if DEBUG:
        print("appel sur",s.label)

    nb_step +=1
        
    if s.color == "green" :
        return False
    if s.color == "red" :
        return True
        
    # colorie tous les prédécesseurs en bleu
    color_pred(s,"blue")
    # colorie les successeurs en orange
    for si in listeSommets(g):
        if si.color == "white" and si != s:
            si.color = "orange"
    # on compte les bleu et les orange
    nb_b=1
    nb_o=0
    max_o = 1
    max_b = 1
    # cette boucle pourrait aussi être simplifiée. On peut ne pas colorier en orange,
    # mais directement compter dans la boucle précédente.
    for si in listeSommets(g):
        if si.color=="orange":
            nb_o +=1

        if si.color=="blue":
            nb_b +=1

    if DEBUG:
        print(s.label," dans ",g.label," -> non faulty :",nb_b,", faulty :",nb_o)
    # si bleue<=orange, le sommet n'est pas faulty
    if nb_b<=nb_o :
        color_change(g,"blue","white")
        color_change(g,"orange","white")
        if DEBUG :
                dessiner(g)
        return False
    # sinon le sommet est faulty car il y a plus de sommets avant
    else:
        # colorie les successeurs en rouge et les autres oranges en vert,
        # permettant de régler le problème des branches non coloriées
        color_suc2(s,'red')
        s.color = 'red'
        color_change(g,"orange","green")
        color_change(g,"blue","white")
        
        if DEBUG :
            dessiner(g)
        return True

######################## Fonction de recherche evil (encore en dévelopement, absolument pas fini) ########################                        

def evil_dichotomique(g,path):
    a = 0
    b = len(path)-1
    m = (a+b)//2
    ierror=b        
    while a <= b :
        if evil_is_faulty(g,path[m]) :
            b=m-1
            ierror=m
            path[m].color = "red"
            color_suc(path[m],"red")
        else :
            path[m].color = "green"
            color_pred(path[m],"green")
            a=m+1
        m=(a+b)//2
    return path[ierror]


def search_error_in_path_evil(g,path):
    """ Recherche dichotomique en mode evil avec les sommets de g présent dans path """
    global nb_step
        
    s_error = evil_dichotomique(g,path)
        
    pred = listePredecesseurs(s_error)
        
    if len(pred)==2:
        if not evil_is_faulty(g,pred[0]) and not evil_is_faulty(g,pred[1]) and not (pred[0].color == "white" or pred[1].color == "white") :
            return s_error 
        newpath=[]
        if evil_is_faulty(g,pred[0]):
            newpath.append(pred[0])
        else :
            newpath.append(pred[1])
        while len(listePredecesseurs(newpath[len(newpath)-1]))!= 0 and len(listePredecesseurs(newpath[len(newpath)-1])) != 2 and listePredecesseurs(newpath[len(newpath)-1])[0].color!="green" :
            nb_step +=1
            L = listePredecesseurs(newpath[len(newpath)-1])
            newpath.append(L[0])
        reverse(newpath)
        return search_error_in_path_evil(g,newpath)          
    else :
        return s_error

def search_error_evil(g):
    """ Initialise search_error_in_path_evil """
    path=find_path(g)
    return search_error_in_path_evil(g,path)

######################## Fonctions angel (anti-evil) ########################

def predire_evil(g,s):
    """ Return la différence entre nb_orange et nb_blue qu'aurait calculé evil_is_faulty
    (ne doit rien colorier)"""

    color_pred(s,"blue")
    for si in listeSommets(g):
        if si.color=="white" and si!=s:
            si.color="orange"
    nb_b=0
    nb_o=0
    for si in listeSommets(g):
        if si.color=="orange":
            nb_o += 1
        if si.color=="blue":
            nb_b += 1
    color_change(g,"blue","white")
    color_change(g,"orange","white")
    
    return abs(nb_o-nb_b)

def predire_sommet_evil(g):
    """ Return le sommet de g ayant le plus petit predire_evil """
    L = listeSommets(g)
    # recherche de l'indice d'un candidat (blanc)
    j=0 
    while j<len(L) and L[j].color != "white" :
        j+=1
    if j == len(L) :
        if DEBUG:
            print("attention, appel de predire_sommet_evil sur graphe completement coloré!")
        return None
    # minimisation
    s_min = L[j]
    s_min_value = predire_evil(g,s_min)
    for s in [L[i] for i in range(j+1,len(L))]:
        if s.color == "white":
            s_value = predire_evil(g,s)
            if s_min_value > s_value:
                s_min = s
                s_min_value = s_value
    return s_min

def stop(g , b=False):
    """ Return True si g ne contient plus qu'un ou aucun sommet blanc, et False sinon """
    
    nb = 0
    L=listeSommets(g)
    
    for s in L:
        if s.color != "white":
            nb+=1
    if b :
        return (nb==len(L)  or nb==len(L)-1)
    else :
        return nb==len(L)-1

def angel_search(g , s0 = -1):
    """Cherche dans le graphe en essayant de minimiser pour chaque appel la différence
    entre nb_o et nb_b.
    """
    global nb_step
    
    # Initialisation de s_error
    if s0 == -1 :
        path = find_path(g)
        s_error = path[len(path)-1]
    else :
        s_error = s0

    # Recherche de l'équilibre entre nb_o et nb_b et utilisation de 
    if not stop(g) :
        s = predire_sommet_evil(g)
        if evil_is_faulty(g,s) :
            s_error = s
        else :
            s.color = "green"
            color_pred(s,"green")
        return angel_search(g,s_error)

    nb_step = 0
    # Return si il ne reste plus qu'un seul sommet
    return s_error

def pred_is_faulty (s):
    """ Check si tous les prédécesseurs de s sont faulty """
    for v in listePredecesseurs(s):
        if is_faulty(v) :
            return True
    return False

def is_known_target (s):
    """ Check si on sait que s est la cible grace aux seules couleurs.

    c'est à dire s est red et tous ses prédécesseurs sont green.
    """
    if couleurSommet(s) != "red":
        return False
    else: 
        for v in listePredecesseurs(s):
            if couleurSommet(v) != "green":
                return False
        return True

def search_error_angel(g):
    """ Cherche et retourne le sommet présentant l'erreur en suivant le principe
    de search_angel """

    global nb_step
    nb_step = 0
    
    s = predire_sommet_evil(g)
    l = find_path(g)
    s_error = l[len(l)-1]
    colorierSommet(s_error,"red")
    colorierSommet(l[0],"green")
    while not is_known_target(s_error):
        # en bleu les sommets valides si s pas faulty.
        color_pred(s,"blue") 
        # en orange les sommets potentiellement fautifs si s faulty.
        s.color = 'orange'
        for si in listeSommets(g):
            if si.color=="white":
                si.color="orange"
                
        if is_faulty(s):
            # les successeurs de s sont tous fautifs
            color_suc2(s,'red')
            s.color = 'red'
            # les sommets ni successeurs ni predecesseurs (c a d orange restant) sont valides
            color_change(g,"orange","green")
            color_change(g,"blue","white")

            s_error = s
            

        else :
            s.color = 'green'
            color_change(g,"blue","green")
            color_change(g,"orange","white")

        s = predire_sommet_evil(g)
        if DEBUG:
            dessiner(g)
        if s == None :
            return s_error

    return s_error
            
######################## Fonction de recherche de l'erreur ########################

def search_error_dichotomique(path):
    """ Cherche et retourne le premier sommet dans path ayant une erreur """
    
    a = 0
    b = len(path)-1
    m = (a+b)//2
    ierror=b        
    while a <= b :
        if is_faulty(path[m]) :
            b=m-1
            ierror=m
            path[m].color = "red"
            color_suc(path[m],"red")
        else :
            path[m].color = "green"
            color_pred(path[m],"green")
            a=m+1
        m=(a+b)//2
    return path[ierror]


def search_error_in_path(g,path):
    """ Recherche dichotomique classique avec les sommets de g présent dans path """
    global nb_step
    
    s_error = search_error_dichotomique(path)
    pred = listePredecesseurs(s_error)           
    if len(pred)==2:       
        if not is_faulty(pred[0]) and not is_faulty(pred[1]) :
            return s_error 
        newpath=[]
        if is_faulty(pred[0]) :
            newpath.append(pred[0])
        else :
            newpath.append(pred[1])
        while len(listePredecesseurs(newpath[len(newpath)-1])) != 0 and len(listePredecesseurs(newpath[len(newpath)-1])) != 2 and listePredecesseurs(newpath[len(newpath)-1])[0].color!="green":
            nb_step +=1
            L = listePredecesseurs(newpath[len(newpath)-1])
            newpath.append(L[0])
        reverse(newpath)
        return search_error_in_path(g,newpath)          
    else :
        return s_error

def search_error(g):
    """ Initialise search_error_in_path """
    global nb_step
    nb_step = 0
    path=find_path(g)
    return search_error_in_path(g,path)

######################## Fonctions test ########################

def test_brut(graph_nb, search = search_error , k=-1):
    """ Teste la fonction search (search_error par défaut) pour chaque cas possible d'erreur du graphe G,
        et print les sommets qui donne k nombre d'étape, utilise search_error_evil si EVIL = True """
    global nb_step

    if RELOU :
        graph_nb +=1
    
    if graph_nb>len(g):
        print("first argument too big, must be between 1 and",len(g))
    G = g[graph_nb-1]
    
    if EVIL :
        nb_step = 0
        search_error_evil(G)
        print("")
        print("Le search en mode evil à calculé ",nb_step," étapes pour le graph ",G.label)
        print("")
        return 
    step_max = 0
    sommet_max = None
    step_min = -1
    moyenne = 0            
    for s in [listeSommets(G)[i] for i in range(1,len(listeSommets(G)))]:
        reset(G)
        set_error(G,s)
        s.mark=True
        ser = search(G)
        moyenne += nb_step
        if nb_step == k :
            print (ser)
        if nb_step > step_max:
            step_max = nb_step
            sommet_max = s
        if nb_step < step_min or step_min < 0:
            step_min = nb_step
        if ser!=s:
            print("")
            print("Problem avec ",s.label,", ça retourne ",ser.label)
            print("")
            sys.exit()
    M = (float(int((moyenne/len(listeSommets(G)))*100)))/100  
    return (step_min,step_max,M)

def is_it(g):
    """ Test si le maximum d'étape calculé par search_error_evil(g)
        correspond au maximum théorique calculé par test_brut(g) et
        print les 2 maximums si ce n'est pas le cas"""
    global nb_step
    reset(g)
    (a,max1,b) = test_brut(g)
    nb_step = 0
    reset(g)
    search_error_evil(g)
    max2 = nb_step
    if max1 == max2:
        return True
    else :
        print("Pas bon ! max théorique : ",max1,", max evil :",max2)
        return False

def test_search_sommet( graph_nb , label , search = search_error):
    """ Test la fonction search (search_error par défaut) sur le graphe
        numéro graph_nb, présentant une erreur sur le sommet de nom label """

    if RELOU :
        graph_nb +=1
        
    if graph_nb>len(g):
        print("first argument too big, must be between 1 and",len(g))
    G = g[graph_nb-1]
    reset(G)
    s = sommetNom(G,label)

    s.mark = True
    set_error(G , s)
    print(search(G))
    dessiner(G)
