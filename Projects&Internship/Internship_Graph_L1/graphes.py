""" graphes.py en cours de modification pour inclure les graphes orientes. """


# coding=utf-8
import os, platform, subprocess, random, glob, sys
#from random import randrange

if sys.hexversion < 3 << 24:
    print("graphes ne fonctionne qu'avec idle3 (python3), pas avec idle (python)")
    sys.exit(1)

# Attention, eviter que les deux noms globaux qui suivent
# aient meme prefixe que les noms "publics"
# proposes par Idle (completion de nom);
# on pourrait evidemment ajouter un _ en tete

plateforme = platform.system ()
if plateforme != 'Windows':
    import resource
 
def _getGraphViz():
  if plateforme == 'Windows':
      l = glob.glob('C:/Program Files*/Graphviz*/bin/dot.exe')
      if l == []:
          print("Graphviz non trouve, la fonction dessiner ne fonctionnera pas")
          return ""
      else:
          path = l[0]
      return path[0:-7]
  else:
      l = glob.glob('/usr/bin/dot')
      if l == []:
          l = glob.glob('/usr/local/bin/dot')
          if l == []:
              l = glob.glob('/opt/local/bin/dot')
              if l == []:
                  print("Graphviz non trouve, la fonction dessiner ne fonctionnera pas")
                  return ""
      path = l[0]
      return path[0:-3]

pathGraphviz = _getGraphViz()



################ PRIMITIVES GENERIQUES SUR LES LISTES   ##############

def melange (u):
    v = u[:] # v est une copie de u, Python c'est fun
    random.shuffle (v)
    return v

def elementAleatoireListe(u):
    if u.__class__.__name__ != 'list':
        raise ErreurParametre(u, "une liste")
  # u est une liste
  # La fonction renvoie un element pris au hasard de la liste u si 
  # elle est non-vide. Si u est vide, la fonction renvoie une 
  # erreur (exception IndexError)
    return random.choice(u)



# Changelog JB
#   Ete 2009
#     Grand nettoyage et adaptation a Python 3
#   Septembre 2009: variable globale plateforme
#     evite deux appels systeme a chaque dessin
#     simplification des fonctions Graphviz et dessinerGraphe
#     (suggestions Jean-Claude Ville)
#   Octobre 2009: utilisation de la mise en forme de chaines
#     (format % valeurs) pour simplifier les representations de classes
#     (methodes __repr__) et la fonction 'dotify'
#   Janvier 2010: fonction 'voisinPar' -> 'sommetVoisin'

################ PRIMITIVES GRAPHE   #################################

def nomGraphe(G):
    verif_type_graphe(G)
    return G.label

def listeSommets(G):
    verif_type_graphe(G)
    return G.nodes

def nbSommets(G):
    verif_type_graphe(G)
    return len(listeSommets(G))

def sommetNom(G, etiquette):
    verif_type_graphe(G)
    verif_type_chaine(etiquette)
    for s in listeSommets(G):
        if s.label== etiquette:
            return s
    for s in listeSommets(G):
        if s.label.lower() == etiquette.lower():
            raise Exception("le graphe " + nomGraphe(G) + " ne possède pas de sommet d'étiquette '" + etiquette + "'."\
                             " En revanche il possède un sommet d'étiquette '" + s.label + "'. Remarquez la différence majuscule/minuscule.")
    raise Exception("le graphe " + nomGraphe(G) + " ne possède pas de sommet d'étiquette '" + etiquette + "'.")

def sommetNumero(G, i):
    verif_type_graphe(G)
    return listeSommets(G)[i]

################# PRIMITIVES SOMMET   ###################################

def nomSommet(s):
    verif_type_sommet(s)
    return s.label

def marquerSommet(s):
    verif_type_sommet(s)
    s.mark = True

def demarquerSommet(s):
    verif_type_sommet(s)
    s.mark = False

def estMarqueSommet(s):
    verif_type_sommet(s)
    return s.mark

def colorierSommet(s, c):
    verif_type_sommet(s)
    verif_type_couleur(c)
    s.color = c

def couleurSommet(s):
    verif_type_sommet(s)
    return s.color

##Dans cette version on ne colorie pas les aretes
##colorier = colorierSommet
##couleur = couleurSommet
    
# Ici les choses serieuses
def listeAretesIncidentes(s):
    verif_type_sommet(s)
    return s.edges

def areteNumero(s, i):
    return listeAretesIncidentes(s)[i]

def degre(s):
    verif_type_sommet(s)
    return len(listeAretesIncidentes(s))

def degreSortant(s):
    L=listeSuccesseurs(s)
    return len(L)

def degreEntrant(s):
    L=listePredecesseurs(s)
    return len(L)

def listeVoisins(s):
    """Retourne la liste des voisins du sommet, et la liste des successeurs si le graphe est oriente."""
    verif_type_sommet(s)
    inc = listeAretesIncidentes(s)
    v = []
    for a in inc:
        if a.start == s:
            v.append (a.end)
        elif a.end == s and not a.oriente:
            v.append (a.start)
    return v

