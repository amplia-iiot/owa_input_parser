# OWA INPUT ADAPTER
This C program is made for adapt the analog and digital I/O format of OWA450 device, which requires the use of the 
libraries provided by Owasys to access the data, to the sysfs format, which is the interface that ODA expects and use
in all devices.

_Este programa de C sirve para adaptar el formato de E/S analógicas y digitales del dispositivo OWA450, que requiere 
de usar las bibliotecas proporcionadas por Owasys para acceder a los datos, al formato sysfs, que es la interfaz que 
ODA espera y utiliza en todos los dispositivos._

## USE OF ADAPTER
Using this program is as easy as do doing make in the directory that contains it, passing the resulting program into 
OWA and run it.

_Usar este programa es tan fácil como hacer make en el directorio que lo contiene, pasar el programa resultante en el 
OWA y correrlo._

## CONFIGURATION
OWA Adapter can be configured on polling ratio, which is the time that program expects before run the next iteration. This function is handled by a sleep command before to close the current run.
It's expected that configuration file is saved on '/opt/owa_adapter_config.txt' with the next format:

_El Adaptador OWA puede ser configurado en el ratio de polling, que es el tiempo que espera el programa antes de correr la siguiente iteración. Esta función se maneja a través de un comando sleep antes de acabar la ejecución actual._
_Se espera que el fichero de configuración esté guardado en '/opt/owa_adapter_config.txt' con el siguiente formato:_

    pollRatio=<time_in_seconds>