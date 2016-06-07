#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Funcionario
{
    int codigo;
    char nome[30];
    char sobrenome[30];
    int idade;
    char email[30];
    char telefone[10];
    float salario;

    struct Funcionario *next;
} Funcionario;

typedef struct CalculosGerais
{
    float mediaSalarial;
    int mediaIdade;
    int qtdSalariosAcimaMedia;
    int qtdSalariosAbaixoMedia;
    int qtdIdadesAcimaMedia;
    int qtdIdadesAbaixoMedia;
    Funcionario *maiorSalario;
    Funcionario *menorSalario;
    Funcionario *maiorIdade;
    Funcionario *menorIdade;
} CalculosGerais;

char* getDesktopFolderPath()
{
    char homedir[100];
    snprintf(homedir, 100, "%s\\%s\\%s\\", getenv("HOMEDRIVE"), getenv("HOMEPATH"), "Desktop");

    return strdup(homedir);
}

Funcionario* addFuncionario(float salario, Funcionario *lista)
{
    Funcionario *p = lista;
    while( p->next != NULL ) //p contera o ultimo da lista
    {
        //printf("%p - %d\n", p, p->codigo);
        p = p->next;
    }

    Funcionario *novo = malloc(sizeof(Funcionario));
    novo->codigo = p->codigo + 1;
    novo->next = NULL;
    novo->salario = salario;

    p->next = novo;

    return novo;
}

int removeFuncionario(int codigo, Funcionario *lista)
{
    Funcionario *p = lista;
    Funcionario *ant;
    while( p != NULL )
    {
        if (p->codigo == codigo)
        {
            break;
        }
        else
        {
            ant = p;
            p = p->next;
        }
    }

    if (p != NULL) //Encontrou
    {
        ant->next = p->next;
        free(p);
        printf("Codigo %d removido com sucesso\n", codigo);

        return 0;
    }
    else
    {
        puts("Codigo nao encontrado");

        return 1;
    }
}

Funcionario* getFuncionario(int codigo, Funcionario *lista)
{
    Funcionario *p = lista;
    while( p != NULL )
    {
        if (p->codigo == codigo)
        {
            break;
        }
        else
        {
            p = p->next;
        }
    }

    if (p != NULL) //Encontrou
    {
        return p;
    }
    else
    {
        puts("Codigo nao encontrado");
        return NULL;
    }
}

Funcionario* loadFuncionariosFromFile()
{
    Funcionario *f, *prim, *ant;

    prim = malloc(sizeof(Funcionario));
    f = malloc(sizeof(Funcionario));
    prim->next = f;

    //FILE *arquivo;
    //arquivo = fopen("C:\\Users\\fernando.moraes\\Desktop\\IN_200_funcionarios.txt", "r");

    FILE *arquivo;
    char pathArquivo[100];
    strcpy(pathArquivo, getDesktopFolderPath());
    strcat(pathArquivo, "IN_200_funcionarios.txt");
    arquivo = fopen(pathArquivo, "r");

    if(arquivo == NULL)
    {
        puts("Arquivo nao encontrado");

        return NULL;
    }
    else
    {
        int codigo;
        char nome[30];
        char sobrenome[30];
        int idade;
        char email[30];
        char telefone[10];
        float salario;
        int i=0;
        while( (fscanf(arquivo,"%d %s %s %d %s %s %f\n", &codigo, nome, sobrenome, &idade, email, telefone, &salario)) != -1 )
        {
            if ( i == 0 )
            {
                prim->codigo = codigo;
                strcpy(prim->nome, nome);
                strcpy(prim->sobrenome, sobrenome);
                prim->idade = idade;
                strcpy(prim->email, email);
                strcpy(prim->telefone, telefone);
                prim->salario = salario;

                ant = prim;
            }
            else
            {
                f->codigo = codigo;
                strcpy(f->nome, nome);
                strcpy(f->sobrenome, sobrenome);
                f->idade = idade;
                strcpy(f->email, email);
                strcpy(f->telefone, telefone);
                f->salario = salario;

                ant->next = f;
                ant = f;

                f = malloc(sizeof(Funcionario));
            }

            i++;
        }

        fclose(arquivo);

        return prim;
    }
}

int imprimeLista(Funcionario *lista)
{
    Funcionario *p = lista;
    while( p != NULL )
    {
        printf("%p - %d - %.2f\n", p, p->codigo, p->salario);
        p = p->next;
    }

    return 0;
}

