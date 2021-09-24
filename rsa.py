import gmpy2
import random
int_value=212467898708

P= gmpy2.next_prime(random.randint(1,int_value)) #два произвольных натуральных числа
Q= gmpy2.next_prime(random.randint(1,int_value))
N=gmpy2.mul(P,Q)
J=gmpy2.mul((P-1),(Q-1)) #функция эйлера от числа N

while True:
    e=random.randint(1,J)
    if gmpy2.gcd(e,J)==1:
        
        break
    

d=gmpy2.divm(1,e,J) #находим обратный элемент e по модулю m
if d!=e: 
 
    print("Pub : ",  e , " , "  , N )
    print("Priv : ", d , " , " , N )
else:
    print("ключи небезопасны ")


