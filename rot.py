import codecs
import enchant
j=0
text=[]
lenth=0
d = enchant.Dict("en_US")

alph='abcdefghijklmnopqrstuvwxyz'
symb=["-", ",", ".", ":", ";", "-", "+", "_", "?", "/","|" ]
def rot(i, line, alph):
    encode_line=[]
    for char in line:
        if char in alph:    
            n=alph.index(char)
            new_index=n-i
            while new_index < 0:
                new_index= 26 + new_index
            encode_line.append(alph[new_index])
        else:
            encode_line.append(char)
    return encode_line




for i in range(1, 52):
    file=open("txt.txt", 'r')
    for line in file:
        string=rot(i, line, alph)
        text.append("".join(string))
    for lines in text:
        word=lines.split()
        
        lenth+=len(word)
        for value in word:
            if (d.check(value) or (value in symb))==True :
                j+=1  
    
    if j == lenth :
        print("".join(text), " ", i)
    text=[]
    j=0
    file.close()
    lenth=0
    