int imprimeFuncionarios(Funcionario *lista)
{
    fflush(stdin);
    Funcionario *p = lista;
    while( p != NULL )
    {
        //printf("%p - %d %s %s %d %s %s %.2f\n", p, p->codigo, p->nome, p->sobrenome, p->idade, p->email, p->telefone, p->salario);
        printf("%-2d %-12s %-12s %-2d %-26s %-10s %.2f\n", p->codigo, p->nome, p->sobrenome, p->idade, p->email, p->telefone, p->salario);

        if (p->codigo % 120 == 0)
        {
            puts("Pressione enter para proxima pagina...");
            getchar();
        }

        p = p->next;
    }

    return 0;
}

CalculosGerais* calculosGerais(Funcionario *lista)
{
    Funcionario *p = lista;

    float mediaSalarial = 0;
    int mediaIdade = 0;
    int acimaMediaIdade = 0;
    int abaixoMediaIdade = 0;
    int acimaMediaSalarial = 0;
    int abaixoMediaSalarial = 0;

    while( p != NULL )
    {
        mediaSalarial += p->salario;
        mediaIdade += p->idade;

        p = p->next;
    }

    mediaSalarial /= 600;
    mediaIdade /= 600;

    CalculosGerais *cg = malloc(sizeof(CalculosGerais));
    cg->mediaSalarial = mediaSalarial;
    cg->mediaIdade = mediaIdade;

    p = lista;

    float maiorSalario = 0;
    float menorSalario = 0;
    if ( p != NULL )
    {
        maiorSalario = menorSalario = p->salario;
    }

    while( p != NULL )
    {
        if ( p->idade > mediaIdade )
        {
            acimaMediaIdade++;
        }
        else if ( p->idade < mediaIdade )
        {
            abaixoMediaIdade++;
        }

        if ( p->salario > mediaSalarial )
        {
            acimaMediaSalarial++;
        }
        else if ( p->salario < mediaSalarial )
        {
            abaixoMediaSalarial++;
        }

        if ( p->salario > maiorSalario )
        {
            maiorSalario = p->salario;
            cg->maiorSalario = p;
        }

        if ( p->salario < menorSalario )
        {
            menorSalario = p->salario;
            cg->menorSalario = p;
        }

        p = p->next;
    }

    printf("--- Media salarial: %.2f ---\n", mediaSalarial);
    printf("--- Acima da media de salario: %d ---\n", acimaMediaSalarial);
    printf("--- Abaixo da media de salario: %d ---\n", abaixoMediaSalarial);
    printf("--- Maior salario: %.2f ---\n", maiorSalario);
    printf("--- Menor salario: %.2f ---\n", menorSalario);

    printf("--- Media de idade: %d ---\n", mediaIdade);
    printf("--- Acima da media de idade: %d ---\n", acimaMediaIdade);
    printf("--- Abaixo da media de idade: %d ---\n", abaixoMediaIdade);

    cg->qtdSalariosAbaixoMedia = abaixoMediaSalarial;
    cg->qtdSalariosAcimaMedia = acimaMediaSalarial;
    cg->qtdIdadesAbaixoMedia = abaixoMediaIdade;
    cg->qtdIdadesAcimaMedia = acimaMediaIdade;

    return cg;
}

//int saveCalculosGerais(float mediaSalarial, int mediaIdade, int qtdSalariosAcimaMedia, int qtdSalariosAbaixoMedia, int qtdIdadesAcimaMedia, int qtdIdadesAbaixoMedia, Funcionario *maiorSalario, Funcionario *menorSalario, Funcionario *maiorIdade, Funcionario *menorIdade)
int saveCalculosGerais(CalculosGerais *calculosGerais)
{
    FILE *arquivo;
    char pathArquivo[100];
    strcpy(pathArquivo, getDesktopFolderPath());
    strcat(pathArquivo, "OUT_empresa.txt");
    arquivo = fopen(pathArquivo, "w");
    if(arquivo == NULL)
    {
        puts("Arquivo nao encontrado");

        return 1;
    }
    else
    {
        fprintf(arquivo, "--- Relatorio da empresa Randatmail.com ---\n\n");
        fprintf(arquivo, "Media salarial: %.2f\n", calculosGerais->mediaSalarial);
        fprintf(arquivo, "Quantidade de salarios acima da media: %d\n", calculosGerais->qtdSalariosAcimaMedia);
        fprintf(arquivo, "Quantidade de salarios abaixo da media: %d\n", calculosGerais->qtdSalariosAbaixoMedia);
        fprintf(arquivo, "Maior salario %.2f: %s, %s\n", calculosGerais->maiorSalario->salario, calculosGerais->maiorSalario->sobrenome, calculosGerais->maiorSalario->nome);
        fprintf(arquivo, "Menor salario %.2f: %s, %s\n", calculosGerais->menorSalario->salario, calculosGerais->menorSalario->sobrenome, calculosGerais->menorSalario->nome);

        fprintf(arquivo, "\n\nMedia de idade: %d\n", calculosGerais->mediaIdade);
        fprintf(arquivo, "Quantidade de funcionarios acima da media: %d\n", calculosGerais->qtdIdadesAcimaMedia);
        fprintf(arquivo, "Quantidade de funcionarios abaixo da media: %d\n", calculosGerais->qtdIdadesAbaixoMedia);
        //fprintf(arquivo, "Maior idade %d: %s, %s\n", maiorIdade->idade, maiorIdade->sobrenome, maiorIdade->nome);
        //fprintf(arquivo, "Menor idade %d: %s, %s\n", menorIdade->idade, menorIdade->sobrenome, menorIdade->nome);

        fclose(arquivo);

        puts("\nRelatorio da empresa gravado com sucesso\n");

        return 0;
    }
}

