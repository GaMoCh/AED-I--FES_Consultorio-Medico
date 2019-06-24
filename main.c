#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <math.h>
#include <time.h>
#include <conio.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>

#define FOLDER "dados/"
#define EXTENSION ".dat"
#define DADOS_CONSULTAS FOLDER"consultas"EXTENSION
#define DADOS_MEDICOS FOLDER"medicos"EXTENSION
#define DADOS_PACIENTES FOLDER"pacientes"EXTENSION

#define QUANTIDADE_LAUDOS 10

#define TAMANHO_DATA 10
#define TAMANHO_ENDERECO 50
#define TAMANHO_ESPECIALIDADE 14
#define TAMANHO_HORARIO 5
#define TAMANHO_LAUDO 25
#define TAMANHO_NOME 50
#define TAMANHO_TELEFONE 13

#define MENSAGEM_DATA "Data (Ex.: 01/01/2020): "
#define MENSAGEM_DATA_DE_NACIMENTO "Data de nascimento (Ex.: 01/01/1970): "
#define MENSAGEM_ENDERECO "Endereço: "
#define MENSAGEM_ESPECIALIDADE "Especialidade\n  0 - Cardiologia\n  1 - Cermatologia\n  2 - Clínica médica\nOpção: "
#define MENSAGEM_HORARIO "Horário (Ex.: 15:05): "
#define MENSAGEM_NOME "Nome: "
#define MENSAGEM_NOME_DO_MEDICO "Nome do medico: "
#define MENSAGEM_NOME_DO_PACIENTE "Nome do paciente: "
#define MENSAGEM_TELEFONE "Telefone (Ex.: 31 98765-4321): "

typedef struct tm Momento;

typedef struct tipo_medico {
    unsigned int codigo;
    char nome[TAMANHO_NOME + 1];
    char telefone[TAMANHO_TELEFONE + 1];
    char especialidade[TAMANHO_ESPECIALIDADE + 1];
} Medico;

typedef struct tipo_paciente {
    unsigned int codigo;
    char nome[TAMANHO_NOME + 1];
    char endereco[TAMANHO_ENDERECO + 1];
    char telefone[TAMANHO_TELEFONE + 1];
    char data_de_nascimento[TAMANHO_DATA + 1];
} Paciente;

typedef struct tipo_consulta {
    unsigned int codigo;
    char data[TAMANHO_DATA + 1];
    char horario[TAMANHO_HORARIO + 1];
    unsigned int codigo_do_medico;
    unsigned int codigo_do_paciente;
    char laudo[TAMANHO_LAUDO + 1];
    bool cancelada;
    Momento momento;
} Consulta;

void menu_principal(void);
void menu_relatorios(void);
void menu_laudos(void);
void pausar(void);

void mostrar_mensagem_de_informacao_invalida(void);
void mostrar_mensagem_de_informacao_nao_informada(void);

void cadastrar_medico(FILE *dados_medicos);
void cadastrar_paciente(FILE *dados_pacientes);
void cadastrar_consulta(FILE *dados_consultas, FILE *dados_medicos, FILE *dados_pacientes);
void cancelar_consulta(FILE *dados_consultas, FILE *dados_medicos, FILE *dados_pacientes);
void exibir_relatorios(FILE *dados_consultas, FILE *dados_medicos, FILE *dados_pacientes);
void exibir_consultas_pela_data(FILE *dados_consultas, FILE *dados_medicos, FILE *dados_pacientes);
void exibir_consultas_realizadas_pelo_paciente(FILE *dados_consultas, FILE *dados_medicos, FILE *dados_pacientes);
void exibir_consultas_do_medico(FILE *dados_consultas, FILE *dados_medicos, FILE *dados_pacientes);
void exibir_laudos_medicos(FILE *dados_consultas, FILE *dados_medicos, FILE *dados_pacientes);
void exibir_laudos_pelo_medico(FILE *dados_consultas, FILE *dados_medicos, FILE *dados_pacientes, const char LAUDOS[3][QUANTIDADE_LAUDOS][TAMANHO_LAUDO + 1]);
void exibir_laudos_pelo_paciente(FILE *dados_consultas, FILE *dados_medicos, FILE *dados_pacientes, const char LAUDOS[3][QUANTIDADE_LAUDOS][TAMANHO_LAUDO + 1]);

Medico receber_medico_pelo_codigo(FILE *dados_medicos, unsigned int codigo);
Medico receber_medico_pelo_nome(FILE *dados_medicos, char *nome);
Paciente receber_paciente_pelo_codigo(FILE *dados_pacientes, unsigned int codigo);
Paciente receber_paciente_pelo_nome(FILE *dados_pacientes, char *nome);

Momento receber_momento(char *data, char *horario, unsigned int segundos);

unsigned int receber_quantidades_de_consultas_do_medico(FILE *dados_consultas, char *data, unsigned int codigo);

bool verificar_existencia_do_medico(FILE *dados_medicos, char *nome);
bool verificar_existencia_do_paciente(FILE *dados_pacientes, char *nome);

bool verificar_existencia_de_consulta_proxima(FILE *dados_consultas, Momento momento);

bool verificar_se_data_ja_passou(char *data, bool incluirHoje);
bool verificar_se_momento_ja_passou(Momento momento);

bool verificar_data(const char *data);
bool verificar_especialidade(const char *especialidade);
bool verificar_horario(const char *horario);
bool verificar_telefone(const char *telefone);
bool verificar_se_e_numero(const char *numero);

