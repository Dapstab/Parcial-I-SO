all: execute

# Compilar cliente
compilarCliente: p2.client.c
	gcc -o client p2.client.c
	
# Compilar servidor: 
compilarServidor: p2.server.c
	gcc -o server p2.server.c
		
# Crear terminal y compila y ejecuta el servidor y cliente
execute: compilarServidor compilarCliente
	gnome-terminal -- ./server
	gnome-terminal -- ./client



	
clean: client server
	rm -r client server
