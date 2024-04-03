all: Package.pb.cc

Package.pb.cc:
	protoc --cpp_out=. Package.proto

clean:
	rm -rf Package.pb.cc Package.pb.h