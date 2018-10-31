#pragma once

#include"entidade.h"

enum Direcao {LEFT, RIGHT};

class EntidadeDinamica: public Entidade{

protected:

    Vetor<> velocidade;
    Vetor<> aceleracao;
    Direcao dir;

public:
  
    EntidadeDinamica(Vetor<int> p, int w, int h, Vetor<> v, Vetor<> a, Direcao d);
    EntidadeDinamica();
    ~EntidadeDinamica();
    
    void setvel(Vetor<> v);
    void setvel(double vx, double vy);
    Vetor<> getvel();
   
    void setace(Vetor<> a);
    void setace(double ax, double ay);
    Vetor<> getace();
    
    void setdirecao();
  
    virtual void andar();
    virtual void atacar();
    virtual void executar();

};
