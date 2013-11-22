#/bin/bash

count=0

while true
do

	telnet pets-keeper.com 8888 &
	echo $((count = count +1))
	#sleep 1
done
