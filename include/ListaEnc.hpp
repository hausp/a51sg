// Copyright 2015 Ghabriel Nunes
#include "Elemento.hpp"

const int ERROLISTACHEIA = -1;
const int ERROLISTAVAZIA = -2;
const int ERROPOSICAO = -3;
const int ERRODADONAOENCONTRADO = -4;

template<typename T>

//! Define uma lista encadeada.
/*!
 * Implementa uma lista encadeada, cujo tamanho máximo só é limitado pela quantidade
 * de memória do computador.
 * \author Ghabriel Nunes
 * \since 02/04/15
 * \version 1.0
 */
class ListaEnc {
 public:
	//! Construtor principal.
 	/*!
 		Inicializa uma lista vazia.
 	*/
	ListaEnc<T>() {
		size = 0;
		head = NULL;
	}

	//! Destrutor principal.
	/*!
		Chama o método destroiLista(), responsável por limpar toda a memória ocupada
		pela lista.
		\sa destroiLista()
	*/
	/*~ListaEnc() {
		destroiLista();
	}*/

	//! Um método de adicionar dados ao início da lista.
	/*!
		Adiciona um dado na primeira posição da lista, desde que haja memória suficiente
		disponível.
		\param dado o dado a ser adicionado à lista
		\throws ERROLISTACHEIA caso não haja memória suficiente disponível
		\sa adiciona(const T& dado), adicionaNaPosicao(const T& dado, int pos)
	*/
	void adicionaNoInicio(const T& dado) {
		Elemento<T> *novo = new Elemento<T>(dado, head);
		if (novo == NULL) {
			throw ERROLISTACHEIA;
		}
		head = novo;
		size++;
	}

	//! Um método de remover dados do início da lista.
	/*!
		Remove o elemento presente no início da lista e o retorna,
		desde que ela não esteja vazia.
		\throws ERROLISTAVAZIA caso a lista esteja vazia
		\return o elemento do início da lista
		\sa retiraDaPosicao(int pos), retiraEspecifico(const T& dado), destroiLista()
	*/	
	T retiraDoInicio() {
		if (listaVazia()) {
			throw ERROLISTAVAZIA;
		}
		Elemento<T> *removido = head;
		head = removido->getProximo();
		size--;
		T info = removido->getInfo();
		delete removido;
		return info;
	}

	//! Equivalente ao método retiraDoInicio(), porém sem retorno.
	/*!
		\throws ERROLISTAVAZIA caso a lista esteja vazia
		\sa retiraDaPosicao(int pos), retiraEspecifico(const T& dado), destroiLista()		
	*/
	void eliminaDoInicio() {
		retiraDoInicio();
	}

    //! Um método de adicionar dados a uma posição qualquer da lista.
    /*!
    	Adiciona um dado à posição especificada da lista, desde que a posição seja
    	válida (isto é, não-negativa e de modo a não deixar espaços vazios na lista)
    	e haja memória suficiente disponível.
    	\param dado o dado a ser adicionado à lista
    	\param posicao a posição do dado a ser adicionado à lista
    	\throws ERROLISTACHEIA caso a lista esteja cheia
    	\throws ERROPOSICAO caso a posição seja inválida, pelos critérios mencionados acima
    	\sa adiciona(const T& dado), adicionaNoInicio(const T& dado)
    */
	void adicionaNaPosicao(const T& dado, int pos) {
		if (pos < 0 || pos > size) {
			throw ERROPOSICAO;  // (erro de posição)
		}
		if (pos == 0) {
			adicionaNoInicio(dado);
			return;
		}

		Elemento<T> *novo = new Elemento<T>(dado, NULL);
		if (novo == NULL) {
			throw ERROLISTACHEIA;
		}

		Elemento<T> *anterior = head;
		for (int i = 0; i < pos - 1; i++) {
			anterior = anterior->getProximo();
		}
		novo->setProximo(anterior->getProximo());
		anterior->setProximo(novo);
		size++;
	}

