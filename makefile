main : main.cpp Logger.cpp Timestamp.cpp TcpServer.cpp HttpServer.cpp Util.cpp Protocol.cpp  threadPool.cpp threadPool11.cpp taskQueue.cpp
	g++ main.cpp Logger.cpp Timestamp.cpp TcpServer.cpp HttpServer.cpp Util.cpp Protocol.cpp  threadPool.cpp threadPool11.cpp  taskQueue.cpp -o main  -lpthread

.PHONY :
clean :
	rm main