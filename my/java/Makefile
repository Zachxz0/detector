.Phony: compile run clean
start:proto compile

proto:
	protoc -I=../src/zoson/proto --java_out=./src/ ../src/zoson/proto/zoson.proto
	protoc -I=../src/zoson/proto --java_out=./src/ ../src/zoson/proto/sdk.proto

compile:
	./compile.sh
run:
	./run.sh

clean:
	echo "clean"
	rm -rf bin/*
	