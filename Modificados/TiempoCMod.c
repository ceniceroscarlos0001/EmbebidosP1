#include <gpiod.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CHIPNAME "/dev/gpiochip0"
#define NUM_GPIOS 8

int gpios[NUM_GPIOS] = {27, 22, 10, 9, 11, 5, 6, 13};

void calcular_valor() {
    struct gpiod_chip *chip;
    struct gpiod_line_bulk bulk;
    struct gpiod_line *lines[NUM_GPIOS];
    FILE *fp_numero = fopen("amplitudC.txt", "a");
    FILE *fp_tiempo = fopen("tiempoC.txt", "a");

    if (fp_numero == NULL || fp_tiempo == NULL) {
        perror("Error al abrir los archivos");
        return;
    }

    chip = gpiod_chip_open(CHIPNAME);
    if (!chip) {
        perror("Error al abrir el chip GPIO");
        fclose(fp_numero);
        fclose(fp_tiempo);
        return;
    }

    gpiod_line_bulk_init(&bulk);

    for (int i = 0; i < NUM_GPIOS; i++) {
        lines[i] = gpiod_chip_get_line(chip, gpios[i]);
        if (!lines[i]) {
            fprintf(stderr, "Error al obtener la línea GPIO %d\n", gpios[i]);
            gpiod_chip_close(chip);
            fclose(fp_numero);
            fclose(fp_tiempo);
            return;
        }
        gpiod_line_bulk_add(&bulk, lines[i]);
    }

    if (gpiod_line_request_bulk_input(&bulk, "leer_gpio") < 0) {
        perror("Error al configurar las líneas como entrada");
        gpiod_chip_close(chip);
        fclose(fp_numero);
        fclose(fp_tiempo);
        return;
    }

    for (int i = 0; i < 10000; i++) {
        struct timespec timestamp;
        int valores[NUM_GPIOS];
        int numero = 0;

        // Obtener el tiempo como en el script de shell (CLOCK_REALTIME)
        clock_gettime(CLOCK_REALTIME, &timestamp);
        long long marca_tiempo = (long long)timestamp.tv_sec * 1000000000LL + timestamp.tv_nsec;
        fprintf(fp_tiempo, "%lld\n", marca_tiempo);

        for (int j = 0; j < NUM_GPIOS; j++) {
            valores[j] = gpiod_line_get_value(lines[j]);
            if (valores[j] < 0) {
                perror("Error al leer el valor del GPIO");
                gpiod_chip_close(chip);
                fclose(fp_numero);
                fclose(fp_tiempo);
                return;
            }
        }

        for (int j = 0; j < NUM_GPIOS; j++) {
            numero += valores[j] << j;
        }

        fprintf(fp_numero, "%d\n", numero);
    }

    gpiod_chip_close(chip);
    fclose(fp_numero);
    fclose(fp_tiempo);
}

int main(int argc, char *argv[]) {
    calcular_valor();
    return 0;
}
