#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Paciente {
    char codigo[10];
    char nome[50];
    char endereco[100];
    char telefone[20];
    char dataNascimento[10];
};
struct Medico {
    char codigo[10];
    char nome[50];
    char telefone[20];
    char especialidade[15];
};
struct Consulta {
    char codigoConsulta[10];
    char codigoMedico[10];
    char codigoPaciente[10];
    char data[10];
    char hora[5];
};
typedef struct Paciente paciente;
typedef struct Medico medico;
typedef struct Consulta consulta;
int main()
{
    return 0;
}