int main(void) {
    char opcao = '0';

    FILE *dados_medicos;
    FILE *dados_pacientes;
    FILE *dados_consultas;

    setlocale(LC_ALL, "portuguese");
    system("title Clínica Viva Bem");
    srand(time(NULL));

    if (access("dados", F_OK) == -1) system("mkdir dados");

    dados_consultas = fopen(DADOS_CONSULTAS, "rb+");
    if (dados_consultas == NULL) dados_consultas = fopen(DADOS_CONSULTAS, "wb+");

    dados_medicos = fopen(DADOS_MEDICOS, "rb+");
    if (dados_medicos == NULL) dados_medicos = fopen(DADOS_MEDICOS, "wb+");

    dados_pacientes = fopen(DADOS_PACIENTES, "rb+");
    if (dados_pacientes == NULL) dados_pacientes = fopen(DADOS_PACIENTES, "wb+");

    do {
        system("cls");
        printf("Clínica Viva Bem\n\n");
        menu_principal();

        printf("\nOpção: ");
        opcao = getche();

        switch(opcao - '0') {
            case 0:
                fclose(dados_medicos);
                fclose(dados_pacientes);
                fclose(dados_consultas);
                break;
            case 1:
                cadastrar_medico(dados_medicos);
                getch();
                break;
            case 2:
                cadastrar_paciente(dados_pacientes);
                getch();
                break;
            case 3:
                cadastrar_consulta(dados_consultas, dados_medicos, dados_pacientes);
                getch();
                break;
            case 4:
                cancelar_consulta(dados_consultas, dados_medicos, dados_pacientes);
                getch();
                break;
            case 5:
                exibir_relatorios(dados_consultas, dados_medicos, dados_pacientes);
                break;
            case 6:
                exibir_consultas_do_medico(dados_consultas, dados_medicos, dados_pacientes);
                getch();
                break;
            case 7:
                exibir_laudos_medicos(dados_consultas, dados_medicos, dados_pacientes);
                break;
            default:
                printf("\n\nOpção inválida.");
                pausar();
                break;
        }

        if (opcao == '0') {
            system("cls");
            break;
        };

    } while (true);

    return 0;
}

void menu_principal(void) {
    printf("0 - Sair\n");
    printf("1 - Cadastrar médico\n");
    printf("2 - Cadastrar paciente\n");
    printf("3 - Cadastrar consulta\n");
    printf("4 - Cancelar consulta\n");
    printf("5 - Exibir relatórios\n");
    printf("6 - Exibir consultas de um médico\n");
    printf("7 - Exibir laudos médicos\n");
}

void menu_relatorios(void) {
    printf("0 - Voltar\n");
    printf("1 - Consultas de uma determinada data\n");
    printf("2 - Consultas realizadas de um determinado paciente\n");
}

void menu_laudos(void) {
    printf("0 - Voltar\n");
    printf("1 - Laudos de um determinado médico\n");
    printf("2 - Laudos de um determinado paciente\n");
}

void pausar(void) {
    printf("\nPrecione qualquer tecla para inserir novamente...");
    getch();
}

void mostrar_mensagem_de_informacao_invalida(void) {
    printf("\nInformação inválida.");
    pausar();
}

void mostrar_mensagem_de_informacao_nao_informada(void) {
    printf("\nInformação não foi informada.");
    pausar();
}

void cadastrar_medico(FILE *dados_medicos) {
    Medico medico = {0, "", "", ""};

    while (true) {
        system("cls");
        printf("Cadastro de médicos\n\n");

        !strlen(medico.nome)
            ? printf(MENSAGEM_NOME)
            : printf(MENSAGEM_NOME"%s\n", medico.nome);
        if (!strlen(medico.nome)) gets(medico.nome);
        if (!strlen(medico.nome)) {
            mostrar_mensagem_de_informacao_nao_informada();
            continue;
        }
        if (verificar_existencia_do_medico(dados_medicos, medico.nome)) {
            printf("\nEsse nome já foi cadastrado para algum médico.");
            pausar();
            strcpy(medico.nome, "");
            continue;
        }

        !strlen(medico.telefone)
            ? printf(MENSAGEM_TELEFONE)
            : printf(MENSAGEM_TELEFONE"%s\n", medico.telefone);
        if (!strlen(medico.telefone)) gets(medico.telefone);
        if (!strlen(medico.telefone)) {
            mostrar_mensagem_de_informacao_nao_informada();
            continue;
        }
        if (!verificar_telefone(medico.telefone)) {
            mostrar_mensagem_de_informacao_invalida();
            strcpy(medico.telefone, "");
            continue;
        }

        !strlen(medico.especialidade)
            ? printf(MENSAGEM_ESPECIALIDADE)
            : printf(MENSAGEM_ESPECIALIDADE"%s\n", medico.especialidade);
        if (!strlen(medico.especialidade)) gets(medico.especialidade);
        if (!strlen(medico.especialidade)) {
            mostrar_mensagem_de_informacao_nao_informada();
            continue;
        }
        if (!verificar_especialidade(medico.especialidade)) {
            mostrar_mensagem_de_informacao_invalida();
            strcpy(medico.especialidade, "");
            continue;
        }

        switch (medico.especialidade[0]) {
            case '0':
                strcpy(medico.especialidade, "Cardiologia");
                break;
            case '1':
                strcpy(medico.especialidade, "Dermatologia");
                break;
            case '2':
                strcpy(medico.especialidade, "Clínica médica");
                break;
            default:
                break;
        }

        break;
    }

    printf("\nMédico cadastrado com sucesso.");

    fseek(dados_medicos, 0, SEEK_END);
    medico.codigo = ftell(dados_medicos) / sizeof(medico);
    fwrite(&medico, sizeof(medico), 1, dados_medicos);
    fflush(dados_medicos);
}

