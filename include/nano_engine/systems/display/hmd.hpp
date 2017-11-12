#ifndef NANO_ENGINE_SYSTEMS_DISPLAY_HMD_HPP_
#define NANO_ENGINE_SYSTEMS_DISPLAY_HMD_HPP_

#include <openvr.h>

namespace ne
{
class hmd final
{
public:
  hmd ()
  {

  }
  hmd (const hmd&  that)           = delete ;
  hmd (      hmd&& temp)           = default;
  ~hmd()
  {

  }
  hmd& operator=(const hmd&  that) = delete ;
  hmd& operator=(      hmd&& temp) = default;

private:

};
}

#endif
