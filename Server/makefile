run:bianyi ClockInServer
	./ClockInServer

bianyi:MyJson.cpp Json/jsoncpp.cpp Server.cpp DataBase.cpp
	g++ MyJson.cpp Json/jsoncpp.cpp Server.cpp DataBase.cpp main.cpp -o ClockInServer -L/usr/lib64/mysql -lmysqlclient -lpthread -ldl -lssl -lcrypto -lresolv -lm -lrt