def listeSuccesseurs(s):
    """Retourne la liste des successeurs du sommet s"""
    verif_type_sommet(s)
    inc = listeAretesIncidentes(s)
    v = []
    for a in inc:
        if a.start == s:
            v.append (a.end)
    return v

def listePredecesseurs(s):
    """Retourne la liste des predecesseurs du sommet s"""
    verif_type_sommet(s)
    inc = listeAretesIncidentes(s)
    v = []
    for a in inc:
        if a.end == s:
            v.append (a.start)
    return v

def voisinNumero(s, i):
    return listeVoisins(s)[i]

def sommetVoisin(s, a):
    verif_type_sommet(s)
    verif_type_arete(a)
    if a.start == s:
        return a.end
    if a.end == s:
        return a.start
    raise Exception("\n\nle sommet '" + nomSommet(s) + "' n'est pas une extrémité de l'arete ('" + nomSommet(a.start) +"', '"+ nomSommet(a.end) + "').")

################ PRIMITIVES arete ########################

def nomArete(a):
    verif_type_arete(a)
    return a.label

def marquerArete(a):
    verif_type_arete(a)
    a.mark = True

def demarquerArete(a):
    verif_type_arete(a)
    a.mark = False

def estMarqueeArete(a):
    verif_type_arete(a)
    return a.mark

def numeroterArete(a, n):
    verif_type_arete(a)
    a.label = str(n)

################ Classes ########################

class c_graph:
    def __init__(self, label = '', drawopts = '', oriente=False):
        self.nodes = []
        self.label = label
        self.drawopts = drawopts    # pour Graphviz
        self.oriente = oriente
    def __repr__(self):
        if self.oriente:
            return "<digraphe: '%s'>" % self.label
        else:
            return "<graphe: '%s'>" % self.label
    def est_oriente(self):
        return self.oriente

class c_node:
    def __init__(self, label = '', color = 'white', mark = False, drawopts = ''):
        self.label = label
        self.color = color
        self.mark = mark
        self.edges = []
        self.drawopts = drawopts    # pour Graphviz
    def __repr__(self):
        c = "'" + self.color + "'"
        return "<sommet: '%s', %s, %s>" % (self.label, c, self.mark)

class c_edge:
    def __init__(self, label = '', start = None, end = None, mark = False, drawopts = '', oriente=False):
        self.label = label
        self.start = start
        self.end = end
        self.mark = mark
        self.oriente = oriente
        self.drawopts = drawopts    # pour Graphviz
    def __repr__(self):
        if self.oriente:
            return "<arete: '%s' %s->%s >" % (self.label, self.start.label, self.end.label)
        else:
            return "<arete: '%s' %s--%s >" % (self.label, self.start.label, self.end.label)

################ Verifications de types ########################

def verif_type_graphe(G):
    if G.__class__.__name__ != 'c_graph':
        raise ErreurParametre(G, "un graphe")
    
def verif_type_sommet(s):
    if s.__class__.__name__ != 'c_node':
        if (type(s) == str):
            raise TypeError("'" + s + "' est une chaine de caracteres alors que la fonction attend un sommet. Peut-etre voulez-vous utiliser la fonction SommetNom(G, etiquette)?")
        else:
            raise ErreurParametre(s, "un sommet")

def verif_type_arete(a):
    if a.__class__.__name__ != 'c_edge':
        raise ErreurParametre(a, "une arete")

def verif_type_chaine(s):
    if type(s) != str:
        raise ErreurParametre(s, "une chaine de caracteres")

def verif_type_couleur(s):
    if type(s) != str:
        raise ErreurParametre(s, "une chaine de caracteres représentant une couleur comme par exemple : ’red’, ’green’, ’blue’, ’white’, ’cyan’ ou ’yellow’.")


class ErreurParametre (TypeError):
    def __init__(self, arg, param):
        self.arg = arg
        self.param = param
    def __str__(self):
        # affichage discutable
        if type (self.arg) == str:
            strArg = "'" + self.arg + "'"
        else:
            strArg = str (self.arg)
        return "\n\n" + strArg + " n'est pas " + self.param

############### Construction de graphes  #####################
    
def _add_edge (G, label, i, j, oriente=False):
    a = c_edge(label, G.nodes[i], G.nodes[j], oriente=oriente)
    G.nodes[i].edges.append(a)
    G.nodes[j].edges.append(a)
    return a

# retourne le numero du sommet
def _find_add_node (G, nom):
    i = 0
    for s in G.nodes:
        if s.label == nom:
            return i
        i = i + 1
    G.nodes.append (c_node (nom))
    return i

