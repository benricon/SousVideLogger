import socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('XXX.XXX.XXX.XXX', 8090)) #replace with correct IP address
s.listen(1)
conn, addr = s.accept()
while 1:
    data = conn.recv(1024)

    print(data)
conn.close()

