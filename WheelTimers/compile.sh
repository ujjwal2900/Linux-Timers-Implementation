#!/bin/bash

gcc -g -c LinkedListApi.c -o LinkedListApi.o
gcc -g -c wheelTimer.c -o wheelTimer.o
gcc -g -c wheelTimerTest.c -o wheelTimerTest.o
gcc -g LinkedListApi.o wheelTimer.o wheelTimerTest.o -o wheelTimerTest -lpthread