#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <assert.h>
#include <consola/consola.h>
#include "globals/globals.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <utils/console.h>
#include <cspecs/cspec.h>
#include "semaforos_planificadores/semaforos_planificadores.h"

void compareFiles(FILE * fp1, FILE * fp2)
{
    int bufferLength = 255;
    char buffer1[bufferLength];
    char buffer2[bufferLength];

    char* fgets_status1 = fgets(buffer1, bufferLength, fp1); 
    char* fgets_status2 = fgets(buffer2, bufferLength, fp2); 
    while(fgets_status1 != NULL && fgets_status2 != NULL) 
    {
        
        char *cierre_parentesis1 = strchr(buffer1, ')');
        if (cierre_parentesis1 == NULL) 
        {
            // printf("No se encontró el cierre de paréntesis en la primera línea.\n");
            return;
        }

        // Encontrar el primer cierre de paréntesis en la segunda línea
        char *cierre_parentesis2 = strchr(buffer2, ')');
        if (cierre_parentesis2 == NULL) 
        {
            printf("No se encontró el cierre de paréntesis en la segunda línea.\n");
            return;
        }

        should_string (cierre_parentesis1+2) be equal to(cierre_parentesis2+2);
        fgets_status1 = fgets(buffer1, bufferLength, fp1); 
        fgets_status2 = fgets(buffer2, bufferLength, fp2); 
    }

    should_ptr (fgets_status1) be equal to(fgets_status2);

}

context (consola) {

    describe("Consola") {
        t_log * logger;

        before {
            fclose(fopen("logs/main_logger.log", "w")); // limpiar el archivo de logs en cada prueba
            logger = get_logger();
            iniciar_consola();
            inicializar_semaforos_pausa_planificador_corto();
            consola_con_censura(); // Esto es para que los logs no salgan en el testing
        } end

        after {
            destruir_semaforos_pausa_planificador_corto();
            consola_con_libre_expresion(); // Esto hace que la consola vuelva a la normaliad
        } end

        it("debería loguear un output específico para cada input que el usuario escriba") {
            
            sem_init(&semaforo_consola, 0, 0); 
            // Esto es para "simular" el input de un usario
            int stdin_fd = dup(STDIN_FILENO);
            int user_input_fd = open("tests/consola/consola_test_input", O_RDONLY); // consola_test_input tiene lo que el usuario "escribe"
            dup2(user_input_fd, STDIN_FILENO);
            close(user_input_fd);
            
            sem_wait(&semaforo_consola);                        
            destroy_logger(logger);
            FILE *logs = fopen("logs/main_logger.log", "r"); // output real de consola
            assert(logs != NULL);
            FILE *expected_logs = fopen("tests/consola/expected_logs", "r"); // output esperado de consola 
            assert(expected_logs != NULL);
            
            // Restore original stdout and stdin
            dup2(stdin_fd, STDIN_FILENO);
            close(stdin_fd);
            
            compareFiles(logs, expected_logs); // comparar ambos archivos
            fclose(logs);
            fclose(expected_logs);
            
            
        } end

    } end

}