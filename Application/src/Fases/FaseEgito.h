#ifndef FASEEGITO_H
#define FASEEGITO_H

#include "Fase.h"
#include "../LinkedLists.h"
#include "../Inimigos/Mumia.h"

class FaseEgito: public Fase {
	public:
		FaseEgito (Jogador *j1, Jogador* j2);
		~FaseEgito ();

		bool ChecarObjetivo ();
		void CarregaJogadores ();
		void CarregaInimigos (Lista<Inimigo*>* _pLista, Lista<Projetil*>* projeteis);
		void CarregaPlataformas (Lista<Plataforma*>* _pLista);

	private:
		Inimigo* chefe;
		Lista<sf::Vector2i*> _listaSpawnMumia;
		Lista<sf::Vector2i*> _listaSpawnCobra;
		Lista<Plataforma*> _listaPlataformas;

		void IncluiSpawnMumia (int x, int y);
		void IncluiSpawnCobra (int x, int y);
		void IncluiPlataforma (int x, int y, int h, int w);
};

#endif // FASEEGITO_H
