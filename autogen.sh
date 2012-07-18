#!/bin/sh

# TODO put this into scons

python eventc_autogen.py Thread1_pri.h Thread1_autogen.c Thread1_autogen.h
python eventc_autogen.py Thread2_pri.h Thread2_autogen.c Thread2_autogen.h
python eventc_autogen.py Thread_Main_pri.h Thread_Main_autogen.c Thread_Main_autogen.h

