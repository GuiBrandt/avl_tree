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
#include <iterator>
#include <iostream>
#include <utility>
#include <queue>
#include <stack>

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

	T* info;			//! Informação do nó
	avl_tree* left;		//! Nó à esquerda
	avl_tree* right;	//! Nó à direita

	int _size;			//! Número de elementos na árvore
	int _height;		//! Altura da árvore

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
		if (!ptr)
			return;

		if (ptr->empty()) {
			delete ptr;
			ptr = nullptr;
		}
	}

	/**
	 * @brief Rotação à esquerda
	 */
	void rotate_left() {
		using std::swap;

		avl_tree* aux = this->right;

		this->right = aux->left;

		swap(*this, *aux);
		this->left = aux;

		swap(this->_size, aux->_size);
	}

	/**
	 * @brief Rotação à direita
	 */
	void rotate_right() {
		using std::swap;

		avl_tree* aux = this->left;

		this->left = aux->right;

		swap(*this, *aux);
		this->right = aux;

		swap(this->_size, aux->_size);
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
		_height = 0;
		_size = 0;
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
		_height = model._height;
		_size = model._size;

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
		swap(first._height, other._height);
		swap(first._size, other._size);
	}
	
	/**
	 * @brief Obtém a altura da árvore
	 * 
	 * @return int a altura da árvore
	 */
	int height() const {
		int lh = left ? left->height() : 0,
			rh = right ? right->height() : 0;

		return (lh > rh ? lh : rh) + 1;
	}
	
	/**
	 * @brief Obtém a quantidade de elementos da árvore
	 * 
	 * @return int a quantidade de elementos da árvore
	 */
	int size() const {
		return _size;
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

		T aux;

		if (right) {
			aux = right->pop();
			delete_if_empty(right);

		} else {
			aux = *info;
			info = left ? new T(left->popleft()) : nullptr;
			delete_if_empty(left);
		}

		rebalance();

		return aux;
	}

	/**
	 * @brief Remove o menor valor da árvore e retorna
	 * 
	 * @return T Menor valor contido na árvore
	 */
	T popleft() {
		if (empty())
			throw "Can't pop from an empty tree";

		T aux;

		if (left) {
			aux = left->popleft();
			delete_if_empty(left);

		} else {
			aux = *info;
			info = right ? new T(right->pop()) : nullptr;
			delete_if_empty(right);
		}

		rebalance();

		return aux;
	}

	/**
	 * @brief Remove todas as informações da árvore
	 */
	void clear() {
		this->~avl_tree();

		info = nullptr;
		left = right = nullptr;
		_height = 0;
		_size = 0;
	}

	/**
	 * @brief Insere uma informação na árvore
	 * 
	 * @param data Dados a serem inseridos na árvore
	 */
	void insert(T data) {
		Compare is_less;

		if (info == nullptr) {
			info = new T(data);
			_height = 1;

		} else if (is_less(data, *info)) {
			if (left == nullptr)
				left = new avl_tree();

			left->insert(data);
			/*if (!right || right->height() < left->height())
				_height = left->height() + 1;*/

		} else if (data != *info) {
			if (right == nullptr)
				right = new avl_tree();

			right->insert(data);
			/*if (!left || left->height() < right->height())
				_height = right->height() + 1;*/

		} else
			throw "Repeated information";

		_size++;

		rebalance();
	}

	/**
	 * @brief Remove uma informação da árvore
	 * 
	 * @param data Informação a ser removida
	 */
	void remove(const T & data) {
		Compare is_less;

		if (empty())
			throw "Can't remove from empty tree";

		if (*info == data) {
			if (left) {
				*info = left->pop();
				delete_if_empty(left);

			} else if (right) {
				*info = right->popleft();
				delete_if_empty(right);

			} else {
				delete info;
				info = nullptr;
			}

		} else if (left && is_less(data, *info)) {
			left->remove(data);
			delete_if_empty(left);

		} else if (right) {
			right->remove(data);
			delete_if_empty(right);

		} else
			throw "Information not found";

			
		_size--;
	}
	
	/**
	 * @brief Determina se uma informação existe na árvore
	 * 
	 * @param data Dados a serem procurados
	 */
	bool includes(const T & data) const {
		Compare is_less;

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

	/**
	 * @brief Classe de iterador por nível da árvore AVL
	 * 
	 */
	class level_iterator : public std::iterator<std::input_iterator_tag, const T*> {
		friend class avl_tree;

		private:
			typedef std::pair<int, avl_tree*> node;	//! Tipo usado para um nó na árvore

			std::queue<node> q;						//! Fila do iterador
			int _level;								//! Nível atual do iterador na árvore

			/**
			 * @brief Construtor
			 * 
			 * @param t Ponteiro para a árvore AVL de início
			 */
			level_iterator(avl_tree* t) {
				if (t)
					q.push(node(0, t));

				_level = 0;
			}

		public:

			/**
			 * @brief Construtor de cópia
			 * 
			 * @param other Objeto modelo
			 */
			level_iterator(const level_iterator & other) {
				q = other.q;
				_level = other._level;
			}

			/**
			 * @brief Operador de incremento prefixo
			 * 
			 * @return level_iterator& Este iterador, uma posição à frente
			 */
			level_iterator& operator++() {
				node current = q.front();

				int lv = current.first;
				avl_tree* t = current.second;

				if (t->left)
					q.push(node(lv + 1, t->left));

				if (t->right)
					q.push(node(lv + 1, t->right));

				q.pop();

				return *this;
			}

			/**
			 * @brief Operador de swap
			 * 
			 * @param a Um iterador
			 * @param b Outro iterador
			 */
			friend void swap(level_iterator & a, level_iterator & b) {
				using std::swap;

				swap(a.q, b.q);
				swap(a.level, b.level);
			}

			/**
			 * @brief Operador de incremento posfixo
			 * 
			 * @return level_iterator Uma cópia deste iterador
			 */
			level_iterator operator++(int) {
				level_iterator aux(*this);
				operator++();
				return aux;
			}
			
			/**
			 * @brief Operador de igualdade
			 * 
			 * @param other Iterador a ser comparado
			 * @return true se forem iguais
			 * @return false se não
			 */
			bool operator==(const level_iterator & other) const {
				if (q.empty())
					return other.q.empty();

				return q.front() == other.q.front();
			}

			/**
			 * @brief Operador de não-igualdade
			 * 
			 * @param other Iterador a ser comparado
			 * @return true se forem iguais
			 * @return false se não
			 */
			bool operator!=(const level_iterator & other) const {
				return !(*this == other);
			}

			/**
			 * @brief Obtém o nível atual do iterador na árvore
			 * 
			 * @return int O nível atual na árvore
			 */
			int level() {
				return q.front().first;
			}

			/**
			 * @brief Operador de derreferenciação
			 * 
			 * @return T& A informação atual
			 */
			const T& operator*() const {
				return *q.front().second->info;
			}

			/**
			 * @brief Operador de derreferenciação
			 * 
			 * @return T& Ponteiro da informação atual
			 */
			const T* operator->() const {
				return q.front().second->info;
			}
	};
	
	/**
	 * @brief Obtém o iterador por nível para o começo da árvore
	 * 
	 * @return level_iterator Iterador por nível
	 */
	level_iterator level_begin() {
		return empty() ? level_end() : level_iterator(this);
	}

	/**
	 * @brief Obtém o iterador por nível para o fim da árvore
	 * 
	 * @return level_iterator Iterador por nível
	 */
	level_iterator level_end() {
		return level_iterator(nullptr);
	}

	/**
	 * @brief Escreve uma árvore para uma stream de saída em ordem
	 * 
	 * @param out Stream de saída
	 * @param tree Árvore a ser escrita
	 * @return std::ostream& A stream recebida por parâmetro
	 */
	friend std::ostream & operator << (
		std::ostream & out,
		const avl_tree < T > & tree
	) {
		out << "( ";

		if (tree.left)
			out << *tree.left << " ";

		if (tree.info)
			out << *tree.info << " ";

		if (tree.right)
			out << *tree.right << " ";

		out << ")";

		return out;
	}
};

#endif // AVL_TREE_HPP