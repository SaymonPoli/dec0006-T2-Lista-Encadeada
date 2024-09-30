#ifndef DEC0006_MINHA_LISTA_ENCADEADA_H
#define DEC0006_MINHA_LISTA_ENCADEADA_H

#include <cstddef>
// std::size_t

#include "Elemento.h"
// Elemento
#include "ListaEncadeadaAbstrata.h"
#include "excecoes.h"
// ExcecaoDadoInexistente
// ExcecaoListaEncadeadaVazia
// ExcecaoNaoImplementado
// ExcecaoPosicaoInvalida

/**
 * @brief Uma lista encadeada de dados.
 *
 * @tparam T O tipo dos dados armazenados na lista.
 */
template <typename T>
class MinhaListaEncadeada : public ListaEncadeadaAbstrata<T>
{
    /**
     * @brief Destrutor. Destroi todos os elementos da lista
     */
    virtual ~MinhaListaEncadeada()
    {
        Elemento<T> *deletingElement = this->_primeiro;
        Elemento<T> *auxElement = nullptr;

        while (deletingElement != nullptr)
        {
            auxElement = deletingElement;
            deletingElement = deletingElement->proximo;
            delete auxElement;
        }
    }

    /**
     * @brief Obtém o elemento da posição lista
     *
     * @return Um ponteiro para o elemento desejado.
     */
    virtual Elemento<T> *getElemento(std::size_t posicao) const
    {
        if (posicao < this->_tamanho)
        {
            Elemento<T> *currentElement = this->_primeiro;
            Elemento<T> *nextElement = nullptr;

            int i = 0;

            while (i < posicao)
            {
                currentElement = currentElement->proximo;
                i++;
            }
            return currentElement;
        }
        else
        {
            std::cout << "Position not in vector" << std::endl;
            return NULL;
        }
    };

    /**
     * @brief Obtém a quantidade de itens na lista.
     *
     * @return Um inteiro maior ou igual a 0.
     */
    virtual std::size_t tamanho() const
    {
        return this->_tamanho;
    };

    /**
     * @brief Indica se há algum item na lista ou não.
     *
     * @return false se houver algum item na lista; true caso contrário.
     */
    virtual bool vazia() const
    {
        return this->_tamanho == 0;
    };

    /**
     * @brief Obtém a posição de um item na lista. Lança
     * ExcecaoListaEncadeadaVazia caso a lista esteja vazia ou
     * ExcecaoDadoInexistente caso o item não esteja contido na lista.
     *
     * @param dado O item cuja posição deseja-se obter.
     * @return Um inteiro na faixa [0, tamanho); se houver mais que um mesmo
     * item na lista, a posição da primeira ocorrência.
     */
    virtual std::size_t posicao(T dado) const
    {
        if (this->vazia())
            throw ExcecaoListaEncadeadaVazia();

        Elemento<T> *element = this->_primeiro;

        for (std::size_t posicao = 0; posicao < this->_tamanho; posicao++)
        {
            if (dado == element->dado)
            {
                return posicao;
            }
            element = element->proximo;
        }
        throw ExcecaoDadoInexistente();
    };

    /**
     * @brief Indica se um dado item está contido na lista ou não.
     *
     * @param dado O item sendo buscado.
     * @return true se o item está contido na lista; false caso contrário.
     */
    virtual bool contem(T dado) const
    {
        Elemento<T> *elemento = this->_primeiro;
        for (int posicao = 0; posicao < this->_tamanho; posicao++)
        {
            if (dado == elemento->dado)
            {
                return true;
            }
            elemento = elemento->proximo;
        }
        return false;
    };

    /**
     * @brief Insere um item no início da lista.
     *
     * @param dado O item sendo inserido.
     */
    virtual void inserirNoInicio(T dado)
    {
        this->_primeiro = new Elemento<T>(dado, this->_primeiro);
        this->_tamanho++;
    };