# Un chemin p est une liste de noms de sommets ['A', 'B', 'C', ...]
# Le parametre booleen 'chemins' indique si les aretes sont:
#   A-B, B-C, etc. (chemin classique)
#   A-B, A-C, etc. (etoile: le sommet initial est suivi de ses voisins)
# B, C, etc. peuvent aussi etre des couples [nom de sommet, nom d'arete]
# lorsqu'on veut etiqueter explicitement les aretes
# (etiquetees par defaut 'e0', 'e1', etc. dans l'ordre de leur creation)

# Attention: l'etiquette d'un graphe (label) sert aussi de nom de fichier
# pour les dessins, eviter les blancs, etc

def construireGraphe (paths, label, chemins = True, oriente=False):
    G = c_graph(label,oriente=oriente)
    
    # Numeroter les aretes a partir de 0 ou 1 en l'absence
    # d'etiquette explicite?
    # On choisit 1 car les dessins du poly adoptent cette convention
    # mais ce choix est incoherent avec celui pour les sommets:
    # la fonction sommetNumero impose que les sommets des graphes
    # generiques (grilles, etc) soient etiquetes 's0', 's1', etc.
    
    nba = 1
    for p in paths:
        labelsource = p[0]
        i = _find_add_node (G, labelsource);
        
        # ne pas utiliser 'for a in p' car il faut maintenant ignorer p[0]  
        for k in range (1, len(p)):
            a = p[k]
            edge_with_label = type(a) == type([])
            if edge_with_label:
                labeldestination = a[0]
                labeledge = a[1]
            else:
                labeldestination = a
                labeledge = 'e' + str(nba)
                nba = nba + 1
            j = _find_add_node(G, labeldestination)
            _add_edge(G, labeledge, i, j, oriente=oriente)
            if chemins:
                i = j
    return G

############### Dessin du graphe  #####################

# fonction necessaire a cause par exemple des 'Pays Bas' (blanc dans label)
def _proteger (label):
    return '"' + label + '"'

# La fonction 'dotify' transforme le graphe en un fichier texte
# qui servira de source (suffixe .dot) pour les programmes de Graphviz.
# Cette fonction ne depend pas du systeme d'exploitation.

def dotify (G, etiquettesAretes = True, colormark = 'Black', suffixe = 'dot'):
    if plateforme != 'Windows':
        (soft,maximum) = resource.getrlimit(resource.RLIMIT_NOFILE)
        if soft == 0:
            resource.setrlimit(resource.RLIMIT_NOFILE, (maximum,maximum))

    # graphe non oriente, il faut eviter de traiter chaque arete deux fois
    for s in G.nodes:
        for a in s.edges:
            a.ecrite = False
            
    if G.label == '':
        nom_graphe = 'G'
    else:
        nom_graphe = G.label

    graph_dot = 'tmp/' + nom_graphe + '-' + str(dotify.num) + '.' + suffixe
    dotify.num = dotify.num + 1
        
    try:
        f = open (graph_dot, 'w')
    except IOError:
        os.mkdir ('tmp')
        f = open (graph_dot, 'w')

    if G.est_oriente(): 
        f.write ('digraph "' + nom_graphe + '" {\n' + G.drawopts + '\n')
    else:
        f.write ('digraph "' + nom_graphe + '" {\n' + G.drawopts + '\n')
    
    for s in G.nodes:
        d = len (s.edges)
        snom = _proteger (s.label)
        for a in s.edges:
            if a.start == s:
                t = a.end
                if a.oriente:
                    f.write ('  ' + snom + ' -> ' + _proteger (t.label))
                else:
                    f.write ('  ' + snom + ' -- ' + _proteger (t.label))
                if etiquettesAretes:
                    f.write (' [label = ' + _proteger (a.label) + ']')
                if a.mark:
                    f.write (' [style = bold, color = orange]')
                # Semicolons aid readability but are not required (dotguide.pdf)
                f.write (a.drawopts + ';\n')
        bord = 'black'
        if s.mark:
            entoure = 2
            bord = colormark
        else:
            entoure = 1
        if s.color:
            if s.color == "black":
                fontcolor = "white"
            else:
                fontcolor = "black"
            f.write ('  %s [style = filled, peripheries = %s, fillcolor = %s, fontcolor = %s, color = %s] %s;\n' %
                     (snom, entoure, s.color, fontcolor, bord, s.drawopts))
        elif s.mark:
##            f.write ('  ' + snom + ' [peripheries = 2, color = ' + bord + ']' +
##                     s.drawopts + ';\n');
            f.write ('  %s [peripheries = 2, color = %s] %s;\n' %
                     (snom, bord, s.drawopts))
        elif d == 0 or s.drawopts:
            f.write ('  ' + snom + s.drawopts + ';\n')
            
    f.write ('}\n')
    f.close ()
    if plateforme != 'Windows':
        if soft == 0:
            resource.setrlimit(resource.RLIMIT_NOFILE, (0,maximum))
    return graph_dot
dotify.num = 0

# La fonction 'Graphviz' lance l'execution d'un programme
# de la distribution Graphviz (dot, neato, twopi, circo, fdp)
# pour transformer un fichier texte source de nom 'racine.suffixe'
# en une image de nom 'racine.format' (peut-etre un fichier PostScript)

