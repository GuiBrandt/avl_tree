/**
 * @brief Cabeçalho para a classe de árvore AVL
 * 
 * @file avl_tree.hpp
 * @author Guilherme Brandt
 * @date 2018-09-08
 */

#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include <functional>
#include <iostream>

/**
 * @brief Árvore AVL
 * 
 * @tparam T Tipo de valor armazenado na árvore
 */
template <
	class T,
	class Key = T,
	class Compare = std::less<T>
> class avl_tree {
private:

	T* info;
	avl_tree* left;
	avl_tree* right;
	
	Compare is_less = Compare();

	/**
	 * @brief Calcula o fator de balanceamento da árvore
	 * 
	 * @return int o fator de balanceamento do nó
	 */
	int balance_factor() const {
		return (right ? right->height() : 0) - (left ? left->height() : 0);
	}

	/**
	 * @brief Libera a memória de um galho e apaga ele, caso esteja vazio
	 * 
	 * @param ptr Referência de ponteiro do galho
	 */
	inline void delete_if_empty(avl_tree* & ptr) {
		if (ptr->empty()) {
			delete ptr;
			ptr = nullptr;
		}
	}

	/**
	 * @brief Rotação à esquerda
	 */
	void rotate_left() {
		avl_tree* aux = this->right;

		this->right = aux->left;

		swap(*this, *aux);
		this->left = aux;
	}

	/**
	 * @brief Rotação à direita
	 */
	void rotate_right() {
		avl_tree* aux = this->left;

		this->left = aux->right;

		swap(*this, *aux);
		this->right = aux;
	}

	/**
	 * @brief Ajusta a árvore de forma a recuperar o balanceamento
	 */
	void rebalance() {
		int balance = balance_factor();

		if (balance < -1) {
			if (left->balance_factor() > 0)
				left->rotate_left();
			
			rotate_right();
		} else if (balance > 1) {
			if (right->balance_factor() < 0)
				right->rotate_right();
				
			rotate_left();
		}
	}

	/**
	 * @brief Clona um ponteiro
	 */
	template <class ptrT> inline ptrT* clone_ptr(ptrT* ptr) {
		return ptr ? new ptrT(*ptr) : nullptr;
	}

public:

	/**
	 * @brief Construtor
	 */
	avl_tree() {
		info = nullptr;
		left = nullptr;
		right = nullptr;
	}
	
	/**
	 * @brief Destrutor
	 */
	~avl_tree() {
		if (info)
			delete info;

		if (left)
			delete left;

		if (right)
			delete right;
	}

	/**
	 * @brief Construtor de cópia
	 */
	avl_tree(const avl_tree & model) {
		*this = model;
	}
	
	/**
	 * @brief Operador de cópia
	 * 
	 * @param model Objeto modelo
	 * @return avl_tree& Cópia do objeto modelo
	 */
	avl_tree & operator = (const avl_tree & model) {
		if (this == & model)
			return *this;

		this->~avl_tree();

		info = clone_ptr(model.info);
		left = clone_ptr(model.left);
		right = clone_ptr(model.right);

		return *this;
	}
	
	/**
	 * @brief Operador de swap
	 * 
	 * @param first Primeiro objeto
	 * @param other Outro objeto
	 */
	friend void swap(avl_tree & first, avl_tree & other) {
		using std::swap;

		swap(first.info, other.info);
		swap(first.left, other.left);
		swap(first.right, other.right);
	}
	
	/**
	 * @brief Calcula a aCompareura da árvore
	 * 
	 * @return int a aCompareura da árvore
	 */
	int height() const {
		int lh = left ? left->height() : 0,
			rh = right ? right->height() : 0,

			h = lh > rh ? lh : rh;

		if (!empty())
			h++;

		return h;
	}

	/**
	 * @brief Determina se a árvore é uma folha
	 * 
	 * @return true se a árvore for uma folha
	 * @return false caso contrário
	 */
	bool is_leaf() const {
		return left == nullptr && right == nullptr;
	}

	/**
	 * @brief Determina se a árvore está vazia
	 * 
	 * @return true se a árvore não tiver nenhum elemento ou nó filho
	 * @return false caso contrário
	 */
	bool empty() const {
		return info == nullptr;
	}
	
	/**
	 * @brief Obtém o menor valor da árvore
	 * 
	 * @return T Menor valor contido na árvore
	 */
	T min() const {
		if (empty())
			throw "Empty tree has no minimum value";

		if (left)
			return left->min();

		return *info;
	}

	/**
	 * @brief Obtém o maior valor da árvore
	 * 
	 * @return T Maior valor contido na árvore
	 */
	T max() const {
		if (empty())
			throw "Empty tree has no maximum value";

		if (right)
			return right->max();

		return *info;
	}
	
	/**
	 * @brief Remove o maior valor da árvore e retorna
	 * 
	 * @return T Maior valor contido na árvore
	 */
	T pop() {
		if (empty())
			throw "Can't pop from an empty tree";

		if (right) {
			T aux = right->pop();
			delete_if_empty(right);

			rebalance();

			return aux;
		} else {
			T aux = *info;
			info = left ? new T(left->popleft()) : nullptr;

			rebalance();

			return aux;
		}
	}

	/**
	 * @brief Remove o menor valor da árvore e retorna
	 * 
	 * @return T Menor valor contido na árvore
	 */
	T popleft() {
		if (empty())
			throw "Can't pop from an empty tree";

		if (left) {
			T aux = left->popleft();
			delete_if_empty(right);

			rebalance();

			return aux;
		} else {
			T aux = *info;
			info = right ? new T(right->pop()) : nullptr;

			rebalance();

			return aux;
		}
	}

	/**
	 * @brief Remove todas as informações da árvore
	 */
	void clear() {
		~avl_tree();
		avl_tree();
	}

	/**
	 * @brief Insere uma informação na árvore
	 * 
	 * @param data Dados a serem inseridos na árvore
	 */
	void insert(T data) {
		if (info == nullptr)
			info = new T(data);

		else if (is_less(data, *info)) {
			if (left == nullptr)
				left = new avl_tree();

			left->insert(data);

		} else if (data != *info) {
			if (right == nullptr)
				right = new avl_tree();

			right->insert(data);

		} else
			throw "Repeated information";

		rebalance();
	}

	/**
	 * @brief Remove uma informação da árvore
	 * 
	 * @param data Informação a ser removida
	 */
	void remove(const T & data) {
		if (empty())
			throw "Can't remove from empty tree";

		if (*info == data) {
			if (left) {
				*info = left->pop();
				delete_if_empty(left);

			} else if (right) {
				*info = right->popleft();
				delete_if_empty(right);

			} else
				info = nullptr;

		} else if (left && is_less(data, *info)) {
			left->remove(data);
			delete_if_empty(left);

		} else if (right) {
			right->remove(data);
			delete_if_empty(right);

		} else
			throw "Information not found";
	}
	
	/**
	 * @brief Determina se uma informação existe na árvore
	 * 
	 * @param data Dados a serem procurados
	 */
	bool includes(const T & data) const {
		if (empty())
			return false;

		if (*info == data)
			return true;
		else if (left && is_less(data, *info))
			return left->includes(data);
		else if (right)
			return right->includes(data);
		else
			return false;
	}
};

#endif // AVL_TREE_HPP