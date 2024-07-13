#include <stdio.h>
#include <stdlib.h>

typedef struct arvore
{
    int info;
    int FB;
    struct arvore *esq;
    struct arvore *dir;
} arvore;

arvore *LerArvore(FILE *arq)
{
    char c;
    int num;

    fscanf(arq, "%c", &c);
    fscanf(arq, "%d", &num);

    if (num == -1)
    {
        fscanf(arq, "%c", &c);
        return NULL;
    }
    else
    {
        arvore *a = (arvore *)malloc(sizeof(arvore));
        a->info = num;
        a->esq = LerArvore(arq);
        a->dir = LerArvore(arq);
        fscanf(arq, "%c", &c);
        return a;
    }
}

int altura(arvore *a)
{

    if (a == NULL)
    {
        return 0;
    }
    else
    {
        int he, hd;
        he = altura(a->esq);
        hd = altura(a->dir);

        if (he > hd)
        {
            return he + 1;
        }
        else
        {
            return hd + 1;
        }
    }
}

void AjustarFB(arvore *a)
{
    if (a != NULL)
    {
        int he = altura(a->esq);
        int hd = altura(a->dir);
        a->FB = he - hd;

        AjustarFB(a->esq);
        AjustarFB(a->dir);
    }
}

arvore *LerAvl(FILE *arq)
{
    arvore *a = LerArvore(arq);
    AjustarFB(a);
    return a;
}

void ImprimirPreOrdem(arvore *a)
{

    if (a == NULL)
    {
        return;
    }

    printf("%d ", a->info);
    ImprimirPreOrdem(a->esq);
    ImprimirPreOrdem(a->dir);
}

void ImprimirEmOrdem(arvore *a)
{

    if (a == NULL)
    {
        return;
    }

    ImprimirEmOrdem(a->esq);
    printf("%d ", a->info);
    ImprimirEmOrdem(a->dir);
}

void ImprimirPosOrdem(arvore *a)
{

    if (a == NULL)
    {
        return;
    }

    ImprimirPosOrdem(a->esq);
    ImprimirPosOrdem(a->dir);
    printf("%d ", a->info);
}

void ImprimirNivel(arvore *a, int cont, int nivel)
{

    if (a != NULL)
    {
        if (cont == nivel)
        {
            printf("%d ", a->info);
        }
        else
        {
            ImprimirNivel(a->esq, cont + 1, nivel);
            ImprimirNivel(a->dir, cont + 1, nivel);
        }
    }
}

void ImprimirEmLargura(arvore *a)
{

    if (a != NULL)
    {
        int i;
        int h = altura(a);

        for (i = 0; i < h; i++)
        {
            ImprimirNivel(a, 0, i);
            printf("\n\n");
        }
    }
}

int VerificarSeExisteX(arvore *a, int x)
{

    if (a == NULL)
    {
        return 0;
    }

    if (a->info == x)
    {
        return 1;
    }
    else if (a->info > x)
    {
        return VerificarSeExisteX(a->esq, x);
    }
    else
    {
        return VerificarSeExisteX(a->dir, x);
    }
}

int ImprimirNivelNo(arvore *a, int x)
{

    if (a != NULL)
    {
        if (a->info == x)
        {
            return 0;
        }

        int ret;

        if (a->info > x)
        {
            ret = ImprimirNivelNo(a->esq, x);
            if (ret != -1)
            {
                return ret + 1;
            }
        }
        else
        {
            ret = ImprimirNivelNo(a->dir, x);
            if (ret != -1)
            {
                return ret + 1;
            }
        }
    }
    return -1;
}

void ImprimirFolhasMenoresX(arvore *a, int x)
{
    if (a == NULL)
    {
        return;
    }
    if (a->esq == NULL && a->dir == NULL && a->info < x)
    {
        printf("%d ", a->info);
    }
    ImprimirFolhasMenoresX(a->esq, x);
    ImprimirFolhasMenoresX(a->dir, x);
}

arvore *RotacaoEsqSimples(arvore *r)
{
    arvore *a = r;
    arvore *b = a->dir;

    a->dir = b->esq;
    b->esq = a;
    if (b->FB == 1)
    {
        a->FB = 0;
        b->FB = 0;
    }
    else
    {
        a->FB = 1;
        b->FB = 1;
    }

    r = b;
    return r;
}