def Graphviz (source, algo = 'dot', format = 'svg', suffixe = 'dot'):
    if plateforme != 'Windows':
        (soft,maximum) = resource.getrlimit(resource.RLIMIT_NOFILE)
        if soft == 0:
            resource.setrlimit(resource.RLIMIT_NOFILE, (maximum,maximum))
    image = source.replace ('.' + suffixe, '.' + format)
    algo = pathGraphviz + algo
    if plateforme == 'Windows':
        algo = algo + '.exe'
    subprocess.call ([algo, '-T' + format, source, '-o', image])
    if plateforme != 'Windows':
        if soft == 0:
            resource.setrlimit(resource.RLIMIT_NOFILE, (0,maximum))
    return image


# Enchaine dotify et Graphviz avec des arguments standard adaptes au systeme
# et lance le programme ad hoc pour afficher l'image

def dessinerGraphe (G, etiquettesAretes = False, algo = 'dot', colormark = 'Black'):
    verif_type_graphe (G)
    if nbSommets(G) > 100:
        print("Attention, le graphe a "+str(nbSommets(G))+" sommets, le dessin va prendre du temps...")
    sys = platform.system ()
    if plateforme == 'Windows':
        # eviter toute embrouille avec les modeles de document de MS Word
        graph_dot = dotify (G, etiquettesAretes, colormark, 'txt')
        image = Graphviz (graph_dot, algo, suffixe = 'txt')
        image = image.replace ('/', '\\')
        os.startfile (image)
        return

    graph_dot = dotify (G, etiquettesAretes, colormark)
    image = Graphviz (graph_dot, algo)
    if nbSommets(G) > 100:
        print("Le dessin est enfin fini.")
    if plateforme == 'Linux':
        #subprocess.call (['firefox', image])
        subprocess.Popen (['firefox ' + image + ' &'], shell=True)
    elif plateforme == 'Darwin':
        subprocess.call (['open', image])
    else:
        print("Systeme " + plateforme + " imprevu, abandon du dessin")
        
dessiner = dessinerGraphe

# Cela pourrait être mieux écrit avec des règles standards de lexing/parsing, mais cela évite des dépendances

def _charclass (c):
    if c >= 'a' and c <= 'z' or \
           c >= 'A' and c <= 'Z' or \
           c >= '1' and c <= '9' or \
           c == '0' or  c == '_' or c == '.':
        return 'a'
    if c == '-' or c == '>':
        return '-'
    return c

# Lexing. On commence à regarder à la position i
# Retourne le mot et la position à laquelle on est arrivé
def _mot (s, debut):
    while debut < len(s) and (s[debut] == ' ' or s[debut] == '\t' or s[debut] == '\n' or s[debut] == '\r'):
        debut+=1

    if debut >= len(s):
        return "",debut

    if s[debut:debut+2] == '/*':
        fin = debut + 2
        while s[fin:fin+2] != '*/':
            fin+=1
        return _mot(s, fin+2)

    fin = debut
    if s[debut] == '"':
        fin+=1
        echappe = False
        while fin < len(s):
            if echappe:
                echappe = False
            else:
                if s[fin] == '"':
                    #print(s[debut:fin+1],fin+1)
                    return s[debut:fin+1],fin+1
                if s[fin] == '\\':
                    echappe = True
            fin+=1
        raise SyntaxError("Fichier incorrect: \" not terminé à la fin du fichier")

    charclass = _charclass(s[fin])
    while fin < len(s) and (s[fin] != ' ' and s[fin] != '\t' and s[fin] != '\n' and s[fin] != '\r'):
        if s[fin] == '#':
            # Commentaire, ignore jusqu'à la fin de ligne
            fin2 = fin
            while fin2 < len(s) and (s[fin2] != '\n' and s[fin2] != '\r'):
                fin2+=1
            if debut == fin:
                # Pas de mot avant le commentaire, on recommence à lire à la ligne suivante
                return _mot(s, fin2)
            return s[debut:fin],fin2

        if _charclass(s[fin]) != charclass:
            # On change de class de caractère, cela découpe le mot
            #print(s[debut:fin],fin)
            return s[debut:fin],fin

        if s[fin] == '"':
            raise SyntaxError("Fichier incorrect: \" au milieu d'un mot à "+str(debut))
        fin+=1
    #print(s[debut:fin],fin+1)
    return s[debut:fin],fin+1

# Parsing

# Lit le contenu d'attributs. Le [ initial a déjà été consommé. On commence à regarder à la position i
# Retourne un dictionnaire des attributs et la position à laquelle on est arrivé
def _attributs(s,i):
    nom,i = _mot(s,i)
    attributs = {}
    while nom != ']':
        if nom == "":
            raise SyntaxError("Fichier incorrect: pas de crochet fermant à "+str(i))
        if nom == ",":
            nom,i = _mot(s,i)
        egal,i = _mot(s,i)
        if egal != '=':
            raise SyntaxError("Fichier incorrect: trouvé "+egal+" au lieu d'un '=' à "+str(i))
        val,i = _mot(s,i)
        #print("attribut "+nom+" défini à "+val+" .")
        attributs[nom] = val
        nom,i = _mot(s,i)
    return attributs,i

