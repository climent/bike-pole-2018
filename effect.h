#ifndef EFFECT_H
#define EFFECT_H

class Effect
{
public:
  Effect();
  String Identify();
  void Animate();
  void Render();

private:
  int _pos;
}

#endif
