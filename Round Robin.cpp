#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Proceso {
    char nombre;
    int llegada;
    int cpu_original; // Duracion original del proceso
    int cpu;          // Duracion restante del proceso
    int instante;
    int t_fin;
    int t_e;
    int t_r;
    double penalizacion;
};

// Funcion de comparacion para qsort
int compararProcesos(const void *a, const void *b) {
    struct Proceso *procesoA = (struct Proceso *)a;
    struct Proceso *procesoB = (struct Proceso *)b;

    // Primero ordenar por tiempo de llegada
    if (procesoA->llegada < procesoB->llegada) {
        return -1;
    } else if (procesoA->llegada > procesoB->llegada) {
        return 1;
    } else {
        // En caso de empate en tiempo de llegada, ordenar alfabeticamente por nombre
        return procesoA->nombre - procesoB->nombre;
    }
}

int main() {
	
	printf("+------------------------------------------------------+\n");
    printf("|                 Round Robin                          |\n");
    printf("+------------------------------------------------------+\n");
    
    int num_procesos;
    printf("Ingrese el numero de procesos: ");
    scanf("%d", &num_procesos);

    int quantum;
    printf("Ingrese el quantum: ");
    scanf("%d", &quantum);

    struct Proceso procesos[num_procesos];

    // Ingresar informacion de los procesos
    for (int i = 0; i < num_procesos; i++) {
        printf("Ingrese la hora de llegada del proceso %d: ", i + 1);
        scanf("%d", &procesos[i].llegada);
        printf("Ingrese la duracion del proceso %d: ", i + 1);
        scanf("%d", &procesos[i].cpu_original); // Guardamos la duracion original
        procesos[i].cpu = procesos[i].cpu_original; // Inicializamos la duracion restante igual a la original
        procesos[i].nombre = 'A' + i;  // Asignar nombres A, B, C, ...
        procesos[i].instante = -1; // Inicializamos el instante en -1
        procesos[i].t_fin = -1; // Inicializamos el tiempo de finalizacion en -1
        procesos[i].t_e = 0; // Inicializamos t_e en 0
        procesos[i].t_r = 0; // Inicializamos t_r en 0
        procesos[i].penalizacion = 0; // Inicializamos la penalizacion en 0
    }

    // Ordenar los procesos por hora de llegada
    qsort(procesos, num_procesos, sizeof(struct Proceso), compararProcesos);

    int tiempo_actual = 0;
    int proceso_actual = 0;
    int procesos_terminados = 0;
    double penalizacion_total = 0;

    while (procesos_terminados < num_procesos) {
        struct Proceso *p = &procesos[proceso_actual];

        if (p->cpu > 0 && p->llegada <= tiempo_actual) {
            if (p->instante == -1) {
                p->instante = tiempo_actual;
                p->t_e = p->instante - p->llegada;
                p->t_r = p->t_e + p->cpu;
                // Evitar division por 0
                if (p->t_e == 0) {
                    p->penalizacion = 0;
                } else {
                    p->penalizacion = (double)p->t_r / p->t_e;
                }
            }
            if (p->cpu <= quantum) {
                tiempo_actual += p->cpu;
                p->t_fin = tiempo_actual;
                p->cpu = 0;
                procesos_terminados++;
            } else {
                tiempo_actual += quantum;
                p->cpu -= quantum;
            }
        }

        proceso_actual = (proceso_actual + 1) % num_procesos;

        if (proceso_actual == 0) {
            // Si hemos recorrido todos los procesos, volvemos a verificar si hay procesos pendientes
            int todos_terminados = 1;
            for (int i = 0; i < num_procesos; i++) {
                if (procesos[i].cpu > 0) {
                    todos_terminados = 0;
                    break;
                }
            }

            if (todos_terminados) {
                break;
            }
        }
    }
	
	printf("+------------------------------------------------------+\n");
    printf("|      Los procesos pueden estar desordenados          |\n");
    printf("+------------------------------------------------------+\n");
    printf("+---------+---------+-----+----------+-------+------+-------+--------------+\n");
    printf("| proceso | llegada | cpu | instante | t.fin | t.e  |  t.r  | penalizacion |\n");
    printf("+---------+---------+-----+----------+-------+------+-------+--------------+\n");

    for (int i = 0; i < num_procesos; i++) {
        if (procesos[i].t_fin != -1) {
            printf("|    %c    |    %2d   |  %2d |    %3d   |  %3d  |  %2d  |  %3d  |   %.8f |\n",
                   procesos[i].nombre, procesos[i].llegada, procesos[i].cpu_original,
                   procesos[i].instante, procesos[i].t_fin, procesos[i].t_e, procesos[i].t_r, procesos[i].penalizacion);
            penalizacion_total += procesos[i].penalizacion;
        }
    }

    printf("+---------+---------+-----+----------+-------+------+-------+--------------+\n");

    // Calcular y mostrar el promedio de penalizacion
    double promedio_penalizacion = penalizacion_total / num_procesos;

    // Reemplazar "INF" en el promedio con 0
    if (isnan(promedio_penalizacion)) {
        promedio_penalizacion = 0;
    }

    printf("Promedio de penalizacion: %.8f\n", promedio_penalizacion);

    return 0;
}

//IG:rodrigoruiz_11