def _litgrapheDOT(s,i):
    """ Convertit une définition de graphe en liste de chemins.
    
    Lit une définion de graphe, en commançant par son nom à la position i
    Retourne une liste de chemins et la nouvelle position"""
    chemins = []
    couleurs = []
    nom,i = _mot(s,i)
    if nom[0] == '"':
        nom = nom[1:-1]
    accolade,i = _mot(s,i)
    if accolade != "{":
        raise SyntaxError("Fichier incorrect: trouvé "+accolade+" au lieu d'une accolade ouvrante à "+str(i))

    mot,i = _mot(s,i)
    while mot != "}":
        # print("starting new read with "+mot+" "+str(i))
        if mot == "":
            raise SyntaxError("Fichier incorrect: pas d'accolade fermante terminale à la fin du fichier")

        if mot == "graph" or mot == "node" or mot == "edge":
            # attributs par défaut, on ignore
            crochet,j = _mot(s,i)
            if crochet != '[':
                raise SyntaxError("Fichier incorrect: trouvé "+crochet+" au lieu d'un crochet ouvrant à "+str(i)+' '+str(j))
            i = j
            attr,i = _attributs(s,i)
            if mot == "node" and "fillcolor" in attr:
                couleurDefautSommet = attr["fillcolor"]
            mot,i = _mot(s,i)

        elif mot == "subgraph":
            # récursion!
            # idéalement il faudrait séparer les espaces de noms de sommets
            _,chemins_sousgraphe,couleurs_sougraphe,i = _litgrapheDOT(s,i)
            couleurs =  couleurs + couleurs_sousgraphe
            chemins = chemins + chemins_sousgraphe
            mot,i = _mot(s,i)

        else:
            # Nom d'un sommet
            if mot[0] == '"':
                mot = mot[1:-1]
            mot2,i = _mot(s,i)
            chemins += [[mot]]
            if mot2 == '[':
                # attributs d'un nœud
                attr,i = _attributs(s,i)
                if "fillcolor" in attr:
                    couleurSommet = attr["fillcolor"]
                    couleurs += [(mot, couleurSommet)]
                mot,i = _mot(s,i)
            elif mot2 == '--' or mot2 == '->':
                # Un chemin
                chemin = [mot]
                mot = mot2
                while mot == '--' or mot == '->':
                    mot,i = _mot(s,i)
                    if mot[0] == '"':
                        mot = mot[1:-1]
                    chemin = chemin + [mot]
                    mot,i = _mot(s,i)
                chemins += [chemin]
                if mot == '[':
                    # attributs d'un chemin, ignore
                    attr,i = _attributs(s,i)
                    mot,i = _mot(s,i)
            elif mot2 == '=':
                # attribut par défaut
                mot3,i = _mot(s,i)
                if mot == 'fillcolor':
                    couleurDefautSommet = mot3
                mot,i = _mot(s,i)
            elif mot2 == ';':
                # Rien d'intéressant, en fait
                mot = mot2
            else:
                raise SyntaxError("Fichier non supporté: trouvé "+mot2+" à "+str(i))
        while mot == ';':
            mot,i = _mot(s,i)
    return nom,chemins,couleurs,i

def _litgrapheGML(s,i):
    chemins = []
    couleurs = []
    noms = {}
    graph,i = _mot(s,i)
    if graph != "graph":
        raise SyntaxError('Attendu "graph", trouvé '+graph+' à la place')
    mot,i = _mot(s,i)
    if mot != '[':
        raise SyntaxError('Attendu "[", trouvé '+mot+' à la place')
    mot,i = _mot(s,i)
    while mot != ']':
        if mot == "directed":
            oriente,i = _mot(s,i)
            mot,i = _mot(s,i)
        elif mot == "node":
            # un sommet
            mot,i = _mot(s,i)
            if mot != '[':
                raise SyntaxError('Attendu "[", trouvé '+mot+' à la place')
            mot,i = _mot(s,i)
            ID = -1
            nom = ""
            while mot != ']':
                if mot == 'id':
                    ID,i = _mot(s,i)
                    mot,i = _mot(s,i)
                elif mot == 'label':
                    nom,i = _mot(s,i)
                    if nom[0] == '"':
                        nom = nom[1:-1]
                    mot,i = _mot(s,i)
                elif mot == 'value' or mot == 'source':
                    # ignore
                    value,i = _mot(s,i)
                    mot,i = _mot(s,i)
                else:
                    raise SyntaxError('mot-clé de sommet '+mot+' non supporté à '+str(i))
            if nom == "":
                nom = str(ID)
            #print("sommet "+nom)
            noms[ID] = nom
            chemins += [[nom]]
            mot,i = _mot(s,i)
        elif mot == "edge":
            # une arête
            mot,i = _mot(s,i)
            if mot != '[':
                raise SyntaxError('Attendu "[", trouvé '+mot+' à la place')
            mot,i = _mot(s,i)
            src = ''
            dst = ''
            while mot != ']':
                if mot == 'source':
                    src,i = _mot(s,i)
                    mot,i = _mot(s,i)
                elif mot == 'target':
                    dst,i = _mot(s,i)
                    mot,i = _mot(s,i)
                elif mot == 'value':
                    value,i = _mot(s,i)
                    mot,i = _mot(s,i)
                else:
                    raise SyntaxError('mot-clé de sommet '+mot+' non supporté à '+str(i))
            if src == '':
                raise SyntaxError("source de l'arête manquante")
            if dst == '':
                raise SyntaxError("destination de l'arête manquante")
            #print("arête "+noms[src]+"-"+noms[dst])
            chemins += [[noms[src],noms[dst]]]
            mot,i = _mot(s,i)
        else:
            raise SyntaxError('mot-clé '+mot+' non supporté à '+str(i))

    return "graphe",chemins,couleurs,i

