#pragma once
//pre define
class MarioState;
class Mario;

class MarioData
{
public:
    MarioData();
    ~MarioData();

    Mario* mario;
    MarioState* state;

protected:

};