	//! Retorna a posição de um determinado dado na lista.
	/*!
		\throws ERRODADONAOENCONTRADO caso a lista não contenha o dado especificado
		\return o índice da lista no qual o dado se encontra
		\sa retiraEspecifico(const T& dado), posicaoMem(const T& dado), contem(const T& dado)
	*/
	int posicao(const T& dado) const {
		if (listaVazia()) {
			throw ERRODADONAOENCONTRADO;
		}

		Elemento<T> *atual = head;
		int pos = 0;
		while (atual != NULL && atual->getInfo() != dado) {
			atual = atual->getProximo();
			pos++;
		}

		if (atual == NULL) {
			throw ERRODADONAOENCONTRADO;
		}
		return pos;
	}

	//! Retorna um ponteiro que aponta para a posição de um dado na lista.
	/*!
		\throws ERRODADONAOENCONTRADO caso a lista não contenha o dado especificado
		\return um ponteiro para a posição da lista na qual o dado se encontra
		\sa retiraEspecifico(const T& dado), contem(const T& dado)
	*/
	T* posicaoMem(const T& dado) const {
		if (listaVazia()) {
			throw ERRODADONAOENCONTRADO;
		}

		Elemento<T> *atual = head;
		int pos = 0;
		while (atual != NULL && !igual(atual->getInfo(), dado)) {
			atual = atual->getProximo();
			pos++;
		}

		if (atual == NULL) {
			throw ERRODADONAOENCONTRADO;
		}
		return &(atual->getInfo());
	}

	//! Retorna um booleano indicando se a lista contém ou não o dado especificado.
	/*!
		\return true se a lista contém o dado especificado; caso contrário, false
		\sa posicao(const T& dado), posicaoMem(const T& dado)
	*/
	bool contem(const T& dado) {
		try {
			posicao(dado);
		} catch (int e) {
			return false;
		}
		return true;
	}

	//! Um método de remover dados de uma determinada posição da lista.
	/*!
		Remove o elemento presente na posição especificada da lista e o retorna,
		desde que a posição seja válida e ela não esteja vazia.
		\throws ERROLISTAVAZIA caso a lista esteja vazia
		\throws ERROPOSICAO caso a posição seja inválida
		\return o elemento da posição especificada da lista
		\sa retira(), retiraDoInicio(), retiraEspecifico(const T& dado), destroiLista()
	*/
	T retiraDaPosicao(int pos) {
		if (pos < 0 || pos > size - 1) {
			throw ERROPOSICAO;
		}
		if (listaVazia()) {
			throw ERROLISTAVAZIA;
		}
		if (pos == 0) {
			return retiraDoInicio();
		}

		Elemento<T> *anterior = head;
		for (int i = 0; i < pos - 1; i++) {
			anterior = anterior->getProximo();
		}

		Elemento<T> *atual = anterior->getProximo();
		if (pos == size - 1) {
			anterior->setProximo(NULL);
		} else {
			anterior->setProximo(atual->getProximo());
		}
		atual->setProximo(NULL);
		size--;
		T info = atual->getInfo();
		delete atual;
		return info;
	}

    //! Um método de adicionar dados ao fim da lista.
    /*!
    	Adiciona um dado na última posição da lista, desde que haja memória
    	suficiente disponível.
    	\param dado o dado a ser adicionado à lista
    	\throws ERROLISTACHEIA caso não haja memória suficiente disponível
    	\sa adicionaNoInicio(const T& dado), adicionaNaPosicao(const T& dado, int posicao)
    */
	void adiciona(const T& dado) {
		adicionaNaPosicao(dado, size);
	}

	//! Um método de remover dados do fim da lista.
	/*!
		Remove o elemento presente no fim da lista e o retorna,
		desde que ela não esteja vazia.
		\throws ERROLISTAVAZIA caso a lista esteja vazia
		\return o elemento do fim da lista
		\sa retiraDaPosicao(int pos), retiraEspecifico(const T& dado), destroiLista()
	*/
	T retira() {
		return retiraDaPosicao(size - 1);
	}

