# Voltex Compiler

## Compilación

Para compilar el proyecto se debe ejecutar el siguiente comando, estando ubicado en la raíz del proyecto.

En Windows:
```sh
.\script\build.bat
```
Esto generará un archivo llamado "Compiler.sln" en la carpeta "bin" dentro del proyecto. 
Abrir el archivo y compilarlo. Por ejemplo usando Visual Studio Code

En Linux y WSL:

```sh
.\script\build.sh
```
En WSL:
```sh
script/build.sh
```
## Tests

Para correr los archivos de prueba se debe ejecutar el siguiente comando, estando ubicado en la raíz del proyecto.

En Windows:
```sh
.\script\test.bat
```

En Linux:
```sh
.\script\test.sh
```

En WSL:
```sh
script/test.sh
```
Al ejecutar el comando se desplegaran todos los archivos de prueba, tanto los que debe aceptar como los que debe rechazar y tambien se indica si efectivamente los rechaza o los acepta en cada caso.

## Caso de uso

Para generar el circuito en Latex se deben ejecutar los siguiente comandos, estando ubicado en la raíz del proyecto.

Primero se debe compilar el proyecto como se indicó anteriormente.

Luego se deben ejecutar los siguientes comandos:

En Windows:

>Importante: Para poder usar el comando "pdflatex", se debe instalar el programa MikTex, que puedes descargar desde el siguiente enlace: https://miktex.org/download. 
Asegúrate de seleccionar la opción "yes" al momento de indicarle que instale los paquetes necesarios mientras corre. 
Después de instalarlo, abre el programa e instala las actualizaciones.

```sh
./script/start.bat <path_al_programa>
```
```sh
pdflatex output.tex
```
Eso generará un archivo en formato pdf llamado "output.pdf" donde se modela el circuito

En Linux:

```sh
./script/start.sh <path_al_programa>
```
```sh
pdflatex output.tex
```
```sh
xdg-open output.pdf
```
En WSL:
>Importante: Para poder usar los comandos "pdflatex" se deben correr los siguientes comandos:
```sh

sudo apt install texlive-latex-base
```
```sh
sudo apt-get install texlive-fonts-recommended texlive-fonts-extra
```
Luego se deben correr los sigueinte comandos:
```sh
./script/start.sh <path_al_programa>
```
```sh
pdflatex output.tex
```
Eso generará un archivo en formato pdf llamado "output.pdf" donde se modela el circuito
