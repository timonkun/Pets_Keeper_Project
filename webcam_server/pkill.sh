#!/bin/bash

ps | grep $1 | while read pid others
do
	echo "kill $pid"
	kill -9 $pid
done