	//! Um método de remover um dado específico da lista.
	/*!
		Remove o dado especificado da lista, caso tal dado se encontre nela.
		\throws ERROLISTAVAZIA caso a lista esteja vazia
		\throws ERRODADONAOENCONTRADO caso a lista não contenha o dado especificado
		\return o dado removido
		\sa retira(), retiraDoInicio(), destroiLista(), contem(const T& dado)
	*/
	T retiraEspecifico(const T& dado) {
		if (listaVazia()) {
			throw ERROLISTAVAZIA;
		}

		Elemento<T> *atual = head;
		int pos = 0;
		while (atual != NULL && !igual(atual->getInfo(), dado)) {
			atual = atual->getProximo();
			pos++;
		}

		if (atual == NULL) {
			throw ERRODADONAOENCONTRADO;
		}
		return retiraDaPosicao(pos);
	}

    //! Um método de adicionar dados à lista de forma ordenada.
    /*!
    	Adiciona um dado na posição i da lista, de forma que todos os elementos em
    	índices j < i sejam menores que esse dado.
    	\param data o dado a ser adicionado à lista
    	\throws ERROLISTACHEIA caso não haja memória suficiente disponível
    	\sa adicionaNaPosicao(const T& dado, int pos)
    */
	void adicionaEmOrdem(const T& data) {
		if (listaVazia()) {
			adicionaNoInicio(data);
		}

		Elemento<T> *novo = new Elemento<T>(data, NULL);
		if (novo == NULL) {
			throw ERROLISTACHEIA;
		}

		Elemento<T> *atual = head;
		int pos = 0;
		while (atual != NULL && maior(data, atual->getInfo())) {
			atual = atual->getProximo();
			pos++;
		}
		adicionaNaPosicao(data, pos);
	}

	//! Retorna um booleano indicando se a lista está vazia.
	/*
		\return true se a lista estiver vazia; caso contrário, false
	*/
	bool listaVazia() const {
		return size == 0;
	}

	//! Retorna um booleano indicando se os dados passados são iguais.
	/*
		\return true se os dados são iguais; caso contrário, false
		\sa maior(T dado1, T dado2), menor(T dado1, T dado2)
	*/
	bool igual(T dado1, T dado2) {
		return dado1 == dado2;
	}

	//! Indica se o primeiro dado é maior que o segundo.
	/*
		Retorna um booleano indicando se o primeiro dado passado é maior que o outro.
		\return true se o primeiro dado é maior que o segundo; caso contrário, false
		\sa igual(T dado1, T dado2), menor(T dado1, T dado2)
	*/
	bool maior(T dado1, T dado2) {
		return dado1 > dado2;
	}

	//! Indica se o primeiro dado é menor que o segundo.
	/*
		Retorna um booleano indicando se o primeiro dado passado é menor que o outro.
		\return true se o primeiro dado é menor que o segundo; caso contrário, false
		\sa igual(T dado1, T dado2), maior(T dado1, T dado2)
	*/
	bool menor(T dado1, T dado2) {
		return dado1 < dado2;
	}

	//! Limpa a lista.
	/*
		Limpa toda a memória ocupada pela lista.
		\sa retiraDaPosicao(int posicao), retiraEspecifico(T dado)
	*/
	void destroiLista() {
		 while (true) {
		 	try {
		 		eliminaDoInicio();
		 	} catch(int e) {
		 		return;
		 	}
		 }
	}

	lista_iterator<T> begin() const {
		return lista_iterator<T>(head);
	}

	lista_iterator<T> end()  const {
		return lista_iterator<T>(NULL);
	}

	int getSize() const {
		return size;
	}

 private:
	//! Um ponteiro para o primeiro elemento da lista.
 	/*!
 		Contém um ponteiro para o primeiro elemento da lista. Caso a lista esteja
 		vazia, head é um ponteiro nulo.
 	*/
	Elemento<T>* head;

	//! Quantidade de elementos na lista.
	/*!
		Contém um inteiro que representa a quantidade de elementos presente na lista.
	*/
	int size;
};

