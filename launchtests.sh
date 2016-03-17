#!/bin/bash

PathToClientTests="/home/travis/build/studnitz/octopus/tests/ClientTest"
PathToCommunicationTests="/home/travis/build/studnitz/octopus/tests/ClientServerTest"
PathToServerTests="/home/travis/build/studnitz/octopus/tests/ServerTest"

$PathToCommunicationTests/tst_communication
if [ $? = 0 ]; then
    echo "Keine Fehler in den Tests"
else
    exit -1
fi

$PathToClientTests/tst_clientbefehle
if [ $? = 0 ]; then
    echo "Keine Fehler in den Tests"
else
    exit -1
fi

$PathToServerTests/tst_serverbefehle
if [ $? = 0 ]; then
    echo "Keine Fehler in den Tests"
else
    exit -1
fi

exit 0
