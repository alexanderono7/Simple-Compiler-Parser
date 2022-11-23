# Alexander Ono
# amono1@asu.edu
# SID: 1214959876
# CSE340 - Dr. Bazzi
# Project #1 - Regex Parser
# 9/24/2022

.SILENT:
build:
	g++ -std=c++11 -g -ggdb -Wall -Wno-sign-compare execute.cc inputbuf.cc lexer.cc parser.cc other.cc -o a.out 
demo:
	g++ -std=c++11 -g -ggdb -Wall execute.cc inputbuf.cc lexer.cc demo.cc -o a.out 