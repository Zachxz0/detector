PROJECT_PATH=$(cd "$(dirname "$0")";pwd)
JAR_PATH=$PROJECT_PATH/lib
BIN_PATH=$PROJECT_PATH/bin
SRC_PATH=$PROJECT_PATH/src
JNI_PATH=$PROJECT_PATH/jni

JAR_DEP=""

#find java files
rm -f $SRC_PATH/sources.list
find $SRC_PATH/ -name *.java > $SRC_PATH/sources.list

#find jar
for i in $JAR_PATH/*.jar;do
	LIB_SET="$i:$LIB_SET"
done

JNI_FILE=com.zoson.detection.jni.DetectorJni

#compile
javac -d $BIN_PATH -classpath $LIB_SET @$SRC_PATH/sources.list
javah -d $JNI_PATH -classpath $BIN_PATH $JNI_FILE

#complie .so
cd jni
make
cd ..

# # 4. find class files
# for f in `cd ./bin&&find . -name "*.class"`;do
#   classfiles="$f $classfiles"
# done

# # 5. package class files to jar
# cd ./bin && jar -cvf $curPath/$targetjarName $classfiles

# # 6. find resource files
# for f in `cd $curPath/resources && find . -name "*"`;do
#   resfiles="$f $resfiles"
# done

# # 7. add resources to jar
# cd $curPath/resources && jar -uf $curPath/$targetjarName $resfiles
