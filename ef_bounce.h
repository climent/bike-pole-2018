#ifndef EF_BOUNCE_H
#define EF_BOUNCE_H

#include effect.h

class Bounce : public Effect
{
public:
  Effect();
  String Identify();
  void Animate();
  void Render();

private:
  int _pos;
} Bounce;

#endif
