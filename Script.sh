#!/bin/bash
clear
#Declaramos los colores de la salida estandar
red='\e[0;31m'
blue='\e[4;34m' #Subrayado
green='\e[0;32m'
yellow='\e[4;33m' #Subrayado
endColor='\e[0m'
#Mostramos el logo de inicio
echo -e "\t${green}_________               .__            ${endColor}"
echo -e "\t${green}\_   ___ \  ____   ____ |  |__   ____  	${endColor}"
echo -e "\t${green}/    \  \/ /  _ \_/ ___\|  |  \_/ __ \ ${endColor}"
echo -e "\t${green}\     \___(  <_> )  \___|   Y  \  ___/ ${endColor}"
echo -e "\t${green}\______  /\____/ \___  >___|  /\___  >${endColor}"
echo -e "\t${green}        \/            \/     \/     \/ ${endColor}"
echo -e "\t${green}_________                                     __  .__    .___      ${endColor}"
echo -e "\t${green}\_   ___ \  ____   _____ ___________ ________/  |_|__| __| _/____  ${endColor}"
echo -e "\t${green}/    \  \/ /  _ \ /      \____ \__   \_  __ \   __\  |/ __ |/  _ \ ${endColor}"
echo -e "\t${green}\     \___(  <_> )  Y Y  \  |_> > __ \|  | \/|  | |  / /_/ (  <_> )${endColor}"
echo -e "\t${green} \______  /\____/|__|_|  /   __(____  /__|   |__| |__\____ |\____/ ${endColor}"
echo -e "\t${green}        \/             \/|__|       \/                    \/       ${endColor}"
echo -e "\n\n${red}NOTA:Este script debe estar en el mismo directorio que el codigo fuente del programa.${endColor}\n"
while true
do
	#Mostramos el menu.
	echo -e "Seleciona una opcion:\n"
	echo -e "\t ${blue}1.Mostrar codido fuente.${endColor}"
	echo -e "\t ${blue}2.Compilar el codigo.${endColor}"
	echo -e "\t ${blue}3.Ejecutar el codigo.${endColor}"
	echo -e "\t ${yellow}4.Salir del script.${endColor}"
	#leemos la opcion que el usuario meta por pantalla
	read option
	case $option in
		1) clear
			less Cochecompartido.c  ;; #Mostramos el codigo.
		2) clear
			cc Cochecompartido.c -o Coche #Compilamos, mostramos un mensaje con el resultado de la compilacion
			  	if test $? -eq 0 
			  	then
					echo -e "\t${green}Compilacion realizada con exito${endColor}"
				else
					echo -e "\t${red}Error en la Compilacion${endColor}"
				fi 
				;;

		3) 	clear
			./Coche #Intentamos ejecutar, error 126 falta permisos, error 127 no se encuentra el archivo
			case $? in
				126) echo -e "\t${red}Este usuario no tiene permiso para ejecutar el programa. Intente \"chmod u=x Coche\" ${endColor}"	
					;;
			 	127) echo -e "\t${red}No se encuentra el archivo ejecutable, compile el codigo primero ${endColor}"	
					;;
			esac
			;; 
		4) break ;; #Interrumpimos el bucle while para salir
		99) clear
			less Script.sh ;; #Codigo fuente del script
		*) clear
			echo -e "\t${red}Opcion invalida. ${endColor}\n" ;;
	esac
done
exit 0 #El programa a finalizado correctamente
