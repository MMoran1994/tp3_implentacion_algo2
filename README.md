# Trabajo práctico de implementación

## Objetivo
Poner a prueba conocimientos sobre OOB mediante la especificación y luego 
implementación de estructuras de datos abstractas vistas a lo largo de la materia. Para ello se realizó una implementación simplificada del videojuego [Extreme Exorcism](https://www.youtube.com/watch?v=yqgM4GEYlds).

A continuación se deja una copia de la sección enunciado del tp extraído del archivo del mismo nombre (`enunciado.pdf`):


# Enunciado

Este TP consiste en implementar en C++ todos los módulos correspondientes al juego especificado en el [TP1](tp2/tp1.pdf) y diseñado en el [TP2](tp2/enunciado.pdf).
El código debería respetar el diseño elegido en el TP 2 de la manera más fiel posible. Obviamente se permite y
se espera que corrijan todos los potenciales bugs que puedan llegar a encontrar en el diseño así como adaptaciones
requeridas durante la corrección del TP 2.


# Para los tests

## Compilación

1. `mkdir build`
2. `cd build`
3. `cmake ..`
4. `make`

## Ejecución

Desde el inicio del proyecto:

1. `cd build`
2. `./bin/correrTests


# Para la interfaz gráfica

## Dependencias

Se necesitan dependencias que pueden ser instaladas en Ubuntu de la siguiente
manera:

    sudo apt install libboost-iostream-dev libboost-filesystem-dev libsdl2-dev libsdl2-image-dev

## Compilación

Si ya se armó la carpeta `build`, es necesario borrarla:

    rm -r build

Luego:

1. `mkdir build`
2. `cd build`
3. `cmake -DWITH-GUI=ON ..`
4. `make`

## Ejecución

Desde el inicio del proyecto:

1. `cd build/bin`
2. `./ExtremeExorcism`

# Sobre la interfaz gráfica

El mapa se obtiene a partir del archivo `resources/maps/m1.txt` donde los `.` son los espacios
en blanco y los `#` son las paredes. El archivo puede modificarse para crear mapas nuevos. Es necesario recompilar el 
juego para que se actualice. Alternativamente se puede copiar el archivo de texto a `build/bin/resources/maps`.

En la versión actual, el juego soporta un solo jugador que se mueve son la siguiente disposición de teclas:
```
  I  
J K L
```

Y dispara con la tecla `U`.
Se puede salir del juego con la tecla `Q`.

