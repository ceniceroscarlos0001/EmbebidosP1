import java.io.*;
import java.nio.file.*;
import java.util.*;

public class TiempoJava {

    // GPIO utilizados para generar número
    static int[] gpios = {27, 22, 10, 9, 11, 5, 6, 13};

    public static void main(String[] args) {
        configurarGPIOs();   // Configura GPIOs
        leerValor();         // Lee valores y guarda en archivos
        cerrarGPIOs();       // Cierra GPIOs al final
    }

    private static void configurarGPIOs() {
        for (int gpio : gpios) {
            System.out.println("Exportando GPIO número " + gpio);
            try {
                Files.write(Paths.get("/sys/class/gpio/export"), String.valueOf(gpio).getBytes());
                Thread.sleep(1000);
                Files.write(Paths.get("/sys/class/gpio/gpio" + gpio + "/direction"), "in".getBytes());
            } catch (Exception e) {
                System.out.println("Error al configurar GPIO " + gpio + ": " + e.getMessage());
            }
        }
    }

    private static void cerrarGPIOs() {
        for (int gpio : gpios) {
            System.out.println("Cerrando el GPIO " + gpio);
            try {
                Files.write(Paths.get("/sys/class/gpio/unexport"), String.valueOf(gpio).getBytes());
            } catch (Exception e) {
                System.out.println("Error al cerrar GPIO " + gpio + ": " + e.getMessage());
            }
        }
    }

    private static void leerValor() {
        try (BufferedWriter numeroWriter = new BufferedWriter(new FileWriter("numeroJava.txt", true));
             BufferedWriter tiempoWriter = new BufferedWriter(new FileWriter("tiempoJava.txt", true))) {

            for (int i = 1; i <= 10000; i++) {
                long currentTimeNano = System.currentTimeMillis() * 1_000_000L +
                                       (System.nanoTime() % 1_000_000L);

                tiempoWriter.write(currentTimeNano + "\n");

                int[] bits = new int[8];
                for (int j = 0; j < 8; j++) {
                    String valuePath = "/sys/class/gpio/gpio" + gpios[j] + "/value";
                    try {
                        bits[j] = Integer.parseInt(Files.readString(Paths.get(valuePath)).trim());
                    } catch (Exception e) {
                        System.out.println("Error leyendo gpio " + gpios[j] + ": " + e.getMessage());
                        bits[j] = 0;
                    }
                }

                int numero = bits[0] + bits[1]*2 + bits[2]*4 + bits[3]*8 +
                             bits[4]*16 + bits[5]*32 + bits[6]*64 + bits[7]*128;

                numeroWriter.write(numero + "\n");
            }

        } catch (IOException e) {
            System.out.println("Error escribiendo en archivo: " + e.getMessage());
        }
    }
}
