
#ifndef MINHA_ARVORE_AVL_HPP
#define MINHA_ARVORE_AVL_HPP

#include "ArvoreBinariaDeBusca.h"
#include "ListaEncadeadaAbstrata.h"
#include "MinhaListaEncadeada.h"
#include <algorithm>
// #include <cinttypes>
// #include <cstddef>
#include <iostream>
#include <optional>
// #include <vector>

/**
 * @brief Representa uma árvore AVL.
 *
 * @tparam T O tipo de dado guardado na árvore.
 */
template <typename T>
class MinhaArvoreAVL final : public ArvoreBinariaDeBusca<T>
{
    virtual ~MinhaArvoreAVL() {

    };

    void destrutorRecursivo(Nodo<T>*raiz)
    {
        if(!raiz) return;
        destrutorRecursivo(raiz->filhoEsquerda);
        destrutorRecursivo(raiz->filhoDireita);
        delete raiz;
    }

    /**
     * @brief Verifica se a arvore esta vazia
     * @return Verdade se a arvore esta vazia.
     */
    virtual bool vazia() const
    {
        if (this->raiz == nullptr)
        {
            return true;
        }
        return false;
    };

    /**
     * @brief Retornar quantidade de chaves na arvore
     * @return Numero natural que representa a quantidade de chaves na arvore
     */
    virtual int quantidade() const
    {
        return quantidadeDeNodos(this->raiz);
    };

    /**
     * @brief Verifica se a arvore contem uma chave
     * @param chave chave a ser procurada na arvore
     * @return Verdade se a arvore contem a chave
     */
    virtual bool contem(T chave) const
    {
        if (!this->raiz)
        {
            return false;
        }
        Nodo<T> *nodoAtual = this->raiz;

        while (true)
        {
            if (nodoAtual == nullptr)
                return false;
            if (nodoAtual->chave == chave)
                return true;
            if (nodoAtual->chave > chave)
            {
                nodoAtual = nodoAtual->filhoEsquerda;
            }
            else
            {
                nodoAtual = nodoAtual->filhoDireita;
            }
        }
    };

    /**
     * @brief Retorna a altura da (sub)arvore
     * @param chave chave que é raiz da (sub)arvore cuja altura queremos.
     * @return Numero inteiro representando a altura da (subarvore). Se chave nao esta na arvore, retorna std::nullopt
     */
    virtual std::optional<int> altura(T chave) const
    {
        Nodo<T> *nodoAtual = this->raiz;

        while (true)
        {
            if (nodoAtual == nullptr)
                return std::nullopt;
            if (nodoAtual->chave == chave)
                return nodoAtual->altura;

            if (nodoAtual->chave > chave)
            {
                nodoAtual = nodoAtual->filhoEsquerda;
            }
            else
            {
                nodoAtual = nodoAtual->filhoDireita;
            }
        }
    };

    void inserirNewNodo(Nodo<T>*& raiz, T dado)
    {
        if (dado < raiz->chave) {
            if (!raiz->filhoEsquerda) {
                raiz->filhoEsquerda = new Nodo<T>{dado};
            } else {
                inserirNewNodo(raiz->filhoEsquerda, dado);
            }
        } else if(dado > raiz->chave) {
            if (!raiz->filhoDireita) {
                raiz->filhoDireita = new Nodo<T>{dado};
            } else {
                inserirNewNodo(raiz->filhoDireita, dado);
            }
        } else {
            return ;
        }

        raiz->altura = 1 + std::max(alturaPorNodo(raiz->filhoDireita),
                                    alturaPorNodo(raiz->filhoEsquerda));

        int fatorB = getBFactor(raiz);

        if(fatorB == -2 && getBFactor(raiz->filhoDireita) <= 0) {
            rotacaoSimplesEsquerda(raiz);
        } else if (fatorB == 2 && getBFactor(raiz->filhoEsquerda) >= 0) {
            rotacaoSimplesDireita(raiz);
        } else if(fatorB == -2 && getBFactor(raiz->filhoDireita) > 0) {
            rotacaoDireitaEsquerda(raiz);
        } else if(fatorB == 2 && getBFactor(raiz->filhoEsquerda) < 0){
            rotacaoEsquerdaDireita(raiz);
        }
        raiz->altura = 1 + std::max(alturaPorNodo(raiz->filhoDireita),
                                           alturaPorNodo(raiz->filhoEsquerda));
    }

    /**
     * @brief Insere uma chave na arvore
     * @param chave chave a ser inserida
     */
    virtual void inserir(T chave)
    {
        if (!this->raiz){
            this->raiz = new Nodo<T>{chave};
        } else {
            this->inserirNewNodo(this->raiz, chave);
        }
    };

    /**
     * @brief Remove uma chave da arvore
     * @param chave chave a removida
     */

