#!/bin/sh

java -cp JSpace.jar:. TestProgram write double dgs 1282.2312
java -cp JSpace.jar:. TestProgram write int cbc 12
java -cp JSpace.jar:. TestProgram write String gffgd uw1
java -cp JSpace.jar:. TestProgram write char xnbx x
java -cp JSpace.jar:. TestProgram write byte qdfd 100
java -cp JSpace.jar:. TestProgram read char xnbx
java -cp JSpace.jar:. TestProgram read String gffgd
java -cp JSpace.jar:. TestProgram read int cbc
java -cp JSpace.jar:. TestProgram read double dgs
java -cp JSpace.jar:. TestProgram read byte qdfd
java -cp JSpace.jar:. TestProgram take double dgs
java -cp JSpace.jar:. TestProgram take byte qdfd