void cadastrar_paciente(FILE *dados_pacientes) {
    Paciente paciente = {0, "", "", "", ""};

    while (true) {
        system("cls");
        printf("Cadastro de pacientes\n\n");

        !strlen(paciente.nome)
            ? printf(MENSAGEM_NOME)
            : printf(MENSAGEM_NOME"%s\n", paciente.nome);
        if (!strlen(paciente.nome)) gets(paciente.nome);
        if (!strlen(paciente.nome)) {
            mostrar_mensagem_de_informacao_nao_informada();
            continue;
        }
        if (verificar_existencia_do_paciente(dados_pacientes, paciente.nome)) {
            printf("\nEsse nome já foi cadastrado para algum paciente.");
            pausar();
            strcpy(paciente.nome, "");
            continue;
        }

        !strlen(paciente.endereco)
            ? printf(MENSAGEM_ENDERECO)
            : printf(MENSAGEM_ENDERECO"%s\n", paciente.endereco);
        if (!strlen(paciente.endereco)) gets(paciente.endereco);
        if (!strlen(paciente.endereco)) {
            mostrar_mensagem_de_informacao_nao_informada();
            continue;
        }

        !strlen(paciente.telefone)
            ? printf(MENSAGEM_TELEFONE)
            : printf(MENSAGEM_TELEFONE"%s\n", paciente.telefone);
        if (!strlen(paciente.telefone)) gets(paciente.telefone);
        if (!strlen(paciente.telefone)) {
            mostrar_mensagem_de_informacao_nao_informada();
            continue;
        }
        if (!verificar_telefone(paciente.telefone)) {
            mostrar_mensagem_de_informacao_invalida();
            strcpy(paciente.telefone, "");
            continue;
        }

        !strlen(paciente.data_de_nascimento)
            ? printf(MENSAGEM_DATA_DE_NACIMENTO)
            : printf(MENSAGEM_DATA_DE_NACIMENTO"%s\n", paciente.data_de_nascimento);
        if (!strlen(paciente.data_de_nascimento)) gets(paciente.data_de_nascimento);
        if (!strlen(paciente.data_de_nascimento)) {
            mostrar_mensagem_de_informacao_nao_informada();
            continue;
        }
        if (!verificar_data(paciente.data_de_nascimento)) {
            mostrar_mensagem_de_informacao_invalida();
            strcpy(paciente.data_de_nascimento, "");
            continue;
        }
        if (!verificar_se_data_ja_passou(paciente.data_de_nascimento, false)) {
            mostrar_mensagem_de_informacao_invalida();
            strcpy(paciente.data_de_nascimento, "");
            continue;
        }

        break;
    }

    printf("\nPaciente cadastrado com sucesso.");

    fseek(dados_pacientes, 0, SEEK_END);
    paciente.codigo = ftell(dados_pacientes) / sizeof(paciente);
    fwrite(&paciente, sizeof(paciente), 1, dados_pacientes);
    fflush(dados_pacientes);
}

void cadastrar_consulta(FILE *dados_consultas, FILE *dados_medicos, FILE *dados_pacientes) {
    Consulta consulta = {0, "", "", 0, 0, "", false, {0}};
    Medico medico;
    Paciente paciente;

    char nome_do_medico[TAMANHO_NOME + 1] = "";
    char nome_do_paciente[TAMANHO_NOME + 1] = "";

    while (true) {
        system("cls");
        printf("Cadastro de consultas\n\n");

        !strlen(consulta.data)
            ? printf(MENSAGEM_DATA)
            : printf(MENSAGEM_DATA"%s\n", consulta.data);
        if (!strlen(consulta.data)) gets(consulta.data);
        if (!strlen(consulta.data)) {
            mostrar_mensagem_de_informacao_nao_informada();
            continue;
        }
        if (!verificar_data(consulta.data)) {
            mostrar_mensagem_de_informacao_invalida();
            strcpy(consulta.data, "");
            continue;
        }
        if (verificar_se_data_ja_passou(consulta.data, true)) {
            mostrar_mensagem_de_informacao_invalida();
            strcpy(consulta.data, "");
            continue;
        }

        !strlen(consulta.horario)
            ? printf(MENSAGEM_HORARIO)
            : printf(MENSAGEM_HORARIO"%s\n", consulta.horario);
        if (!strlen(consulta.horario)) gets(consulta.horario);
        if (!strlen(consulta.horario)) {
            mostrar_mensagem_de_informacao_nao_informada();
            continue;
        }
        if (!verificar_horario(consulta.horario)) {
            mostrar_mensagem_de_informacao_invalida();
            strcpy(consulta.horario, "");
            continue;
        }

        consulta.momento = receber_momento(consulta.data, consulta.horario, 0);
        if (verificar_se_momento_ja_passou(consulta.momento)) {
            mostrar_mensagem_de_informacao_invalida();
            strcpy(consulta.data, "");
            strcpy(consulta.horario, "");
            continue;
        }
        if (verificar_existencia_de_consulta_proxima(dados_consultas, consulta.momento)) {
            pausar();
            strcpy(consulta.data, "");
            strcpy(consulta.horario, "");
            continue;
        }

        !strlen(nome_do_medico)
            ? printf(MENSAGEM_NOME_DO_MEDICO)
            : printf(MENSAGEM_NOME_DO_MEDICO"%s\n", nome_do_medico);
        if (!strlen(nome_do_medico)) gets(nome_do_medico);
        if (!strlen(nome_do_medico)) {
            mostrar_mensagem_de_informacao_nao_informada();
            continue;
        }
        if (verificar_existencia_do_medico(dados_medicos, nome_do_medico)) {
            medico = receber_medico_pelo_nome(dados_medicos, nome_do_medico);
            consulta.codigo_do_medico = medico.codigo;
        } else {
            printf("\nNão foi encontrado um médico com esse nome.");
            pausar();
            strcpy(nome_do_medico, "");
            continue;
        }

        if (receber_quantidades_de_consultas_do_medico(dados_consultas, consulta.data, medico.codigo) >= 2) {
            printf("\nO médico já possui 2 consultas agendadas na data informada.");
            pausar();
            strcpy(consulta.data, "");
            strcpy(consulta.horario, "");
            strcpy(nome_do_medico, "");
            continue;
        }

        !strlen(nome_do_paciente)
            ? printf(MENSAGEM_NOME_DO_PACIENTE)
            : printf(MENSAGEM_NOME_DO_PACIENTE"%s\n", nome_do_paciente);
        if (!strlen(nome_do_paciente)) gets(nome_do_paciente);
        if (!strlen(nome_do_paciente)) {
            mostrar_mensagem_de_informacao_nao_informada();
            continue;
        }
        if (verificar_existencia_do_paciente(dados_pacientes, nome_do_paciente)) {
            paciente = receber_paciente_pelo_nome(dados_pacientes, nome_do_paciente);
            consulta.codigo_do_paciente = paciente.codigo;
        } else {
            printf("\nNão foi encontrado um paciente com esse nome.");
            pausar();
            strcpy(nome_do_paciente, "");
            continue;
        }

        printf("\nConsulta cadastrada com sucesso.");

        fseek(dados_consultas, 0, SEEK_END);
        consulta.codigo = ftell(dados_consultas) / sizeof(consulta);
        fwrite(&consulta, sizeof(consulta), 1, dados_consultas);
        fflush(dados_consultas);

        break;
    }
}

