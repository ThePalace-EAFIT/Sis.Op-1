#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024
#define XOR_KEY 0x5A 

void show_help() {
    std::cout << "Uso: filetool [opción] <archivo>\n"
              << "Opciones:\n"
              << "  -h, --help         Muestra este mensaje\n"
              << "  -v, --version      Muestra la versión del programa\n"
              << "  -e, --encrypt      Encripta el archivo\n"
              << "  -d, --decrypt      Desencripta el archivo\n"
              << "  -c, --compress     Comprime el archivo\n"
              << "  -x, --decompress   Desencripta el archivo\n";
}

void show_version() {
    std::cout << "filetool v1.0\n";
}

void process_file(const std::string &filename, bool encrypt) {
    int fd_in = open(filename.c_str(), O_RDONLY);
    if (fd_in < 0) {
        perror("Error al abrir el archivo de entrada");
        return;
    }

    if (encrypt && filename.size() > 4 && filename.substr(filename.size() - 4) == ".enc") {
        std::cerr << "Error: El archivo ya está encriptado\n";
        close(fd_in);
        return;
    }

    if (encrypt) {
        if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".txt") {
            std::cerr << "Error: Solo se pueden encriptar archivos .txt\n";
            return;
        }
    }

    if (!encrypt) {
        if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".enc") {
            std::cerr << "Error: Solo se pueden desencriptar archivos .enc\n";
            return;
        }
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
            buffer[i] ^= XOR_KEY; 
        }
        write(fd_out, buffer, bytesRead);
    }

    close(fd_in);
    close(fd_out);
    std::cout << "Archivo procesado: " << out_filename << "\n";
}

void compress_file(const std::string &filename) {
    int fd_in = open(filename.c_str(), O_RDONLY);
    if (fd_in < 0) {
        perror("Error al abrir el archivo de entrada");
        return;
    }

    if (filename.size() > 4 && filename.substr(filename.size() - 4) == ".rle") {
        std::cerr << "Error: El archivo ya está comprimido\n";
        close(fd_in);
        return;
    }

    std::string out_filename = filename + ".rle";
    int fd_out = open(out_filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out < 0) {
        perror("Error al crear el archivo de salida");
        close(fd_in);
        return;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;
    std::string compressed_data;

    while ((bytesRead = read(fd_in, buffer, BUFFER_SIZE)) > 0) {
        for (ssize_t i = 0; i < bytesRead; i++) {
            char current = buffer[i];
            int count = 1;

            while (i + 1 < bytesRead && buffer[i + 1] == current) {
                count++;
                i++;
            }

            compressed_data += current;
            compressed_data += std::to_string(count);
        }

        write(fd_out, compressed_data.c_str(), compressed_data.size());
        compressed_data.clear();  // Limpiar el buffer para la próxima lectura
    }

    close(fd_in);
    close(fd_out);
    std::cout << "Archivo comprimido: " << out_filename << "\n";
}

void decompress_file(const std::string &filename) {
    int fd_in = open(filename.c_str(), O_RDONLY);
    if (fd_in < 0) {
        perror("Error al abrir el archivo de entrada");
        return;
    }

    if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".rle") {
        std::cerr << "Error: Solo se pueden descomprimir archivos .rle\n";
        close(fd_in);
        return;
    }

    std::string out_filename = filename.substr(0, filename.size() - 4) + ".dec";
    int fd_out = open(out_filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out < 0) {
        perror("Error al crear el archivo de salida");
        close(fd_in);
        return;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;
    std::string decompressed_data;

    while ((bytesRead = read(fd_in, buffer, BUFFER_SIZE)) > 0) {
        for (ssize_t i = 0; i < bytesRead; i++) {
            char current = buffer[i];
            std::string count_str;

            while (i + 1 < bytesRead && std::isdigit(buffer[i + 1])) {
                count_str += buffer[++i];  // Concatenar los dígitos de la cantidad
            }

            int count = std::stoi(count_str);
            decompressed_data.append(count, current);  // Expandir el carácter
        }

        write(fd_out, decompressed_data.c_str(), decompressed_data.size());
        decompressed_data.clear();  // Limpiar buffer
    }

    close(fd_in);
    close(fd_out);
    std::cout << "Archivo descomprimido: " << out_filename << "\n";
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
    } else if ((option == "-c" || option == "--compress") && argc == 3) {
        compress_file(argv[2]);
    } else if ((option == "-x" || option == "--descompress") && argc == 3) {
        decompress_file(argv[2]);
    } else {
        std::cerr << "Opción inválida o falta de argumentos.\n";
        show_help();
        return 1;
    }
    return 0;
}