int alterarFuncionario(Funcionario *lista, int codigo)
{
    if (codigo < 0 || codigo > 600)
    {
        return 1;
    }

    char nome[30];
    char sobrenome[30];
    int idade;
    char email[30];
    char telefone[10];
    float salario;

    Funcionario *p = lista;
    int count = 1;
    while( p != NULL )
    {
        if (count == codigo)
        {
            break;
        }
        else
        {
            p = p->next;
            count++;
        }
    }

    int opcao;
    printf("Funcionario: %d %s %s %d %s %s %.2f\n", p->codigo, p->nome, p->sobrenome, p->idade, p->email, p->telefone, p->salario);
    printf("Alterar:\n1 - Nome\n2 - Sobrenome\n3 - Idade\n4 - Email\n5 - Telefone\n6 - Salario\n");
    scanf("%d", &opcao);

    if (opcao == 1)
    {
        printf("Nome: ");
        scanf("%s", nome);
        strcpy(p->nome, nome);
    }
    else if (opcao == 2)
    {
        printf("Sobrenome: ");
        scanf("%s", sobrenome);
        strcpy(p->sobrenome, sobrenome);
    }
    else if (opcao == 3)
    {
        printf("Idade: ");
        scanf("%d", &idade);
        p->idade = idade;
    }
    else if (opcao == 4)
    {
        printf("Email: ");
        scanf("%s", email);
        strcpy(p->email, email);
    }
    else if (opcao == 5)
    {
        printf("Telefone: ");
        scanf("%s", telefone);
        strcpy(p->telefone, telefone);
    }
    else if (opcao == 6)
    {
        printf("Salario: ");
        scanf("%f", &salario);
        p->salario = salario;
    }

    printf("Funcionario (alterado): %d %s %s %d %s %s %.2f\n", p->codigo, p->nome, p->sobrenome, p->idade, p->email, p->telefone, p->salario);
    getchar();

    return 0;
}

int menu(int *opcao)
{
    puts("======================================================================\n\n");

    puts("== Funcionarios ==\n\n");

    printf("Menu:\n\n");

    printf("1. Lista de funcionarios\n\n");

    printf("2. Relatorio da empresa\n\n");

    printf("3. Alterar registro\n\n");

    printf("4. Ranking salarial\n\n");

    printf("5. Sair\n\n");

    puts("\n======================================================================");

    printf("Escolha uma opcao: ");
    scanf("%d", opcao);

    return 0;
}

int main()
{
    Funcionario *lista;
    lista = loadFuncionariosFromFile();
    //imprimeFuncionarios(lista);
    //calculosGerais(lista);

    int opcao = 0;
    do
    {
        menu(&opcao);

        if (opcao == 1)
        {
            imprimeFuncionarios(lista);
            int opcaoFuncionarios;
            printf("\n1 - Gravar em arquivo\n2 - Voltar\n");
            scanf("%d", &opcaoFuncionarios);

            if (opcaoFuncionarios == 1)
            {
                //saveFuncionarios(lista);
            }
        }
        else if (opcao == 2)
        {
            CalculosGerais *cg = calculosGerais(lista);
            int opcaoEmpresa;
            printf("\n1 - Gravar em arquivo\n2 - Voltar\n");
            scanf("%d", &opcaoEmpresa);

            if (opcaoEmpresa == 1)
            {
                saveCalculosGerais(cg);
            }
        }
        else if (opcao == 3)
        {
            int codigo = -1;
            imprimeFuncionarios(lista);
            printf("\nDigite o codigo do registro que deseja alterar (0 cancela operacao): ");
            scanf("%d", &codigo);

            if (codigo != 0)
            {
                alterarFuncionario(lista, codigo);
                calculosGerais(lista);
            }
        }
        else if (opcao == 4)
        {
            //rankingSalarial(lista);
            imprimeFuncionarios(lista);
        }

    } while(opcao != 5);

    return 0;
}
