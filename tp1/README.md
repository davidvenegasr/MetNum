# TP1  METODOS NUMERICOS -- GRUPO #17

Rankings Deportivos utilizando los métodos de Colley (CMM), Winning Percentage (WP) y Massey.

## Integrantes

- David Venegas
- Luis Bustamante
- Leandro Rodriguez

## Uso y Compilado

El Tp continen un ```Makefile``` para facilitar el compilado de tal forma que basta con escribir en consola: ```make``` para compilar usando g++. A su vez se puede usar ```make clean``` para borrar los binarios. Otra forma es hacer `python3 src/metnum.py [opcion]` donde opcion puede ser:

compile: Compila todos los *.cpp que hayan en el directorio.
link: Genera el ejecutable en base a los *.o generados previamente.
build: compile + link
clean: borra los *.o y el ejecutable.
test: hace el build, busca lo archivos *.in en la carpeta tests/, ejecuta el programa y guarda el
resultado para cada corrida en el correspondiente .out. Despues, chequea que el resultado sea el "mismo" que el .expected. En este caso, la comparacion es por tolerancia coordenada a coordeanda del vector solucion.

Una vez compilado se puede ejecutar el programa desde consola como:

`./tp1 [entrada] [salida] [método]`

donde metodo vale: 0 para CMM, 1 para WP, y 2 Massey.

## Analisis de Experimentos

 En la carpeta ```data/``` se tienen los archivos de entrada y salida con las extensiones ```.in``` y ```.out``` respectivamente. Por otra parte se tiene la carpeta ```experimentos/``` donde se encuentra el jupyter notebook ```experimentos.ipynb```con el análisis de datos correspondiente y la generación de los gráficos utilizados en el informe. Es cuaderno se puede ejecutar utilizando un entorno virtual e instalando las librerias adecuadas con: 

```
python3 -m venv env
source env/bin/activate
pip3 install -r requirements.txt 
```