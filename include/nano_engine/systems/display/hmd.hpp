#ifndef NANO_ENGINE_SYSTEMS_DISPLAY_HMD_HPP_
#define NANO_ENGINE_SYSTEMS_DISPLAY_HMD_HPP_

namespace ne
{
class hmd
{
public:
  hmd()
  {

  }
  hmd(const hmd&  that) = delete ;
  hmd(      hmd&& temp) = default;
  virtual ~hmd()
  {
  }
  hmd& operator=(const hmd&  that) = delete ;
  hmd& operator=(      hmd&& temp) = default;

protected:

};
}

#endif
