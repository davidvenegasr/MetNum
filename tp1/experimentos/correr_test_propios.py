#!/usr/bin/python
import unittest
from sys import argv

#modificamos el path
import sys, os
sys.path.append(os.path.abspath(os.path.join('..', '')))


from scripts.fabricate import *
from scripts.settings import *
from scripts.utils import listfiles

# Acciones
def build():
  compile()
  link()

def compile():
  for source in sources:
    run(compiler, '-std=c++11', '-O2', '-c', source+'.cpp', '-o', source+'.o')

def link():
  objects = [s+'.o' for s in sources]
  run(compiler, '-o', executable, objects)

def clean():
  autoclean()


def test():
  build()
  print(argv[:1])
  unittest.main(module='scripts.tptests', exit=False, argv=argv[:1], verbosity=3)

if __name__ == "__main__":
  test()