void cancelar_consulta(FILE *dados_consultas, FILE *dados_medicos, FILE *dados_pacientes) {
    Consulta consulta;
    Medico medico;
    Paciente paciente;

    bool existe_consulta_agendada = false;
    bool existe_consulta_com_o_codigo = false;
    char codigo[11] = "";
    unsigned int posicao = 0;

    while (true) {
        system("cls");

        printf("Cancelamento de consulta\n\n");

        fseek(dados_consultas, 0, SEEK_SET);
        fread(&consulta, sizeof(consulta), 1, dados_consultas);

        while (!feof(dados_consultas)) {
            medico = receber_medico_pelo_codigo(dados_medicos, consulta.codigo_do_medico);
            paciente = receber_paciente_pelo_codigo(dados_pacientes, consulta.codigo_do_paciente);

            if (!verificar_se_momento_ja_passou(consulta.momento) && !consulta.cancelada) {
                existe_consulta_agendada = true;

                printf("Código: %d\n", consulta.codigo);
                printf("Data: %s\n", consulta.data);
                printf("Horário: %s\n", consulta.horario);
                printf("Especialidade: %s\n", medico.especialidade);
                printf("Médico: %s\n", medico.nome);
                printf("Paciente: %s\n", paciente.nome);
                printf("\n");
            }

            fread(&consulta, sizeof(consulta), 1, dados_consultas);
        }

        if (!existe_consulta_agendada) printf("Não existe consulta agendada.");
        else {
            printf("\nCancelar consulta com o código: ");
            gets(codigo);
            if (!strlen(codigo)) {
                mostrar_mensagem_de_informacao_nao_informada();
                continue;
            }
            if (!verificar_se_e_numero(codigo)) {
                mostrar_mensagem_de_informacao_invalida();
                strcpy(codigo, "");
                continue;
            }

            fseek(dados_consultas, 0, SEEK_SET);
            fread(&consulta, sizeof(consulta), 1, dados_consultas);

            while (!feof(dados_consultas) && !existe_consulta_com_o_codigo) {
                if (consulta.codigo == atoi(codigo) && verificar_se_e_numero(codigo)) {
                    existe_consulta_com_o_codigo = true;

                    if (consulta.cancelada) printf("\nConsulta com esse código já foi cancelada.");
                    else if (verificar_se_momento_ja_passou(consulta.momento)) printf("\nConsulta com esse código já foi realizada.");
                    else {
                        consulta.cancelada = true;
                        printf("\nConsulta cancelada com sucesso.");
                    }

                    fseek(dados_consultas, sizeof(consulta) * posicao, SEEK_SET);
                    fwrite(&consulta, sizeof(consulta), 1, dados_consultas);
                    fflush(dados_consultas);
                }

                posicao++;
                fread(&consulta, sizeof(consulta), 1, dados_consultas);
            }

            if (!existe_consulta_com_o_codigo) printf("\nNão existe uma consulta com esse código.");
        }

        break;
    }
}

void exibir_relatorios(FILE *dados_consultas, FILE *dados_medicos, FILE *dados_pacientes) {
    char opcao = '0';

    do {
        system("cls");
        printf("Exibição de relatórios\n\n");
        menu_relatorios();

        printf("\nOpção: ");
        opcao = getche();

        switch(opcao - '0') {
            case 0:
                break;
            case 1:
                exibir_consultas_pela_data(dados_consultas, dados_medicos, dados_pacientes);
                break;
            case 2:
                exibir_consultas_realizadas_pelo_paciente(dados_consultas, dados_medicos, dados_pacientes);
                break;
            default:
                printf("\n\nOpção inválida.");
                pausar();
                break;
        }

        if (opcao == '0') break;

    } while (true);
}

