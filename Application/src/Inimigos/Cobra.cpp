#include "Cobra.h"

const int C_total_anims = 3;
const int C_anim_parado = 0;
const int C_anim_atacando = 1;
const int C_anim_andando = 2;

Cobra::Cobra(Jogador *j1, Jogador *j2, Lista<Projetil*>* projeteis, int x, int y):
Inimigo(x, y, projeteis, C_total_anims) {
	_sp.setTexture(*GerenciadorDeTexturas::GetInstance()->GetCobra());

	_j1 = j1;
	_j2 = j2;
	_estado = EstadoCobra::Perdida;

	_max_vx = ConstsPersonagens::M_max_vx;
	_max_vy = Fisica::velocidade_terminal;

	_aceleracao_caminhada = ConstsPersonagens::M_aceleracao / 2;
	_desaceleracao = ConstsPersonagens::M_desaceleracao / 2;

	_velocidade_pulo = ConstsPersonagens::M_v_pulo;

	_range = ConstsPersonagens::M_distancia_deteccao * 30;
	_range_ataque = 300;

	_x0 = x;

	_animador.SetTextureBox(&_tex_rect);
	_animador.SetSprite(&_sp);
	_animador.SetTamanhoQuadro(Resources::block_size);

	_animador.SetOffSetY(0);
	_animador.SetPeriodo(0.1);

	_animador.SetFrameCount(C_anim_parado, 1);
	_animador.SetFrameCount(C_anim_atacando, 1);
	_animador.SetFrameCount(C_anim_andando, 2);

	_projetil_protipo.SetTexture(GerenciadorDeTexturas::GetInstance()->GetCobra());
	_projetil_protipo.SetTempoPraMorrer(1.5);
	_projetil_protipo.SetOffSetY(C_total_anims * Resources::block_size);
	_vx_ataque = 300;

	_timerDescanso = -1;
}

Cobra::~Cobra() {
	//dtor
}

void Cobra::Executar (float dt) {
	if (_posicao.y + _sp.getGlobalBounds().height > Janela::altura) {
		_vida = -1;
	}

	float dist_j1;
	float dist_j2;
	float mag_dist_j1;
	float mag_dist_j2;

	if (_j1 != NULL) {
		dist_j1 = _j1->GetPosicao().x - _posicao.x;
		mag_dist_j1 = dist_j1 > 0? dist_j1: -dist_j1;
	}
	if (_j2 != NULL) {
		dist_j2 = _j2->GetPosicao().x - _posicao.x;
		mag_dist_j2 = dist_j2 > 0? dist_j2: -dist_j2;
	}

	if (!_esta_no_chao) {
		_aceleracao.y = Fisica::G;
	}
	else {
		_aceleracao.y = 0;
	}

	int delta_x = 50;
        switch (_estado) {
        case EstadoCobra::Perdida:
		_animador.Play(C_anim_andando);
		_animador.SetLoop(true);

		if (!_esta_no_chao) {
			_estado = EstadoCobra::Perdida;
		}

		if (_posicao.x > _x0 && _posicao.x - _x0 >= delta_x) {
			Acelerar(dt, -_aceleracao_caminhada);
		}
		else if (_posicao.x < _x0 && _x0 - _posicao.x >= delta_x) {
			Acelerar(dt, _aceleracao_caminhada);
		}
		else if (_velocidade.x > 0) {
			Acelerar(dt, _aceleracao_caminhada);
		}
		else {
			Acelerar(dt, -_aceleracao_caminhada);
		}

		if (_esta_no_chao) {
			if (_batendo_esquerda || _batendo_direta) {
				_estado = EstadoCobra::Perdida;
			}
		}

		if (_j1 != NULL) {
			if (mag_dist_j1 < _range && mag_dist_j1 < mag_dist_j2) {
				_estado = EstadoCobra::SeguindoJ1;
			}
		}

		if (_j2 != NULL) {
			if (mag_dist_j2 < _range && mag_dist_j2 < mag_dist_j1) {
				_estado = EstadoCobra::SeguindoJ2;
			}
		}
		break;

	case EstadoCobra::SeguindoJ1:
		_animador.Play(C_anim_andando);
		_animador.SetLoop(true);

		if (!_esta_no_chao) {
			_estado = EstadoCobra::Perdida;
		}
		else if (mag_dist_j1 < _range_ataque && _podeAtacar) {
			_estado = EstadoCobra::Atacando;
		}
		else if (_j2 != NULL && mag_dist_j2 < _range && mag_dist_j2 < mag_dist_j1) {
			_estado = EstadoCobra::SeguindoJ2;
		}
		else if (mag_dist_j1 > _range) {
			_estado = EstadoCobra::Perdida;
		}
		else if (dist_j1 > 0) {
			Acelerar(dt, _aceleracao_caminhada);
		}
		else if (dist_j1 < 0) {
			Acelerar(dt,-_aceleracao_caminhada);
		}

		if (_esta_no_chao) {
			if (_batendo_esquerda || _batendo_direta) {
				_estado = EstadoCobra::SeguindoJ1;
			}
		}
		break;

	case EstadoCobra::SeguindoJ2:
		_animador.Play(C_anim_andando);
		_animador.SetLoop(true);

		if (!_esta_no_chao) {
			_estado = EstadoCobra::Perdida;
		}
		else if (mag_dist_j2 < _range_ataque && _podeAtacar) {
			_estado = EstadoCobra::Atacando;
		}
		else if (_j1 != NULL && mag_dist_j1 < _range && mag_dist_j1 < mag_dist_j2) {
			_estado = EstadoCobra::SeguindoJ1;
		}
		else if (mag_dist_j2 > _range) {
			_estado = EstadoCobra::Perdida;
		}
		else if (dist_j2 > 0) {
			Acelerar(dt, _aceleracao_caminhada);
		}
		else if (dist_j2 < 0) {
			Acelerar(dt, -_aceleracao_caminhada);
		}

		if (_esta_no_chao) {
			if (_batendo_esquerda || _batendo_direta) {
				_estado = EstadoCobra::SeguindoJ2;
			}
		}
		break;

	case EstadoCobra::Atacando:
		_animador.Play(C_anim_atacando);
		_animador.SetLoop(false);

		_velocidade.x = 0;

		if (_podeAtacar) {
			Atacar();
			_timerDescanso = 4;
			_podeAtacar = false;
		}

		if (_animador.GetTerminou()) {
			if (mag_dist_j1 < _range) {
				_estado = EstadoCobra::SeguindoJ1;
			}
			else if (mag_dist_j2 < _range) {
				_estado = EstadoCobra::SeguindoJ2;
			}
			else {
				_estado = EstadoCobra::Perdida;
			}
		}
		break;

	default:
		_estado = EstadoCobra::Perdida;
	}

	if (_timerDescanso > 0) {
		_timerDescanso -= dt;
		_podeAtacar = false;
	}
	else {
		_podeAtacar = true;
	}

	AtualizarFisica(dt);
	_animador.Executar(dt);
}

