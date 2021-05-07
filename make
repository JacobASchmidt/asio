#!/usr/bin/env python3
from libmake import File, make, extension, name
import os
import sys
FLAGS = ["-O3", "-Wall", "-Wextra"]


def compile_(compiler, file, *, output, flags=[""]):
    return f"{compiler} {file.path()}  -o {output} {' '.join(flags)}"


CC = "gcc"

src = [
    File(directory="src",
         name=name(f),
         extension=".c")
    for f in os.scandir("src") if extension(f) == ".c"
]

obj = [
    File(directory="obj",
         name=f.name,
         extension=".o",
         dependencies=[f],
         make=lambda self: compile_(CC, f, output=self.path(), flags=["-c", *FLAGS]))
    for f in src
]
asm = [
    File(directory="asm",
         name=f.name,
         extension=".s",
         dependencies=[f],
         make=lambda self: compile_(CC, f, output=self.path(), flags=["-S", *FLAGS]))
    for f in src
]

lib = [
    File(directory="lib",
         name="libjasio",
         extension=".a",
         dependencies=obj,
         make=lambda self: f"ar rcs {self.path()} {' '.join(f.path() for f in obj)}")
]


def clean():
    for f in asm:
        try:
            os.remove(f.path())
        except:
            continue
    for f in obj:
        try:
            os.remove(f.path())
        except:
            continue
    for f in lib:
        try:
            os.remove(f.path())
        except:
            continue


cmd = {
    "lib": lambda: make(*lib),
    "asm": lambda: make(*asm),
    "obj": lambda: make(*obj),
    "clean": clean
}
if len(sys.argv) > 1:
    for arg in sys.argv[1:]:
        cmd[arg]()
else:
    make(*lib, *asm)
