#!/bin/sh

ps | grep $1 | while read p o
do
	echo $p
	kill -9 $p
done