arvore *RotacaoDirSimples(arvore *r)
{
    arvore *a = r;
    arvore *b = r->esq;

    a->esq = b->dir;
    b->dir = a;

    if (a->FB == -1)
    {
        a->FB = 0;
        b->FB = 0;
    }
    else
    {
        a->FB = 1;
        b->FB = -1;
    }

    r = b;
    return r;
}

arvore *RotacaoEsqDupla(arvore *r)
{
    arvore *a = r;
    arvore *c = a->dir;
    arvore *b = c->esq;

    c->esq = b->dir;
    a->dir = b->esq;

    b->esq = a;
    b->dir = c;

    switch (b->FB)
    {
    case -1:
        a->FB = 0;
        c->FB = 1;
        break;
    case 0:
        a->FB = 0;
        c->FB = 0;
        break;
    case +1:
        a->FB = -1;
        c->FB = 0;
        break;
    }

    b->FB = 0;
    r = b;
    return r;
}

arvore *RotacaoDirDupla(arvore *r)
{
    arvore *c = r;
    arvore *a = c->esq;
    arvore *b = a->dir;

    c->esq = b->dir;
    a->dir = b->esq;

    b->esq = a;
    b->dir = c;

    switch (b->FB)
    {
    case -1:
        a->FB = 0;
        c->FB = 1;
        break;
    case 0:
        a->FB = 0;
        c->FB = 0;
        break;
    case +1:
        a->FB = -1;
        c->FB = 0;
        break;
    }
    b->FB = 0;
    r = b;
    return r;
}

arvore *RotacaoEsq(arvore *a)
{
    if (a->dir->FB == -1)
        a = RotacaoEsqDupla(a);
    else
        a = RotacaoEsqSimples(a);
    return a;
}

arvore *RotacaoDir(arvore *a)
{
    if (a->esq->FB == 1)
        a = RotacaoDirDupla(a);
    else
        a = RotacaoDirSimples(a);
    return a;
}

arvore *Inserir(arvore *r, int x, int *hMudou)
{
    if (r == NULL) // Momento de inserir
    {
        r = (arvore *)malloc(sizeof(arvore));
        r->info = x;
        r->esq = NULL;
        r->dir = NULL;
        r->FB = 0;
        *hMudou = 1;
    }
    else
    {
        if (x <= r->info)

        {
            r->esq = Inserir(r->esq, x, hMudou);
            if (*hMudou == 1)
            {

                switch (r->FB)

                {
                case -1:
                    r = RotacaoDir(r);
                    *hMudou = 0;
                    break;
                case 0:
                    r->FB = -1;
                    *hMudou = 1;
                    break;
                case +1:
                    r->FB = 0;
                    *hMudou = 0;
                    break;
                }
            }
        }
        else
        {
            r->dir = Inserir(r->dir, x, hMudou);
            if (*hMudou == 1)

            {
                switch (r->FB)
                {
                case -1:

                    r->FB = 0;
                    *hMudou = 0;
                    break;
                case 0:
                    r->FB = +1;
                    *hMudou = 1;
                    break;
                case +1:
                    r = RotacaoEsq(r);
                    *hMudou = 0;
                    break;
                }
            }
        }
    }
    return r;
}

