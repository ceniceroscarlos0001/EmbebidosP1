#include <gpiod.h>    // <-- Nueva librería
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CHIPNAME "/dev/gpiochip0" // Normalmente es gpiochip0 en Raspberry Pi
#define NUM_GPIOS 8

int gpios[NUM_GPIOS] = {4, 17, 27, 22, 5, 6, 13, 26}; // Tu arreglo de pines

void calcular_valor() {
    struct gpiod_chip *chip;
    struct gpiod_line_bulk bulk;
    struct gpiod_line *lines[NUM_GPIOS];
    FILE *fp_numero = fopen("numeroC.txt", "a");
    FILE *fp_tiempo = fopen("tiempoC.txt", "a");

    if (fp_numero == NULL || fp_tiempo == NULL) {
        perror("Error al abrir los archivos");
        return;
    }

    // Abrimos el chip
    chip = gpiod_chip_open(CHIPNAME);
    if (!chip) {
        perror("Error al abrir el chip GPIO");
        fclose(fp_numero);
        fclose(fp_tiempo);
        return;
    }

    // Pedimos las líneas
    for (int i = 0; i < NUM_GPIOS; i++) {
        lines[i] = gpiod_chip_get_line(chip, gpios[i]);
        if (!lines[i]) {
            fprintf(stderr, "Error al obtener la línea GPIO %d\n", gpios[i]);
            gpiod_chip_close(chip);
            fclose(fp_numero);
            fclose(fp_tiempo);
            return;
        }
    }

    // Agrupamos
    gpiod_line_bulk_init(&bulk);
    gpiod_line_bulk_add_array(&bulk, lines, NUM_GPIOS);

    // Configuramos las líneas como entrada
    if (gpiod_line_request_bulk_input(&bulk, "leer_gpio") < 0) {
        perror("Error al configurar las líneas como entrada");
        gpiod_chip_close(chip);
        fclose(fp_numero);
        fclose(fp_tiempo);
        return;
    }

    // Empezamos a leer
    for (int i = 0; i < 10000; i++) {
        struct timespec start, end;
        int valores[NUM_GPIOS];
        int numero = 0;

        clock_gettime(CLOCK_MONOTONIC, &start);

        if (gpiod_line_get_value_bulk(&bulk, valores) < 0) {
            perror("Error al leer los valores");
            break;
        }

        // Reconstruimos el número
        for (int j = 0; j < NUM_GPIOS; j++) {
            numero += valores[j] << j;
        }

        fprintf(fp_numero, "%d\n", numero);

        clock_gettime(CLOCK_MONOTONIC, &end);
        long long tiempo = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
        fprintf(fp_tiempo, "Tiempo total: %lld nanosegundos\n", tiempo);
    }

    gpiod_chip_close(chip);
    fclose(fp_numero);
    fclose(fp_tiempo);
}

int main(int argc, char *argv[]) {
    calcular_valor();
    return 0;
}