void exibir_consultas_pela_data(FILE *dados_consultas, FILE *dados_medicos, FILE *dados_pacientes) {
    Consulta consulta;
    Medico medico;
    Paciente paciente;

    char data[TAMANHO_DATA + 1] = "";
    bool existe_consulta = false;

    while (true) {
        system("cls");
        printf("Exibição de consultas pela data\n\n");

        !strlen(data)
            ? printf(MENSAGEM_DATA)
            : printf(MENSAGEM_DATA"%s\n", data);
        if (!strlen(data)) gets(data);
        if (!strlen(data)) {
            mostrar_mensagem_de_informacao_nao_informada();
            continue;
        }
        if (!verificar_data(data)) {
            mostrar_mensagem_de_informacao_invalida();
            strcpy(data, "");
            continue;
        }

        break;
    }

    fseek(dados_consultas, 0, SEEK_SET);
    fread(&consulta, sizeof(consulta), 1, dados_consultas);

    while (!feof(dados_consultas)) {
        medico = receber_medico_pelo_codigo(dados_medicos, consulta.codigo_do_medico);
        paciente = receber_paciente_pelo_codigo(dados_pacientes, consulta.codigo_do_paciente);

        if (!consulta.cancelada)
            if (strcmp(consulta.data, data) == 0) {
                existe_consulta = true;

                printf("\nCódigo: %d", consulta.codigo);
                printf("\nHorário: %s", consulta.horario);
                printf("\nEspecialidade: %s", medico.especialidade);
                printf("\nMédico: %s", medico.nome);
                printf("\nPaciente: %s", paciente.nome);
                printf("\n");
            }

        fread(&consulta, sizeof(consulta), 1, dados_consultas);
    }

    if (!existe_consulta) printf("\nNenhuma consulta.");

    getch();
}

void exibir_consultas_realizadas_pelo_paciente(FILE *dados_consultas, FILE *dados_medicos, FILE *dados_pacientes) {
    Consulta consulta;
    Medico medico;
    Paciente paciente;

    char nome[TAMANHO_NOME + 1] = "";
    bool existe_consulta = false;

    while (true) {
        system("cls");
        printf("Exibição de consultas realizadas pelo paciente\n\n");

        !strlen(nome)
            ? printf(MENSAGEM_NOME)
            : printf(MENSAGEM_NOME"%s\n", nome);
        if (!strlen(nome)) gets(nome);
        if (!strlen(nome)) {
            mostrar_mensagem_de_informacao_nao_informada();
            continue;
        }
        if (!verificar_existencia_do_paciente(dados_pacientes, nome)) {
            printf("\nNão existe um paciente com o nome informado.");
            pausar();
            strcpy(nome, "");
            continue;
        }

        break;
    }

    fseek(dados_consultas, 0, SEEK_SET);
    fread(&consulta, sizeof(consulta), 1, dados_consultas);

    while (!feof(dados_consultas)) {
        medico = receber_medico_pelo_codigo(dados_medicos, consulta.codigo_do_medico);
        paciente = receber_paciente_pelo_codigo(dados_pacientes, consulta.codigo_do_paciente);

        if (!consulta.cancelada)
            if (strcmpi(paciente.nome, nome) == 0 && verificar_se_momento_ja_passou(consulta.momento)) {
                existe_consulta = true;

                printf("\nCódigo: %d", consulta.codigo);
                printf("\nData: %s", consulta.data);
                printf("\nHorário: %s", consulta.horario);
                printf("\nEspecialidade: %s", medico.especialidade);
                printf("\nMédico: %s", medico.nome);
                printf("\n");
            }

        fread(&consulta, sizeof(consulta), 1, dados_consultas);
    }

    if (!existe_consulta) printf("\nNenhuma consulta.");

    getch();
}

void exibir_consultas_do_medico(FILE *dados_consultas, FILE *dados_medicos, FILE *dados_pacientes) {
    Consulta consulta;
    Medico medico;
    Paciente paciente;

    char nome[TAMANHO_NOME + 1] = "";
    bool existe_consulta = false;

    while (true) {
        system("cls");
        printf("Exibição de consultas de um médico\n\n");

        !strlen(nome)
            ? printf(MENSAGEM_NOME)
            : printf(MENSAGEM_NOME"%s\n", nome);
        if (!strlen(nome)) gets(nome);
        if (!strlen(nome)) {
            mostrar_mensagem_de_informacao_nao_informada();
            continue;
        }
        if (!verificar_existencia_do_medico(dados_medicos, nome)) {
            printf("\nNão existe um médico com o nome informado.");
            pausar();
            strcpy(nome, "");
            continue;
        }

        break;
    }

    fseek(dados_consultas, 0, SEEK_SET);
    fread(&consulta, sizeof(consulta), 1, dados_consultas);

    while (!feof(dados_consultas)) {
        medico = receber_medico_pelo_codigo(dados_medicos, consulta.codigo_do_medico);
        paciente = receber_paciente_pelo_codigo(dados_pacientes, consulta.codigo_do_paciente);

        if (!consulta.cancelada)
            if (strcmpi(medico.nome, nome) == 0) {
                existe_consulta = true;

                printf("\nCódigo: %d", consulta.codigo);
                printf("\nData: %s", consulta.data);
                printf("\nHorário: %s", consulta.horario);
                printf("\nEspecialidade: %s", medico.especialidade);
                printf("\nPaciente: %s", paciente.nome);
                printf("\n");
            }

        fread(&consulta, sizeof(consulta), 1, dados_consultas);
    }

    if (!existe_consulta) printf("\nNenhuma consulta.");
}

