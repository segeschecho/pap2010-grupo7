
import random
import string

#genero 25 mil strings aleatorios
palabras = 25000
largo = 16

random.seed(14)
for i in range(palabras):
    s = "".join( [random.choice(string.lowercase) for x in range(largo)] )
    print s
    