arvore *RemoverElemento(arvore *a, int x, int *hMudou)
{
    if (a != NULL)
    {
        if (a->info == x)
        {
            // Verificar se é folha
            if (a->esq == NULL && a->dir == NULL)
            {
                free(a);
                *hMudou = 1;
                return NULL;
            }
            // Verificar se um dos filhos é nulo
            else if (a->esq == NULL || a->dir == NULL)
            {
                arvore *aux;
                if (a->esq == NULL)
                    aux = a->dir;
                else
                    aux = a->esq;
                free(a);
                *hMudou = 1;
                return aux;
            }
            // Nenhum filho nulo
            else
            {
                arvore *maiorEsq = a->esq;
                while (maiorEsq->dir != NULL)
                    maiorEsq = maiorEsq->dir;
                a->info = maiorEsq->info;
                a->esq = RemoverElemento(a->esq, a->info, hMudou);
                if (*hMudou == 1)
                {
                    switch (a->FB)
                    {
                    case -1:
                        a->FB = 0;
                        *hMudou = 1;
                        break;
                    case 0:

                        a->FB = 1;
                        *hMudou = 0;
                        break;
                    case +1:
                    {
                        int aux = a->dir->FB;
                        a = RotacaoEsq(a);
                        if (aux == 0)
                            *hMudou = 0;
                        else
                            *hMudou = 1;
                        break;
                    }
                    }
                }
            }
        }
        else if (x < a->info)
        {
            a->esq = RemoverElemento(a->esq, x, hMudou);
            if (*hMudou == 1)
            {
                switch (a->FB)
                {
                case -1:
                    a->FB = 0;
                    *hMudou = 1;
                    break;
                case 0:
                    a->FB = 1;
                    *hMudou = 0;
                    break;
                case +1:
                {
                    int aux = a->dir->FB;
                    a = RotacaoEsq(a);
                    if (aux == 0)
                        *hMudou = 0;
                    else
                        *hMudou = 1;
                    break;
                }
                }
            }
        }
        else
        {
            a->dir = RemoverElemento(a->dir, x, hMudou);
            if (*hMudou == 1)
            {
                switch (a->FB)
                {
                case +1:
                    a->FB = 0;
                    *hMudou = 1;
                    break;
                case 0:
                    a->FB = -1;
                    *hMudou = 0;
                    break;
                case -1:
                {
                    int aux = a->esq->FB;
                    a = RotacaoDir(a);
                    if (aux == 0)
                        *hMudou = 0;
                    else
                        *hMudou = 1;
                    break;
                }
                }
            }
        }
        return a;
    }
    return NULL;
}

void ApagaArvore(arvore *a)
{
    if (a == NULL)
    {
        return;
    }
    ApagaArvore(a->esq);
    ApagaArvore(a->dir);
    free(a);
}

int main()
{

    FILE *arq;
    int aux, aux2;
    arvore *a;
    int hMudou = 0;

    do
    {
        printf("\nEscolha uma opcao:\n");
        printf("1. Ler arvore do arquivo\n");
        printf("2. Imprimir arvore\n");
        printf("3. Verificar se existe um elemento\n");
        printf("4. Imprimir Nivel de um No X\n");
        printf("5. Imprimir Folhas menores que:\n");
        printf("6. Inserir Elemento\n");
        printf("7. Remover Elemento\n");
        printf("8. Sair\n");

        scanf("%d", &aux);
        switch (aux)
        {
        case 1:
            arq = fopen("arvore.txt", "rt");
            if (arq == NULL)
            {
                printf("Erro ao abrir o arquivo");
                return 1;
            }
            a = LerAvl(arq);
            fclose(arq);
            break;
        case 2:
            printf("\nEscolha uma opcao:\n");
            printf("1. Pre-Ordem\n");
            printf("2. Em-Ordem\n");
            printf("3. Pos-Ordem\n");
            printf("4. Em Largura\n");

            scanf("%d", &aux);

            switch (aux)
            {
            case 1:
                ImprimirPreOrdem(a);
                break;
            case 2:
                ImprimirEmOrdem(a);
                break;

            case 3:
                ImprimirPosOrdem(a);
                break;
            case 4:
                ImprimirEmLargura(a);
            }
            break;
        case 3:
            printf("Valor:\n");
            scanf("%d", &aux);

            aux2 = VerificarSeExisteX(a, aux);

            if (aux2 == 1)
            {
                printf("Existe");
            }
            else
            {
                printf("Nao Existe");
            }
            break;
        case 4:
            printf("Valor:\n");
            scanf("%d", &aux2);
            int aux3 = ImprimirNivelNo(a, aux2);
            if (aux3 != -1)
            {
                printf("Nivel: %d\n", aux3);
            }
            else
            {
                printf("Valor %d nao encontrado na arvore.\n", aux2);
            }
            break;
        case 5:
            printf("Valor:\n");
            scanf("%d", &aux2);
            ImprimirFolhasMenoresX(a, aux2);
            break;
        case 6:
            printf("Valor:\n");
            scanf("%d", &aux2);
            a = Inserir(a, aux2, &hMudou);
            break;
        case 7:
            printf("Valor:\n");
            scanf("%d", &aux2);
            a = RemoverElemento(a, aux2, &hMudou);
            break;
        case 8:
            ApagaArvore(a);
            printf("Fim..");
            break;
        }
    } while (aux != 8);

    return 0;
}