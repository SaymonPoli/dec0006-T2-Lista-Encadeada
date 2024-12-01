
#ifndef MINHA_TABELA_ESPALHAMENTO_H
#define MINHA_TABELA_ESPALHAMENTO_H

#include "MinhaListaEncadeada.h"
// MinhaListaEncadeada
#include "TabelaEspalhamentoAbstrata.h"
// TabelaEspalhamentoAbstrata
#include "excecoes.h"
#include <cstddef>
#include <iterator>
#include <cmath>
// ExcecaoDadoInexistente

template <typename T, std::size_t capac>
class MinhaTabelaEspalhamento final
    : public TabelaEspalhamentoAbstrata<T, capac> {
public:
  /**
   * @brief Obtém a capacidade da tabela.
   *
   * @return Um inteiro maior do que 0.
   */
  virtual std::size_t capacidade() const { return capac; };

  /**
   * @brief Insere um dado na tabela. Se a tabela já contiver o dado, ele não
   * é inserido novamente.
   *
   * @param dado O dado a ser inserido.
   */
  virtual void inserir(T dado) {
    if(this->tabela[funcaoEspalhamento(dado)].contem(dado))
    {
        return;
    }else
    {
        this->tabela[funcaoEspalhamento(dado)].inserirNoFim(dado);
    }
  };

  /**
   * @brief Remove um dado da tabela. Se a tabela não contiver o dado, uma
   * exceção ExcecacaoDadoInexistente é lançada.
   *
   * @param dado O dado a ser removido.
   */
  virtual void remover(T dado) {
      this->tabela[funcaoEspalhamento(dado)].remover(dado);
    };

  /**
   * @brief Verifica se \p dado está contido na tabela.
   *
   * @param dado O dado sendo buscado.
   * @return true se o dado está contido na tabela; false caso contrário.
   */
  virtual bool contem(T dado) const {
   return this->tabela[funcaoEspalhamento(dado)].contem(dado);
  };

  /**
   * @brief Obtém a quantidade de dados contidos na árvore.
   *
   * @return Um inteiro maior ou igual a 0.
   */
  virtual std::size_t quantidade() const {
    std::size_t tamanho{0};
    for(ListaEncadeadaAbstrata<T>const&  element : this->tabela) {
        if(!element.vazia()) tamanho += element.tamanho();
    }
    return tamanho;
  };

protected:
  /**
   * @brief Obtém a posição correspondente na tabela para o dado.
   *
   * @param dado O dado.
   * @return Um inteiro na faixa [0, capacidade).
   */
  virtual std::size_t funcaoEspalhamento(T dado) const {
    return (codigoEspalhamento(dado) % capacidade());
  };

private:
  /**
   * @brief Calcula a representação numérica de um tipo integral.
   *
   * @tparam U O tipo integral.
   * @param integral Um valor integral.
   * @return Um inteiro calculado através de static_cast<std::size_t>(integral)
   */
  template <typename U> std::size_t codigoEspalhamento(U integral) const {
    // substitua a linha abaixo pelo algoritmo esperado. Dica use
    // std::is_integral_v<U> para garantir que U é um tipo integral
    if (!std::is_integral_v<U>)
      return -1;

    return static_cast<std::size_t>(integral);
  };

  /**
   * @brief Calcula a representação numérica de um string.
   *
   * @param s um string.
   * @return Um inteiro calculado conforme s[0]*31^(n-1) + s[1]*31^(n-2) + ... +
   * s[n-1].
   */
  std::size_t codigoEspalhamento(std::string const &string) const {
    size_t stringEquivalent{0}, stringSize{string.size()};
    for (size_t i = 0; i < stringSize; i++) {
        stringEquivalent += string[i] * pow(31 ,((stringSize) - i + 1));
    }
    return stringEquivalent;
  }
};

#endif
