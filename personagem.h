#pragma once

#include"entidadedinamica.h"

class Personagem: public EntidadeDinamica{

protected:



public:

    Personagem(int x, int y, int w, int h);
    Personagem();
    ~Personagem();
    
    virtual void andar();
    virtual void atacar();
    
    virtual void executar();

};