    /**
     * @brief Insere um item em uma posição específica da lista. Lança
     * ExcecaoPosicaoInvalida caso a posição não esteja na faixa
     * [0, tamanho].
     *
     * @param posicao Um inteiro dentro da faixa [0, tamanho]. Ao se inserir em
     * uma posição já ocupada, a posição do item que já estava naquela posição
     * será posicao + 1; inserir-se na posicao tamanho significa inserir-se no
     * fim da lista.
     * @param dado O item sendo inserido.
     */
    virtual void inserir(std::size_t posicao, T dado)
    {
        if (posicao > this->_tamanho)
            throw ExcecaoPosicaoInvalida();

        if (posicao == 0)
        {
            inserirNoInicio(dado);
        }
        else
        {
            Elemento<T> *anterior = getElemento(posicao - 1);
            anterior->proximo = new Elemento<T>(dado, anterior->proximo);
            this->_tamanho++;
        }
    };

    /**
     * @brief Insere um item no fim da lista.
     *
     * @param dado O item sendo inserido.
     */
    virtual void inserirNoFim(T dado)
    {

        if (vazia())
        {
            inserirNoInicio(dado);
        }
        else
        {
            getElemento(this->_tamanho - 1)->proximo = new Elemento<T>(dado, nullptr);
            this->_tamanho++;
        }
    };

    /**
     * @brief Remove o primeiro item da lista. Lança ExcecaoListaEncadeadaVazia
     * caso não haja nenhum item na lista.
     *
     * @return O item removido.
     */
    virtual T removerDoInicio()
    {
        if (vazia())
            throw ExcecaoListaEncadeadaVazia();

        Elemento<T> *removedElement = this->_primeiro;

        this->_primeiro = removedElement->proximo;
        this->_tamanho--;
        T dado = removedElement->dado;

        delete removedElement;

        return dado;
    };

    /**
     * @brief Remove um item de uma posição específica da lista. Lança
     * ExcecaoPosicaoInvalida caso a posição não esteja na faixa [0, tamanho).
     *
     * @param posicao Um inteiro dentro da faixa [0, tamanho).
     * @return O item removido.
     */
    virtual T removerDe(std::size_t posicao)
    {
        if (vazia() || posicao >= this->_tamanho)
        {
            throw ExcecaoPosicaoInvalida();
        }

        Elemento<T> *deletedElement = nullptr;

        if (posicao == 0)
        {
            deletedElement = this->_primeiro;
            this->_primeiro = deletedElement->proximo;
        }
        else
        {
            Elemento<T> *previusElement = getElemento(posicao - 1);
            deletedElement = previusElement->proximo;
            previusElement->proximo = deletedElement->proximo;
        }
        T dado = deletedElement->dado;
        delete deletedElement;
        this->_tamanho--;
        return dado;
    };

    /**
     * @brief Remove o último item da lista. Lança ExcecaoListaEncadeadaVazia
     * caso não haja nenhum item na lista.
     *
     * @return O item removido.
     */
    virtual T removerDoFim()
    {
        if (vazia())
            throw ExcecaoListaEncadeadaVazia();

        if (this->_tamanho == 1)
        {
            return removerDoInicio();
        }
        else
        {
            Elemento<T> *secondToLastElement = getElemento(this->_tamanho - 2);
            Elemento<T> *lastElement = secondToLastElement->proximo;
            secondToLastElement->proximo = nullptr;

            T dado = lastElement->dado;
            this->_tamanho--;
            delete lastElement;
            return dado;
        }
    };

    /**
     * @brief Remove um item específico da lista. Lança
     * ExcecaoListaEncadeadaVazia caso não haja nenhum item na lista ou
     * ExcecaoDadoInexistente caso o item não esteja contido na lista.
     *
     * @param dado O item a ser removido. Se houver mais que um item com
     * o mesmo valor, remove a primeira ocorrência.
     */
    virtual void remover(T dado)
    {
        if (vazia())
            throw ExcecaoListaEncadeadaVazia();

        Elemento<T> *removeElement = this->_primeiro;
        for (int i = 0; i <= this->_tamanho - 1; i++)
        {
            if (removeElement->dado == dado)
            {
                removerDe(i);
                return;
            }
            removeElement = removeElement->proximo;
        }
        throw ExcecaoDadoInexistente();
    };
};

#endif