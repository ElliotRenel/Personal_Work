def genere_relou50v_2():
    print("digraph \"oriente_relou_50v_2\" {")
    for i in range(5):
        for j in range(5):
            print("\"aa\" -> \"a%d%d\";"%(i,j))
        print("\"a%d0\" -> \"b%d0\";"%(i,i))
        for j in range(4):
            print("\"a%d%d\" -> \"b%d%d\";"%(i,j+1,i,j))
        for j in range(3):
            print("\"b%d%d\" -> \"b%d%d\";"%(i,j,i,j+1))     
        print()
    print("\"b03\" -> \"c0\";")
    for i in range(4):
        print("\"b%d3\" -> \"c%d\";"%(i+1,i))
    for i in range(3):
        print("\"c%d\" -> \"c%d\";"%(i,i+1))     
    print("}")


def genere_relou50v_3():
    print("digraph \"oriente_relou_50v_3\" {")
    for i in range(5):
        for j in range(5):
            print("\"aa\" -> \"a%d%d\";"%(i,j))
        print("\"a%d0\" -> \"b%d0\";"%(i,i))
        print("\"a%d1\" -> \"b%d0\";"%(i,i))
        print("\"a%d2\" -> \"b%d1\";"%(i,i))
        print("\"a%d3\" -> \"b%d1\";"%(i,i))
        print("\"a%d4\" -> \"b%d3\";"%(i,i))
        print("\"b%d0\" -> \"b%d2\";"%(i,i))
        print("\"b%d1\" -> \"b%d2\";"%(i,i))
        print("\"b%d2\" -> \"b%d3\";"%(i,i))    
        print()
    print("\"b03\" -> \"c0\";")
    for i in range(4):
        print("\"b%d3\" -> \"c%d\";"%(i+1,i))
    for i in range(3):
        print("\"c%d\" -> \"c%d\";"%(i,i+1))     
    print("}")
    
genere_relou50v_3()
