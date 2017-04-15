PROJECT_PATH=$(cd "$(dirname "$0")";pwd)
JAR_PATH=$PROJECT_PATH/lib
BIN_PATH=$PROJECT_PATH/bin

#find jar
for i in $JAR_PATH/*.jar;do
	LIB_SET="$i:$LIB_SET"
done

#java -classpath $BIN_PATH com.zoson.detection.MainForTest
java -Djava.library.path=./lib -classpath $BIN_PATH:$LIB_SET com.zoson.detection.MainForTest