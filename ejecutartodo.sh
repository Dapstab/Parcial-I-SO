#!/bin/bash

# Ejecutables
gcc archivos.c -o archivo
gcc memoria.c -o memoria
gcc pipe.c -o pipe
gcc mkfifo_rd.c -o  mkrd
gcc mkfifo_wr2.c -o  mkwr
gcc cliente.c -o cliente 
gcc server.c -o server 

# Array de KB a enviar
paquetes=("1024" "10240" "102400" "1048576" "10485760" "104857600")

# Array de ejecutables
ruta=( "./archivo" "./memoria" "./mkrd" "./pipe" "./server")

tipo=("archivos " "Memoria Compartida" "Tuberías mkfifo()" "Tuberías pipe()" "Paso de mensajes ")

for k in ${paquetes[@]};
do  
    contador=0
    echo "Tamaño paquete: $k"
    for j in ${ruta[@]}
    do  
        Y=0
        X=0
        n=5 # Num iteraciones
        for ((i=1;i<=$n;i++))
        do
            # Switch donde vinculamos el server con el client y mkrd con mkwr
            case $j in 
            ${ruta[2]})
            	# Abrimos una terminal extra para ejecutar los comandos respectivos (-e exit status for non-zero errors).
                x-terminal-emulator -e ./mkwr  $k &
            ;;
            ${ruta[4]})
                x-terminal-emulator -e ./cliente  $k &  
            ;;
            esac
            # Guardamos los resultados en Y.
            Y=$("$j" $k) 
            
            # Guardamos sumas parciales de cada iteración en X.
            X=$(echo "$X+$Y" | bc)
        done
        # Promedio
        X=$(echo "scale=6;$X/$n" | bc)
        echo "Tiempo promedio ${tipo[$contador]}: $X"
        contador=$((contador+1))
    done
    echo -e " "
done

