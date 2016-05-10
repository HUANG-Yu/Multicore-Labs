{\rtf1\ansi\ansicpg1252\cocoartf1404\cocoasubrtf340
{\fonttbl\f0\fnil\fcharset0 Monaco;\f1\fnil\fcharset0 Menlo-Regular;}
{\colortbl;\red255\green255\blue255;}
\paperw11900\paperh16840\margl1440\margr1440\vieww13780\viewh8400\viewkind0
\deftab720
\pard\pardeftab720\partightenfactor0

\f0\fs22 \cf0 ############################################################################\
 Yu HUANG\
 N18447876 \
 \
 Lab01 Generate Prime Numbers (C version)\
 03.21.16\
############################################################################\
0. Link to CIMS\
Source code runs on crunchy1 and using default GCC compiler\
\
1. Compilation command\
Unzip the archive. Use cd enter the relevant folder and run the following command.\
\
	$ 
\f1 \CocoaLigature0 gcc -g -Wall -fopenmp -o genprime genprimes.c -std=c99	// 
\f0 \CocoaLigature1 compiles the C source code\
	$ 
\f1 \CocoaLigature0 gcc -g -Wall -fopenmp -o genprimeopt genprimes_opt.c -std=c99 -lm
\f0 \CocoaLigature1 \
\
2. Running Program\
Run the following command:
\f1 \CocoaLigature0 \
	$ ./executable num num_of_thread}