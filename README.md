# Filetool

## Introducción

Filetool es una herramienta de línea de comandos diseñada para encriptar y desencriptar archivos de texto utilizando el cifrado XOR. Adicionalmente tambien permite comprimir y descomprimir archivos de texto usando el algoritmo RLE(Run-Length Encoding). Permite realizar estas operaciones de manera sencilla mediante argumentos específicos, asegurando la integridad de los archivos de texto sin afectar archivos binarios.

## Uso

```sh
./filetool [opción] <archivo>
```

Donde:  
- `filetool` es el nombre del programa.  
- `[opción]` es un argumento opcional que modifica el comportamiento del programa, como `-e` para cifrar o `-d` para descifrar. Si no se especifica una opción, el programa no sabrá qué acción ejecutar.  
- `<archivo>` es obligatorio e indica el archivo sobre el cual se aplicará la acción.  

Por ejemplo, para cifrar un archivo llamado `documento.txt` se usa:  
```sh
filetool -e documento.txt
```

### Opciones disponibles:

- `-h, --help` : Muestra un mensaje de ayuda.
- `-v, --version` : Muestra la versión del programa.
- `-e, --encrypt <archivo>` : Cifra un archivo de texto.
- `-d, --decrypt <archivo>` : Descifra un archivo previamente cifrado.
- `-c, --compress <archivo>` : Comprime un archivo de texto
- `-x, --decompress <archivo>` : Descomprime un archivo previamente compreso 

## Justificación de la detección del tipo de archivo

El programa incluye una función `process_file()` que incluye multiples verificaciones en el archivo de entrada asegurandose de que sea un archivo de texto .txt para que los algoritmos puedan funcionar correctamente.

Se implementa esta verificación para evitar que el cifrado XOR y la compresion RLE corrompan archivos no textuales. Los archivos de otros tipos pueden contener estructuras internas que, al ser modificadas por un cifrado simple como XOR o una compresion basica como RLE, podrían volverse inutilizables. Por este motivo, el programa solo permite procesar archivos de texto.

## Ejemplos de uso

### Cifrar un archivo de texto

```sh
filetool -e documento.txt
```

Esto genera un archivo `documento.txt.enc` con el contenido cifrado.

### Descifrar un archivo previamente cifrado

```sh
filetool -d documento.txt.enc
```

Esto genera un archivo `documento.txt.enc.dec` con el contenido original recuperado.

### Mostrar ayuda

```sh
filetool -h
```

### Mostrar versión

```sh
filetool -v
```

## Requisitos

Este programa está desarrollado en C y requiere un compilador compatible, como gcc.
En Linux y WSL (Windows Subsystem for Linux), gcc se puede instalar con sudo apt install gcc en distribuciones basadas en Debian (Ubuntu, Kali, etc.).
En Windows, si no se usa WSL, se recomienda instalar MinGW para obtener gcc.

## Compilación

Para compilar el programa, usa el siguiente comando:

```sh
g++ filetool.cpp -o filetool
```

## Autores

- Erick Ochoa
- Silvia Katheryn Cáceres Vásquez
- Santiago Palacio García
- Santiago Vélez Blandón