void exibir_laudos_medicos(FILE *dados_consultas, FILE *dados_medicos, FILE *dados_pacientes) {
    Consulta consulta;
    Medico medico;

    const char LAUDOS[3][QUANTIDADE_LAUDOS][TAMANHO_LAUDO + 1] = {
        {
            "AAAAAAAAAAAAAAAAAAAA", "BBBBBBBBBBBBBBBBBBBB", "CCCCCCCCCCCCCCCCCCCC", "DDDDDDDDDDDDDDDDDDDD", "EEEEEEEEEEEEEEEEEEEE", "FFFFFFFFFFFFFFFFFFFF", "GGGGGGGGGGGGGGGGGGGG", "HHHHHHHHHHHHHHHHHHHH", "IIIIIIIIIIIIIIIIIIII", "JJJJJJJJJJJJJJJJJJJJ",
        },
        {
            "KKKKKKKKKKKKKKKKKKKK", "LLLLLLLLLLLLLLLLLLLL", "MMMMMMMMMMMMMMMMMMMM", "NNNNNNNNNNNNNNNNNNNN", "OOOOOOOOOOOOOOOOOOOO", "PPPPPPPPPPPPPPPPPPPP", "QQQQQQQQQQQQQQQQQQQQ", "RRRRRRRRRRRRRRRRRRRR", "SSSSSSSSSSSSSSSSSSSS", "TTTTTTTTTTTTTTTTTTTT",
        },
        {
            "UUUUUUUUUUUUUUUUUUUU", "VVVVVVVVVVVVVVVVVVVV", "WWWWWWWWWWWWWWWWWWWW", "XXXXXXXXXXXXXXXXXXXX", "YYYYYYYYYYYYYYYYYYYY", "ZZZZZZZZZZZZZZZZZZZZ", "@@@@@@@@@@@@@@@@@@@@", "####################", "$$$$$$$$$$$$$$$$$$$$", "&&&&&&&&&&&&&&&&&&&&",
        }
    };

    char opcao = '0';
    unsigned int posicao = 0;

    fseek(dados_consultas, 0, SEEK_SET);
    fread(&consulta, sizeof(consulta), 1, dados_consultas);

    while (!feof(dados_consultas)) {
        medico = receber_medico_pelo_codigo(dados_medicos, consulta.codigo_do_medico);

        if (!consulta.cancelada && verificar_se_momento_ja_passou(consulta.momento))
            if (!strlen(consulta.laudo)) {
                if (strcmp(medico.especialidade, "Cardiologia") == 0)
                    strcpy(consulta.laudo, LAUDOS[0][rand() % QUANTIDADE_LAUDOS]);
                else if (strcmp(medico.especialidade, "Dermatologia") == 0)
                    strcpy(consulta.laudo, LAUDOS[1][rand() % QUANTIDADE_LAUDOS]);
                else if (strcmp(medico.especialidade, "Clínica médica") == 0)
                    strcpy(consulta.laudo, LAUDOS[2][rand() % QUANTIDADE_LAUDOS]);

                fseek(dados_consultas, sizeof(consulta) * posicao, SEEK_SET);
                fwrite(&consulta, sizeof(consulta), 1, dados_consultas);
                fflush(dados_consultas);
            }

        posicao++;
        fread(&consulta, sizeof(consulta), 1, dados_consultas);
    }

    do {
        system("cls");
        printf("Exibição de laudos\n\n");
        menu_laudos();

        printf("\nOpção: ");
        opcao = getche();

        switch(opcao - '0') {
            case 0:
                break;
            case 1:
                exibir_laudos_pelo_medico(dados_consultas, dados_medicos, dados_pacientes, LAUDOS);
                break;
            case 2:
                exibir_laudos_pelo_paciente(dados_consultas, dados_medicos, dados_pacientes, LAUDOS);
                break;
            default:
                printf("\n\nOpção inválida.");
                pausar();
                break;
        }

        if (opcao == '0') break;

    } while (true);
}

void exibir_laudos_pelo_medico(FILE *dados_consultas, FILE *dados_medicos, FILE *dados_pacientes, const char LAUDOS[3][QUANTIDADE_LAUDOS][TAMANHO_LAUDO + 1]) {
    Consulta consulta;
    Medico medico;
    Paciente paciente;

    char nome[TAMANHO_NOME + 1] = "";
    bool existe_consulta = false;

    while (true) {
        system("cls");
        printf("Exibição de laudos de um determinado médico\n\n");

        !strlen(nome)
            ? printf(MENSAGEM_NOME)
            : printf(MENSAGEM_NOME"%s\n", nome);
        if (!strlen(nome)) gets(nome);
        if (!strlen(nome)) {
            mostrar_mensagem_de_informacao_nao_informada();
            continue;
        }
        if (!verificar_existencia_do_medico(dados_medicos, nome)) {
            printf("\nNão existe um médico com o nome informado.");
            pausar();
            strcpy(nome, "");
            continue;
        }

        break;
    }

    fseek(dados_consultas, 0, SEEK_SET);
    fread(&consulta, sizeof(consulta), 1, dados_consultas);

    while (!feof(dados_consultas)) {
        medico = receber_medico_pelo_codigo(dados_medicos, consulta.codigo_do_medico);
        paciente = receber_paciente_pelo_codigo(dados_pacientes, consulta.codigo_do_paciente);

        if (!consulta.cancelada)
            if (strcmpi(medico.nome, nome) == 0 && verificar_se_momento_ja_passou(consulta.momento)) {
                existe_consulta = true;

                printf("\nCódigo: %d", consulta.codigo);
                printf("\nData: %s", consulta.data);
                printf("\nHorário: %s", consulta.horario);
                printf("\nEspecialidade: %s", medico.especialidade);
                printf("\nPaciente: %s", paciente.nome);
                printf("\nLaudo: %s", consulta.laudo);
                printf("\n");
        }

        fread(&consulta, sizeof(consulta), 1, dados_consultas);
    }

    if (!existe_consulta) printf("\nNenhuma consulta.");

    getch();
}