def _litgraphePAJ(s,i):
    chemins = []
    couleurs = []
    noms = {}

    mot,i = _mot(s,i)
    if mot != "Vertices":
            raise SyntaxError('Attendu Vertices, trouvé '+mot+' à la place')

    nbvert,i = _mot(s,i)

    mot,i = _mot(s,i)
    while mot != "*":
        # un sommet
        ID = mot
        nom,i = _mot(s,i)
        if nom[0] == '"':
            nom = nom[1:-1]
        noms[ID] = nom
        chemins += [[nom]]
        #print("sommet "+nom)
        mot,i = _mot(s,i)

    mot,i = _mot(s,i)
    if mot != "Edges" and mot != "Arcs":
            raise SyntaxError('Attendu Edges ou Arcs, trouvé '+mot+' à la place')

    mot,i = _mot(s,i)
    while mot != "":
        # une arête
        src = mot
        dst,i = _mot(s,i)
        chemins += [[noms[src],noms[dst]]]
        #print("arête "+noms[src]+"-"+noms[dst])
        mot,i = _mot(s,i)

    return "graphe",chemins,couleurs,i

# Parsing
def ouvrirGraphe(nom):
    f = open(nom)
    s = f.read()
    i = 0

    couleurDefautSommet = "white"

    graph,i = _mot(s,i)
    if graph == "Creator":
        # .gml format
        creator,i = _mot(s,i)
        nom,chemins,couleurs,i = _litgrapheGML(s,i)
    elif graph == "*":
        # .paj format
        nom,chemins,couleurs,i = _litgraphePAJ(s,i)
    else:
        if graph == "strict":
            graph,i = _mot(s,i)
        if graph == "digraph":
            oriente = True
        elif graph == "graph":
            oriente = False
        else:
            raise SyntaxError("Fichier graphe de type "+graph+" non supporté")

        nom,chemins,couleurs,i = _litgrapheDOT(s,i)
    #print("construction")
    g = construireGraphe(chemins, nom, oriente=oriente)
    #print("coloration")
    for (s,c) in couleurs:
        colorierSommet(sommetNom(g,s),c)
    #print("fini")
    return g

fig32 = construireGraphe (
    [ ['A', 'B', 'A', 'C', 'D'],
      ['B', 'B'], # boucle
      ['D', 'D', 'D'] # boucles
    ], "fig32")

# Construction ad hoc pour indiquer a Graphviz (algo 'neato')
# les longueurs des aretes
def _makePetersen ():
    g = construireGraphe (
        [ ['A', 'B', 'C', 'D', 'E', 'A'],
          ['a', 'c', 'e', 'b', 'd', 'a'],
          ['A', 'a'],
          ['B', 'b'],
          ['C', 'c'],
          ['D', 'd'],
          ['E', 'e']
        ], "Petersen")
    # Semicolons aid readability but are not required (dotguide.pdf)
    # start = germe du generateur aleatoire pour le placement initial des sommets
    # valeurs OK au CREMI [0, 12, 16, 18, 23, 24, 30, 33]
    # start = 4 interessant aussi
    if plateforme == 'Windows':
        g.drawopts = 'edge [len = 2]'
    else:
        g.drawopts = 'start = 23; edge [len = 2]'
    for i in range (5):
        s = sommetNumero (g, i)
        a = areteNumero (s, 2)
        a.drawopts = '[len = 1]'
    return g

Petersen = _makePetersen ()

Koenigsberg = construireGraphe (
    [ ['A', 'B', 'C', 'D', 'B', 'A', 'D'],
      ['B', 'C']
    ], "Koenigsberg")
Koenigsberg.drawopts = 'rankdir=LR'





