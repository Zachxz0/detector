.Phony: compile run clean
start:proto compile

proto:
	protoc -I=../src/zoson/module/proto --java_out=./src/ ../src/zoson/module/proto/zoson.proto

compile:
	./compile.sh
run:
	./run.sh

clean:
	echo "clean"
	rm -rf bin/*
	