void exibir_laudos_pelo_paciente(FILE *dados_consultas, FILE *dados_medicos, FILE *dados_pacientes, const char LAUDOS[3][QUANTIDADE_LAUDOS][TAMANHO_LAUDO + 1]) {
    Consulta consulta;
    Medico medico;
    Paciente paciente;

    char nome[TAMANHO_NOME + 1] = "";
    bool existe_consulta = false;

    while (true) {
        system("cls");
        printf("Exibição de laudos de um determinado paciente\n\n");

        !strlen(nome)
            ? printf(MENSAGEM_NOME)
            : printf(MENSAGEM_NOME"%s\n", nome);
        if (!strlen(nome)) gets(nome);
        if (!strlen(nome)) {
            mostrar_mensagem_de_informacao_nao_informada();
            continue;
        }
        if (!verificar_existencia_do_paciente(dados_pacientes, nome)) {
            printf("\nNão existe um paciente com o nome informado.");
            pausar();
            strcpy(nome, "");
            continue;
        }

        break;
    }

    fseek(dados_consultas, 0, SEEK_SET);
    fread(&consulta, sizeof(consulta), 1, dados_consultas);

    while (!feof(dados_consultas)) {
        medico = receber_medico_pelo_codigo(dados_medicos, consulta.codigo_do_medico);
        paciente = receber_paciente_pelo_codigo(dados_pacientes, consulta.codigo_do_paciente);

        if (!consulta.cancelada)
            if (strcmpi(paciente.nome, nome) == 0 && verificar_se_momento_ja_passou(consulta.momento)) {
                existe_consulta = true;

                printf("\nCódigo: %d", consulta.codigo);
                printf("\nData: %s", consulta.data);
                printf("\nHorário: %s", consulta.horario);
                printf("\nEspecialidade: %s", medico.especialidade);
                printf("\nMédico: %s", medico.nome);
                printf("\nLaudo: %s", consulta.laudo);
                printf("\n");
        }

        fread(&consulta, sizeof(consulta), 1, dados_consultas);
    }

    if (!existe_consulta) printf("\nNenhuma consulta.");

    getch();
}

Medico receber_medico_pelo_codigo(FILE *dados_medicos, unsigned int codigo) {
    Medico medico;

    fseek(dados_medicos, 0, SEEK_SET);
    fread(&medico, sizeof(medico), 1, dados_medicos);

    while (!feof(dados_medicos)) {
        if (medico.codigo == codigo) break;
        fread(&medico, sizeof(medico), 1, dados_medicos);
    }

    return medico;
}

Medico receber_medico_pelo_nome(FILE *dados_medicos, char *nome) {
    Medico medico;

    fseek(dados_medicos, 0, SEEK_SET);
    fread(&medico, sizeof(medico), 1, dados_medicos);

    while (!feof(dados_medicos)) {
        if (strcmpi(medico.nome, nome) == 0) break;
        fread(&medico, sizeof(medico), 1, dados_medicos);
    }

    return medico;
}

Paciente receber_paciente_pelo_codigo(FILE *dados_pacientes, unsigned int codigo) {
    Paciente paciente;

    fseek(dados_pacientes, 0, SEEK_SET);
    fread(&paciente, sizeof(paciente), 1, dados_pacientes);

    while (!feof(dados_pacientes)) {
        if (paciente.codigo == codigo) break;
        fread(&paciente, sizeof(paciente), 1, dados_pacientes);
    }

    return paciente;
}

Paciente receber_paciente_pelo_nome(FILE *dados_pacientes, char *nome) {
    Paciente paciente;

    fseek(dados_pacientes, 0, SEEK_SET);
    fread(&paciente, sizeof(paciente), 1, dados_pacientes);

    while (!feof(dados_pacientes)) {
        if (strcmpi(paciente.nome, nome) == 0) break;
        fread(&paciente, sizeof(paciente), 1, dados_pacientes);
    }

    return paciente;
}

Momento receber_momento(char *data, char *horario, unsigned int segundos) {
    Momento momento;

    char dia[3], mes[3], ano[5];
    char hora[3], minuto[3];

    memcpy(dia, &data[0], 2);
    memcpy(mes, &data[3], 2);
    memcpy(ano, &data[6], 4);

    memcpy(hora, &horario[0], 2);
    memcpy(minuto, &horario[3], 2);

    dia[2] = '\0';
    mes[2] = '\0';
    ano[4] = '\0';

    hora[2] = '\0';
    minuto[2] = '\0';

    momento.tm_mday = atoi(dia);
    momento.tm_mon = atoi(mes) - 1;
    momento.tm_year = atoi(ano) - 1900;

    momento.tm_hour = atoi(hora);
    momento.tm_min = atoi(minuto);
    momento.tm_sec = segundos;

    return momento;
}

unsigned int receber_quantidades_de_consultas_do_medico(FILE *dados_consultas, char *data, unsigned int codigo) {
    Consulta consulta;

    unsigned int quantidade_de_consultas = 0;

    fseek(dados_consultas, 0, SEEK_SET);
    fread(&consulta, sizeof(consulta), 1, dados_consultas);

    while (!feof(dados_consultas)) {
        if (!consulta.cancelada)
            if (strcmp(consulta.data, data) == 0 && consulta.codigo_do_medico == codigo) quantidade_de_consultas++;
        fread(&consulta, sizeof(consulta), 1, dados_consultas);
    }

    return quantidade_de_consultas;
}

