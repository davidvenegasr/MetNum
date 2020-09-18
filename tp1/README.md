# TP1  METODOS NUMERICOS -- GRUPO #17

Rankings Deportivos utilizando los métodos de Colley (CMM), Winning Percentage (WP) y Massey.

## Integrantes

- David Venegas
- Luis Bustamante
- Leandro Rodriguez

## Uso y Compilado

El Tp continen un ```Makefile``` para facilitar el compilado de tal forma que basta con escribir en consola: ```make``` para compilar usando g++. A su vez se puede usar ```make clean``` para borrar los binarios. Otra forma es hacer `python3 src/metnum.py build` para compilar el código con pytho

Una vez compilado se puede ejecutar el programa desde consola como:

`./tp1 [entrada] [salida] [método]`

donde metodo vale: 0 para CMM, 1 para WP, y 2 Massey.

## Analisis de Experimentos

En la carpeta ```data/``` se tienen los archivos de entrada y salida con las extensiones ```.in``` y ```.out``` respectivamente. Por otra parte se tiene la carpeta ```experimentos/``` donde se encuentra el jupyter notebook ```experimentos.ipynb``` el cual se puede ejecutar utilizando un entorno virtual e instalando las librerias adecuadas con: 

```
python3 -m venv env
source env/bin/activate
pip3 install -r requirements.txt 
```