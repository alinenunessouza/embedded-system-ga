import atexit
import time

pinos_alocados = []

def alocar_pino(pino):
        pinos_alocados.append(pino)
        with open("/sys/class/gpio/export", "w+") as arquivo:
            arquivo.write(str(pino))

def setar_direcao(pino, direcao):
    direcoes = ["out", "in"]
    if direcao in direcoes:
        with open(f"/sys/class/gpio/gpio{pino}/direction", "w+") as arquivo:
            arquivo.write(direcao)

def setar_valor(pino, valor):
    with open(f"/sys/class/gpio/gpio{pino}/value", "w+") as arquivo:
        arquivo.write(str(valor))

def desalocar_pino(pino):
    pinos_alocados.remove(pino)
    with open(f"/sys/class/gpio/unexport", "w+") as arquivo:
        arquivo.write(str(pino))

@atexit.register
def desalocar_todos_pinos():
    for pino in pinos_alocados:
        desalocar_pino(pino)

if __name__ == "__name__":
    alocar_pino(21)
    setar_direcao(21,"out")
    for i in range(20):
        setar_valor(21,1)
        time.sleep(2)
        setar_valor(21,0)
        time.sleep(2)