bool verificar_existencia_do_medico(FILE *dados_medicos, char *nome) {
    Medico medico;

    bool existe = false;

    fseek(dados_medicos, 0, SEEK_SET);
    fread(&medico, sizeof(medico), 1, dados_medicos);

    while (!feof(dados_medicos) && !existe) {
        if (strcmpi(medico.nome, nome) == 0) existe = true;
        fread(&medico, sizeof(medico), 1, dados_medicos);
    }

    return existe;
}

bool verificar_existencia_do_paciente(FILE *dados_pacientes, char *nome) {
    Paciente paciente;

    bool existe = false;

    fseek(dados_pacientes, 0, SEEK_SET);
    fread(&paciente, sizeof(paciente), 1, dados_pacientes);

    while (!feof(dados_pacientes) && !existe) {
        if (strcmpi(paciente.nome, nome) == 0) existe = true;
        fread(&paciente, sizeof(paciente), 1, dados_pacientes);
    }

    return existe;
}

bool verificar_existencia_de_consulta_proxima(FILE *dados_consultas, Momento momento) {
    Consulta consulta;

    fseek(dados_consultas, 0, SEEK_SET);
    fread(&consulta, sizeof(consulta), 1, dados_consultas);

    double diferenca_em_minutos = 0;
    const char ANTES_DEPOIS [][7] = {"antes", "depois"};

    while (!feof(dados_consultas)) {
        if (!consulta.cancelada) {
            diferenca_em_minutos = difftime(mktime(&momento), mktime(&consulta.momento)) / 60;

            if (abs(momento.tm_year - consulta.momento.tm_year) <= 1 && abs(diferenca_em_minutos) < 30) {
                if (diferenca_em_minutos == 0) printf("\nJá existe uma consulta agendada no momento informado.");
                else {
                    printf("\nJá existe uma consulta agendada %s do momento informado em %d ", ANTES_DEPOIS[diferenca_em_minutos < 0], abs(diferenca_em_minutos));
                    (abs(diferenca_em_minutos) == 1) ? printf("minuto.") : printf("minutos.");
                }

                return true;
            }
        }

        fread(&consulta, sizeof(consulta), 1, dados_consultas);
    }

    return false;
}

bool verificar_se_data_ja_passou(char *data, bool incluirHoje) {
    return verificar_se_momento_ja_passou(receber_momento(data, incluirHoje ? "23:59" : "00:00", incluirHoje ? 59 : 0));
}

bool verificar_se_momento_ja_passou(Momento momento) {
    time_t atual = time(NULL);
    Momento momento_atual = *localtime(&atual);

    if (momento.tm_year < momento_atual.tm_year) return true;
    if (momento.tm_year > momento_atual.tm_year) return false;
    if (difftime(mktime(&momento), mktime(&momento_atual)) + 3600 >= 0) return false;

    return true;
}

bool verificar_data(const char *data) {
    unsigned int i;
    unsigned int dia, mes, ano;
    char _dia[3], _mes[3], _ano[5];

    if (strlen(data) != TAMANHO_DATA) return false;
    if (data[2] != '/') return false;
    if (data[5] != '/') return false;

    for (i = 0; i < TAMANHO_DATA; i++) {
        if (i == 2) continue;
        if (i == 5) continue;
        if (data[i] < '0' || data[i] > '9') return false;
    }

    memcpy(_dia, &data[0], 2);
    memcpy(_mes, &data[3], 2);
    memcpy(_ano, &data[6], 4);

    _dia[2] = '\0';
    _mes[2] = '\0';
    _ano[4] = '\0';

    dia = atoi(_dia);
    mes = atoi(_mes);
    ano = atoi(_ano);

    if (ano < 1900) return false;
    if (mes < 1 || mes > 12) return false;
    if (dia < 1 || dia > 31) return false;

    if (mes == 2) {
        if (((ano % 4 == 0)
            && (ano % 100 != 0))
            || (ano % 400 == 0))
            return dia <= 29;
        else return dia <= 28;
    }

    if (mes == 4 || mes == 6 ||
        mes == 9 || mes == 11)
        return dia <= 30;

    return true;
}

bool verificar_especialidade(const char *especialidade) {
    return especialidade[0] >= '0'
        && especialidade[0] <= '2'
        && especialidade[1] == '\0';
}

bool verificar_horario(const char *horario) {
    if (strlen(horario) != TAMANHO_HORARIO) return false;
    if (horario[0] < '0' || horario[0] > '2') return false;
    if (horario[1] < '0' || horario[1] > '9'
    || (horario[0] == '2' && (horario[1] < '0' || horario[1] > '3'))) return false;
    if (horario[2] != ':') return false;
    if (horario[3] < '0' || horario[3] > '5') return false;
    if (horario[4] < '0' || horario[4] > '9') return false;

    return true;
}

bool verificar_telefone(const char *telefone) {
    unsigned int i;

    if (strlen(telefone) != TAMANHO_TELEFONE) return false;
    if (telefone[2] != ' ') return false;
    if (telefone[3] != '9') return false;
    if (telefone[8] != '-') return false;

    for (i = 0; i < TAMANHO_TELEFONE; i++) {
        if (i == 2) continue;
        if (i == 3) continue;
        if (i == 8) continue;
        if (telefone[i] < '0' || telefone[i] > '9') return false;
    }

    return true;
}

bool verificar_se_e_numero(const char *numero) {
    int i;

    for (i = 0; i < strlen(numero); i++)
        if (numero[i] < '0' || numero[i] > '9') return false;

    return true;
}