    virtual void remover(T raiz) {
        removerNodo(this->raiz, raiz);
    }
    void removerNodo(Nodo<T>*& raiz, T chave) {
        if (!raiz) return;
        if (chave < raiz->chave) {
            removerNodo(raiz->filhoEsquerda, chave);
        } else if (chave > raiz->chave) {
            removerNodo(raiz->filhoDireita, chave);
        } else {
            if (!raiz->filhoEsquerda && !raiz->filhoDireita) {
                delete raiz;
                raiz = nullptr;
            } else if (!raiz->filhoEsquerda || !raiz->filhoDireita) {
                Nodo<T>* temp = raiz;
                raiz = raiz->filhoEsquerda ? raiz->filhoEsquerda : raiz->filhoDireita;
                delete temp;
            } else {
                Nodo<T>* minimo = getMinimo(raiz->filhoDireita);
                raiz->chave = minimo->chave;
                removerNodo(raiz->filhoDireita, minimo->chave);
            }
        }
        if (raiz) {
            raiz->altura = 1 + std::max(alturaPorNodo(raiz->filhoEsquerda),
                                        alturaPorNodo(raiz->filhoDireita));
            rebalance(raiz);
        }
    }


    /**
     * @brief Busca a chave do filho a esquerda de uma (sub)arvore
     * @param chave chave da arvore que eh pai do filho a esquerda
     * @return Chave do filho a esquerda. Se chave nao esta na arvore, retorna std::nullopt
     */
    virtual std::optional<T> filhoEsquerdaDe(T chave) const
    {
        Nodo<T> *nodoAtual = this->raiz;

        while (true)
        {
            if (!nodoAtual || !nodoAtual->filhoEsquerda)
                break;
            if (nodoAtual->chave == chave)
                return nodoAtual->filhoEsquerda->chave;

            if (nodoAtual->chave > chave) {
                nodoAtual = nodoAtual->filhoEsquerda;
            } else {
                nodoAtual = nodoAtual->filhoDireita;
            }
        }
        return std::nullopt;
    };

    /**
     * @brief Busca a chave do filho a direita de uma (sub)arvore
     * @param chave chave da arvore que eh pai do filho a direita
     * @return Chave do filho a direita. Se chave nao esta na arvore, retorna nullptr
     */
    virtual std::optional<T> filhoDireitaDe(T chave) const
    {
        Nodo<T> *nodoAtual = this->raiz;

        while (true)
        {
            if (!nodoAtual || !nodoAtual->filhoDireita)
                break;
            if (nodoAtual->chave == chave)
                return nodoAtual->filhoDireita->chave;

            if (nodoAtual->chave > chave){
                nodoAtual = nodoAtual->filhoEsquerda;
            } else {
                nodoAtual = nodoAtual->filhoDireita;
            }
        }
        return std::nullopt;
    };

    /**
     * @brief Lista chaves visitando a arvore em ordem
     * @return Lista encadeada contendo as chaves em ordem.
     */
    virtual ListaEncadeadaAbstrata<T> *emOrdem() const
    {
        ListaEncadeadaAbstrata<T> *newLista = new MinhaListaEncadeada<T>;
        getListaEmOrdem(this->raiz, newLista);
        return newLista;
    };

    void getListaEmOrdem (Nodo<T> *raiz, ListaEncadeadaAbstrata<T> *lista) const
    {
        if(!raiz)
            return;
        getListaEmOrdem(raiz->filhoEsquerda, lista);
        lista->inserirNoFim(raiz->chave);
        getListaEmOrdem(raiz->filhoDireita, lista);
    }

    /**
     * @brief Lista chaves visitando a arvore em pre-ordem
     * @return Lista encadeada contendo as chaves em pre-ordem.
     */
    virtual ListaEncadeadaAbstrata<T> *preOrdem() const
    {
        ListaEncadeadaAbstrata<T> *newLista = new MinhaListaEncadeada<T>;
        getListaPreOrdem(this->raiz, newLista);
        return newLista;
    };

    void getListaPreOrdem (Nodo<T> *raiz, ListaEncadeadaAbstrata<T> *lista) const
    {
        if(!raiz)
            return;
        lista->inserirNoFim(raiz->chave);
        getListaPreOrdem(raiz->filhoEsquerda, lista);
        getListaPreOrdem(raiz->filhoDireita, lista);
    }
    /**
     * @brief Lista chaves visitando a arvore em pos-ordem
     * @return Lista encadeada contendo as chaves em pos ordem.
     */
    virtual ListaEncadeadaAbstrata<T> *posOrdem() const
    {
        ListaEncadeadaAbstrata<T> *newLista = new MinhaListaEncadeada<T>;
        getListaPosOrdem(this->raiz, newLista);
        return newLista;
    };

