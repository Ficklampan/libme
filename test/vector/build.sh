#!/bin/sh

g++ -c vector_test.cpp -I../../include -Og -g -std=c++20
g++ -o vector vector_test.o -L../../ -lme -lgtest