# Construction en etoile: Paris, Nantes, Lyon, etc. sont les voisins de Lille
# -> dernier parametre = False
# Les etiquettes des aretes ne servent a rien mais ne mangent pas de pain
tgv2005 = construireGraphe (
    [["Lille",
      ["Paris", "1h00"], ["Nantes", "4h10"], ["Lyon", "2h50"],
      ["Bordeaux", "5h00"], ["Toulouse", "8h20"],
      ["Marseille", "4h30"], ["Montpellier", "4h40"]],
     ["Paris",
      ["Nantes", "2h00"], ["Lyon", "1h55"], ["Bordeaux", "2h55"],
      ["Marseille", "2h56"], ["Montpellier", "3h15"],
      ["Toulouse", "5h14"]],
     ["Nantes", ["Lyon", "4h20"], ["Marseille", "6h20"]],
     ["Lyon",
      ["Toulouse", "4h30"], ["Marseille", "1h20"],
      ["Montpellier", "1h45"]],
     ["Bordeaux", ["Toulouse", "2h10"]],
     ["Toulouse", ["Montpellier", "2h16"]],
     ["Strasbourg"]
    ], "tgv2005", chemins = False)


hypercubeDim3 = construireGraphe (
    [['v0', 'v1', 'v3', 'v2', 'v0', 'v4', 'v5', 'v6', 'v7', 'v4'], 
    ['v1', 'v5'],
    ['v3', 'v6'],
    ['v2', 'v7']], "hypercubeDim3")


# hypercubeDim3 = construireGraphe ([
#  ["v0", "v1"]
#  ["v0", "v2"]
#  ["v0", "v4"]
#  ["v1", "v5"]
#  ["v1", "v3"]
#  ["v2", "v3"]
#  ["v2", "v7"]
#  ["v3", "v6"]
#  ["v6", "v5"]
#  ["v5", "v4"]
#  ["v4", "v7"]
#  ["v7", "v6"]],
# "hypercubeDim3", chemins = False)



# Construction en etoile: Espagne, Belgique, etc. sont les voisins de France
# -> dernier parametre = False
# Attention: Graphviz ne supporte pas les lettres accentuees !
# Les numeros des couches font reference a l'algorithme 'dot'
Europe = construireGraphe ( [
    ["Portugal","Espagne"],     # couches 0 et 1
    ["France", "Espagne"],      # couche 2
    ["Belgique", "France"],     # couche 3
    ["Pays Bas", "Belgique"],   # couche 4
    ["Allemagne", "France", "Belgique", "Pays Bas"],    # couche 5
    ["Danemark", "Allemagne"],  # couche 6
    ["Pologne", "Allemagne"],   # couche 6
    ["Italie", "France"],       # couche 3
    ["Autriche", "Allemagne", "Italie"],    #couche 6
    ["Tchequie", "Pologne", "Allemagne", "Autriche"],   # couche 7
    # ce qui suit est la verite, mais 'dot' fait alors plonger
    # le Danemark au milieu du graphe, too bad
    ["Slovaquie", "Tchequie", "Autriche", "Pologne"],   # couche 8
    ["Hongrie", "Autriche", "Slovaquie"],   # couche 9
    ["Ukraine", "Pologne", "Slovaquie", "Hongrie"], # couche 10
    ["Roumanie", "Hongrie", "Ukraine"],     # couche 11
    ["Angleterre", "Pays de Galles", "Ecosse"],
    ["Irlande"],
    ["Finlande", "Suede", "Norvege"],
    ["Suede", "Norvege"] ], "Europe", chemins = False)

Europe.drawopts = 'rankdir=LR ratio=.5 node[shape=box style=rounded]'

listeGraphes = [tgv2005, fig32, Europe, Koenigsberg, Petersen, hypercubeDim3]

############# A partir d'ici graphes graphes parametres ###########
#############   construits sur des modeles reguliers    ###########

# Exemple: prefixer ([0, 1, 2, ...], 's') = ['s0', 's1', 's2', ...]
# Fonctionne aussi bien pour les listes de listes

# Attention: le premier sommet doit etre 's0'
# a cause de la fonction sommetNumero

def prefixer (paths, prefix):
    e = []
    for p in paths:
        if type (p) == type ([]):
            e.append (prefixer (p, prefix))
        else:
            e.append (prefix + str (p))
    return e

# Graphes complets

def _complet (n):
    a = []
    for i in range (1, n):
        a.append (list (range (i, -1, -1)))
    return a

def construireComplet (n):
    g = prefixer (_complet (n), 's')
    # le 'nom' d'un graphe (label) sert aussi de nom de fichier
    # pour les dessins, eviter les blancs, etc
    nom = 'K' + str (n)
    g = construireGraphe (g, nom, chemins = False)
    # pour l'algo 'neato' (mais le bon algo ici est 'circo')
    g.drawopts = 'edge [len = 2]'
    return g