    void getListaPosOrdem (Nodo<T> *raiz, ListaEncadeadaAbstrata<T> *lista) const
    {
        if(!raiz)
            return;
        getListaPosOrdem(raiz->filhoEsquerda, lista);
        getListaPosOrdem(raiz->filhoDireita, lista);
        lista->inserirNoFim(raiz->chave);
    }

    Nodo<T>*& getNodoByChave(T chave)
    {
        Nodo<T>** nodoAtual = &this->raiz;

        while (*nodoAtual) {
            if ((*nodoAtual)->chave == chave) {
                return *nodoAtual;
            }

            if (chave < (*nodoAtual)->chave) {
                nodoAtual = &(*nodoAtual)->filhoEsquerda;
            } else {
                nodoAtual = &(*nodoAtual)->filhoDireita;
            }
        }
        //Confusão arrumar dps
        throw std::runtime_error("Não achei a chave");
    }

    Nodo<T>*getPaiByChave(int chave)
    {
        Nodo<T> *nodoAtual = this->raiz;
        Nodo<T> *nodoPai = this->raiz;

        while (true)
        {
            if(!nodoAtual)
                break;
            nodoPai = nodoAtual;

            if(nodoAtual->chave > chave){
                nodoAtual = nodoAtual->filhoEsquerda;
            } else {
                nodoAtual = nodoAtual->filhoDireita;
            }
        }
        return nodoPai;
    }

    int alturaPorNodo(Nodo<T> *raiz)
    {
        if(raiz == nullptr) return -1;
        int alturaEsquerda = raiz->filhoEsquerda ?
                                raiz->filhoEsquerda->altura : -1;
        int alturaDireita = raiz->filhoDireita ?
                                raiz->filhoDireita->altura : -1;

        return std::max(alturaEsquerda, alturaDireita) + 1;
    }


    void rotacaoSimplesDireita(Nodo<T>*& raiz)
    {
        Nodo<T>* filhoEsquerda = raiz->filhoEsquerda;
        raiz->filhoEsquerda = filhoEsquerda->filhoDireita;
        filhoEsquerda->filhoDireita = raiz;

        // Update heights after the rotation
        raiz->altura = alturaPorNodo(raiz);
        filhoEsquerda->altura = alturaPorNodo(filhoEsquerda);

        // Update the reference to the new subtree root
        raiz = filhoEsquerda;
    }


    void rotacaoSimplesEsquerda(Nodo<T>*& raiz)
    {
        Nodo<T>* filhoDireita = raiz->filhoDireita;
        raiz->filhoDireita = filhoDireita->filhoEsquerda;
        filhoDireita->filhoEsquerda = raiz;

        // Update the heights after the rotation
        raiz->altura = alturaPorNodo(raiz);
        filhoDireita->altura = alturaPorNodo(filhoDireita);

        // Update the reference to the new subtree root
        raiz = filhoDireita;
    }

    void rotacaoEsquerdaDireita(Nodo<T>*& raiz)
    {
        rotacaoSimplesEsquerda(raiz->filhoEsquerda);
        rotacaoSimplesDireita(raiz);
    }

    void rotacaoDireitaEsquerda(Nodo<T>*& raiz)
    {
        rotacaoSimplesDireita(raiz->filhoDireita);
        rotacaoSimplesEsquerda(raiz);
    }

    int getBFactor(Nodo<T> *nodo)
    {
        if(!nodo) return 0;
        int alturaEsquerda = nodo->filhoEsquerda ? nodo->filhoEsquerda->altura : -1;
        int alturaDireita = nodo->filhoDireita ? nodo->filhoDireita->altura : -1;
        return alturaEsquerda - alturaDireita;
    }

    void rebalance(Nodo<T>*& raiz) {
        int fatorB = getBFactor(raiz);

        if (fatorB > 1) {
            if (getBFactor(raiz->filhoEsquerda) < 0) {
                rotacaoSimplesEsquerda(raiz->filhoEsquerda);
            }
            rotacaoSimplesDireita(raiz);
        }
        else if (fatorB < -1) {
            if (getBFactor(raiz->filhoDireita) > 0) {
                rotacaoSimplesDireita(raiz->filhoDireita);
            }
            rotacaoSimplesEsquerda(raiz);
        }
    }

    Nodo<T>* getMinimo(Nodo<T>* raiz) {
        while (raiz->filhoEsquerda) {
            raiz = raiz->filhoEsquerda;
        }
        return raiz;
    }
};

template <typename T>
const int quantidadeDeNodos(Nodo<T> *nodo)
{
    if (nodo == nullptr)
    {
        return 0;
    }
    return 1 + quantidadeDeNodos(nodo->filhoEsquerda) + quantidadeDeNodos(nodo->filhoDireita);
}

#endif
