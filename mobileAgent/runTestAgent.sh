#!/bin/sh
# $1 = ip1 $2 = ip2

java -cp Mobile.jar Mobile.Inject localhost 62955 MyTestAgent uw1-320-09 uw1-320-10 uw1-320-11