# Graphes bipartis complets
def _biclique (m, n):
    a = []
    for i in range (m):
        a.append ([i] + list(range (m, m + n)))
    return a

def construireBipartiComplet (m, n):
    g = prefixer (_biclique (m, n), 's')
    # le 'nom' d'un graphe (label) sert aussi de nom de fichier
    # pour les dessins, eviter les blancs, etc
    nom = 'K' + str (m) + 'x' + str(n)
    g = construireGraphe (g, nom, chemins = False)
    # pour l'algo 'neato'
    g.drawopts = 'edge [len = 2]'
    return g

# Grilles

def _grille (m, n):
    lignes = []
    debut = 0
    for i in range (m):
        fin = debut + n
        lignes.append (list(range (debut, fin)))
        debut = fin
    for j in range (n):
        lignes.append (list(range (j, fin, n)))
    return lignes

def construireGrille (m, n):
    g = prefixer (_grille (m, n), 's')
    # le 'nom' d'un graphe (label) sert aussi de nom de fichier
    # pour les dessins, eviter les blancs, etc
    nom = 'grille' + str (m) + 'x' + str(n)
    g = construireGraphe (g, nom)
    return g

# Triangles

def _triangle (n):
    lignes = []
    debut = 0
    for i in range (n):
        fin = debut + i + 1
        u = list (range (debut, fin))
        lignes.append (u)
        debut = fin
    debut = -1
    for i in range (n - 1):
        u = []
        debut = debut + i + 1
        k = debut
        for j in range (i, n):
            u.append (k)
            k = k + j + 1
        lignes.append (u)
    debut = 0
    for i in range (n - 1):
        u = []
        debut = debut + i
        k = debut
        for j in range (i, n):
            u.append (k)
            k = k + j + 2
        lignes.append (u)
    return lignes

def construireTriangle (n):
    g = prefixer (_triangle (n), 's')
    # le 'nom' d'un graphe (label) sert aussi de nom de fichier
    # pour les dessins, eviter les blancs, etc
    nom = 'triangle' + str (n)
    g = construireGraphe (g, nom)
    # specifier le rapport hauteur / largeur pour l'algo 'dot'
    # perturbe legerement l'algo 'neato' 
    g.drawopts = "ratio=1.155"  # 2 / sqrt(3)
    return g

# Arbres (complets)

def _arbre (degre, hauteur, origine = 0):
    a = []
    if hauteur == 0:
        return []
    d = degre ** hauteur
    d = (d - 1) // (degre - 1)
    j = origine + 1
    for i in range (degre):
        a.append ([origine, j])
        a = a + _arbre (degre, hauteur - 1, j)
        j = j + d
    return a

def construireArbre (degre, hauteur):
    g = prefixer (_arbre (degre, hauteur), 's')
    # le 'nom' d'un graphe (label) sert aussi de nom de fichier
    # pour les dessins, eviter les blancs, etc
    nom = 'arbre' + str (degre) + 'x' + str(hauteur)
    g = construireGraphe (g, nom)
    return g

# Solides platoniciens

# autre description de K4
tetraedre = construireGraphe (
    [ ['A', 'B', 'C', 'A', 'D', 'B'], ['C', 'D'] ],
    'tetraedre')

# 6 faces, 8 sommets et 12 aretes
cube = construireGraphe (
    [ ['A', 'B', 'C', 'D', 'A'],
      ['a', 'b', 'c', 'd', 'a'],
      ['A', 'a'],
      ['B', 'b'],
      ['C', 'c'],
      ['D', 'd']
    ], 'cube')
cube.drawopts = 'edge [len = 2]'

octaedre = construireGraphe (
    [ ['A', 'B', 'C', 'D', 'A'],
      ['A', 'E', 'C', 'F', 'A'],
      ['B', 'E', 'D', 'F', 'B']
    ], 'octaedre')
octaedre.drawopts = 'edge [len = 2]'

# 12 faces, 20 sommets et 30 aretes
def _dodecaedre ():
    u = [list (range (20))]
    u = u + [[1, 9], [2, 11], [3, 13], [4, 0, 7]]
    u = u + [[5, 14], [6, 16], [8, 17], [10, 18], [12, 19, 15]]
    return u

dodecaedre = construireGraphe (
    prefixer (_dodecaedre (), 's'), 'dodecaedre')
dodecaedre.drawopts = 'edge [len = 2]'

def _icosaedre ():
    u = [list (range (12))]
    u.append ([2, 0, 3, 8, 11, 6, 1, 5, 0, 4, 9, 3])
    u = u + [[1, 7, 2, 8], [4, 10, 5], [6, 10], [7, 11, 9]]
    return u

icosaedre = construireGraphe (
    prefixer (_icosaedre (), 's'), 'icosaedre')
icosaedre.drawopts = 'edge [len = 2]'

graphesPlanairesReguliers = [tetraedre, cube, octaedre, dodecaedre, icosaedre]


print("graphes.py")
