#include <iostream>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <cstring>
#include <vector>

#define BUFFER_SIZE 1024
#define XOR_KEY 0x5A // Clave para el cifrado XOR

void show_help() {
    std::cout << "Uso: filetool [opción] <archivo>\n"
              << "Opciones:\n"
              << "  -h, --help       Muestra este mensaje\n"
              << "  -v, --version    Muestra la versión del programa\n"
              << "  -e, --encrypt    Encripta el archivo\n"
              << "  -d, --decrypt    Desencripta el archivo\n";
}

void show_version() {
    std::cout << "filetool v1.0\n";
}

bool is_text_file(const std::string &filename) {
    int fd = open(filename.c_str(), O_RDONLY);
    if (fd < 0) {
        perror("Error al abrir el archivo");
        return false;
    }
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead = read(fd, buffer, BUFFER_SIZE);
    close(fd);
    if (bytesRead < 0) return false;
    for (ssize_t i = 0; i < bytesRead; ++i) {
        if (buffer[i] == 0) return false; // Si hay un byte nulo, probablemente sea binario
    }
    return true;
}

void process_file(const std::string &filename, bool encrypt) {
    int fd_in = open(filename.c_str(), O_RDONLY);
    if (fd_in < 0) {
        perror("Error al abrir el archivo de entrada");
        return;
    }
    

    // Verificamos si el archivo es de texto antes de procesarlo, ya que el cifrado XOR podría corromper archivos binarios.
    if (!is_text_file(filename)) {
        std::cerr << "Error: El archivo no parece ser de texto.\n";
        close(fd_in); 
        return;
    }

    std::string out_filename = filename + (encrypt ? ".enc" : ".dec");
    int fd_out = open(out_filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out < 0) {
        perror("Error al crear el archivo de salida");
        close(fd_in);
        return;
    }
    
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;
    while ((bytesRead = read(fd_in, buffer, BUFFER_SIZE)) > 0) {
        for (ssize_t i = 0; i < bytesRead; ++i) {
            buffer[i] ^= XOR_KEY; // Cifrado XOR
        }
        write(fd_out, buffer, bytesRead);
    }
    
    close(fd_in);
    close(fd_out);
    std::cout << "Archivo procesado: " << out_filename << "\n";
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        show_help();
        return 1;
    }
    
    std::string option = argv[1];
    if (option == "-h" || option == "--help") {
        show_help();
    } else if (option == "-v" || option == "--version") {
        show_version();
    } else if ((option == "-e" || option == "--encrypt") && argc == 3) {
        process_file(argv[2], true);
    } else if ((option == "-d" || option == "--decrypt") && argc == 3) {
        process_file(argv[2], false);
    } else {
        std::cerr << "Opción inválida o falta de argumentos.\n";
        show_help();
        return 1;
    }
    return 0;
}